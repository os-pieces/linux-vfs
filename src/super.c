#include <linux/vfs/fs.h>

/**
 *	alloc_super	-	create new superblock
 *	@type:	filesystem type superblock should belong to
 *	@flags: the mount flags
 *	@user_ns: User namespace for the super_block
 *
 *	Allocates and initializes a new &struct super_block.  alloc_super()
 *	returns a pointer new superblock or %NULL if allocation had failed.
 */
static struct super_block *alloc_super(struct file_system_type *type, int flags,
				       struct user_namespace *user_ns)
{
	struct super_block *s = kzalloc(sizeof(struct super_block),  GFP_USER);

	INIT_LIST_HEAD(&s->s_mounts);
	INIT_LIST_HEAD(&s->s_inodes);
	spin_lock_init(&s->s_inode_list_lock);

    pr_todo();

	return s;
}

void kill_litter_super(struct super_block *sb)
{
	pr_todo();
}

/**
 * vfs_get_tree - Get the mountable root
 * @fc: The superblock configuration context.
 *
 * The filesystem is invoked to get or create a superblock which can then later
 * be used for mounting.  The filesystem places a pointer to the root to be
 * used for mounting in @fc->root.
 */
int vfs_get_tree(struct fs_context *fc)
{
	struct super_block *sb;
	int error;

	if (fc->root)
		return -EBUSY;

	/* Get the mountable root in fc->root, with a ref on the root and a ref
	 * on the superblock.
	 */
	error = fc->ops->get_tree(fc);
	if (error < 0)
		return error;

	return 0;
}

struct super_block *sget_fc(struct fs_context *fc,
			    int (*test)(struct super_block *, struct fs_context *),
			    int (*set)(struct super_block *, struct fs_context *))
{
	struct super_block *s = NULL;
    struct user_namespace *user_ns = NULL;

	pr_todo();

	s = alloc_super(fc->fs_type, fc->sb_flags, user_ns);
	if (!s)
		return ERR_PTR(-ENOMEM);

    return s;
}

int get_anon_bdev(dev_t *p)
{
    pr_todo();
	return 0;
}

int set_anon_super(struct super_block *s, void *data)
{
	return get_anon_bdev(&s->s_dev);
}

int set_anon_super_fc(struct super_block *sb, struct fs_context *fc)
{
	return set_anon_super(sb, NULL);
}

void deactivate_locked_super(struct super_block *s)
{
    pr_todo();
}

static int vfs_get_super(struct fs_context *fc,
		int (*test)(struct super_block *, struct fs_context *),
		int (*fill_super)(struct super_block *sb,
				  struct fs_context *fc))
{
	struct super_block *sb;
	int err;

	sb = sget_fc(fc, test, set_anon_super_fc);
	if (IS_ERR(sb))
		return PTR_ERR(sb);

	if (!sb->s_root) {
		err = fill_super(sb, fc);
		if (err)
			goto error;

		sb->s_flags |= SB_ACTIVE;
	}

	fc->root = dget(sb->s_root);
	return 0;

error:
	deactivate_locked_super(sb);
	return err;
}

int get_tree_nodev(struct fs_context *fc,
		  int (*fill_super)(struct super_block *sb,
				    struct fs_context *fc))
{
	return vfs_get_super(fc, NULL, fill_super);
}

void kill_block_super(struct super_block *sb)
{
    
}


int sb_issue_discard(struct super_block *sb, sector_t block,
		sector_t nr_blocks, gfp_t gfp_mask, unsigned long flags)
{
	pr_todo();
	return 0;
}

void super_set_uuid(struct super_block *sb, const u8 *uuid, unsigned len)
{
	pr_todo();
}
