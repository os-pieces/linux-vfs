#include <linux/vfs/fs.h>
#include <linux/vfs/private/file.h>

static loff_t vfs_llseek(struct file *file, loff_t offset, int whence)
{
	if (!(file->f_mode & FMODE_LSEEK))
		return -ESPIPE;

	return file->f_op->llseek(file, offset, whence);
}

static off_t ksys_lseek(filedesc_t *fdp, unsigned int fd, off_t offset, unsigned int whence)
{
	off_t retval;
	struct fd f;

	retval = fdget_pos(fdp, fd, &f);
	if (retval == 0)
	{
		if (whence < 4)
		{
			loff_t res = vfs_llseek(f.file, offset, whence);

			retval = res;
			if (res != (loff_t)retval)
				retval = -EOVERFLOW; /* LFS: should only happen on 32 bit platforms */
		}
		else
		{
			retval = -EINVAL;
		}

		fdput_pos(fdp, f);
	}

	return retval;
}

int vfs_lseek_api(filedesc_t *fdp, int fd, off_t offset, int whence)
{
	return ksys_lseek(fdp, fd, offset, whence);
}
