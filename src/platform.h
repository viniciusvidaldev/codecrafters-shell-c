#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef unix
#include <linux/limits.h> // IWYU pragma: export
#else
#include <sys/syslimits.h> // IWYU pragma: export
#endif

#endif
