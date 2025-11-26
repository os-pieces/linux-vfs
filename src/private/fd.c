#include <linux/vfs/fs.h>
#include <linux/vfs/private/file.h>

int __fget_light(filedesc_t *fdp, unsigned int fd,  struct fd *f, fmode_t mask)
{
    int error = 0;

    spin_lock(&fdp->lock);
    f->file = filedesc_file_get(fdp, fd, false);
    if (f->file)
    {
        get_file(f->file);
    }
    else
    {
        error = -EBADF;
    }
    spin_unlock(&fdp->lock);

    return error;
}

int fdget_pos(filedesc_t *fdp, unsigned int fd, struct fd *f)
{
    int error;

    error = fdget(fdp, fd, f);
    if (!error)
    {
        // TODO
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
