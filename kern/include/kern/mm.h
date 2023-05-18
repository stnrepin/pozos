#ifndef SYS_MM_H
#define SYS_MM_H

#include "kern/sysdef.h"

#define PZ_MEM_SIZE (32 * 1024 * 1024)

void pz_mm_init(void);
void pz_mm_init_cpu(void);

#endif // !SYS_MM_H
