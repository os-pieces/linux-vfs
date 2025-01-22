#include <linux/vfs/fs.h>
#include <linux/buffer_head.h>

struct buffer_head *sb_bread(struct super_block *sb, sector_t block)
{
     return 0;
}

void map_bh(struct buffer_head *bh, struct super_block *sb, sector_t block)
{
     pr_todo();
}

void mark_buffer_dirty_inode(struct buffer_head *bh, struct inode *inode)
{
     
}

void write_dirty_buffer(struct buffer_head *bh, blk_opf_t op_flags)
{

}

void mark_buffer_dirty(struct buffer_head *bh)
{

}

void __mark_inode_dirty(struct inode *inode, int flags)
{

}

loff_t i_size_read(const struct inode *inode)
{
     return inode->i_size;
}

struct buffer_head *
sb_find_get_block(struct super_block *sb, sector_t block)
{
     return 0;
}

int buffer_uptodate(const struct buffer_head *bh)
{
     return 0;
}

void sb_breadahead(struct super_block *sb, sector_t block)
{

}

void set_buffer_new(struct buffer_head *bh)
{

}

int sync_dirty_buffer(struct buffer_head *bh)
{
     return 0;
}

void wait_on_buffer(struct buffer_head *bh)
{
     pr_todo();
}

struct buffer_head *sb_getblk(struct super_block *sb,
		sector_t block)
{
     return 0;
}

void lock_buffer(struct buffer_head *bh)
{
     pr_todo();
}

void unlock_buffer(struct buffer_head *bh)
{
     pr_todo();
}

void get_bh(struct buffer_head *bh)
{
     
}

void set_buffer_uptodate(struct buffer_head *bh)
{
     
}

void bforget(struct buffer_head *bh)
{

}

int generic_cont_expand_simple(struct inode *inode, loff_t size)
{
     return 0;
}

int blkdev_issue_flush(struct block_device *bdev)
{
     return 0;
}

int sync_mapping_buffers(struct address_space *mapping)
{
     return 0;
}
