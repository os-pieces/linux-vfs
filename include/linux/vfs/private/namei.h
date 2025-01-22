#pragma once

struct nameidata;

enum
{
    LAST_NORM,
    LAST_ROOT,
    LAST_DOT,
    LAST_DOTDOT
};

#define MAXSYMLINKS 40


/* These tell filesystem methods that we are dealing with the final component... */
#define LOOKUP_OPEN		0x0100	/* ... in open */

#define LOOKUP_EXCL		0x0400	/* ... in exclusive creation */

/* pathwalk mode */
#define LOOKUP_FOLLOW		0x0001	/* follow links at the end */
#define LOOKUP_DIRECTORY	0x0002	/* require a directory */
#define LOOKUP_AUTOMOUNT	0x0004  /* force terminal automount */
#define LOOKUP_EMPTY		0x4000	/* accept empty path [user_... only] */
#define LOOKUP_DOWN		0x8000	/* follow mounts in the starting point */
#define LOOKUP_MOUNTPOINT	0x0080	/* follow mounts in the end */

#define LOOKUP_REVAL		0x0020	/* tell ->d_revalidate() to trust no cache */

/* internal use only */
#define LOOKUP_PARENT		0x0010

/* Scoping flags for lookup. */
#define LOOKUP_NO_SYMLINKS	0x010000 /* No symlink crossing. */
#define LOOKUP_NO_MAGICLINKS	0x020000 /* No nd_jump_link() crossing. */
#define LOOKUP_NO_XDEV		0x040000 /* No mountpoint crossing. */
#define LOOKUP_BENEATH		0x080000 /* No escaping from starting point. */
#define LOOKUP_IN_ROOT		0x100000 /* Treat dirfd as fs root. */
#define LOOKUP_CACHED		0x200000 /* Only do cached lookup */
/* LOOKUP_* flags which do scope-related checks based on the dirfd. */
#define LOOKUP_IS_SCOPED (LOOKUP_BENEATH | LOOKUP_IN_ROOT)

extern int user_path_at_empty(int, const char __user *, unsigned, struct path *, int *empty);

static inline int user_path_at(int dfd, const char __user *name, unsigned flags,
		 struct path *path)
{
	return user_path_at_empty(dfd, name, flags, path, NULL);
}

static inline bool retry_estale(const long error, const unsigned int flags)
{
	return unlikely(error == -ESTALE && !(flags & LOOKUP_REVAL));
}

struct nameiargs {
    filedesc_t *ni_fdp;
    const char *ni_name;
    int ni_atfd;
    int ni_lookflags;

    struct path ni_ret_parent;
    struct dentry *ni_ret_dentry;
};

void namei_init(struct nameiargs *ni, struct filedesc *fdp, const char *namep,
                    int atfd, int lookflags);

int namei_create(struct nameiargs *ni);

struct open_flags;
int namei_open(struct nameiargs *ni, struct open_flags *op);

int namei_lookup(struct nameiargs *ni);

int vfs_path_parent_lookup(struct nameiargs *ni, struct qstr *last, int *type,
			   const struct path *root);


struct dentry *lookup_one_qstr_excl(const struct qstr *name,
				    struct dentry *base,
				    unsigned int flags);
int may_delete(struct mnt_idmap *idmap, struct inode *dir,
               struct dentry *victim, bool isdir);

struct open_flags {
	int open_flag;
	umode_t mode;
	int acc_mode;
	int intent;
	int lookup_flags;
	struct file *file;
	int (*do_open)(const struct path *path, struct file *file);
};

extern bool may_open_dev(const struct path *path);
int kern_path(filedesc_t *fdp, const char *name, unsigned int flags, struct path *path);

int may_follow_link(struct nameidata *nd, const struct inode *inode);
int may_lookup(struct mnt_idmap *idmap, struct nameidata *nd);
int may_o_create(struct mnt_idmap *idmap,
                 const struct path *dir, struct dentry *dentry,
                 umode_t mode);
int may_delete(struct mnt_idmap *idmap, struct inode *dir,
               struct dentry *victim, bool isdir);
