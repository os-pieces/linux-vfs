#pragma once

struct audit_names;
struct filename {
	const char		*name;	/* pointer to actual string */
	const __user char	*uptr;	/* original userland pointer */
	int			refcnt;
	struct audit_names	*aname;
	const char		iname[];
};

struct filename *getname_flags(const char __user *filename, 
                               int flags, int *empty);
void putname(struct filename *name);
