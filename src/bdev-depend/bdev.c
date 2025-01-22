#include <linux/vfs/fs.h>
#include <linux/vfs/private/namei.h>

/**
 * __lookup_bdev() - Look up a struct block_device by name.
 * @pathname: Name of the block device in the filesystem.
 * @dev: Pointer to the block device's dev_t, if found.
 *
 * Lookup the block device's dev_t at @pathname in the current
 * namespace if possible and return it in @dev.
 *
 * Context: May sleep.
 * Return: 0 if succeeded, negative errno otherwise.
 */
int __lookup_bdev(filedesc_t *fdp, const char *pathname, dev_t *dev)
{
    struct inode *inode;
    struct path path;
    int error;

    if (!pathname || !*pathname)
        return -EINVAL;

    error = kern_path(fdp, pathname, LOOKUP_FOLLOW, &path);
    if (error)
        return error;

    inode = d_backing_inode(path.dentry);
    error = -ENOTBLK;
    if (!S_ISBLK(inode->i_mode))
        goto out_path_put;
    error = -EACCES;
    if (!may_open_dev(&path))
        goto out_path_put;

    *dev = inode->i_rdev;
    error = 0;
out_path_put:
    path_put(&path);
    return error;
}

int sb_min_blocksize(struct super_block *sb, int size)
{
    return 0;
}

int sb_set_blocksize(struct super_block *sb, int size)
{
    return 0;
}
