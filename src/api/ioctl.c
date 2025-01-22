#include <linux/vfs/fs.h>
#include <linux/vfs/private/file.h>

long vfs_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int error;

    if (filp->f_op->unlocked_ioctl)
    {
        error = filp->f_op->unlocked_ioctl(filp, cmd, arg);
        if (error == -ENOIOCTLCMD)
            error = -ENOTTY;
    }
    else
    {
        error = -ENOTTY;
    }

    return error;
}

int vfs_ioctl_api(filedesc_t *fdp, int fd, int cmd, unsigned long arg)
{
    struct fd f;
    int error;

    error = fdget(fdp, fd, &f);
    if (error == 0)
    {
        error = vfs_ioctl(f.file, cmd, arg);

        fdput(f);
    }

    return error;
}
