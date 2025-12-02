#pragma once

#include <linux/wait.h>

struct poll_table_struct;

/*
 * structures and helpers for f_op->poll implementations
 */
typedef void (*poll_queue_proc)(struct file *, wait_queue_head_t *, struct poll_table_struct *);

typedef unsigned __poll_t;

/*
 * Do not touch the structure directly, use the access function
 * poll_requested_events() instead.
 */
typedef struct poll_table_struct
{
    poll_queue_proc _qproc;
    unsigned long _key;
} poll_table;

static inline void poll_wait(struct file *filp, wait_queue_head_t *wait_address, poll_table *p)
{
    if (p && p->_qproc)
    {
        p->_qproc(filp, wait_address, p);
        /*
         * This memory barrier is paired in the wq_has_sleeper().
         * See the comment above prepare_to_wait(), we need to
         * ensure that subsequent tests in this thread can't be
         * reordered with __add_wait_queue() in _qproc() paths.
         */
        smp_mb();
    }
}
