#pragma once

#include <linux/vfs/fs_context.h>

struct pseudo_fs_context
{
	const struct super_operations *ops;
	const struct export_operations *eops;
	const struct xattr_handler *const *xattr;
	const struct dentry_operations *dops;
	unsigned long magic;
};

int init_pseudo(struct fs_context *fc, unsigned long magic,
                struct pseudo_fs_context **pctx);
