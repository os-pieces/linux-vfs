#include <linux/vfs/fs.h>

unsigned char fs_umode_to_dtype(umode_t mode)
{
    if (S_ISDIR(mode))
        return DT_DIR;
    else if (S_ISREG(mode))
        return DT_REG;
    else if (S_ISLNK(mode))
        return DT_LNK;
    else if (S_ISCHR(mode))
        return DT_CHR;
    else if (S_ISBLK(mode))
        return DT_BLK;

    return DT_UNKNOWN;
}
