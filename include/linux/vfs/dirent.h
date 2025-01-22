#pragma once

struct dirent
{
    unsigned long d_ino;
    unsigned long d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char d_name[256];
};
