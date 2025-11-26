#pragma once

#include <linux/kconfig.h>

#ifndef CONFIG_PATH_MAX
#define PATH_MAX 4096
#else
#define PATH_MAX CONFIG_PATH_MAX
#endif

#if (PATH_MAX < 128 || PATH_MAX > 4096)
#error "PATH_MAX must be between 128 and 4096"
#endif
