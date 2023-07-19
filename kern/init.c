#include "kern/init.h"

#include "kern/arch/init.h"

#include "kern/kalloc.h"
#include "kern/mm.h"
#include "kern/sched.h"
#include "kern/uart.h"
#include "kern/printf.h"

#include "kern/sysdef.h"

static void pz_init();
static void pz_start_kern_vm(void);

// May be defined by user.
int ATTR_WEAK pz_umain(void)
{
    return 42;
}

void pz_start_kern()
{
    printf("Loading kernel...\n");

    pz_arch_start_kern(pz_start_kern_vm);
}

static void pz_start_kern_vm(void)
{
    printf("Initializing allocator...\n");
    pz_alloc_init();

    printf("Initializing VM...\n");
    pz_mm_init();
    pz_mm_init_cpu();

    printf("Initializing scheduler...\n");
    pz_sch_init();

    printf("Starting init process...\n");
    // fork()
    pz_init();
}

static void pz_init(void)
{
    // Open /dev/tty1 for stdin, stdout, stderr and run /bin/init, or
    // /bin/sh

    (void)pz_umain();
    printf("umain() returned\n");

    while (1) {
        // @todo: pz_arch_wfi();
    }
}
