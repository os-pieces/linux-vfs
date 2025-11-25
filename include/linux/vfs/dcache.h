#pragma once

#include <linux/list_bl.h>
#include <linux/stringhash.h>

struct qstr;
struct dentry;

void dont_mount(struct dentry *dentry);
struct dentry *d_lookup(const struct dentry *parent, const struct qstr *name);
extern void shrink_dcache_parent(struct dentry *);
void d_move(struct dentry *dentry, struct dentry *target);

extern struct dentry *d_find_alias(struct inode *);

extern void __d_drop(struct dentry *dentry);
