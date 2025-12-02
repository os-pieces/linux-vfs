#include <linux/vfs/private/fs.h>

static long do_fcntl(int fd, unsigned int cmd, unsigned long arg,
                     struct file *filp)
{
    pr_todo();

    return 0;
}

long vfs_fcntl_api(filedesc_t *fdp, int fd, int cmd, unsigned long arg)
{
    struct fd f;
    long error;

    error = fdget_raw(fdp, fd, &f);
    if (error == 0)
    {
        error = do_fcntl(fd, cmd, arg, f.file);

        fdput(f);
    }

    return error;
}
