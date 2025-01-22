#include <linux/vfs/fs.h>
#include <linux/vfs/private/namei.h>

static int readlink_copy(char __user *buffer, int buflen, const char *link)
{
    int len = PTR_ERR(link);

    if (!IS_ERR(link))
    {
        len = strlen(link);
        if (len > (unsigned)buflen)
            len = buflen;
        if (copy_to_user(buffer, link, len))
            len = -EFAULT;
    }

    return len;
}

static int vfs_readlink(struct dentry *dentry, char *buffer, int buflen)
{
    struct inode *inode = d_inode(dentry);
    DEFINE_DELAYED_CALL(done);
    const char *link;
    int ret;

    link = inode->i_op->get_link(dentry, inode, &done);
    if (IS_ERR(link))
    {
        ret = PTR_ERR(link);
    }
    else
    {
        ret = readlink_copy(buffer, buflen, link);

        do_delayed_call(&done);
    }

    return ret;
}

int vfs_readlinkat_api(filedesc_t *fdp, int atfd,
                       const char *name, char *buf, int bufsz)
{
    int error;
    struct nameiargs ni;

    namei_init(&ni, fdp, name, atfd, LOOKUP_EMPTY);

    error = namei_lookup(&ni);
    if (error == 0)
    {
        error = vfs_readlink(ni.ni_ret_parent.dentry, buf, bufsz);
    }

    return error;
}

int vfs_readlink_api(filedesc_t *fdp, const char *pathname, char *buf, int bufsz)
{
    return vfs_readlinkat_api(fdp, AT_FDCWD, pathname, buf, bufsz);
}
