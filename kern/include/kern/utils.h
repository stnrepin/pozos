#ifndef SYS_UTILS_H
#define SYS_UTILS_H

#include "kern/sysdef.h"

PZ_ATTR_NORETURN void pz_panic(const char *mes);
PZ_ATTR_NORETURN void pz_shutdown(void);

#endif // !SYS_UTILS_H
