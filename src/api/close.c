#include <linux/vfs/private/fs.h>

static inline int close_fd_get_file(filedesc_t *fdp, unsigned int fd, struct file **fpp)
{
    int error = 0;
    struct file *file;

    file = filedesc_file_get(fdp, fd, true);
    if (!file)
    {
        error = -EBADF;
    }

    *fpp = file;

    return error;
}

int vfs_close_api(filedesc_t *fdp, int fd)
{
    struct file *file;
    int err;

    err = close_fd_get_file(fdp, fd, &file);
    if (err == 0)
    {
        fput_close_sync(file);
    }

    return err;
}
