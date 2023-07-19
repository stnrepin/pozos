#include "kern/arch/utils.h"

#include "kern/arch/sysdef.h"

#include "sbi.h"

void pz_arch_shutdown(void)
{
    pz_arch_sbi_shutdown();
    PZ_ATTR_UNREACHABLE();
}
