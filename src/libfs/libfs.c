#include <linux/vfs/fs.h>
#include <linux/vfs/private/dcache.h>

int simple_positive(const struct dentry *dentry)
{
	return d_really_is_positive(dentry) && !d_unhashed(dentry);
}

int simple_empty(struct dentry *dentry)
{
	return 0;
}
