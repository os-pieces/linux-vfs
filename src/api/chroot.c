#include <linux/vfs/fs.h>
#include <linux/vfs/private/namei.h>
#include <linux/vfs/private/path.h>

int vfs_chroot_api(filedesc_t *fdp, const char *name)
{
    int error;
    unsigned int lookup_flags = LOOKUP_FOLLOW | LOOKUP_DIRECTORY;
    struct nameiargs ni;

    namei_init(&ni, fdp, name, AT_FDCWD, lookup_flags);

    error = namei_lookup(&ni);
    if (error == 0)
    {
        set_fs_root(fdp, &ni.ni_ret_parent);
    }

    return error;
}
