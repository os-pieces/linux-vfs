#include <linux/vfs/fs.h>
#include <linux/vfs/private/file.h>

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
    int retval;
    struct file *file;

    retval = close_fd_get_file(fdp, fd, &file);
    if (retval == 0)
    {
        __fput_sync(file);
    }

    return retval;
}
