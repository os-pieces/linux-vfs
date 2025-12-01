/* SPDX-License-Identifier: GPL-2.0-only */

#include <linux/vfs/private/fs.h>

void mntput(struct vfsmount *mnt)
{
    pr_todo();
}

struct vfsmount *mntget(struct vfsmount *mnt)
{
    pr_todo();
    return mnt;
}

int mnt_want_write(struct vfsmount *m)
{
    int ret = 0;

    pr_todo();

    return ret;
}

/**
 * mnt_want_write_file - get write access to a file's mount
 * @file: the file who's mount on which to take a write
 *
 * This is like mnt_want_write, but if the file is already open for writing it
 * skips incrementing mnt_writers (since the open file already has a reference)
 * and instead only does the freeze protection and the check for emergency r/o
 * remounts.  This must be paired with mnt_drop_write_file.
 */
int mnt_want_write_file(struct file *file)
{
    int ret = 0;

    pr_todo();

    return ret;
}

void mnt_drop_write(struct vfsmount *mnt)
{
    pr_todo();
}
