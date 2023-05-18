#ifndef SYS_UART_H
#define SYS_UART_H

#include "kern/sysdef.h"

void pz_sys_uart_init(void);
uintptr_t pz_sys_uart_base(void);
void pz_sys_uart_putc(char c);
void pz_sys_uart_puts(const char *s);

#endif // !SYS_UART_H
