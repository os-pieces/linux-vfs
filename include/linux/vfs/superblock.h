#pragma once

#include <linux/rwsem.h>

/*
 * sb->s_flags.  Note that these mirror the equivalent MS_* flags where
 * represented in both.
 */
#define SB_RDONLY BIT(0)      /* Mount read-only */
#define SB_NOSUID BIT(1)      /* Ignore suid and sgid bits */
#define SB_NODEV BIT(2)       /* Disallow access to device special files */
#define SB_NOEXEC BIT(3)      /* Disallow program execution */
#define SB_SYNCHRONOUS BIT(4) /* Writes are synced at once */
#define SB_MANDLOCK BIT(6)    /* Allow mandatory locks on an FS */
#define SB_DIRSYNC BIT(7)     /* Directory modifications are synchronous */
#define SB_NOATIME BIT(10)    /* Do not update access times. */
#define SB_NODIRATIME BIT(11) /* Do not update directory access times */
#define SB_SILENT BIT(15)
#define SB_POSIXACL BIT(16)    /* Supports POSIX ACLs */
#define SB_INLINECRYPT BIT(17) /* Use blk-crypto for encrypted files */
#define SB_KERNMOUNT BIT(22)   /* this is a kern_mount call */
#define SB_I_VERSION BIT(23)   /* Update inode I_version field */
#define SB_LAZYTIME BIT(25)    /* Update the on-disk [acm]times lazily */

/* These sb flags are internal to the kernel */
#define SB_DEAD BIT(21)
#define SB_DYING BIT(24)
#define SB_SUBMOUNT BIT(26)
#define SB_FORCE BIT(27)
#define SB_NOSEC BIT(28)
#define SB_BORN BIT(29)
#define SB_ACTIVE BIT(30)
#define SB_NOUSER BIT(31)

/* These flags relate to encoding and casefolding */
#define SB_ENC_STRICT_MODE_FL (1 << 0)

struct super_operations
{
    struct inode *(*alloc_inode)(struct super_block *sb);
    void (*destroy_inode)(struct inode *);
    void (*free_inode)(struct inode *);
};

struct super_block
{
    const struct super_operations *s_op;
    struct list_head s_mounts;              /* list of mounts; _not_ for fs use */
    const struct dentry_operations *s_d_op; /* default d_op for dentries */
    void *s_fs_info;                        /* Filesystem private info */
    struct rw_semaphore s_umount;
    dev_t s_dev; /* search index; _not_ kdev_t */
    struct dentry *s_root;
    unsigned long s_flags;
    atomic_t s_active;
    char s_id[32]; /* Informational name */
    unsigned char s_blocksize_bits;
    unsigned long s_blocksize;
    int s_maxbytes;
    struct backing_dev_info *s_bdi;
    struct block_device *s_bdev;
    int s_magic;
    u32 s_time_gran;
    u32 s_time_min;
    u32 s_time_max;
    const struct export_operations *s_export_op;

    /* s_inode_list_lock protects s_inodes */
    spinlock_t s_inode_list_lock;
    struct list_head s_inodes; /* all inodes */
};

void kill_litter_super(struct super_block *sb);
void kill_block_super(struct super_block *sb);

int sb_issue_discard(struct super_block *sb, sector_t block,
                     sector_t nr_blocks, gfp_t gfp_mask, unsigned long flags);

extern int sync_filesystem(struct super_block *);

void super_set_uuid(struct super_block *sb, const u8 *uuid, unsigned len);
