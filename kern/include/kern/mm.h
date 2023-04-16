#ifndef SYS_MM_H
#define SYS_MM_H

#include "kern/sysdef.h"

uintptr_t pz_mm_init(void);

typedef void(pz_mm_kern_init_vm_f)(uintptr_t);

void pz_mm_init_and_run(pz_mm_kern_init_vm_f *start_f);

#endif // !SYS_MM_H
