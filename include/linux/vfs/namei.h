#pragma once

#define LOOKUP_CREATE 0x0200        /* ... in object creation */
#define LOOKUP_RCU 0x0040           /* RCU pathwalk mode; semi-internal */
#define LOOKUP_RENAME_TARGET 0x0800 /* ... in destination of rename() */

void done_path_create(struct path *path, struct dentry *dentry);
