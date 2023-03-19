#include "kern/kernel.h"

#include "kern/sysdef.h"
#include "kern/uart.h"

// Defined in `start.S`
extern void pz_sys_irq_enable_all(void);

// May be defined by user.
int __attribute__((weak)) pz_user_main()
{
    return 0;
}

void pz_sys_init(void)
{
    pz_sys_uart_init();

    pz_sys_irq_enable_all();
}

int pz_sys_start(void)
{
    pz_sys_init();

    (void)pz_user_main();

    while (1) {
    }

    return 0;
}

// For printf-lib.
void _putchar(char c)
{
    pz_sys_uart_putc(c);
}
