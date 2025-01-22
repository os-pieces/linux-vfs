#pragma once

int path_mount(filedesc_t *fdp, const char *dev_name, struct path *path,
		const char *type_page, unsigned long flags, void *data_page);
struct vfsmount *lookup_mnt(const struct path *path);
extern struct mount *__lookup_mnt(struct vfsmount *, struct dentry *);
