#include <linux/vfs/fs.h>

#define i_callback(...)

int inode_init_always(struct super_block *sb, struct inode *inode)
{
    inode->i_sb = sb;
    init_rwsem(&inode->i_rwsem);

    pr_todo();

    return 0;
}

static struct inode *alloc_inode(struct super_block *sb)
{
    const struct super_operations *ops = sb->s_op;
    struct inode *inode = NULL;

    if (ops->alloc_inode)
        inode = ops->alloc_inode(sb);

    if (!inode)
        return NULL;

    if (unlikely(inode_init_always(sb, inode)))
    {
        if (ops->destroy_inode)
        {
            ops->destroy_inode(inode);
            if (!ops->free_inode)
                return NULL;
        }
        inode->free_inode = ops->free_inode;
        i_callback(&inode->i_rcu);
        return NULL;
    }

    return inode;
}

struct inode *new_inode_pseudo(struct super_block *sb)
{
    struct inode *inode = alloc_inode(sb);

    if (inode)
    {
        spin_lock(&inode->i_lock);
        inode->i_state = 0;
        spin_unlock(&inode->i_lock);
    }
    return inode;
}

struct inode *new_inode(struct super_block *sb)
{
    struct inode *inode;

    inode = new_inode_pseudo(sb);
    if (inode)
        inode_sb_list_add(inode);

    return inode;
}

/**
 * inode_sb_list_add - add inode to the superblock list of inodes
 * @inode: inode to add
 */
void inode_sb_list_add(struct inode *inode)
{
	struct super_block *sb = inode->i_sb;

	spin_lock(&sb->s_inode_list_lock);
	list_add(&inode->i_sb_list, &sb->s_inodes);
	spin_unlock(&sb->s_inode_list_lock);
}

void inc_nlink(struct inode *inode)
{
    inode->__i_nlink++;
}

void iput(struct inode *inode)
{
    pr_todo();
}

void inode_lock_shared(struct inode *inode)
{
    down_read(&inode->i_rwsem);
}

void inode_unlock_shared(struct inode *inode)
{
    up_read(&inode->i_rwsem);
}

void inode_lock_nested(struct inode *inode, unsigned subclass)
{
    down_write_nested(&inode->i_rwsem, subclass);
}

void ihold(struct inode *inode)
{
    pr_todo();
    WARN_ON(atomic_inc_return(&inode->i_count) < 2);
}

void inode_lock(struct inode *inode)
{
	down_write(&inode->i_rwsem);
}

void inode_unlock(struct inode *inode)
{
	up_write(&inode->i_rwsem);
}

void touch_atime(const struct path *path)
{
	pr_todo();
}

bool atime_needs_update(const struct path *path, struct inode *inode)
{
	pr_todo();
	return true;
}

void mark_inode_dirty(struct inode *inode)
{
	pr_todo();
}

void inode_inc_iversion(struct inode *inode)
{
	pr_todo();
}

u64 inode_query_iversion(struct inode *inode)
{
	pr_todo();
	return 0;
}

void drop_nlink(struct inode *inode)
{
	pr_todo();
}

struct timespec64 current_time(struct inode *inode)
{
    struct timespec64 now;

    return now;
}

/**
 * clear_nlink - directly zero an inode's link count
 * @inode: inode
 *
 * This is a low-level filesystem helper to replace any
 * direct filesystem manipulation of i_nlink.  See
 * drop_nlink() for why we care about i_nlink hitting zero.
 */
void clear_nlink(struct inode *inode)
{
    pr_todo();
}

void set_nlink(struct inode *inode, unsigned int nlink)
{
    pr_todo();
}

int inode_needs_sync(struct inode *inode)
{
	if (IS_SYNC(inode))
		return 1;
	if (S_ISDIR(inode->i_mode) && IS_DIRSYNC(inode))
		return 1;
	return 0;
}

ino_t iunique(struct super_block *sb, ino_t max_reserved)
{
    return 0;
}

struct inode *igrab(struct inode *inode)
{
    return inode;
}

void insert_inode_hash(struct inode *inode)
{
    pr_todo();
}

void inode_init_once(struct inode *inode)
{
    pr_todo();
}

void free_inode_nonrcu(struct inode *inode)
{
    pr_todo();
}
