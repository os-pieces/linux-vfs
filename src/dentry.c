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

/**
 * __d_alloc	-	allocate a dcache entry
 * @sb: filesystem it will belong to
 * @name: qstr of the name
 *
 * Allocates a dentry. It returns %NULL if there is insufficient memory
 * available. On a success the dentry is returned. The name passed in is
 * copied and the copy passed in may be reused after this call.
 */

static struct dentry *__d_alloc(struct super_block *sb, const struct qstr *name)
{
    struct dentry *dentry;
    char *dname;

    dentry = kzalloc(sizeof(*dentry), GFP_KERNEL);
    if (!dentry)
        return NULL;

    pr_todo();
    dname = dentry->d_iname;

    dentry->d_parent = dentry;
    dentry->d_sb = sb;
    INIT_LIST_HEAD(&dentry->d_subdirs);
    INIT_LIST_HEAD(&dentry->d_child);
    INIT_HLIST_BL_NODE(&dentry->d_hash);

    dentry->d_name.name = dname;
    dentry->d_name.len = name->len;
    dentry->d_name.hash = name->hash;
    memcpy(dname, name->name, name->len);
    dname[name->len] = 0;

    return dentry;
}

/* This must be called with d_lock held */
static inline void __dget_dlock(struct dentry *dentry)
{
    pr_todo();
}

/**
 * d_alloc	-	allocate a dcache entry
 * @parent: parent of entry to allocate
 * @name: qstr of the name
 *
 * Allocates a dentry. It returns %NULL if there is insufficient memory
 * available. On a success the dentry is returned. The name passed in is
 * copied and the copy passed in may be reused after this call.
 */
struct dentry *d_alloc(struct dentry *parent, const struct qstr *name)
{
    struct dentry *dentry = __d_alloc(parent->d_sb, name);

    if (!dentry)
        return NULL;

    spin_lock(&parent->d_lock);
    /*
     * don't need child lock because it is not subject
     * to concurrency here
     */
    __dget_dlock(parent);
    dentry->d_parent = parent;
    list_add(&dentry->d_child, &parent->d_subdirs);
    spin_unlock(&parent->d_lock);

    return dentry;
}

struct dentry *d_alloc_parallel(struct dentry *parent, const struct qstr *name,
                                wait_queue_head_t *wq)
{
    struct dentry *new;
    struct dentry *dentry;

    new = d_alloc(parent, name);
    if (unlikely(!new))
        return ERR_PTR(-ENOMEM);

    pr_todo();
    dentry = new;

    return dentry;
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

struct dentry *d_alloc_anon(struct super_block *sb)
{
    struct qstr slash_name = QSTR_INIT("/", 1);

    return __d_alloc(sb, &slash_name);
}

struct dentry *d_make_root(struct inode *root_inode)
{
    struct dentry *res = NULL;

    if (root_inode)
    {
        res = d_alloc_anon(root_inode->i_sb);
        if (res)
            d_instantiate(res, root_inode);
        else
            iput(root_inode);
    }

    return res;
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

struct dentry *d_alloc_name(struct dentry *parent, const char *name)
{
    struct qstr q;

	q.name = name;
	q.hash_len = hashlen_string(parent, name);

	return d_alloc(parent, &q);
}

struct dentry *dget_parent(struct dentry *dentry)
{
    struct dentry *ret;

    ret = dentry->d_parent;

    return ret;
}
