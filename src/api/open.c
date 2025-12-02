#include <linux/vfs/fs.h>

#include <linux/vfs/private/namei.h>
#include <linux/vfs/private/fd.h>

struct open_how
{
    u64 flags;
    u64 mode;
    u64 resolve;
};

#define fops_get(o) (o)

#define WILL_CREATE(flags) (flags & (O_CREAT | __O_TMPFILE))
#define O_PATH_FLAGS (O_DIRECTORY | O_NOFOLLOW | O_PATH | O_CLOEXEC)

static inline int get_fops(struct file *f, struct inode *inode)
{
    f->f_op = fops_get(inode->i_fop);

    return (!f->f_op) ? -ENODEV : 0;
}

static inline int call_open(struct file *f, struct inode *inode)
{
    return f->f_op->open(inode, f);
}

static inline void set_fmode(struct file *f)
{
    f->f_mode |= FMODE_OPENED;
    if ((f->f_mode & FMODE_READ) &&
        likely(f->f_op->read  || f->f_op->read_iter))
        f->f_mode |= FMODE_CAN_READ;
    if ((f->f_mode & FMODE_WRITE) &&
        likely(f->f_op->write || f->f_op->write_iter))
        f->f_mode |= FMODE_CAN_WRITE;
    if ((f->f_mode & FMODE_LSEEK) && !f->f_op->llseek)
        f->f_mode &= ~FMODE_LSEEK;
}

static int do_dentry_open(struct file *f, struct inode *inode)
{
    int error;

    error = get_fops(f, inode);
    if (error == 0)
    {
        f->f_inode = inode;

        error = call_open(f, inode);
        if (error == 0)
        {
            set_fmode(f);
        }
    }

    return error;
}

static struct open_how build_open_how(int flags, umode_t mode)
{
    struct open_how how = {
        .flags = flags & VALID_OPEN_FLAGS,
        .mode = mode & S_IALLUGO,
    };

    /* O_PATH beats everything else. */
    if (how.flags & O_PATH)
        how.flags &= O_PATH_FLAGS;
    /* Modes should only be set for create-like flags. */
    if (!WILL_CREATE(how.flags))
        how.mode = 0;

    return how;
}

