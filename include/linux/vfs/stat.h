#pragma once

#include <sys/stat.h>

#define S_ISUID 0004000
#define S_ISGID 0002000
#define S_ISVTX 0001000
#define S_IFDIR 0040000
#define S_IFLNK 0120000

#define S_IRWXUGO	(S_IRWXU|S_IRWXG|S_IRWXO)
#define S_IALLUGO	(S_ISUID|S_ISGID|S_ISVTX|S_IRWXUGO)
#define S_IRUGO		(S_IRUSR|S_IRGRP|S_IROTH)
#define S_IWUGO		(S_IWUSR|S_IWGRP|S_IWOTH)
#define S_IXUGO		(S_IXUSR|S_IXGRP|S_IXOTH)

#define S_ISLNK(m) (((m) & S_IFMT) == S_IFLNK)

struct kstat
{
    u64		ino;
    unsigned blksize;
    unsigned result_mask;
    struct timespec64 btime;
};


#define STATX_BTIME		0x00000800U	/* Want/got stx_btime */
