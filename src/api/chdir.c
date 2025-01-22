#include <linux/vfs/fs.h>
#include <linux/vfs/private/namei.h>
#include <linux/vfs/private/path.h>

int vfs_chdir_api(filedesc_t *fdp, const char *name)
{
    struct nameiargs ni;
    int error;

    namei_init(&ni, fdp, name, AT_FDCWD, LOOKUP_FOLLOW | LOOKUP_DIRECTORY);

    error = namei_lookup(&ni);
    if (error == 0)
    {
        set_fs_pwd(fdp, &ni.ni_ret_parent);

        path_put(&ni.ni_ret_parent);
    }

    return error;
}
