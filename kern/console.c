#include <kern/arch/console.h>

// For printf-lib.
void _putchar(char c)
{
    pz_arch_console_putchar(c);
}
