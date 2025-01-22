#include <linux/vfs/fs.h>

void d_delete_notify(struct inode *dir, struct dentry *dentry)
{
    pr_todo();
}

void fsnotify_create(struct inode *dir, struct dentry *dentry)
{

}

void fsnotify_access(struct file *file)
{

}
