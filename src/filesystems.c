#include <linux/vfs/fs.h>

#include <string.h>

static struct file_system_type *file_systems = NULL;

static struct file_system_type **find_filesystem(const char *name, unsigned len)
{
	struct file_system_type **p;
	for (p = &file_systems; *p; p = &(*p)->next)
		if (strncmp((*p)->name, name, len) == 0 &&
			!(*p)->name[len])
			break;
	return p;
}

static struct file_system_type *__get_fs_type(const char *name, int len)
{
	struct file_system_type *fs;

	fs = *(find_filesystem(name, len));

	return fs;
}

int register_filesystem(struct file_system_type *fs)
{
	int res = 0;
	struct file_system_type **p;

	if (fs->next)
		return -EBUSY;

	p = find_filesystem(fs->name, strlen(fs->name));
	if (*p)
		res = -EBUSY;
	else
		*p = fs;

	return res;
}

int unregister_filesystem(struct file_system_type *fs)
{
	return 0;
}

void put_filesystem(struct file_system_type *fs)
{

}

struct file_system_type *get_fs_type(const char *name)
{
	struct file_system_type *fs;
	const char *dot = strchr(name, '.');
	int len = dot ? dot - name : strlen(name);

	fs = __get_fs_type(name, len);
	if (dot && fs && !(fs->fs_flags & FS_HAS_SUBTYPE)) {
		put_filesystem(fs);
		fs = NULL;
	}

	return fs;
}
