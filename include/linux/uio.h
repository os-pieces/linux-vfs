#pragma once

#include <linux/kernel.h>
#include <linux/types.h>

#define ITER_SOURCE 1 // == WRITE
#define ITER_DEST 0   // == READ

enum iter_type
{
    /* iter types */
    ITER_UBUF,
    ITER_IOVEC,
    ITER_BVEC,
    ITER_KVEC,
    ITER_FOLIOQ,
    ITER_XARRAY,
    ITER_DISCARD,
};

struct iov_iter
{
    void *ubuf;
    size_t iov_offset;
    union
    {
        struct
        {
            size_t count;
        };
    };

    unsigned char iter_type;
};

static inline size_t iov_iter_count(const struct iov_iter *i)
{
    return i->count;
}

size_t copy_from_iter(void *addr, size_t bytes, struct iov_iter *i);
size_t copy_to_iter(const void *addr, size_t bytes, struct iov_iter *i);

static inline void iov_iter_ubuf(struct iov_iter *i, unsigned int direction,
                                 void __user *buf, size_t count)
{
    struct iov_iter iter = {
        .ubuf = buf,
        .count = count,
        .iter_type = ITER_UBUF,
    };

    *i = iter;
}
