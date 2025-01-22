#pragma once

#include <linux/types.h>
#include <linux/uidgid.h>

struct fs_parse_result;

struct p_log
{
	const char *prefix;
	struct fc_log *log;
};

typedef int fs_param_type(struct p_log *,
						  const struct fs_parameter_spec *,
						  struct fs_parameter *,
						  struct fs_parse_result *);

/*
 * Specification of the type of value a parameter wants.
 *
 * Note that the fsparam_flag(), fsparam_string(), fsparam_u32(), ... macros
 * should be used to generate elements of this type.
 */
struct fs_parameter_spec
{
	const char *name;
	fs_param_type *type; /* The desired parameter type */
	unsigned short opt;	 /* Option number (returned by fs_parse()) */
	unsigned short flags;
#define fs_param_neg_with_no 0x0002	 /* "noxxx" is negative param */
#define fs_param_can_be_empty 0x0004 /* "xxx=" is allowed */
#define fs_param_deprecated 0x0008	 /* The param is deprecated */
	const void *data;
};

struct constant_table
{
	const char *name;
	int value;
};

struct fs_parse_result
{
	bool negated; /* T if param was "noxxx" */
	union
	{
		bool boolean;		  /* For spec_bool */
		int int_32;			  /* For spec_s32/spec_enum */
		unsigned int uint_32; /* For spec_u32{,_octal,_hex}/spec_enum */
		u64 uint_64;		  /* For spec_u64 */
		kuid_t uid;
		kgid_t gid;
	};
};

#define __fsparam(TYPE, NAME, OPT, FLAGS, DATA) \
	{                                           \
		.name = NAME,                           \
		.opt = OPT,                             \
		.type = TYPE,                           \
		.flags = FLAGS,                         \
		.data = DATA}

#define fsparam_flag(NAME, OPT) __fsparam(NULL, NAME, OPT, 0, NULL)
#define fsparam_flag_no(NAME, OPT) __fsparam(NULL, NAME, OPT, fs_param_neg_with_no, NULL)
#define fsparam_bool(NAME, OPT) __fsparam(fs_param_is_bool, NAME, OPT, 0, NULL)

/*
 * The type of parameter expected.
 */
fs_param_type fs_param_is_bool, fs_param_is_u32, fs_param_is_s32, fs_param_is_u64,
	fs_param_is_enum, fs_param_is_string, fs_param_is_blob, fs_param_is_blockdev,
	fs_param_is_path, fs_param_is_fd, fs_param_is_uid, fs_param_is_gid;


int fs_parse(struct fs_context *fc,
	     const struct fs_parameter_spec *desc,
	     struct fs_parameter *param,
	     struct fs_parse_result *result);
