#pragma once

enum writeback_sync_modes
{
    WB_SYNC_NONE, /* Don't wait on anything */
    WB_SYNC_ALL,  /* Wait on every mapping */
};

struct writeback_control
{
    enum writeback_sync_modes sync_mode;
};
