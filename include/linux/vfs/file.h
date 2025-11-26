#pragma once

#include <linux/stddef.h>
#include <linux/uio.h>

#include <linux/vfs/kiocb.h>
#include <linux/vfs/dir.h>
#include <linux/vfs/path.h>

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
};

/*
 * flags in file.f_mode.  Note that FMODE_READ and FMODE_WRITE must correspond
 * to O_WRONLY and O_RDWR via the strange trick in do_dentry_open()
 */

/* file is open for reading */
#define FMODE_READ (0x1)
/* file is open for writing */
#define FMODE_WRITE (0x2)
/* file is seekable */
#define FMODE_LSEEK (0x4)
/* file can be accessed using pread */
#define FMODE_PREAD (0x8)
/* file can be accessed using pwrite */
#define FMODE_PWRITE (0x10)
/* File is opened for execution with sys_execve / sys_uselib */
#define FMODE_EXEC (0x20)
/* File is opened with O_NDELAY (only set for block devices) */
#define FMODE_NDELAY (0x40)
/* File is opened with O_EXCL (only set for block devices) */
#define FMODE_EXCL (0x80)

/* Has read method(s) */
#define FMODE_CAN_READ (0x20000)
/* Has write method(s) */
#define FMODE_CAN_WRITE (0x40000)

#define FMODE_OPENED (0x80000)
#define FMODE_CREATED (0x100000)

/* File is opened with O_PATH; almost nothing can be done with it */
#define FMODE_PATH ((__force fmode_t)0x4000)

#define OPEN_FMODE(flag) ((__force fmode_t)(((flag + 1) & O_ACCMODE) | \
                                            (flag & __FMODE_NONOTIFY)))

static inline struct inode *file_inode(const struct file *f)
{
    return f->f_inode;
}

void file_accessed(struct file *file);

#define replace_fops(f, fops) ((f)->f_op = (fops))
