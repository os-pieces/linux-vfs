#include <linux/vfs/fs.h>

extern int __lookup_bdev(filedesc_t *fdp, const char *pathname, dev_t *dev);

/**
 * get_tree_bdev - Get a superblock based on a single block device
 * @fc: The filesystem context holding the parameters
 * @fill_super: Helper to initialise a new superblock
 */
int get_tree_bdev(struct fs_context *fc,
				  int (*fill_super)(struct super_block *,
									struct fs_context *))
{
	struct super_block *s;
	int error = 0;
	dev_t dev;

	pr_todo();
	if (!fc->source)
	{
		return -EINVAL;
	}

	error = __lookup_bdev(fc->fc_fdp, fc->source, &dev);
	if (error)
	{
		return error;
	}

	return 0;
}
