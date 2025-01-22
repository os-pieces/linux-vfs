#include <linux/vfs/fs.h>
#include <linux/vfs/private/security.h>
#include <linux/vfs/private/namei.h>
#include <linux/vfs/private/mount.h>

static int __vfs_unlink(struct mnt_idmap *idmap, struct inode *dir,
						struct dentry *dentry, struct inode **delegated_inode)
{
	struct inode *target = dentry->d_inode;
	int error = may_delete(idmap, dir, dentry, 0);

	if (error)
		return error;

	if (!dir->i_op->unlink)
		return -EPERM;

	inode_lock(target);
	if (IS_SWAPFILE(target))
		error = -EPERM;
	else if (is_local_mountpoint(dentry))
		error = -EBUSY;
	else
	{
		error = security_inode_unlink(dir, dentry);
		if (!error)
		{
			error = try_break_deleg(target, delegated_inode);
			if (error)
				goto out;
			error = dir->i_op->unlink(dir, dentry);
			if (!error)
			{
				dont_mount(dentry);
				detach_mounts(dentry);
			}
		}
	}

out:
	inode_unlock(target);

	return error;
}

static int do_unlinkat(filedesc_t *fdp, int dfd, const char *name)
{
	int error;
	struct nameiargs ni;
	struct qstr last;
	struct inode *inode = NULL;
	int type;

	namei_init(&ni, fdp, name, dfd, 0);

	error = vfs_path_parent_lookup(&ni, &last, &type, NULL);
	if (error == 0)
	{
		if (type != LAST_NORM)
		{
			error = -EISDIR;
		}
		else
		{
			error = mnt_want_write(path_mnt(&ni.ni_ret_parent));
			if (error == 0)
			{
				struct dentry *dentry;

				dentry = lookup_one_qstr_excl(&last, ni.ni_ret_dentry, 0);
				if (!IS_ERR(dentry))
				{
					/* Why not before? Because we want correct error value */
					if (last.name[last.len] || d_is_negative(dentry))
					{
						error = -ENOENT;
					}
					else
					{
						inode = dentry->d_inode;
						ihold(inode);
					}

					dput(dentry);
				}
				else
				{
					error = PTR_ERR(dentry);
				}
			}
		}

		path_put(&ni.ni_ret_parent);
	}

	return error;
}

int vfs_unlinkat_api(filedesc_t *fdp, int dirfd, const char *pathname, int flags)
{

	return do_unlinkat(fdp, dirfd, pathname);
}
