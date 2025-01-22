#pragma once

static inline int security_path_unlink(const struct path *dir, struct dentry *dentry)
{
    return 0;
}

static inline int security_inode_unlink(struct inode *dir,
					 struct dentry *dentry)
{
	return 0;
}

static inline int security_path_rmdir(const struct path *dir, struct dentry *dentry)
{
	return 0;
}

static inline int security_inode_rmdir(struct inode *dir,
					struct dentry *dentry)
{
	return 0;
}

static inline int security_inode_follow_link(struct dentry *dentry,
					     struct inode *inode,
					     bool rcu)
{
	return 0;
}

static inline int security_file_permission(struct file *file, int mask)
{
	return 0;
}
