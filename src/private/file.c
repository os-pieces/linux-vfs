#include <linux/vfs/fs.h>
#include <linux/vfs/private/file.h>

int __fget_light(filedesc_t *fdp, unsigned int fd,  struct fd *f, fmode_t mask)
{
    int error = 0;

    f->file = filedesc_file_get(fdp, fd, false);
    if (f->file)
    {

    }
    else
    {
        error = -EBADF;
    }

    return error;
}

int fdget_pos(filedesc_t *fdp, unsigned int fd, struct fd *f)
{
    int error;

    error = fdget(fdp, fd, f);
    if (!error)
    {
        pr_todo();
    }

    return error;
}

int fdget_raw(filedesc_t *fdp, unsigned int fd, struct fd *f)
{
    return __fget_light(fdp, fd, f, 0);
}

int fdget(filedesc_t *fdp, unsigned int fd, struct fd *f)
{
    return __fget_light(fdp, fd, f, FMODE_PATH);
}

void fdput(struct fd fd)
{
    pr_todo();
}

void fdput_pos(filedesc_t *fdp, struct fd f)
{

}
