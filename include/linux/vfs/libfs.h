#pragma once

#include "private/offset_ctx.h"

extern struct dentry *simple_lookup(struct inode *, struct dentry *, unsigned int flags);

/* dir */
ssize_t generic_read_dir(struct file *, char __user *, size_t, loff_t *);
bool dir_emit_dots(struct file *file, struct dir_context *ctx);
bool dir_emit_dot(struct file *file, struct dir_context *ctx);
bool dir_emit_dotdot(struct file *file, struct dir_context *ctx);
int simple_empty(struct dentry *dentry);


extern loff_t generic_file_llseek(struct file *file, loff_t offset, int whence);
int __generic_file_fsync(struct file *file, loff_t start, loff_t end,
				 int datasync);

void generic_fillattr(struct mnt_idmap *, u32, struct inode *, struct kstat *);


int simple_positive(const struct dentry *dentry);

void simple_offset_init(struct offset_ctx *octx);
int simple_offset_add(struct offset_ctx *octx, struct dentry *dentry);
