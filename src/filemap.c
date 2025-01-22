#include <linux/vfs/fs.h>

int filemap_fdatawrite_range(struct address_space *mapping,
		loff_t start, loff_t end)
{
	return 0;
}

int filemap_fdatawait_range(struct address_space *mapping, loff_t start_byte,
			    loff_t end_byte)
{
	return 0;
}
