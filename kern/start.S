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

    la sp, __stack_end

    jal clear_bss
    jal pz_sys_start
_loop:
    wfi
    j _loop

clear_bss:
    la t0, __bss_start
    la t1, __bss_end
clear_bss_loop:
    sw zero, 0(t0)
    addi t0, t0, 4
    blt t0, t1, clear_bss_loop
    ret

    .global pz_sys_irq_set_handler
pz_sys_irq_set_handler:
    csrw mtvec, a0
    ret

    .global pz_sys_irq_enable_all
pz_sys_irq_enable_all:
    csrs mstatus, 0x8
    ret

    .global pz_sys_irq_disable_all
pz_sys_irq_disable_all:
    IRQ_DISABLE_ALL
    ret
