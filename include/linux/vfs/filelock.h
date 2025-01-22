#pragma once

static inline int try_break_deleg(struct inode *inode, struct inode **delegated_inode)
{
	return 0;
}

static inline int break_deleg_wait(struct inode **delegated_inode)
{
	return 0;
}
