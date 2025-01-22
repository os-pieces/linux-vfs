#include <linux/vfs/fs.h>
#include <linux/vfs/private/namei.h>
#include <linux/vfs/private/namespace.h>

static int do_mount(filedesc_t *fdp, const char *dev_name, const char *dir_name,
                    const char *type_page, unsigned long flags, void *data_page)
{
    struct nameiargs ni;
    int error;

    namei_init(&ni, fdp, dir_name, AT_FDCWD, LOOKUP_FOLLOW);

    error = namei_lookup(&ni);
    if (error == 0)
    {
        error = path_mount(fdp, dev_name, &ni.ni_ret_parent, type_page, flags, data_page);

        path_put(&ni.ni_ret_parent);
    }

    return error;
}

int vfs_mount_api(filedesc_t *fdp, char *dev, char *dir, char *type,
                  unsigned long flags, void *data)
{
    int ret;

    ret = do_mount(fdp, dev, dir, type, flags, data);

    return ret;
}
