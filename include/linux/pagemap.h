#pragma once

struct address_space;
struct inode;

int write_inode_now(struct inode *, int sync);
int filemap_fdatawrite_range(struct address_space *mapping,
							 loff_t start, loff_t end);
int filemap_fdatawait_range(struct address_space *mapping, loff_t start_byte,
							loff_t end_byte);
