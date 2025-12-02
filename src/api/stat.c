#include <linux/vfs/private/fs.h>
#include <linux/vfs/private/namei.h>

static int vfs_statx(filedesc_t *fdp, int dfd, const char *name, int flags,
                     struct kstat *stat, u32 request_mask)
{
    int error;
    unsigned int lookup_flags = 0;
    struct nameiargs ni;

    namei_init(&ni, fdp, name, dfd, lookup_flags);

    error = namei_lookup(&ni);
    if (error == 0)
    {

    }

    return error;
}
