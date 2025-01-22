#include <linux/sched/signal.h>

#include <linux/vfs/private/fs.h>
#include <linux/vfs/private/security.h>
#include <linux/vfs/fsnotify.h>
#include <linux/vfs/dirent.h>

/*
 * New, all-improved, singing, dancing, iBCS2-compliant getdents()
 * interface.
 */
struct linux_dirent
{
    unsigned long d_ino;
    unsigned long d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char d_name[];
};

struct getdents_callback
{
    struct dir_context ctx;
    struct linux_dirent *current_dir;
    int prev_reclen;
    int count;
    int error;
};

/*
 * Note the "unsafe_put_user()" semantics: we goto a
 * label for errors.
 */
#define unsafe_copy_dirent_name(_dst, _src, _len, label) do {	\
	char __user *dst = (_dst);				\
	const char *src = (_src);				\
	size_t len = (_len);					\
	unsafe_put_user(0, dst+len, label);			\
	unsafe_copy_to_user(dst, src, len, label);		\
} while (0)

int iterate_dir(struct file *file, struct dir_context *ctx)
{
    struct inode *inode = file_inode(file);
    int res = -ENOTDIR;

    if (!file->f_op->iterate_shared)
        goto out;

    res = security_file_permission(file, MAY_READ);
    if (res)
        goto out;

    res = down_read_killable(&inode->i_rwsem);
    if (res)
        goto out;

    res = -ENOENT;
    if (!IS_DEADDIR(inode))
    {
        ctx->pos = file->f_pos;
        res = file->f_op->iterate_shared(file, ctx);
        file->f_pos = ctx->pos;
        fsnotify_access(file);
        file_accessed(file);
    }
    inode_unlock_shared(inode);
out:
    return res;
}

static int verify_dirent_name(const char *name, int len)
{
    int ret = 0;

    if (len <= 0 || len >= PATH_MAX)
        ret = -EIO;
    if (memchr(name, '/', len))
        ret = -EIO;

    return ret;
}

static bool filldir(struct dir_context *ctx, const char *name, int namlen,
                    loff_t offset, u64 ino, unsigned int d_type)
{
    struct linux_dirent __user *dirent, *prev;
    struct getdents_callback *buf;
    unsigned long d_ino;
    int reclen;
    int prev_reclen;

    buf = container_of(ctx, struct getdents_callback, ctx);
    reclen = ALIGN(offsetof(struct linux_dirent, d_name) + namlen + 2, sizeof(long));

    buf->error = verify_dirent_name(name, namlen);
    if (unlikely(buf->error))
        return false;
    buf->error = -EINVAL; /* only used if we fail.. */
    if (reclen > buf->count)
        return false;
    d_ino = ino;
    if (sizeof(d_ino) < sizeof(ino) && d_ino != ino)
    {
        buf->error = -EOVERFLOW;
        return false;
    }
    prev_reclen = buf->prev_reclen;
    if (prev_reclen && signal_pending_current())
        return false;
    dirent = buf->current_dir;
    prev = (void __user *)dirent - prev_reclen;
    if (!user_write_access_begin(prev, reclen + prev_reclen))
        goto efault;

    /* This might be 'dirent->d_off', but if so it will get overwritten */
    unsafe_put_user(offset, &prev->d_off, efault_end);
    unsafe_put_user(d_ino, &dirent->d_ino, efault_end);
    unsafe_put_user(reclen, &dirent->d_reclen, efault_end);
    unsafe_put_user(d_type, (char __user *)&dirent->d_type, efault_end);
    unsafe_copy_dirent_name(dirent->d_name, name, namlen, efault_end);
    user_write_access_end();

    buf->current_dir = (void __user *)dirent + reclen;
    buf->prev_reclen = reclen;
    buf->count -= reclen;
    return true;

efault_end:
    user_write_access_end();
efault:
    buf->error = -EFAULT;
    return false;
}

static int __getdents(filedesc_t *fdp, unsigned int fd, struct linux_dirent *dirent, unsigned int count)
{
    struct fd f;
    struct getdents_callback buf = {
        .ctx.actor = filldir,
        .count = count,
        .current_dir = dirent,
    };
    int error;

    error = fdget_pos(fdp, fd, &f);
    if (error == 0)
    {
        error = iterate_dir(f.file, &buf.ctx);
	    if (error >= 0)
		    error = buf.error;

        if (buf.prev_reclen)
        {
            struct linux_dirent *lastdirent;

            lastdirent = (void *)buf.current_dir - buf.prev_reclen;

            if (put_user(buf.ctx.pos, &lastdirent->d_off))
                error = -EFAULT;
            else
                error = count - buf.count;
        }

        fdput_pos(fdp, f);
    }

    return error;
}

int vfs_getdents_api(filedesc_t *fdp, int fd, struct dirent *dirent, unsigned int count)
{
    return __getdents(fdp, fd, (struct linux_dirent *)dirent, count);
}
