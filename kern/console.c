#include "kern/uart.h"

// For printf-lib.
void _putchar(char c)
{
    pz_sys_uart_putc(c);
}
