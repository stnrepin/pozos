#ifndef ARCH_RISCV64_INIT_H
#define ARCH_RISCV64_INIT_H

#include "kern/arch/sysdef.h"

typedef void(start_kern_f)(void);
void pz_arch_start_kern(start_kern_f sk_f);

#endif // !ARCH_RISCV64_INIT_H
