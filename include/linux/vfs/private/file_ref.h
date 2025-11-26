/* SPDX-License-Identifier: GPL-2.0-only */
#pragma once

#define FILE_REF_ONEREF 0x00000000U
#define FILE_REF_MAXREF 0x7FFFFFFFU
#define FILE_REF_SATURATED 0xA0000000U
#define FILE_REF_RELEASED 0xC0000000U
#define FILE_REF_DEAD 0xE0000000U
#define FILE_REF_NOREF 0xFFFFFFFFU

bool __file_ref_put(file_ref_t *ref, unsigned int cnt);

/**
 * file_ref_put -- Release a file reference
 * @ref:	Pointer to the reference count
 *
 * Provides release memory ordering, such that prior loads and stores
 * are done before, and provides an acquire ordering on success such
 * that free() must come after.
 *
 * Return: True if this was the last reference with no future references
 *         possible. This signals the caller that it can safely release
 *         the object which is protected by the reference counter.
 *         False if there are still active references or the put() raced
 *         with a concurrent get()/put() pair. Caller is not allowed to
 *         release the protected object.
 */
static inline bool file_ref_put(file_ref_t *ref)
{
    int cnt;

    /*
     * Unconditionally decrease the reference count. The saturation
     * and dead zones provide enough tolerance for this. If this
     * fails then we need to handle the last reference drop and
     * cases inside the saturation and dead zones.
     */
    cnt = atomic_dec_return(&ref->refcnt);
    if (cnt >= 0)
        return false;

    return __file_ref_put(ref, cnt);
}

/**
 * file_ref_put_close - drop a reference expecting it would transition to FILE_REF_NOREF
 * @ref:	Pointer to the reference count
 *
 * Semantically it is equivalent to calling file_ref_put(), but it trades lower
 * performance in face of other CPUs also modifying the refcount for higher
 * performance when this happens to be the last reference.
 *
 * For the last reference file_ref_put() issues 2 atomics. One to drop the
 * reference and another to transition it to FILE_REF_DEAD. This routine does
 * the work in one step, but in order to do it has to pre-read the variable which
 * decreases scalability.
 *
 * Use with close() et al, stick to file_ref_put() by default.
 */
static inline bool file_ref_put_close(file_ref_t *ref)
{
    int old;

    old = atomic_read(&ref->refcnt);
    if (likely(old == FILE_REF_ONEREF))
    {
        if (likely(atomic_try_cmpxchg(&ref->refcnt, &old, FILE_REF_DEAD)))
            return true;
    }

    return file_ref_put(ref);
}
