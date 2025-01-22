#include <linux/vfs/private/fs.h>

static inline int fmode_check(struct file *file)
{
    int ret = 0;

    if (!(file->f_mode & FMODE_READ))
    {
        ret = -EBADF;
    }
    else if (!(file->f_mode & FMODE_CAN_READ))
    {
        ret = -EINVAL;
    }

    return ret;
}

static inline ssize_t call_readops(struct file *file, char __user *buf, size_t count, loff_t *pos)
{
    ssize_t ret;

    if (file->f_op->read)
        ret = file->f_op->read(file, buf, count, pos);
    else
        ret = -EINVAL;

    return ret;
}

static ssize_t do_read(struct file *file, char *buf, size_t count, loff_t *pos)
{
    ssize_t ret;

    ret = fmode_check(file);
    if (ret == 0)
    {
        ret = rw_verify_area(READ, file, pos, count);
        if (ret == 0)
        {
            ret = call_readops(file, buf, count, pos);
        }
    }

    return ret;
}

ssize_t vfs_read_api(filedesc_t *fdp, unsigned int fd, char *buf, size_t size)
{
    struct fd f;
    ssize_t ret;

    ret = fdget_pos(fdp, fd, &f);
    if (ret == 0)
    {
        loff_t pos;
 
        pos = f.file->f_pos;
        ret = do_read(f.file, buf, size, &pos);

        if (ret >= 0)
        {
            f.file->f_pos = pos;
        }

        fdput_pos(fdp, f);
    }

    return ret;
}
