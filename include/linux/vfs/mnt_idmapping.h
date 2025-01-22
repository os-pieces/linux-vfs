#pragma once

struct mnt_idmap;

typedef struct {
	unsigned val;
} vfsuid_t;

typedef struct {
	unsigned val;
} vfsgid_t;

static inline bool vfsuid_valid(vfsuid_t uid)
{
	return true;
}

static inline bool vfsuid_eq(vfsuid_t left, vfsuid_t right)
{
	return true;
}

static inline bool vfsgid_valid(vfsgid_t gid)
{
	return true;
}

static inline vfsgid_t i_gid_into_vfsgid(struct mnt_idmap *idmap,
					 const struct inode *inode)
{
	vfsgid_t v;

	pr_todo();
	return v;
}
