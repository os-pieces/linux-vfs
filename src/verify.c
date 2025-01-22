#include <linux/vfs/fs.h>

int rw_verify_area(int read_write, struct file *file, const loff_t *ppos, size_t count)
{
    return 0;
}
