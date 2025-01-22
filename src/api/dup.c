#include <linux/vfs/fs.h>

static int ksys_dup3(filedesc_t *fdp, unsigned int oldfd, unsigned int newfd, int flags)
{
    int error;

    if ((flags & ~O_CLOEXEC) != 0)
    {
        error = -EINVAL;
    }
    else if (unlikely(oldfd == newfd))
    {
        error = -EINVAL;
    }

    return error;
}
