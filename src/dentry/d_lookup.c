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

    dcache_rlock(&_dcache);

    hlist_bl_for_each_entry(dentry, node, b, d_hash)
    {
        if (dentry->d_name.hash != hash)
            continue;
        if (dentry->d_parent != parent)
            continue;
        if (d_unhashed(dentry))
            continue;

        if (!d_same_name(dentry, parent, name))
            continue;

        found = dentry;
        break;
    }

    dcache_runlock(&_dcache);

    return found;
}

/**
 * d_lookup - search for a dentry
 * @parent: parent dentry
 * @name: qstr of name we wish to find
 * Returns: dentry, or NULL
 *
 * d_lookup searches the children of the parent dentry for the name in
 * question. If the dentry is found its reference count is incremented and the
 * dentry is returned. The caller must use dput to free the entry when it has
 * finished using it. %NULL is returned if the dentry does not exist.
 */
struct dentry *d_lookup(const struct dentry *parent, const struct qstr *name)
{
    struct dentry *dentry;

    dentry = __d_lookup(parent, name);

    return dentry;
}
