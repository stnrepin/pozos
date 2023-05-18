#define PZ_ASSEMBLY
#include "kern/arch/mm.h"

    .section .init
    .global _start
_start:
    # sp = g_stacks + (mhardid * PZ_PAGESZ)
    la sp, g_stacks
    li a0, PZ_PAGESZ
    csrr a1, mhartid
    addi a1, a1, 1
    mul a0, a0, a1
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
