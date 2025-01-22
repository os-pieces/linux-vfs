#pragma once

/*
 * This is the "filldir" function type, used by readdir() to let
 * the kernel specify what kind of dirent layout it wants to have.
 * This allows the kernel to read directories into kernel space or
 * to have different dirent layouts depending on the binary type.
 * Return 'true' to keep going and 'false' if there are no more entries.
 */
struct dir_context;
typedef bool (*filldir_t)(struct dir_context *, const char *, int, loff_t, u64,
                          unsigned);

struct dir_context
{
    filldir_t actor;
    loff_t pos;
};

static inline bool dir_emit(struct dir_context *ctx,
                            const char *name, int namelen,
                            u64 ino, unsigned type)
{
    return ctx->actor(ctx, name, namelen, ctx->pos, ino, type);
}
