#include <linux/vfs/fs.h>
#include <linux/vfs/private/dcache.h>

extern struct dcache _dcache;
extern int mnt_init(void);

int vfs_init(void)
{
    int ret;

    ret = dcache_init(&_dcache, 32);
    ret = mnt_init();

    return 0;
}
