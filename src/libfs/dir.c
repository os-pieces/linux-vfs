#include <linux/vfs/fs.h>

ssize_t generic_read_dir(struct file *filp, char __user *buf, size_t siz, loff_t *ppos)
{
	return -EISDIR;
}

bool dir_emit_dots(struct file *file, struct dir_context *ctx)
{
	if (ctx->pos == 0)
	{
		if (!dir_emit_dot(file, ctx))
			return false;
		ctx->pos = 1;
	}

	if (ctx->pos == 1)
	{
		if (!dir_emit_dotdot(file, ctx))
			return false;
		ctx->pos = 2;
	}

	return true;
}

bool dir_emit_dot(struct file *file, struct dir_context *ctx)
{
	struct dentry *dir = path_dentry(&file->f_path);

	return ctx->actor(ctx, ".", 1, ctx->pos,
					  dir->d_inode->i_ino, DT_DIR);
}

bool dir_emit_dotdot(struct file *file, struct dir_context *ctx)
{
	struct dentry *dir = path_dentry(&file->f_path);

	return ctx->actor(ctx, "..", 2, ctx->pos,
					  parent_ino(dir), DT_DIR);
}
