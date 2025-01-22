#include <linux/vfs/fs.h>
#include <linux/vfs/private/security.h>

#include <linux/vfs/private/namei.h>

static int __rename(struct nameiargs *oldni, struct nameiargs *newni,
                    struct qstr *old_last, struct qstr *new_last)
{
    int error;
    struct dentry *old_dentry, *new_dentry;
    struct renamedata rd;

    old_dentry = lookup_one_qstr_excl(old_last, path_dentry_var(oldni->ni_ret_parent),
                                      oldni->ni_lookflags);

    if (IS_ERR(old_dentry))
    {
        error = PTR_ERR(old_dentry);
    }
    else
    {
        if (d_is_negative(old_dentry))
        {
            error = -ENOENT;
        }
        else
        {
            new_dentry = lookup_one_qstr_excl(new_last, path_dentry_var(newni->ni_ret_parent),
                                              oldni->ni_lookflags | newni->ni_lookflags);

            if (IS_ERR(new_dentry))
            {
                error = PTR_ERR(new_dentry);
            }
            else
            {
                rd.old_dir = path_dentry_var(oldni->ni_ret_parent)->d_inode;
                rd.old_dentry = old_dentry;
                rd.old_mnt_idmap = mnt_idmap(path_mnt_var(oldni->ni_ret_parent));
                rd.new_dir = path_dentry_var(newni->ni_ret_parent)->d_inode;
                rd.new_dentry = new_dentry;
                rd.new_mnt_idmap = mnt_idmap(path_mnt_var(newni->ni_ret_parent));

                dput(new_dentry);
            }
        }

        dput(old_dentry);
    }

    return error;
}

static int do_renameat2(filedesc_t *fdp, int olddfd, const char *from, int newdfd,
                        const char *to, unsigned int flags)
{
    int error;
    struct qstr old_last, new_last;
    int old_type, new_type;
    unsigned int lookup_flags = 0, target_flags = LOOKUP_RENAME_TARGET;
    struct nameiargs oldni, newni;

    namei_init(&oldni, fdp, from, olddfd, lookup_flags);

    error = vfs_path_parent_lookup(&oldni, &old_last, &old_type, NULL);
    if (error == 0)
    {
        namei_init(&newni, fdp, to, newdfd, target_flags);

        error = vfs_path_parent_lookup(&newni, &new_last, &new_type, NULL);
        if (error == 0)
        {
            if (path_mnt_var(oldni.ni_ret_parent) != path_mnt_var(newni.ni_ret_parent))
            {
                error = -EXDEV;
            }
            else if (old_type != LAST_NORM || new_type != LAST_NORM)
            {
                error = -EBUSY;
            }
            else
            {
                error = mnt_want_write(path_mnt_var(oldni.ni_ret_parent));
                if (error == 0)
                {
                    error = __rename(&oldni, &newni, &old_last, &new_last);

                    mnt_drop_write(path_mnt_var(oldni.ni_ret_parent));
                }
            }
        }
    }

    return error;
}
