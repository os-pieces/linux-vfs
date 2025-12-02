#pragma once

struct audit_names;

struct filename
{
    const char *name;        /* pointer to actual string */
    const __user char *uptr; /* original userland pointer */
    struct audit_names *aname;
    atomic_t refcnt;
    const char iname[];
};

int getname_flags(const char __user *filename,
                  int flags, struct filename **res);
void putname(struct filename *name);
