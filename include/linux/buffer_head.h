#pragma once

#include <linux/types.h>
#include <linux/wait.h>
#include <linux/fs.h>
#include <linux/blk_types.h>

#define MAX_BUF_PER_PAGE (4096 / 512)

struct buffer_head
{
	sector_t b_blocknr;		/* start block number */
	size_t b_size;			/* size of mapping */
	char *b_data;			/* pointer to data within the page */
  
};

static inline void brelse(struct buffer_head *bh)
{

}

struct buffer_head *sb_bread(struct super_block *sb, sector_t block);
void map_bh(struct buffer_head *bh, struct super_block *sb, sector_t block);
void mark_buffer_dirty_inode(struct buffer_head *bh, struct inode *inode);
void write_dirty_buffer(struct buffer_head *bh, blk_opf_t op_flags);
void mark_buffer_dirty(struct buffer_head *bh);
struct buffer_head *sb_find_get_block(struct super_block *sb, sector_t block);
int buffer_uptodate(const struct buffer_head *bh);
void sb_breadahead(struct super_block *sb, sector_t block);
void set_buffer_new(struct buffer_head *bh);
int sync_dirty_buffer(struct buffer_head *bh);
void wait_on_buffer(struct buffer_head *bh);
struct buffer_head *sb_getblk(struct super_block *sb, sector_t block);
void lock_buffer(struct buffer_head *bh);
void unlock_buffer(struct buffer_head *bh);
void get_bh(struct buffer_head *bh);
void set_buffer_uptodate(struct buffer_head *bh);
void bforget(struct buffer_head *bh);
int generic_cont_expand_simple(struct inode *inode, loff_t size);
int blkdev_issue_flush(struct block_device *bdev);
int sync_mapping_buffers(struct address_space *mapping);
