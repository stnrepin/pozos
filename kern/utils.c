#include "kern/utils.h"

#include "kern/printf.h"
#include "kern/arch/utils.h"

void pz_panic(const char *mes)
{
    printf("Kernel panic: %s", mes);
    pz_shutdown();
}

void pz_shutdown(void)
{
    printf("System shutdowning...\n");
    pz_arch_shutdown();
}
