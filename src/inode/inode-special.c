#include <linux/vfs/fs.h>

static special_inode_initializer_t blk_initializer = NULL;
static special_inode_initializer_t chr_initializer = NULL;
static special_inode_initializer_t fifo_initializer = NULL;

void set_special_inode_initializer(umode_t mode, special_inode_initializer_t initializer)
{
    if (mode & S_IFBLK)
        blk_initializer = initializer;
    else if (mode & S_IFCHR)
        chr_initializer = initializer;
    else if (S_ISFIFO(mode))
    {
        fifo_initializer = initializer;
    }
}

void init_special_inode(struct inode *inode, umode_t mode, dev_t rdev)
{
    if (mode & S_IFBLK)
    {
        inode->i_rdev = rdev;
        if (blk_initializer)
            blk_initializer(inode);
    }
    else if (mode & S_IFCHR)
    {
        inode->i_rdev = rdev;
        if (chr_initializer)
            chr_initializer(inode);
    }
    else if (S_ISFIFO(mode))
    {
        if (fifo_initializer)
            fifo_initializer(inode);
    }
}
