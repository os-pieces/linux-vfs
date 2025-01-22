#pragma once

#define EMBEDDED_LEVELS 2
struct nameidata
{
    struct path path;
    struct path root;
    struct qstr last;
    unsigned flags;
    unsigned state;
    int last_type;
    struct inode *inode;
    unsigned depth;
    int total_link_count;
    struct saved
    {
        struct path link;
        struct delayed_call done;
        const char *name;
        unsigned seq;
    } *stack, internal[EMBEDDED_LEVELS];
    umode_t dir_mode;
    vfsuid_t dir_vfsuid;
    int dfd;
    struct filename *name;
    struct nameidata *saved;
    unsigned seq, next_seq, m_seq, r_seq;
    unsigned root_seq;
    filedesc_t *filedesc;
};

#define ND_ROOT_PRESET 1
#define ND_ROOT_GRABBED 2
#define ND_JUMPED 4

enum
{
    WALK_TRAILING = 1,
    WALK_MORE = 2,
    WALK_NOFOLLOW = 4
};
