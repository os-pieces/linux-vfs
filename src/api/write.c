#include <linux/vfs/private/fs.h>

static inline int fmode_check(struct file *file)
{
    int ret = 0;

    if (!(file->f_mode & FMODE_WRITE))
    {
        ret = -EBADF;
    }
    else if (!(file->f_mode & FMODE_CAN_WRITE))
    {
        ret = -EINVAL;
    }

    return ret;
}

static ssize_t new_sync_write(struct file *filp, const char *buf, size_t len, loff_t *ppos)
{
    struct kiocb kiocb;
    struct iov_iter iter;
    ssize_t ret;

    init_sync_kiocb(&kiocb, filp);
    kiocb.ki_pos = (ppos ? *ppos : 0);
    iov_iter_ubuf(&iter, ITER_SOURCE, (void __user *)buf, len);

	ret = filp->f_op->write_iter(&kiocb, &iter);

	if (ret > 0 && ppos)
		*ppos = kiocb.ki_pos;

    return ret;
}

static inline ssize_t call_write(struct file *file, const char __user *buf, size_t count, loff_t *pos)
{
    ssize_t ret;

    if (file->f_op->write)
        ret = file->f_op->write(file, buf, count, pos);
    else if (file->f_op->write_iter)
        ret = new_sync_write(file, buf, count, pos);
    else
        ret = -EINVAL;

    return ret;
}

static ssize_t do_write(struct file *file, const char *buf, size_t count, loff_t *pos)
{
    ssize_t ret;

    ret = fmode_check(file);
    if (ret == 0)
    {
        ret = rw_verify_area(WRITE, file, pos, count);
        if (ret == 0)
        {
            ret = call_write(file, buf, count, pos);
        }
    }

    return ret;
}

ssize_t vfs_write_api(filedesc_t *fdp, unsigned int fd, const void *buf, size_t size)
{
    struct fd f;
    ssize_t ret;

    ret = fdget_pos(fdp, fd, &f);
    if (ret == 0)
    {
        loff_t pos;

        pos = f.file->f_pos;
        ret = do_write(f.file, buf, size, &pos);

        if (ret >= 0)
        {
            f.file->f_pos = pos;
        }

        fdput_pos(fdp, f);
    }

    return ret;
}
