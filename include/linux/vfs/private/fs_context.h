#pragma once

extern int parse_monolithic_mount_data(struct fs_context *, void *);
bool mount_capable(struct fs_context *fc);
void put_fs_context(struct fs_context *fc);
