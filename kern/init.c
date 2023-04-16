#include "kern/init.h"

#include "kern/mm.h"
#include "kern/sched.h"
#include "kern/uart.h"
#include "kern/printf.h"

#include "kern/sysdef.h"

// Defined in `start.S`
extern void pz_arch_irq_enable_all(void);

static void pz_init();
static void pz_start_kern_vm(uintptr_t kstack_top);

// May be defined by user.
int ATTR_WEAK pz_umain()
{
    return 42;
}

void pz_start_kern() {
    pz_sys_uart_init();

    printf("Starting the system...\n");

    pz_mm_init_and_run(pz_start_kern_vm);
}

static void pz_start_kern_vm(uintptr_t kstack_top)
{
    printf("VM initialized.\n");
    (void)kstack_top;

    pz_sch_init();
    printf("Kernel Scheduler initialized.\n");

    pz_arch_irq_enable_all();
    printf("IRQ enabled.\n");

    // Initialize the subsystems.
    // Scheduler, VM, FS, TTY
    // fork() with init()

    printf("Starting the init process...\n");
    // fork()
    pz_init();
}

static void pz_init(void)
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
