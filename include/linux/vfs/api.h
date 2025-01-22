#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <linux/types.h>
#include <linux/filedesc.h>

struct dirent;

int vfs_mount_api(filedesc_t *fdp, char *dev, char *dir, char *type,
                  unsigned long flags, void *data);

int vfs_openat_api(filedesc_t *fdp, int atfd,
                          const char *name, int flags, umode_t mode);
int vfs_mkdirat_api(filedesc_t *fdp, int atfd,
                           const char *name, umode_t mode);
int vfs_mkdir_api(filedesc_t *fdp, const char *name, umode_t mode);
int vfs_mknodat_api(filedesc_t *fdp, int atfd,
                           const char *name, umode_t mode, unsigned dev);
ssize_t vfs_write_api(filedesc_t *fdp, unsigned int fd,
                             const void *buf, size_t size);
ssize_t vfs_read_api(filedesc_t *fdp, unsigned int fd,
                            char *buf, size_t size);
int vfs_symlinkat_api(filedesc_t *fdp, const char *oldname,
                             int atfd, const char *newname);
int vfs_readlinkat_api(filedesc_t *fdp, int atfd,
                              const char *pathname, char *buf, int bufsiz);
int vfs_chdir_api(filedesc_t *fdp, const char *name);
int vfs_chroot_api(filedesc_t *fdp, const char *name);
int vfs_close_api(filedesc_t *fdp, int fd);
int vfs_ioctl_api(filedesc_t *fdp, int fd, int cmd, unsigned long arg);
long vfs_fcntl_api(filedesc_t *fdp, int fd, int cmd, unsigned long arg);
int vfs_lseek_api(filedesc_t *fdp, int fd, off_t offset, int whence);
int vfs_rmdir_api(filedesc_t *fdp, int dfd, const char *pathname);
int vfs_unlinkat_api(filedesc_t *fdp, int dirfd, const char *pathname, int flags);
int vfs_readlink_api(filedesc_t *fdp, const char *pathname, char *buf, int bufsz);
int vfs_getdents_api(filedesc_t *fdp, int fd, struct dirent *dirent, unsigned int count);

#ifdef __cplusplus
}
#endif
