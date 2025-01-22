#include <linux/vfs/fs.h>

#include <linux/vfs/private/namei.h>

static int vfs_mknod(struct mnt_idmap *idmap, struct inode *dir,
                     struct dentry *dentry, umode_t mode, dev_t dev)
{
    int error;

    error = dir->i_op->mknod(idmap, dir, dentry, mode, dev);

    return error;
}

int vfs_mknodat_api(filedesc_t *fdp, int atfd, 
    const char *name, umode_t mode, unsigned dev)
{
    int error;
    struct nameiargs ni;

    namei_init(&ni, fdp, name, atfd, 0);

    error = namei_create(&ni);
    if (error == 0)
    {
        struct mnt_idmap *idmap;

        idmap = mnt_idmap(ni.ni_ret_parent.mnt);

        error = vfs_mknod(idmap, ni.ni_ret_parent.dentry->d_inode,
                          ni.ni_ret_dentry, mode, dev);
    }

    return error;
}
