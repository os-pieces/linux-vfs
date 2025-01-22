#pragma once

#include <linux/vfs/fs.h>

#define I_VERSION_QUERIED_SHIFT	(1)
#define I_VERSION_QUERIED	(1ULL << (I_VERSION_QUERIED_SHIFT - 1))
#define I_VERSION_INCREMENT	(1ULL << I_VERSION_QUERIED_SHIFT)

void inode_inc_iversion(struct inode *inode);
u64 inode_query_iversion(struct inode *inode);

static inline u64
inode_peek_iversion_raw(const struct inode *inode)
{
	return inode->i_version;
}

static inline u64
inode_peek_iversion(const struct inode *inode)
{
	return inode_peek_iversion_raw(inode) >> I_VERSION_QUERIED_SHIFT;
}

static inline bool
inode_eq_iversion(const struct inode *inode, u64 old)
{
	return inode_peek_iversion(inode) == old;
}

static inline void
inode_set_iversion(struct inode *inode, u64 val)
{
    inode->i_version = val;
}
