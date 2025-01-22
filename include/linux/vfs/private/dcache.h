#pragma once

#include <linux/vfs/dcache.h>
#include <linux/rculist.h>

#define DCACHE_MOUNTED			BIT(16) /* is a mountpoint */
#define DCACHE_NEED_AUTOMOUNT		BIT(17) /* handle automount on this dir */
#define DCACHE_MANAGE_TRANSIT		BIT(18) /* manage transit from this dirent */
#define DCACHE_MANAGED_DENTRY \
	(DCACHE_MOUNTED|DCACHE_NEED_AUTOMOUNT|DCACHE_MANAGE_TRANSIT)

int dcache_init(struct dcache *c, unsigned hash_shift);

int d_unhashed(const struct dentry *dentry);

struct dentry *__d_lookup_rcu(const struct dentry *parent,
							  const struct qstr *name,
							  unsigned *seqp);
struct dentry *__d_lookup(const struct dentry *parent, const struct qstr *name);

static inline bool d_mountpoint(const struct dentry *dentry)
{
	return dentry->d_flags & DCACHE_MOUNTED;
}

bool is_subdir(struct dentry *, struct dentry *);

int d_unlinked(const struct dentry *dentry);

extern int d_set_mounted(struct dentry *dentry);

static inline int cant_mount(const struct dentry *dentry)
{
	return (dentry->d_flags & DCACHE_CANT_MOUNT);
}

void dput_to_list(struct dentry *dentry, struct list_head *list);

static inline unsigned int d_flags_get_smp(struct dentry *dentry)
{
    return dentry->d_flags;
}

static inline bool d_flags_negative(unsigned flags)
{
	return (flags & DCACHE_ENTRY_TYPE) == DCACHE_MISS_TYPE;
}

void d_clear_mounted(struct dentry *dentry);
