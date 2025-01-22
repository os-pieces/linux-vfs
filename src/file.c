#include <linux/vfs/fs.h>

#include <linux/vfs/private/file.h>

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
