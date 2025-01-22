#pragma once

void set_fs_root(filedesc_t *fdp, const struct path *path);
void get_fs_root(filedesc_t *fdp, struct path *root);
void set_fs_pwd(filedesc_t *fdp, const struct path *path);
