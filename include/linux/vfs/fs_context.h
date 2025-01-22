#pragma once

struct file_system_type;
struct fs_parameter_spec;
struct fs_context;

/*
 * Type of parameter value.
 */
enum fs_value_type
{
    fs_value_is_undefined,
    fs_value_is_flag,	  /* Value not given a value */
    fs_value_is_string,	  /* Value is a string */
    fs_value_is_blob,	  /* Value is a binary blob */
    fs_value_is_filename, /* Value is a filename* + dirfd */
    fs_value_is_file,	  /* Value is a file* */
};

struct fs_parameter
{
    const char *key;
    enum fs_value_type type;
    union
    {
        char *string;
    };
    size_t size;
};

enum fs_context_purpose
{
    FS_CONTEXT_FOR_MOUNT,		/* New superblock for explicit mount */
    FS_CONTEXT_FOR_SUBMOUNT,	/* New superblock for automatic submount */
    FS_CONTEXT_FOR_RECONFIGURE, /* Superblock reconfiguration (remount) */
};

struct fs_context_operations
{
    void (*free)(struct fs_context *fc);
    int (*dup)(struct fs_context *fc, struct fs_context *src_fc);
    int (*parse_param)(struct fs_context *fc, struct fs_parameter *param);
    int (*parse_monolithic)(struct fs_context *fc, void *data);
    int (*get_tree)(struct fs_context *fc);
    int (*reconfigure)(struct fs_context *fc);
};

struct fs_context
{
    const struct fs_context_operations *ops;
    struct dentry *root;		/* The root and superblock */
    unsigned int sb_flags;		/* Proposed superblock flags (SB_*) */
    unsigned int sb_flags_mask; /* Superblock flags that were changed */
    const char *source;			/* The source name (eg. dev path) */
    void *s_fs_info;			/* Proposed s_fs_info */
    struct file_system_type *fs_type;
    void *fs_private; /* The filesystem's context */
    enum fs_context_purpose purpose;
    filedesc_t *fc_fdp;
};

extern struct fs_context *fs_context_for_mount(struct file_system_type *fs_type,
                                               unsigned int sb_flags);
extern int vfs_parse_fs_string(struct fs_context *fc, const char *key,
                               const char *value, size_t v_size);
extern int get_tree_nodev(struct fs_context *fc,
                          int (*fill_super)(struct super_block *sb,
                                            struct fs_context *fc));
extern int get_tree_bdev(struct fs_context *fc,
		int (*fill_super)(struct super_block *,
				  struct fs_context *));
