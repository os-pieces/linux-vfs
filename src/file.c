#include <linux/vfs/fs.h>

#include <linux/vfs/private/file.h>
#include <linux/vfs/private/file_ref.h>

/* the real guts of fput() - releasing the last reference to file
 */
static void __fput(struct file *file)
{
    pr_todo();
}

struct file *alloc_empty_file(int flags)
{
    struct file *f;

    f = kcalloc(1, sizeof(*f), 0);
    if (f)
    {
        f->f_flags = flags;
        f->f_mode = OPEN_FMODE(flags);
    }

    return f;
}

void file_inc_ref(struct file *f)
{
}

void fput(struct file *file)
{
    pr_todo();
}

void __fput_sync(struct file *file)
{
    pr_todo();
}

void file_accessed(struct file *file)
{
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
    pr_todo();
    return true;
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
