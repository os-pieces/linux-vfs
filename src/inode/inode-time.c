#include <linux/vfs/fs.h>

bool atime_needs_update(const struct path *path, struct inode *inode)
{
	pr_todo();
	return true;
}

void touch_atime(const struct path *path)
{
	pr_todo();
}

struct timespec64 current_time(struct inode *inode)
{
    struct timespec64 now;

    return now;
}
