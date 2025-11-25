#pragma once

#include <linux/wait.h>
#include <linux/seqlock.h>
#include <linux/spinlock.h>
#include <linux/vfs/dcache.h>
#include <linux/lockref.h>

#define DNAME_INLINE_LEN 20

struct qstr
{
    union
    {
        struct
        {
            u32 hash;
            u32 len;
        };
        u64 hash_len;
    };
    const char *name;
};

#define QSTR_INIT(n, l) {{{.len = l}}, .name = n}

struct dentry_operations
{
    int (*d_revalidate)(struct dentry *, unsigned int);
    int (*d_hash)(const struct dentry *, struct qstr *);
    int (*d_compare)(const struct dentry *, unsigned int, const char *, const struct qstr *);
    int (*d_manage)(const struct path *, bool);
    int (*d_delete)(const struct dentry *);
    void (*d_prune)(struct dentry *);
    void (*d_release)(struct dentry *);
};

struct dentry
{
    unsigned int d_flags;
    char d_iname[DNAME_INLINE_LEN];
    struct inode *d_inode;
    const struct dentry_operations *d_op;
    struct super_block *d_sb; /* The root of the dentry tree */
    struct qstr d_name;
    struct hlist_node d_sib;      /* child of parent list */
    struct hlist_head d_children; /* our children */
    struct dentry *d_parent;
    struct hlist_bl_node d_hash; /* lookup hash list */
    seqcount_spinlock_t d_seq;   /* per dentry seqlock */
    struct lockref d_lockref;    /* per-dentry lock and refcount
                                  * keep separate from RCU lookup area if
                                  * possible!
                                  */

    void *d_fsdata;
};

/*
 * dentry->d_lock spinlock nesting subclasses:
 *
 * 0: normal
 * 1: nested
 */
enum dentry_d_lock_class
{
    DENTRY_D_LOCK_NORMAL, /* implicitly used by plain spin_lock() APIs. */
    DENTRY_D_LOCK_NESTED
};

/* d_flags entries */
enum dentry_flags
{
    DCACHE_OP_HASH = BIT(0),
    DCACHE_OP_COMPARE = BIT(1),
    DCACHE_OP_REVALIDATE = BIT(2),
    DCACHE_OP_DELETE = BIT(3),
    DCACHE_OP_PRUNE = BIT(4),
    /*
     * This dentry is possibly not currently connected to the dcache tree,
     * in which case its parent will either be itself, or will have this
     * flag as well.  nfsd will not use a dentry with this bit set, but will
     * first endeavour to clear the bit either by discovering that it is
     * connected, or by performing lookup operations.  Any filesystem which
     * supports nfsd_operations MUST have a lookup function which, if it
     * finds a directory inode with a DCACHE_DISCONNECTED dentry, will
     * d_move that dentry into place and return that dentry rather than the
     * passed one, typically using d_splice_alias.
     */
    DCACHE_DISCONNECTED = BIT(5),
    DCACHE_REFERENCED = BIT(6), /* Recently used, don't discard. */
    DCACHE_DONTCACHE = BIT(7),  /* Purge from memory on final dput() */
    DCACHE_CANT_MOUNT = BIT(8),
    DCACHE_GENOCIDE = BIT(9),
    DCACHE_SHRINK_LIST = BIT(10),
    DCACHE_OP_WEAK_REVALIDATE = BIT(11),
    /*
     * this dentry has been "silly renamed" and has to be deleted on the
     * last dput()
     */
    DCACHE_NFSFS_RENAMED = BIT(12),
    DCACHE_FSNOTIFY_PARENT_WATCHED = BIT(13), /* Parent inode is watched by some fsnotify listener */
    DCACHE_DENTRY_KILLED = BIT(14),
    DCACHE_MOUNTED = BIT(15),        /* is a mountpoint */
    DCACHE_NEED_AUTOMOUNT = BIT(16), /* handle automount on this dir */
    DCACHE_MANAGE_TRANSIT = BIT(17), /* manage transit from this dirent */
    DCACHE_LRU_LIST = BIT(18),
    DCACHE_ENTRY_TYPE = (7 << 19),     /* bits 19..21 are for storing type: */
    DCACHE_MISS_TYPE = (0 << 19),      /* Negative dentry */
    DCACHE_WHITEOUT_TYPE = (1 << 19),  /* Whiteout dentry (stop pathwalk) */
    DCACHE_DIRECTORY_TYPE = (2 << 19), /* Normal directory */
    DCACHE_AUTODIR_TYPE = (3 << 19),   /* Lookupless directory (presumed automount) */
    DCACHE_REGULAR_TYPE = (4 << 19),   /* Regular file type */
    DCACHE_SPECIAL_TYPE = (5 << 19),   /* Other file type */
    DCACHE_SYMLINK_TYPE = (6 << 19),   /* Symlink */
    DCACHE_NOKEY_NAME = BIT(22),       /* Encrypted name encoded without key */
    DCACHE_OP_REAL = BIT(23),
    DCACHE_PAR_LOOKUP = BIT(24), /* being looked up (with parent locked shared) */
    DCACHE_DENTRY_CURSOR = BIT(25),
    DCACHE_NORCU = BIT(26), /* No RCU delay for freeing */
};

