#include <linux/vfs/fs.h>

static struct fs_context *alloc_fs_context(struct file_system_type *fs_type,
										   struct dentry *reference,
										   unsigned int sb_flags,
										   unsigned int sb_flags_mask,
										   enum fs_context_purpose purpose)
{
	int (*init_fs_context)(struct fs_context *);
	struct fs_context *fc;
	int ret = -ENOMEM;

	fc = kzalloc(sizeof(struct fs_context), GFP_KERNEL_ACCOUNT);
	if (!fc)
		return ERR_PTR(-ENOMEM);

	pr_todo();
	fc->fs_type = fs_type;

	ret = fs_type->init_fs_context(fc);

	return fc;
}

/*
 * Check for a common mount option that manipulates s_flags.
 */
static int vfs_parse_sb_flag(struct fs_context *fc, const char *key)
{
    return -ENOPARAM;
}

struct fs_context *fs_context_for_mount(struct file_system_type *fs_type,
										unsigned int sb_flags)
{
	return alloc_fs_context(fs_type, NULL, sb_flags, 0,
							FS_CONTEXT_FOR_MOUNT);
}

/**
 * vfs_parse_fs_param_source - Handle setting "source" via parameter
 * @fc: The filesystem context to modify
 * @param: The parameter
 *
 * This is a simple helper for filesystems to verify that the "source" they
 * accept is sane.
 *
 * Returns 0 on success, -ENOPARAM if this is not  "source" parameter, and
 * -EINVAL otherwise. In the event of failure, supplementary error information
 *  is logged.
 */
int vfs_parse_fs_param_source(struct fs_context *fc, struct fs_parameter *param)
{
	if (strcmp(param->key, "source") != 0)
		return -ENOPARAM;

	if (param->type != fs_value_is_string)
		return -EINVAL;

	if (fc->source)
		return -EINVAL;

	fc->source = param->string;
	param->string = NULL;

	return 0;
}

int vfs_parse_fs_param(struct fs_context *fc, struct fs_parameter *param)
{
	int ret;

	if (!param->key)
	{
		return -EINVAL;
	}

	ret = vfs_parse_sb_flag(fc, param->key);
	if (ret != -ENOPARAM)
		return ret;

	if (fc->ops->parse_param)
	{
		ret = fc->ops->parse_param(fc, param);
		if (ret != -ENOPARAM)
			return ret;
	}

	/* If the filesystem doesn't take any arguments, give it the
	 * default handling of source.
	 */
	ret = vfs_parse_fs_param_source(fc, param);
	if (ret == -ENOPARAM)
		ret = -EINVAL;

	return ret;
}

/**
 * vfs_parse_fs_string - Convenience function to just parse a string.
 * @fc: Filesystem context.
 * @key: Parameter name.
 * @value: Default value.
 * @v_size: Maximum number of bytes in the value.
 */
int vfs_parse_fs_string(struct fs_context *fc, const char *key,
						const char *value, size_t v_size)
{
	int ret;

	struct fs_parameter param = {
		.key = key,
		.type = fs_value_is_flag,
		.size = v_size,
	};

	if (value)
	{
		param.string = kmemdup_nul(value, v_size, GFP_KERNEL);
		if (!param.string)
			return -ENOMEM;
		param.type = fs_value_is_string;
	}

	ret = vfs_parse_fs_param(fc, &param);
	kfree(param.string);

	return ret;
}

int parse_monolithic_mount_data(struct fs_context *fc, void *data)
{
	pr_todo();
	return 0;
}

void put_fs_context(struct fs_context *fc)
{
	pr_todo();
}

bool mount_capable(struct fs_context *fc)
{
	pr_todo();
	return true;
}