static inline int build_open_flags(const struct open_how *how, struct open_flags *op)
{
    u64 flags = how->flags;
    u64 strip = __FMODE_NONOTIFY | O_CLOEXEC;
    int lookup_flags = 0;
    int acc_mode = ACC_MODE(flags);

//TODO    BUILD_BUG_ON_MSG(upper_32_bits(VALID_OPEN_FLAGS),
//                     "struct open_flags doesn't yet handle flags > 32 bits");

    /*
     * Strip flags that either shouldn't be set by userspace like
     * FMODE_NONOTIFY or that aren't relevant in determining struct
     * open_flags like O_CLOEXEC.
     */
    flags &= ~strip;

    /*
     * Older syscalls implicitly clear all of the invalid flags or argument
     * values before calling build_open_flags(), but openat2(2) checks all
     * of its arguments.
     */
    if (flags & ~VALID_OPEN_FLAGS)
        return -EINVAL;
    if (how->resolve & ~VALID_RESOLVE_FLAGS)
        return -EINVAL;

    /* Scoping flags are mutually exclusive. */
    if ((how->resolve & RESOLVE_BENEATH) && (how->resolve & RESOLVE_IN_ROOT))
        return -EINVAL;

    /* Deal with the mode. */
    if (WILL_CREATE(flags))
    {
        if (how->mode & ~S_IALLUGO)
            return -EINVAL;
        op->mode = how->mode | S_IFREG;
    }
    else
    {
        if (how->mode != 0)
            return -EINVAL;
        op->mode = 0;
    }

    /*
     * Block bugs where O_DIRECTORY | O_CREAT created regular files.
     * Note, that blocking O_DIRECTORY | O_CREAT here also protects
     * O_TMPFILE below which requires O_DIRECTORY being raised.
     */
    if ((flags & (O_DIRECTORY | O_CREAT)) == (O_DIRECTORY | O_CREAT))
        return -EINVAL;

    /* Now handle the creative implementation of O_TMPFILE. */
    if (flags & __O_TMPFILE)
    {
        /*
         * In order to ensure programs get explicit errors when trying
         * to use O_TMPFILE on old kernels we enforce that O_DIRECTORY
         * is raised alongside __O_TMPFILE.
         */
        if (!(flags & O_DIRECTORY))
            return -EINVAL;
        if (!(acc_mode & MAY_WRITE))
            return -EINVAL;
    }
    if (flags & O_PATH)
    {
        /* O_PATH only permits certain other flags to be set. */
        if (flags & ~O_PATH_FLAGS)
            return -EINVAL;
        acc_mode = 0;
    }

    /*
     * O_SYNC is implemented as __O_SYNC|O_DSYNC.  As many places only
     * check for O_DSYNC if the need any syncing at all we enforce it's
     * always set instead of having to deal with possibly weird behaviour
     * for malicious applications setting only __O_SYNC.
     */
    if (flags & __O_SYNC)
        flags |= O_DSYNC;

    op->open_flag = flags;

    /* O_TRUNC implies we need access checks for write permissions */
    if (flags & O_TRUNC)
        acc_mode |= MAY_WRITE;

    /* Allow the LSM permission hook to distinguish append
       access from general write access. */
    if (flags & O_APPEND)
        acc_mode |= MAY_APPEND;

    op->acc_mode = acc_mode;

    op->intent = flags & O_PATH ? 0 : LOOKUP_OPEN;

    if (flags & O_CREAT)
    {
        op->intent |= LOOKUP_CREATE;
        if (flags & O_EXCL)
        {
            op->intent |= LOOKUP_EXCL;
            flags |= O_NOFOLLOW;
        }
    }

    if (flags & O_DIRECTORY)
        lookup_flags |= LOOKUP_DIRECTORY;
    if (!(flags & O_NOFOLLOW))
        lookup_flags |= LOOKUP_FOLLOW;

    if (how->resolve & RESOLVE_NO_XDEV)
        lookup_flags |= LOOKUP_NO_XDEV;
    if (how->resolve & RESOLVE_NO_MAGICLINKS)
        lookup_flags |= LOOKUP_NO_MAGICLINKS;
    if (how->resolve & RESOLVE_NO_SYMLINKS)
        lookup_flags |= LOOKUP_NO_SYMLINKS;
    if (how->resolve & RESOLVE_BENEATH)
        lookup_flags |= LOOKUP_BENEATH;
    if (how->resolve & RESOLVE_IN_ROOT)
        lookup_flags |= LOOKUP_IN_ROOT;
    if (how->resolve & RESOLVE_CACHED)
    {
        /* Don't bother even trying for create/truncate/tmpfile open */
        if (flags & (O_TRUNC | O_CREAT | __O_TMPFILE))
            return -EAGAIN;
        lookup_flags |= LOOKUP_CACHED;
    }

    op->lookup_flags = lookup_flags;
    return 0;
}

static int __build_openflags(struct open_flags *op, int flags, umode_t mode)
{
    struct open_how how = build_open_how(flags, mode);
    
    return build_open_flags(&how, op);
}

static int __file_alloc(filedesc_t *fdp, struct file **fpp, struct filedesc_slot *slot, int flags)
{
    int error;

    error = filedesc_slot_alloc(fdp, slot);
    if (error == 0)
    {
        *fpp = alloc_empty_file(flags);
        if (*fpp == NULL)
        {
            filedesc_slot_free(fdp, slot);
            error = -ENOMEM;
        }
    }

    return error;
}

static int do_open(const struct path *path, struct file *file)
{
    int error;
    struct inode *inode;

    path_get(path);
    file->f_path = *path;

    inode = d_backing_inode(path->dentry);

    error = do_dentry_open(file, inode);

    return error;
}

int vfs_openat_api(filedesc_t *fdp, int atfd, const char *name, int flags, umode_t mode)
{
    struct file *fp;
    struct filedesc_slot slot;
    struct open_flags op;
    int error;

    __build_openflags(&op, flags, mode);

    error = __file_alloc(fdp, &fp, &slot, op.open_flag);
    if (error == 0)
    {
        struct nameiargs ni;

        namei_init(&ni, fdp, name, atfd, 0);

        op.file = fp;
        op.do_open = do_open;

        error = namei_open(&ni, &op);
        if (error == 0)
        {
            filedesc_slot_finstall(fdp, &slot, fp);

            error = slot.result_fd;
        }
        else
        {
            pr_todo();
        }
    }

    return error;
}
