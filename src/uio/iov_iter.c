#include <linux/uio.h>
#include <linux/uaccess.h>

// TODO

typedef size_t (*iov_step_f)(void *iter_base, size_t progress, size_t len,
                             void *priv, void *priv2);
typedef size_t (*iov_ustep_f)(void __user *iter_base, size_t progress, size_t len,
                              void *priv, void *priv2);

static inline enum iter_type iov_iter_type(const struct iov_iter *i)
{
    return i->iter_type;
}

static inline bool iter_is_ubuf(const struct iov_iter *i)
{
    return iov_iter_type(i) == ITER_UBUF;
}

static size_t copy_from_user_iter(void __user *iter_from, size_t progress,
                                  size_t len, void *to, void *priv2)
{
    size_t res = len;

    if (1)
    {
        res = raw_copy_from_user(to, iter_from, len);
    }

    return res;
}

static size_t memcpy_from_iter(void *iter_from, size_t progress,
                               size_t len, void *to, void *priv2)
{
    memcpy(to + progress, iter_from, len);

    return 0;
}

static inline size_t iterate_ubuf(struct iov_iter *iter, size_t len, void *priv, void *priv2,
                                  iov_ustep_f step)
{
    void __user *base = iter->ubuf;
    size_t progress = 0, remain;

    remain = step(base + iter->iov_offset, 0, len, priv, priv2);
    progress = len - remain;
    iter->iov_offset += progress;
    iter->count -= progress;

    return progress;
}

static inline size_t iterate_and_advance2(struct iov_iter *iter, size_t len, void *priv,
                                          void *priv2, iov_ustep_f ustep, iov_step_f step)
{
    size_t ret;

    if (unlikely(iter->count < len))
        len = iter->count;

    if (unlikely(!len))
    {
        ret = 0;
    }
    else if (iter_is_ubuf(iter))
    {
        ret = iterate_ubuf(iter, len, priv, priv2, ustep);
    }

    return ret;
}

static inline size_t iterate_and_advance(struct iov_iter *iter, size_t len, void *priv,
                                         iov_ustep_f ustep, iov_step_f step)
{
    return iterate_and_advance2(iter, len, priv, NULL, ustep, step);
}

static inline size_t __copy_from_iter(void *addr, size_t bytes, struct iov_iter *i)
{
    return iterate_and_advance(i, bytes, addr,
                               copy_from_user_iter, memcpy_from_iter);
}

size_t copy_from_iter(void *addr, size_t bytes, struct iov_iter *i)
{
    size_t ret;

    ret = __copy_from_iter(addr, bytes, i);

    return ret;
}

size_t copy_to_iter(const void *addr, size_t bytes, struct iov_iter *i)
{
    return 0;
}
