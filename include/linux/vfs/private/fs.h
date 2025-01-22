#pragma once

#include <linux/vfs/fs.h>

#include <linux/vfs/private/file.h>
#include <linux/vfs/private/mount.h>
#include <linux/vfs/private/fs_context.h>
#include <linux/vfs/private/dcache.h>

extern int vfs_get_tree(struct fs_context *fc);
extern int parse_monolithic_mount_data(struct fs_context *, void *);
extern void put_fs_context(struct fs_context *fc);

extern void put_filesystem(struct file_system_type *fs);

