#ifndef SYS_UART_H
#define SYS_UART_H

void pz_sys_uart_init(void);
void pz_sys_uart_putc(char c);
void pz_sys_uart_puts(const char* s);

#endif // !SYS_UART_H
