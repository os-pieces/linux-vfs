#include <linux/vfs/fs.h>
#include <linux/vfs/private/path.h>

void get_fs_root(filedesc_t *fdp, struct path *root)
{
    filedesc_path_t pa;

    filedesc_root_get(fdp, &pa);
    root->dentry = pa.dentry;
    root->mnt = pa.mnt;

    root->dentry = pa.dentry;
    root->mnt = pa.mnt;

    path_get(root);
}

/*
 * Replace the fs->{rootmnt,root} with {mnt,dentry}. Put the old values.
 * It can block.
 */
void set_fs_root(filedesc_t *fdp, const struct path *path)
{
    struct path old_root;
    filedesc_path_t fpath;

    fpath.dentry = path->dentry;
    fpath.mnt = path->mnt;

    filedesc_root_set(fdp, fpath);

    path_get(path);

    if (old_root.dentry)
        path_put(&old_root);
}

/*
 * Replace the fs->{pwdmnt,pwd} with {mnt,dentry}. Put the old values.
 * It can block.
 */
void set_fs_pwd(filedesc_t *fdp, const struct path *path)
{
    struct path old_pwd;
    filedesc_path_t fpath;

    path_get(path);

    filedesc_pwd_get(fdp, &fpath);
    old_pwd.dentry = fpath.dentry;
    old_pwd.mnt = fpath.mnt;

    fpath.dentry = path_dentry(path);
    fpath.mnt = path_mnt(path);

    filedesc_pwd_set(fdp, fpath);

    if (old_pwd.dentry)
        path_put(&old_pwd);
}
