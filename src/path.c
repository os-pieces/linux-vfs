#include <linux/vfs/fs.h>

void path_get(const struct path *path)
{
    pr_todo();
}

void path_put(const struct path *path)
{
    pr_todo();
}

void __putname(void *ptr)
{

}

void* __getname(void)
{
    return kmalloc(128, 0);
}
