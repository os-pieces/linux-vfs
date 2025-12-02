#include <linux/vfs/private/fs.h>
#include <linux/vfs/private/file_ref.h>

static inline void file_free(struct file *f)
{
    kfree(f);
}

/* the real guts of fput() - releasing the last reference to file
 */
static void __fput(struct file *file)
{
    struct dentry *dentry = file->f_path.dentry;
    struct vfsmount *mnt = file->f_path.mnt;

    pr_todo();

    dput(dentry);
    mntput(mnt);

    file_free(file);
}

struct file *alloc_empty_file(int flags)
{
    struct file *f;

    f = kzalloc(sizeof(*f), GFP_KERNEL);
    if (f)
    {
        f->f_flags = flags;
        f->f_mode = OPEN_FMODE(flags);
        file_ref_init(&f->f_ref, 1);
    }

    return f;
}

void __fput_sync(struct file *file)
{
	if (file_ref_put(&file->f_ref))
		__fput(file);
}

void file_accessed(struct file *file)
{
}

static bool __file_ref_put_badval(file_ref_t *ref, unsigned int cnt)
{
    /*
     * If the reference count was already in the dead zone, then this
     * put() operation is imbalanced. Warn, put the reference count back to
     * DEAD and tell the caller to not deconstruct the object.
     */
    if (WARN_ONCE(cnt >= FILE_REF_RELEASED, "imbalanced put on file reference count"))
    {
        atomic_set(&ref->refcnt, FILE_REF_DEAD);
        return false;
    }

    /*
     * This is a put() operation on a saturated refcount. Restore the
     * mean saturation value and tell the caller to not deconstruct the
     * object.
     */
    if (cnt > FILE_REF_MAXREF)
        atomic_set(&ref->refcnt, FILE_REF_SATURATED);

    return false;
}

/**
 * __file_ref_put - Slowpath of file_ref_put()
 * @ref:	Pointer to the reference count
 * @cnt:	Current reference count
 *
 * Invoked when the reference count is outside of the valid zone.
 *
 * Return:
 *	True if this was the last reference with no future references
 *	possible. This signals the caller that it can safely schedule the
 *	object, which is protected by the reference counter, for
 *	deconstruction.
 *
 *	False if there are still active references or the put() raced
 *	with a concurrent get()/put() pair. Caller is not allowed to
 *	deconstruct the protected object.
 */
bool __file_ref_put(file_ref_t *ref, unsigned int cnt)
{
    /* Did this drop the last reference? */
    if (likely(cnt == FILE_REF_NOREF))
    {
        /*
         * Carefully try to set the reference count to FILE_REF_DEAD.
         *
         * This can fail if a concurrent get() operation has
         * elevated it again or the corresponding put() even marked
         * it dead already. Both are valid situations and do not
         * require a retry. If this fails the caller is not
         * allowed to deconstruct the object.
         */
        if (!atomic_try_cmpxchg_release(&ref->refcnt, &cnt, FILE_REF_DEAD))
            return false;

        /*
         * The caller can safely schedule the object for
         * deconstruction. Provide acquire ordering.
         */
        smp_acquire__after_ctrl_dep();
        return true;
    }

    return __file_ref_put_badval(ref, cnt);
}

/*
 * Equivalent to __fput_sync(), but optimized for being called with the last
 * reference.
 *
 * See file_ref_put_close() for details.
 */
void fput_close_sync(struct file *file)
{
    if (likely(file_ref_put_close(&file->f_ref)))
        __fput(file);
}

struct file *get_file(struct file *f)
{
    file_ref_inc(&f->f_ref);

    return f;
}
