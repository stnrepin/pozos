#include "kern/init.h"

#include "kern/sysdef.h"
#include "kern/uart.h"
#include "kern/printf.h"

// Defined in `start.S`
extern void pz_sys_irq_enable_all(void);

// May be defined by user.
int ATTR_WEAK pz_umain()
{
    return 42;
}

void pz_start_kern(void)
{
    pz_sys_uart_init();

    pz_sys_irq_enable_all();

    // Initialize the subsystems.
    // Scheduler, VM, FS, TTY
    // fork() with init()
}

void pz_init(void)
{
    // Open /dev/tty1 for stdin, stdout, stderr and run /bin/init, or
    // /bin/sh

    int rc;

    rc = pz_umain();
    printf("umain() returned code: %d\n", rc);

    while (1) {
        // @todo: pz_arch_wfi();
    }
}
