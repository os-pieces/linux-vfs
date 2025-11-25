#include <linux/vfs/fs.h>
#include <linux/vfs/private/filename.h>

#include <linux/vfs/private/namei.h>

static int vfs_symlink(struct mnt_idmap *idmap, struct inode *dir,
                       struct dentry *dentry, const char *oldname)
{
    int error;

    if (dir->i_op->symlink)
    {
        error = dir->i_op->symlink(idmap, dir, dentry, oldname);
    }
    else
    {
        error = -EPERM;
    }

    return error;
}

static int do_symlinkat(filedesc_t *fdp, const char *oldname,
                        int atfd, const char *newname)
{
    int error;
    struct nameiargs ni;
    struct filename *from;

    error = getname(oldname, &from);
    if (error == 0)
    {
        namei_init(&ni, fdp, newname, atfd, 0);

        error = namei_create(&ni);
        if (error == 0)
        {
            struct mnt_idmap *idmap = mnt_idmap(ni.ni_ret_parent.mnt);

            error = vfs_symlink(idmap, ni.ni_ret_parent.dentry->d_inode,
                                ni.ni_ret_dentry, from->name);
        }

        putname(from);
    }

    return error;
}

int vfs_symlinkat_api(filedesc_t *fdp, const char *oldname,
                      int atfd, const char *newname)
{
    return do_symlinkat(fdp, oldname, atfd, newname);
}
