#include <kern/arch/console.h>

#include "sbi.h"

void pz_arch_console_putchar(int ch)
{
    pz_arch_sbi_console_putchar(ch);
}
