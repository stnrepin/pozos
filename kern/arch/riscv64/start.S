#include "kern/arch/mm.h"

.macro IRQ_DISABLE_ALL
    csrrci x0, mstatus, 0x8
.endm

    .global _start
_start:
    IRQ_DISABLE_ALL
    csrw mie, zero

.option push
.option norelax
    la t0, __global_pointer$
    ld gp, (t0)
.option pop

    la sp, stacks + PZ_PAGESZ - PZ_MENTRY_FRAME_SIZE

    jal clear_bss
    jal pz_start_kern
_loop:
    wfi
    j _loop

clear_bss:
    la t0, _fbss
    la t1, _bss_end
clear_bss_loop:
    sw zero, 0(t0)
    addi t0, t0, 4
    blt t0, t1, clear_bss_loop
    ret

    .global pz_arch_irq_set_handler
pz_arch_irq_set_handler:
    csrw mtvec, a0
    ret

    .global pz_arch_irq_enable_all
pz_arch_irq_enable_all:
    csrs mstatus, 0x8
    ret

    .global pz_arch_irq_disable_all
pz_arch_irq_disable_all:
    IRQ_DISABLE_ALL
    ret

    .bss
    .align PZ_PAGESZ
stacks:
    .skip PZ_PAGESZ * 1
