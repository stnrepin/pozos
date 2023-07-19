#ifndef ARCH_RISCV64_SBI_H
#define ARCH_RISCV64_SBI_H

#include <kern/arch/sysdef.h>

#define PZ_ARCH_SBI_CONSOLE_PUTCHAR 1
#define PZ_ARCH_SBI_SHUTDOWN 8

static inline uint64_t pz_arch_sbi_call(uint64_t cmd, uint64_t arg_0,
                                        uint64_t arg_1, uint64_t arg_2)
{
    register uint64_t a0 asm("a0") = arg_0;
    register uint64_t a1 asm("a1") = arg_1;
    register uint64_t a2 asm("a2") = arg_2;
    register uint64_t a7 asm("a7") = cmd;
    register uint64_t result asm("a0");
    asm volatile("ecall"
                 : "=r"(result)
                 : "r"(a0), "r"(a1), "r"(a2), "r"(a7)
                 : "memory");
    return result;
}

static inline uint64_t pz_arch_sbi_call1(uint64_t cmd, uint64_t arg0)
{
    return pz_arch_sbi_call(cmd, arg0, 0, 0);
}

static inline uint64_t pz_arch_sbi_call0(uint64_t cmd)
{
    return pz_arch_sbi_call1(cmd, 0);
}

static inline void pz_arch_sbi_console_putchar(int ch)
{
    pz_arch_sbi_call1(PZ_ARCH_SBI_CONSOLE_PUTCHAR, ch);
}

static inline void pz_arch_sbi_shutdown(void)
{
    pz_arch_sbi_call0(PZ_ARCH_SBI_SHUTDOWN);
}

#endif // !ARCH_RISCV64_SBI_H
