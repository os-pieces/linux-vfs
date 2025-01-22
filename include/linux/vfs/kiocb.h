#pragma once

struct kiocb
{
    struct file *ki_filp;
    loff_t ki_pos;
};

static inline void init_sync_kiocb(struct kiocb *kiocb, struct file *filp)
{
    kiocb->ki_filp = filp;
    kiocb->ki_pos = 0;
}
