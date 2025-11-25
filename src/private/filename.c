#include <linux/vfs/fs.h>
#include <linux/vfs/private/filename.h>
#include <linux/vfs/private/namei.h>

#define EMBEDDED_NAME_MAX 128

struct filename *__getname0(void)
{
    return kmalloc(sizeof(struct filename) + PATH_MAX, GFP_KERNEL);
}

void putname(struct filename *n)
{
    pr_todo();
}

int getname_flags(const char __user *filename,
                  int flags, int *empty, struct filename **res)
{
    struct filename *result;
    char *kname;
    int len;

    result = __getname0();
    if (unlikely(!result))
        return -ENOMEM;

    /*
     * First, try to embed the struct filename inside the names_cache
     * allocation
     */
    kname = (char *)result->iname;
    result->name = kname;

    len = strncpy_from_user(kname, filename, EMBEDDED_NAME_MAX);
    if (unlikely(len < 0))
    {
        __putname(result);
        return len;
    }

    /*
     * Uh-oh. We have a name that's approaching PATH_MAX. Allocate a
     * separate struct filename so we can dedicate the entire
     * names_cache allocation for the pathname, and re-do the copy from
     * userland.
     */
    if (unlikely(len == EMBEDDED_NAME_MAX))
    {
        const size_t size = offsetof(struct filename, iname[1]);
        kname = (char *)result;

        /*
         * size is chosen that way we to guarantee that
         * result->iname[0] is within the same object and that
         * kname can't be equal to result->iname, no matter what.
         */
        result = kzalloc(size, GFP_KERNEL);
        if (unlikely(!result))
        {
            __putname(kname);
            return -ENOMEM;
        }
        result->name = kname;
        len = strncpy_from_user(kname, filename, PATH_MAX);
        if (unlikely(len < 0))
        {
            __putname(kname);
            kfree(result);
            return len;
        }
        if (unlikely(len == PATH_MAX))
        {
            __putname(kname);
            kfree(result);
            return -ENAMETOOLONG;
        }
    }

    result->refcnt = 1;
    /* The empty path is special. */
    if (unlikely(!len))
    {
        if (empty)
            *empty = 1;
        if (!(flags & LOOKUP_EMPTY))
        {
            putname(result);
            return -ENOENT;
        }
    }

    result->uptr = filename;
    result->aname = NULL;
    *res = result;

    return 0;
}

int getname(const char __user *name, struct filename **res)
{
    return getname_flags(name, 0, NULL, res);
}

int filename_get(const char *name)
{

    return 0;
}
