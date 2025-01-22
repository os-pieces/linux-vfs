#include <linux/vfs/fs.h>

int __generic_file_fsync(struct file *file, loff_t start, loff_t end,
				 int datasync)
{
	return 0;
}

loff_t generic_file_llseek(struct file *file, loff_t offset, int whence)
{
	return 0;
}

void generic_fillattr(struct mnt_idmap *idmap, u32 request_mask,
		      struct inode *inode, struct kstat *stat)
{
}
