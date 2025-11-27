#include <linux/vfs/fs.h>
#include <linux/vfs/pseudo_fs.h>

static int pseudo_fs_fill_super(struct super_block *s, struct fs_context *fc)
{
    pr_todo();
    return 0;
}

static int pseudo_fs_get_tree(struct fs_context *fc)
{
    return get_tree_nodev(fc, pseudo_fs_fill_super);
}

static void pseudo_fs_free(struct fs_context *fc)
{
    kfree(fc->fs_private);
}

static const struct fs_context_operations pseudo_fs_context_ops = {
    .free = pseudo_fs_free,
    .get_tree = pseudo_fs_get_tree,
};

/*
 * Common helper for pseudo-filesystems (sockfs, pipefs, bdev - stuff that
 * will never be mountable)
 */
struct pseudo_fs_context *init_pseudo(struct fs_context *fc,
                                      unsigned long magic)
{
    struct pseudo_fs_context *ctx;

    ctx = kzalloc(sizeof(struct pseudo_fs_context), GFP_KERNEL);
    if (likely(ctx))
    {
        ctx->magic = magic;
        fc->fs_private = ctx;
        fc->ops = &pseudo_fs_context_ops;
        fc->sb_flags |= SB_NOUSER;
    }

    return ctx;
}
