#include <linux/vfs/fs.h>
#include <linux/vfs/private/namei.h>

static int vfs_mkdir(struct mnt_idmap *idmap, struct inode *dir,
			  struct dentry *dentry, umode_t mode)
{
	int error;

	error = dir->i_op->mkdir(idmap, dir, dentry, mode);

	return error;
}

int vfs_mkdirat_api(filedesc_t *fdp, int atfd, const char *name, umode_t mode)
{
	int error;
	struct nameiargs ni;

	namei_init(&ni, fdp, name, atfd, LOOKUP_DIRECTORY);

	error = namei_create(&ni);
	if (error == 0)
	{
		error = vfs_mkdir(mnt_idmap(ni.ni_ret_parent.mnt),
		                  ni.ni_ret_parent.dentry->d_inode,
						  ni.ni_ret_dentry, mode);
	}

	return error;
}

int vfs_mkdir_api(filedesc_t *fdp, const char *name, umode_t mode)
{
    return vfs_mkdirat_api(fdp, AT_FDCWD, name, mode);
}
