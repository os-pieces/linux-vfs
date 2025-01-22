#pragma once

#include <linux/maple_tree.h>

struct offset_ctx
{
    struct maple_tree mt;
    uintptr_t next_offset;
};
