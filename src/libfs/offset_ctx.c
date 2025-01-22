#include <linux/vfs/fs.h>

/* 0 is '.', 1 is '..', so always start with offset 2 or more */
enum
{
	DIR_OFFSET_MIN	= 2,
};

static uintptr_t dentry2offset(struct dentry *dentry)
{
	return (uintptr_t)dentry->d_fsdata;
}

static void offset_set(struct dentry *dentry, uintptr_t offset)
{
	dentry->d_fsdata = (void *)offset;
}

int simple_offset_add(struct offset_ctx *octx, struct dentry *dentry)
{
	uintptr_t offset;
	int ret;

	if (dentry2offset(dentry) != 0)
		return -EBUSY;

	ret = mtree_alloc_cyclic(&octx->mt, &offset, dentry, DIR_OFFSET_MIN,
				 LONG_MAX, &octx->next_offset, GFP_KERNEL);
	if (ret < 0)
		return ret;

	offset_set(dentry, offset);

    return 0;
}

void simple_offset_init(struct offset_ctx *octx)
{
	mt_init_flags(&octx->mt, MT_FLAGS_ALLOC_RANGE);

	octx->next_offset = DIR_OFFSET_MIN;
}
