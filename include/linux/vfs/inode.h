#pragma once

#include <linux/rwsem.h>

struct inode;
struct super_block;
struct mnt_idmap;
struct kstat;
struct path;
struct iattr;

/*
 * inode->i_mutex nesting subclasses for the lock validator:
 *
 * 0: the object of the current VFS operation
 * 1: parent
 * 2: child/target
 * 3: xattr
 * 4: second non-directory
 * 5: second parent (when locking independent directories in rename)
 *
 * I_MUTEX_NONDIR2 is for certain operations (such as rename) which lock two
 * non-directories at once.
 *
 * The locking order between these classes is
 * parent[2] -> child -> grandchild -> normal -> xattr -> second non-directory
 */
enum inode_i_mutex_lock_class
{
	I_MUTEX_NORMAL,
	I_MUTEX_PARENT,
	I_MUTEX_CHILD,
	I_MUTEX_XATTR,
	I_MUTEX_NONDIR2,
	I_MUTEX_PARENT2,
};

struct inode_operations
{
	struct dentry *(*lookup)(struct inode *, struct dentry *, unsigned int);
	int (*create)(struct mnt_idmap *, struct inode *, struct dentry *,
				  umode_t, bool);
	int (*unlink)(struct inode *, struct dentry *);
	int (*mknod)(struct mnt_idmap *, struct inode *, struct dentry *,
				 umode_t, dev_t);
	int (*mkdir)(struct mnt_idmap *, struct inode *, struct dentry *,
				 umode_t);
	int (*rmdir)(struct inode *, struct dentry *);
	int (*symlink)(struct mnt_idmap *, struct inode *, struct dentry *,
				   const char *);
	int (*rename)(struct mnt_idmap *, struct inode *, struct dentry *,
				  struct inode *, struct dentry *, unsigned int);
	int (*setattr) (struct mnt_idmap *, struct dentry *, struct iattr *);
	int (*getattr) (struct mnt_idmap *, const struct path *,
			struct kstat *, u32, unsigned int);
	const char * (*get_link) (struct dentry *, struct inode *, struct delayed_call *);
	int (*update_time)(struct inode *, int);
};

struct inode
{
	umode_t i_mode;
	unsigned short i_flags;
	kuid_t			i_uid;
	kgid_t			i_gid;
	const struct inode_operations *i_op;
	struct super_block *i_sb;
	union
	{
		const struct file_operations *i_fop; /* former ->i_op->default_file_ops */
		void (*free_inode)(struct inode *);
	};

	union
	{
		const unsigned int i_nlink;
		unsigned int __i_nlink;
	};

	union
	{
		char *i_link;
		void *i_cdev;
	};

	spinlock_t i_lock;	/* i_blocks, i_bytes, maybe i_size */

	unsigned long i_state;
	atomic_t i_count;
	loff_t i_size;

	unsigned long i_ino;

	struct rw_semaphore	i_rwsem;
	blkcnt_t		i_blocks;
	__u32			i_generation;
	u64 i_version;
    u8 i_blkbits;
	struct address_space	*i_mapping;
	dev_t			i_rdev;
};

/*
 * Inode flags - they have no relation to superblock flags now
 */
#define S_SYNC (1 << 0)		  /* Writes are synced at once */
#define S_NOATIME (1 << 1)	  /* Do not update access times */
#define S_APPEND (1 << 2)	  /* Append-only file */
#define S_IMMUTABLE (1 << 3)  /* Immutable file */
#define S_DEAD (1 << 4)		  /* removed, but still open directory */
#define S_NOQUOTA (1 << 5)	  /* Inode is not counted to quota */
#define S_DIRSYNC (1 << 6)	  /* Directory modifications are synchronous */
#define S_NOCMTIME (1 << 7)	  /* Do not update file c/mtime */
#define S_SWAPFILE (1 << 8)	  /* Do not truncate: swapon got its bmaps */
#define S_PRIVATE (1 << 9)	  /* Inode is fs-internal */
#define S_IMA (1 << 10)		  /* Inode has an associated IMA struct */
#define S_AUTOMOUNT (1 << 11) /* Automount/referral quasi-directory */
#define S_NOSEC (1 << 12)	  /* no suid or xattr security attributes */

#define IS_DEADDIR(inode) ((inode)->i_flags & S_DEAD)
#define IS_SWAPFILE(inode) ((inode)->i_flags & S_SWAPFILE)

void inode_lock_shared(struct inode *inode);
void inode_unlock_shared(struct inode *inode);
extern struct inode *new_inode(struct super_block *sb);
extern void inode_sb_list_add(struct inode *inode);
void inode_lock_nested(struct inode *inode, unsigned subclass);
extern void ihold(struct inode *inode);

void inode_lock(struct inode *inode);
void inode_unlock(struct inode *inode);

void mark_inode_dirty(struct inode *inode);

void drop_nlink(struct inode *inode);

struct timespec64 current_time(struct inode *inode);
void clear_nlink(struct inode *inode);
void set_nlink(struct inode *inode, unsigned int nlink);
void inc_nlink(struct inode *inode);

#define I_DIRTY_TIME		(1 << 11)
#define I_DIRTY_SYNC		(1 << 0)

extern void __mark_inode_dirty(struct inode *, int);

loff_t i_size_read(const struct inode *inode);

int inode_needs_sync(struct inode *inode);

static inline struct timespec64 inode_set_atime_to_ts(struct inode *inode,
						      struct timespec64 ts)
{
	//TODO inode->__i_atime = ts;
	return ts;
}

static inline struct timespec64 inode_set_mtime_to_ts(struct inode *inode,
						      struct timespec64 ts)
{
	//inode->__i_mtime = ts;
	return ts;
}

static inline struct timespec64 inode_set_ctime_to_ts(struct inode *inode,
						      struct timespec64 ts)
{
//	inode->__i_ctime = ts;
	return ts;
}

static inline struct timespec64 inode_get_atime(const struct inode *inode)
{
    struct timespec64 a;
    return a;//inode->__i_atime;
}

static inline struct timespec64 inode_get_mtime(const struct inode *inode)
{
    struct timespec64 a;
    return a;//inode->__i_atime;
}

static inline struct timespec64 inode_set_ctime(struct inode *inode,
						time64_t sec, long nsec)
{
	struct timespec64 ts = { .tv_sec  = sec,
				 .tv_nsec = nsec };

	return inode_set_ctime_to_ts(inode, ts);
}

extern ino_t iunique(struct super_block *, ino_t);
struct inode *igrab(struct inode *inode);

void insert_inode_hash(struct inode *inode);
extern void inode_init_once(struct inode *);

extern void init_special_inode(struct inode *inode, umode_t mode, dev_t rdev);

typedef void (*special_inode_initializer_t)(struct inode *inode);

void set_special_inode_initializer(umode_t mode, special_inode_initializer_t initializer);

#define inode_set_fops(inode, fops) ((inode)->i_fop = (fops))
