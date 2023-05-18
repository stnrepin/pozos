#ifndef SYSDEF_H
#define SYSDEF_H

#include "kern/arch/sysdef.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

#define ATTR_WEAK __attribute__((weak))

#define PZ_MIN(a, b) ((a) < (b) ? (a) : (b))
#define PZ_ROUNDUP(a, to) ((((a)-1) / (to) + 1) * (to))
#define PZ_ROUNDDOWN(a, to) (((a) / (to)) * (to))

#endif // !SYSDEF_H
