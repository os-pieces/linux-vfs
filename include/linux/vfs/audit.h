#pragma once

struct filename;
struct dentry;
struct inode;

#define AUDIT_INODE_PARENT	1	/* dentry represents the parent */
#define AUDIT_INODE_NOEVAL	4	/* audit record incomplete */

/* audit_names->type values */
#define	AUDIT_TYPE_UNKNOWN	0	/* we don't know yet */
#define	AUDIT_TYPE_NORMAL	1	/* a "normal" audit record */
#define	AUDIT_TYPE_PARENT	2	/* a parent audit record */
#define	AUDIT_TYPE_CHILD_DELETE 3	/* a child being deleted */
#define	AUDIT_TYPE_CHILD_CREATE 4	/* a child being created */

static inline struct filename *audit_reusename(const __user char *name)
{
	return NULL;
}

static inline void audit_inode(struct filename *name,
				const struct dentry *dentry,
				unsigned int aflags)
{
}

static inline void audit_inode_child(struct inode *parent,
				     const struct dentry *dentry,
				     const unsigned char type)
{ }
