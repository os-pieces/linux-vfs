#pragma once

#include <linux/stddef.h>
#include <linux/uio.h>

#include <linux/vfs/kiocb.h>
#include <linux/vfs/dir.h>
#include <linux/vfs/path.h>
#include <linux/vfs/poll.h>

typedef struct
{
    atomic_t refcnt;
} file_ref_t;

struct file
{
    struct path f_path;
    struct inode *f_inode; /* cached value */
    const struct file_operations *f_op;
    unsigned f_mode;
    unsigned f_flags;
    file_ref_t f_ref;
    loff_t f_pos;
    struct address_space *f_mapping;

    union
    {
        /* pipes */
        unsigned f_pipe;
    };

    void *private_data;
};

struct file_operations
{
    int (*open)(struct inode *, struct file *);
    loff_t (*llseek)(struct file *, loff_t, int);
    ssize_t (*read)(struct file *, char *, size_t, loff_t *);
    ssize_t (*write)(struct file *, const char *, size_t, loff_t *);
    ssize_t (*read_iter)(struct kiocb *, struct iov_iter *);
    ssize_t (*write_iter)(struct kiocb *, struct iov_iter *);
    int (*iterate_shared)(struct file *, struct dir_context *);
    long (*unlocked_ioctl)(struct file *, unsigned int, unsigned long);
    int (*release)(struct inode *, struct file *);
    int (*fsync)(struct file *, loff_t, loff_t, int datasync);
    __poll_t (*poll)(struct file *, struct poll_table_struct *);
};

/*
 * flags in file.f_mode.  Note that FMODE_READ and FMODE_WRITE must correspond
 * to O_WRONLY and O_RDWR via the strange trick in do_dentry_open()
 */

/* file is open for reading */
#define FMODE_READ ((__force fmode_t)(1 << 0))
/* file is open for writing */
#define FMODE_WRITE ((__force fmode_t)(1 << 1))
/* file is seekable */
#define FMODE_LSEEK ((__force fmode_t)(1 << 2))
/* file can be accessed using pread */
#define FMODE_PREAD ((__force fmode_t)(1 << 3))
/* file can be accessed using pwrite */
#define FMODE_PWRITE ((__force fmode_t)(1 << 4))
/* File is opened for execution with sys_execve / sys_uselib */
#define FMODE_EXEC ((__force fmode_t)(1 << 5))
/* File writes are restricted (block device specific) */
#define FMODE_WRITE_RESTRICTED ((__force fmode_t)(1 << 6))
/* File supports atomic writes */
#define FMODE_CAN_ATOMIC_WRITE ((__force fmode_t)(1 << 7))

/* FMODE_* bit 8 */

/* 32bit hashes as llseek() offset (for directories) */
#define FMODE_32BITHASH ((__force fmode_t)(1 << 9))
/* 64bit hashes as llseek() offset (for directories) */
#define FMODE_64BITHASH ((__force fmode_t)(1 << 10))

/*
 * Don't update ctime and mtime.
 *
 * Currently a special hack for the XFS open_by_handle ioctl, but we'll
 * hopefully graduate it to a proper O_CMTIME flag supported by open(2) soon.
 */
#define FMODE_NOCMTIME ((__force fmode_t)(1 << 11))

/* Expect random access pattern */
#define FMODE_RANDOM ((__force fmode_t)(1 << 12))

/* FMODE_* bit 13 */

/* File is opened with O_PATH; almost nothing can be done with it */
#define FMODE_PATH ((__force fmode_t)(1 << 14))

/* File needs atomic accesses to f_pos */
#define FMODE_ATOMIC_POS ((__force fmode_t)(1 << 15))
/* Write access to underlying fs */
#define FMODE_WRITER ((__force fmode_t)(1 << 16))
/* Has read method(s) */
#define FMODE_CAN_READ ((__force fmode_t)(1 << 17))
/* Has write method(s) */
#define FMODE_CAN_WRITE ((__force fmode_t)(1 << 18))

#define FMODE_OPENED ((__force fmode_t)(1 << 19))
#define FMODE_CREATED ((__force fmode_t)(1 << 20))

/* File is stream-like */
#define FMODE_STREAM ((__force fmode_t)(1 << 21))

/* File supports DIRECT IO */
#define FMODE_CAN_ODIRECT ((__force fmode_t)(1 << 22))

#define FMODE_NOREUSE ((__force fmode_t)(1 << 23))

/* File is embedded in backing_file object */
#define FMODE_BACKING ((__force fmode_t)(1 << 24))

/*
 * Together with FMODE_NONOTIFY_PERM defines which fsnotify events shouldn't be
 * generated (see below)
 */
#define FMODE_NONOTIFY ((__force fmode_t)(1 << 25))

/*
 * Together with FMODE_NONOTIFY defines which fsnotify events shouldn't be
 * generated (see below)
 */
#define FMODE_NONOTIFY_PERM ((__force fmode_t)(1 << 26))

/* File is capable of returning -EAGAIN if I/O will block */
#define FMODE_NOWAIT ((__force fmode_t)(1 << 27))

/* File represents mount that needs unmounting */
#define FMODE_NEED_UNMOUNT ((__force fmode_t)(1 << 28))

/* File does not contribute to nr_files count */
#define FMODE_NOACCOUNT ((__force fmode_t)(1 << 29))

/*
 * The two FMODE_NONOTIFY* define which fsnotify events should not be generated
 * for an open file. These are the possible values of
 * (f->f_mode & FMODE_FSNOTIFY_MASK) and their meaning:
 *
 * FMODE_NONOTIFY - suppress all (incl. non-permission) events.
 * FMODE_NONOTIFY_PERM - suppress permission (incl. pre-content) events.
 * FMODE_NONOTIFY | FMODE_NONOTIFY_PERM - suppress only FAN_ACCESS_PERM.
 */
#define FMODE_FSNOTIFY_MASK \
    (FMODE_NONOTIFY | FMODE_NONOTIFY_PERM)

#define OPEN_FMODE(flag) ((__force fmode_t)(((flag + 1) & O_ACCMODE) | \
                                            (flag & __FMODE_NONOTIFY)))

static inline struct inode *file_inode(const struct file *f)
{
    return f->f_inode;
}

void file_accessed(struct file *file);

#define replace_fops(f, fops) ((f)->f_op = (fops))

struct file *get_file(struct file *f);
void fput(struct file *file);

#define file_private(file) ((file)->private_data)
