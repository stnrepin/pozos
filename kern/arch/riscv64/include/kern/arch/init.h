#ifndef ARCH_RISCV64_SYSDEF_h
#define ARCH_RISCV64_SYSDEF_h

#include "kern/arch/sysdef.h"

void pz_arch_enter_privilege(void (*f)(uintptr_t), uintptr_t arg0);

#endif // !ARCH_RISCV64_SYSDEF_h
