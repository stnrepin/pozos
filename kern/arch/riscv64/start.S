#define PZ_ASSEMBLY
#include "kern/arch/mm.h"

    .section .init
    .global _start
_start:
    # Setup stacks
    #   sp = g_stacks + (mhardid * PZ_PAGESZ)
    #   a6: hart id (SBI)
    mv t1, a6
    addi t1, t1, 1
    li t0, PZ_PAGESZ
    mul t0, t0, t1
    la sp, g_stacks
    add sp, sp, a0

    jal _clear_bss
    jal pz_start_kern
_loop:
    wfi
    j _loop

_clear_bss:
    la t0, _bss_start
    la t1, _bss_end
_clear_bss_loop:
    sw zero, 0(t0)
    addi t0, t0, 4
    blt t0, t1, _clear_bss_loop
    ret
