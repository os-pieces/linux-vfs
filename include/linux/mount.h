#pragma once

#include <linux/vfs/mount.h>

extern struct vfsmount *vfs_kern_mount(struct file_system_type *type,
                                       int flags, const char *name,
                                       void *data);