#define IS_ROOT(x) ((x) == (x)->d_parent)

/*
 * Directory cache entry type accessor functions.
 */
static inline unsigned __d_entry_type(const struct dentry *dentry)
{
    return dentry->d_flags & DCACHE_ENTRY_TYPE;
}

static inline bool d_can_lookup(const struct dentry *dentry)
{
    return __d_entry_type(dentry) == DCACHE_DIRECTORY_TYPE;
}

static inline int d_in_lookup(const struct dentry *dentry)
{
    return dentry->d_flags & DCACHE_PAR_LOOKUP;
}

static inline bool d_is_autodir(const struct dentry *dentry)
{
    return __d_entry_type(dentry) == DCACHE_AUTODIR_TYPE;
}

static inline bool d_is_dir(const struct dentry *dentry)
{
    return d_can_lookup(dentry) || d_is_autodir(dentry);
}

extern void dput(struct dentry *);
extern void d_invalidate(struct dentry *);
extern void d_lookup_done(struct dentry *dentry);
extern struct dentry *d_alloc_parallel(struct dentry *, const struct qstr *,
                                       wait_queue_head_t *);
extern struct dentry *d_alloc_anon(struct super_block *);

static inline struct inode *d_backing_inode(const struct dentry *upper)
{
    struct inode *inode = upper->d_inode;

    return inode;
}

static inline void d_lock(struct dentry *dentry)
{
    spin_lock(&dentry->d_lockref.lock);
}

static inline void d_lock_nested(struct dentry *dentry, int subclass)
{
    spin_lock_nested(&dentry->d_lockref.lock, subclass);
}

static inline void d_unlock(struct dentry *dentry)
{
    spin_unlock(&dentry->d_lockref.lock);
}

extern struct dentry *d_alloc(struct dentry *, const struct qstr *);
struct dentry *dget(struct dentry *dentry);
extern void d_instantiate(struct dentry *, struct inode *);
extern void d_add(struct dentry *, struct inode *);
extern void d_set_d_op(struct dentry *dentry, const struct dentry_operations *op);
extern struct dentry *d_make_root(struct inode *);
bool d_is_negative(const struct dentry *dentry);
bool d_is_positive(const struct dentry *dentry);

/**
 * d_inode - Get the actual inode of this dentry
 * @dentry: The dentry to query
 *
 * This is the helper normal filesystems should use to get at their own inodes
 * in their own dentries and ignore the layering superimposed upon them.
 */
static inline struct inode *d_inode(const struct dentry *dentry)
{
    return dentry->d_inode;
}

/**
 * d_really_is_positive - Determine if a dentry is really positive (ignoring fallthroughs)
 * @dentry: The dentry in question
 *
 * Returns true if the dentry represents a name that maps to an inode
 * (ie. ->d_inode is not NULL).  The dentry might still represent a whiteout if
 * that is represented on medium as a 0,0 chardev.
 *
 * Note!  (1) This should be used *only* by a filesystem to examine its own
 * dentries.  It should not be used to look at some other filesystem's
 * dentries.  (2) It should also be used in combination with d_inode() to get
 * the inode.
 */
static inline bool d_really_is_positive(const struct dentry *dentry)
{
    return dentry->d_inode != NULL;
}

static inline bool d_is_symlink(const struct dentry *dentry)
{
    return __d_entry_type(dentry) == DCACHE_SYMLINK_TYPE;
}

struct dentry *d_splice_alias(struct inode *inode, struct dentry *dentry);

static inline struct dentry *dget_dlock(struct dentry *dentry)
{
    dentry->d_lockref.count++;

    return dentry;
}

#define d_set_fsdata(d, v) ((d)->d_fsdata = (void *)(v))

struct dentry *d_alloc_name(struct dentry *parent, const char *name);

struct dentry *dget_parent(struct dentry *dentry);

#define d_flags(d) ((d)->d_flags)
