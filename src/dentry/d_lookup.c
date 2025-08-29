#include <linux/vfs/fs.h>
#include <linux/vfs/private/dcache.h>
#include <linux/rculist_bl.h>
#include <linux/log2.h>
#include <linux/stringhash.h>

extern struct dcache _dcache;

static inline struct hlist_bl_head *__d_hash(struct dcache *cache, unsigned int hash)
{
    return cache->hashtable + (hash >> cache->hash_shift);
}

static inline struct hlist_bl_head *d_hash(unsigned int hash)
{
    return __d_hash(&_dcache, hash);
}

struct dentry *__d_lookup(const struct dentry *parent, const struct qstr *name)
{
    unsigned int hash = name->hash;
    struct hlist_bl_head *b = d_hash(hash);
    struct hlist_bl_node *node;
    struct dentry *found = NULL;
    struct dentry *dentry;

    hlist_bl_for_each_entry_rcu(dentry, node, b, d_hash)
    {

        if (dentry->d_name.hash != hash)
            continue;
        if (dentry->d_parent != parent)
            goto next;
        if (d_unhashed(dentry))
            goto next;

        if (!d_same_name(dentry, parent, name))
            goto next;

        found = dentry;
        break;
    next:
    }

    return found;
}

struct dentry *d_lookup(const struct dentry *parent, const struct qstr *name)
{
    struct dentry *dentry;

    pr_todo();

    dentry = __d_lookup(parent, name);

    return dentry;
}
