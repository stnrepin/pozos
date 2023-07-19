#include "kern/arch/init.h"

#include "kern/arch/sysdef.h"
#include "kern/arch/mm.h"
#include "kern/arch/intrin.h"
#include "kern/arch/console.h"

#define PZ_MAXCPU 4

__attribute__((aligned(16))) char g_stacks[PZ_PAGESZ * PZ_MAXCPU];

void pz_arch_start_kern(start_kern_f sk_f)
{
    sk_f();
}
