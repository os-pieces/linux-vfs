#include <linux/vfs/fs.h>

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

    lockref_init(&dentry->d_lockref);
    seqcount_spinlock_init(&dentry->d_seq, &dentry->d_lockref.lock);

    dentry->d_parent = dentry;
    dentry->d_sb = sb;
    INIT_HLIST_HEAD(&dentry->d_children);
    INIT_HLIST_NODE(&dentry->d_sib);
    INIT_HLIST_BL_NODE(&dentry->d_hash);

    dentry->d_name.name = dname;
    dentry->d_name.len = name->len;
    dentry->d_name.hash = name->hash;
    memcpy(dname, name->name, name->len);
    dname[name->len] = 0;

    return dentry;
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

    d_lock(parent);
    /*
     * don't need child lock because it is not subject
     * to concurrency here
     */
    dentry->d_parent = dget_dlock(parent);
    hlist_add_head(&dentry->d_sib, &parent->d_children);
    d_unlock(parent);

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

struct dentry *d_alloc_anon(struct super_block *sb)
{
    struct qstr slash_name = QSTR_INIT("/", 1);

    return __d_alloc(sb, &slash_name);
}

struct dentry *d_alloc_name(struct dentry *parent, const char *name)
{
    struct qstr q;

    q.name = name;
    q.hash_len = hashlen_string(parent, name);

    return d_alloc(parent, &q);
}
