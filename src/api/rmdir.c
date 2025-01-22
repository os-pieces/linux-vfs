#include <linux/vfs/fs.h>
#include <linux/vfs/private/security.h>
#include <linux/vfs/fsnotify.h>
#include <linux/vfs/private/namei.h>
#include <linux/vfs/private/mount.h>

static int vfs_rmdir(struct mnt_idmap *idmap, struct inode *dir, struct dentry *dentry)
{
    int error;

    error = may_delete(idmap, dir, dentry, 1);
    if (error == 0)
    {
        if (!dir->i_op->rmdir)
        {
            error = -EPERM;
        }
        else
        {
            dget(dentry);
            inode_lock(dentry->d_inode);

            if (is_local_mountpoint(dentry) ||
                (dentry->d_inode->i_flags & S_KERNEL_FILE))
            {
                error = -EBUSY;
            }
            else
            {
                error = security_inode_rmdir(dir, dentry);
                if (error == 0)
                {
                    error = dir->i_op->rmdir(dir, dentry);
                    if (error == 0)
                    {
                        shrink_dcache_parent(dentry);
                        dentry->d_inode->i_flags |= S_DEAD;
                        dont_mount(dentry);
                        detach_mounts(dentry);
                    }
                }
            }

            inode_unlock(dentry->d_inode);
            dput(dentry);
        }
    }

    if (!error)
        d_delete_notify(dir, dentry);

    return error;
}

static inline int check_type(int type)
{
    int error = 0;

    switch (type)
    {
    case LAST_DOTDOT:
        error = -ENOTEMPTY;
        break;
    case LAST_DOT:
        error = -EINVAL;
        break;
    case LAST_ROOT:
        error = -EBUSY;
        break;
    }

    return error;
}

static int parent_rmdir(struct path *parent, struct qstr *last)
{
    int error;
    struct dentry *dentry;

    error = mnt_want_write(path_mnt(parent));
    if (error == 0)
    {
        inode_lock_nested(path_dentry(parent)->d_inode, I_MUTEX_PARENT);

        dentry = lookup_one_qstr_excl(last, path_dentry(parent), 0);
        if (IS_ERR(dentry))
        {
            error = PTR_ERR(dentry);
        }
        else
        {
            if (!dentry->d_inode)
            {
                error = -ENOENT;
            }
            else
            {
                error = security_path_rmdir(parent, dentry);
                if (error == 0)
                {
                    error = vfs_rmdir(mnt_idmap(path_mnt(parent)), path_dentry(parent)->d_inode, dentry);
                }
            }

            dput(dentry);
        }

        inode_unlock(path_dentry(parent)->d_inode);
        mnt_drop_write(path_mnt(parent));
    }

    return error;
}

int do_rmdir(filedesc_t *fdp, int dfd, const char *name)
{
    int error;
    int type;
    struct qstr last;
    struct nameiargs ni;

    namei_init(&ni, fdp, name, dfd, 0);

    error = vfs_path_parent_lookup(&ni, &last, &type, NULL);
    if (error == 0)
    {
        error = check_type(type);
        if (error == 0)
        {
            error = parent_rmdir(&ni.ni_ret_parent, &last);
        }
    }

    path_put(&ni.ni_ret_parent);

    return error;
}

int vfs_rmdir_api(filedesc_t *fdp, int dfd, const char *pathname)
{
    return do_rmdir(fdp, dfd, pathname);
}
