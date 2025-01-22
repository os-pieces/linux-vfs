#pragma once

struct dentry;
struct vfsmount;

#define path_dentry(pp) ((pp)->dentry)
#define path_dentry_var(p) (p.dentry)
#define path_mnt(pp) ((pp)->mnt)
#define path_mnt_var(p) (p.mnt)

struct path
{
    struct vfsmount *mnt;
    struct dentry *dentry;
};

extern void path_get(const struct path *);
extern void path_put(const struct path *);

static inline int path_equal(const struct path *path1, const struct path *path2)
{
	return path1->mnt == path2->mnt && path1->dentry == path2->dentry;
}
