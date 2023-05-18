#ifndef ARCH_RISCV64_MM_H
#define ARCH_RISCV64_MM_H

#define PZ_PAGE_SHIFT 12
#define PZ_PAGESZ (1 << PZ_PAGE_SHIFT)

#ifndef PZ_ASSEMBLY

#include "kern/arch/sysdef.h"

#define PZ_ARCH_MM_PAGE_V (1L << 0)
#define PZ_ARCH_MM_PAGE_R (1L << 1)
#define PZ_ARCH_MM_PAGE_W (1L << 2)
#define PZ_ARCH_MM_PAGE_X (1L << 3)

#define PZ_ARCH_MM_PA2PTE(pa) ((((uint64_t)pa) >> 12) << 10)
#define PZ_ARCH_MM_PTE2PA(pte) (((pte) >> 10) << 12)
#define PZ_ARCH_MM_MAX_VA (1L << (9 + 9 + 9 + 12 - 1))

#define PZ_ARCH_MM_GET_LVL_MASK 0x1ff
#define PZ_ARCH_MM_GET_LVL_SHIFT(lvl) (PZ_PAGE_SHIFT + (9 * (lvl)))
#define PZ_ARCH_MM_GET_LVL(lvl, va)                        \
    ((((uint64_t)(va)) >> PZ_ARCH_MM_GET_LVL_SHIFT(lvl)) & \
     PZ_ARCH_MM_GET_LVL_MASK)

void pz_arch_mm_set_root_pg_tbl(void *root_tbl);
void pz_arch_set_trap(uintptr_t entry_f);

#endif // !PZ_ASSEMBLY

#endif // !ARCH_RISCV64_MM_H
