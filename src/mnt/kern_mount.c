#include <linux/vfs/private/fs.h>

struct vfsmount *vfs_kern_mount(struct file_system_type *type,
                                int flags, const char *name,
                                void *data)
{
    struct fs_context *fc;
    struct vfsmount *mnt;
    int ret = 0;

    if (!type)
        return ERR_PTR(-EINVAL);

    fc = fs_context_for_mount(type, flags);
    if (IS_ERR(fc))
        return ERR_CAST(fc);

    if (name)
        ret = vfs_parse_fs_string(fc, "source",
                                  name, strlen(name));
    if (!ret)
        ret = parse_monolithic_mount_data(fc, data);
    if (!ret)
        mnt = fc_mount(fc);
    else
        mnt = ERR_PTR(ret);

    put_fs_context(fc);

    return mnt;
}

struct vfsmount *kern_mount(struct file_system_type *type)
{
    struct vfsmount *mnt;

    mnt = vfs_kern_mount(type, SB_KERNMOUNT, type->name, NULL);
    if (!IS_ERR(mnt))
    {
        /*
         * it is a longterm mount, don't release mnt until
         * we unmount before file sys is unregistered
         */
        real_mount(mnt)->mnt_ns = MNT_NS_INTERNAL;
    }

    return mnt;
}
