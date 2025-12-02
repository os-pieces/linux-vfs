#include <linux/vfs/fs.h>
#include <linux/vfs/fs_parser.h>

int fs_param_is_bool(struct p_log *log, const struct fs_parameter_spec *p,
		     struct fs_parameter *param, struct fs_parse_result *result)
{
    return 0;
}

int fs_parse(struct fs_context *fc,
	     const struct fs_parameter_spec *desc,
	     struct fs_parameter *param,
	     struct fs_parse_result *result)
{
	pr_todo();

    return -ENOPARAM;
}
