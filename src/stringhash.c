#include <linux/types.h>
#include <linux/stringhash.h>

unsigned int full_name_hash(const void *salt, const char *name, unsigned int len)
{
    uintptr_t hash = init_name_hash(salt);

    while (len--)
        hash = partial_name_hash((unsigned char)*name++, hash);

    return end_name_hash(hash);
}

u64 hashlen_string(const void *salt, const char *name)
{
    uintptr_t hash = init_name_hash(salt);
    unsigned long len = 0, c;

    c = (unsigned char)*name;
    while (c)
    {
        len++;
        hash = partial_name_hash(c, hash);
        c = (unsigned char)name[len];
    }

    return hashlen_create(end_name_hash(hash), len);
}
