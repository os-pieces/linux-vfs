#pragma once

#include <linux/wait.h>
#include <linux/seqlock.h>
#include <linux/spinlock.h>
#include <linux/vfs/dcache.h>

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
};

struct dentry
{
	unsigned int d_flags;
	char d_iname[DNAME_INLINE_LEN];
	struct inode *d_inode;
	const struct dentry_operations *d_op;
	struct super_block *d_sb; /* The root of the dentry tree */
	struct qstr d_name;
	struct list_head d_child;
	struct list_head d_subdirs;
	struct dentry *d_parent;
	struct hlist_bl_node d_hash; /* lookup hash list */
	seqcount_spinlock_t d_seq;	/* per dentry seqlock */
	spinlock_t d_lock; /* per dentry lock */
    void *d_fsdata;
};

/* d_flags entries */
#define DCACHE_OP_HASH 0x00000001
#define DCACHE_OP_COMPARE 0x00000002
#define DCACHE_OP_REVALIDATE 0x00000004
#define DCACHE_OP_DELETE 0x00000008
#define DCACHE_OP_PRUNE 0x00000010

#define DCACHE_ENTRY_TYPE 0x00700000
#define DCACHE_MISS_TYPE 0x00000000		 /* Negative dentry (maybe fallthru to nowhere) */
#define DCACHE_WHITEOUT_TYPE 0x00100000	 /* Whiteout dentry (stop pathwalk) */
#define DCACHE_DIRECTORY_TYPE 0x00200000 /* Normal directory */
#define DCACHE_AUTODIR_TYPE 0x00300000	 /* Lookupless directory (presumed automount) */
#define DCACHE_REGULAR_TYPE 0x00400000	 /* Regular file type (or fallthru to such) */
#define DCACHE_SPECIAL_TYPE 0x00500000	 /* Other file type (or fallthru to such) */
#define DCACHE_SYMLINK_TYPE 0x00600000	 /* Symlink (or fallthru to such) */

#define DCACHE_PAR_LOOKUP 0x10000000 /* being looked up (with parent locked shared) */
#define DCACHE_DENTRY_CURSOR 0x20000000
#define DCACHE_NORCU 0x40000000 /* No RCU delay for freeing */

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

static inline struct inode *d_backing_inode(const struct dentry *upper)
{
	struct inode *inode = upper->d_inode;

	return inode;
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

static inline void d_lock(struct dentry *dentry)
{
    pr_todo();
}

static inline void d_unlock(struct dentry *dentry)
{
    
}

static inline struct dentry *dget_dlock(struct dentry *dentry)
{
	//TODO dentry->d_lockref.count++;
	return dentry;
}

#define d_set_fsdata(d, v) ((d)->d_fsdata = (void*)(v))

struct dentry *d_alloc_name(struct dentry *parent, const char *name);

struct dentry *dget_parent(struct dentry *dentry);

#define d_flags(d) ((d)->d_flags)
