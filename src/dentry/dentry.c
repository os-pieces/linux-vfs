#include <linux/vfs/fs.h>


void dput(struct dentry *d)
{
    pr_todo();
}

void d_invalidate(struct dentry *d)
{
    pr_todo();
}

void d_lookup_done(struct dentry *dentry)
{
    pr_todo();
}

/* This must be called with d_lock held */
static inline void __dget_dlock(struct dentry *dentry)
{
    pr_todo();
}

struct dentry *dget(struct dentry *dentry)
{
    pr_todo();

    return dentry;
}

void d_set_d_op(struct dentry *dentry, const struct dentry_operations *op)
{
    pr_todo();
}

bool d_is_miss(const struct dentry *dentry)
{
    return __d_entry_type(dentry) == DCACHE_MISS_TYPE;
}

bool d_is_negative(const struct dentry *dentry)
{
    return d_is_miss(dentry);
}

bool d_is_positive(const struct dentry *dentry)
{
    return !d_is_negative(dentry);
}

struct dentry *d_splice_alias(struct inode *inode, struct dentry *dentry)
{
    pr_todo();
    return NULL;
}

struct dentry *dget_parent(struct dentry *dentry)
{
    struct dentry *ret;

    ret = dentry->d_parent;

    return ret;
}
