#pragma once

struct fd
{
    struct file *file;
    unsigned int flags;
};

int fdget_pos(filedesc_t *fdp, unsigned int fd, struct fd *f);
int fdget_raw(filedesc_t *fdp, unsigned int fd, struct fd *f);
int fdget(filedesc_t *fdp, unsigned int fd, struct fd *f);
void fdput(struct fd fd);

void file_inc_ref(struct file *f);
extern void __fput_sync(struct file *);

void fdput_pos(filedesc_t *fdp, struct fd f);

struct file *alloc_empty_file(int flags);
void fput_close_sync(struct file *file);
