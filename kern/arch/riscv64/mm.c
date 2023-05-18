#include "kern/arch/mm.h"

#include "kern/arch/intrin.h"
#include "kern/arch/sysdef.h"

void pz_arch_mm_set_root_pg_tbl(void *root_tbl)
{
    pz_arch_intrin_sfence_vma();
    pz_arch_intrin_write_satp(PZ_ARCH_INTRIN_MAKE_SATP(root_tbl));
    pz_arch_intrin_sfence_vma();
}
