#ifndef ARCH_RISCV64_RISCV_INTRIN_H
#define ARCH_RISCV64_RISCV_INTRIN_H

// Took from xv6.

// @todo Replace uint64_t with uintptr_t where needed.

#include "kern/arch/sysdef.h"

static inline uint64_t pz_arch_intrin_read_mhartid()
{
    uint64_t x;
    asm volatile("csrr %0, mhartid" : "=r"(x));
    return x;
}

#define PZ_ARCH_INTRIN_MSTATUS_MPP_MASK (3L << 11) // previous mode.
#define PZ_ARCH_INTRIN_MSTATUS_MPP_M (3L << 11)
#define PZ_ARCH_INTRIN_MSTATUS_MPP_S (1L << 11)
#define PZ_ARCH_INTRIN_MSTATUS_MPP_U (0L << 11)
#define PZ_ARCH_INTRIN_MSTATUS_MIE (1L << 3) // machine-mode interrupt enable.

static inline uint64_t pz_arch_intrin_read_mstatus()
{
    uint64_t x;
    asm volatile("csrr %0, mstatus" : "=r"(x));
    return x;
}

static inline void pz_arch_intrin_write_mstatus(uint64_t x)
{
    asm volatile("csrw mstatus, %0" : : "r"(x));
}

static inline void pz_arch_intrin_write_mepc(uint64_t x)
{
    asm volatile("csrw mepc, %0" : : "r"(x));
}

// Previous mode, 1=Supervisor, 0=User
#define PZ_ARCH_INTRIN_SSTATUS_SPP (1L << 8)
// Supervisor Previous Interrupt Enable
#define PZ_ARCH_INTRIN_SSTATUS_SPIE (1L << 5)
#define PZ_ARCH_INTRIN_SSTATUS_UPIE (1L << 4) // User Previous Interrupt Enable
#define PZ_ARCH_INTRIN_SSTATUS_SIE (1L << 1) // Supervisor Interrupt Enable
#define PZ_ARCH_INTRIN_SSTATUS_UIE (1L << 0) // User Interrupt Enable

static inline uint64_t pz_arch_intrin_read_sstatus()
{
    uint64_t x;
    asm volatile("csrr %0, sstatus" : "=r"(x));
    return x;
}

static inline void pz_arch_intrin_write_sstatus(uint64_t x)
{
    asm volatile("csrw sstatus, %0" : : "r"(x));
}

static inline uint64_t pz_arch_intrin_read_sip()
{
    uint64_t x;
    asm volatile("csrr %0, sip" : "=r"(x));
    return x;
}

static inline void pz_arch_intrin_write_sip(uint64_t x)
{
    asm volatile("csrw sip, %0" : : "r"(x));
}

#define PZ_ARCH_INTRIN_SIE_SEIE (1L << 9) // external
#define PZ_ARCH_INTRIN_SIE_STIE (1L << 5) // timer
#define PZ_ARCH_INTRIN_SIE_SSIE (1L << 1) // software

static inline uint64_t pz_arch_intrin_read_sie()
{
    uint64_t x;
    asm volatile("csrr %0, sie" : "=r"(x));
    return x;
}

static inline void pz_arch_intrin_write_sie(uint64_t x)
{
    asm volatile("csrw sie, %0" : : "r"(x));
}

#define PZ_ARCH_INTRIN_MIE_MEIE (1L << 11) // external
#define PZ_ARCH_INTRIN_MIE_MTIE (1L << 7) // timer
#define PZ_ARCH_INTRIN_MIE_MSIE (1L << 3) // software

static inline uint64_t pz_arch_intrin_read_mie()
{
    uint64_t x;
    asm volatile("csrr %0, mie" : "=r"(x));
    return x;
}

static inline void pz_arch_intrin_write_mie(uint64_t x)
{
    asm volatile("csrw mie, %0" : : "r"(x));
}

static inline void pz_arch_intrin_write_sepc(uint64_t x)
{
    asm volatile("csrw sepc, %0" : : "r"(x));
}

static inline uint64_t pz_arch_intrin_read_sepc()
{
    uint64_t x;
    asm volatile("csrr %0, sepc" : "=r"(x));
    return x;
}

static inline uint64_t pz_arch_intrin_read_medeleg()
{
    uint64_t x;
    asm volatile("csrr %0, medeleg" : "=r"(x));
    return x;
}

static inline void pz_arch_intrin_write_medeleg(uint64_t x)
{
    asm volatile("csrw medeleg, %0" : : "r"(x));
}

static inline uint64_t pz_arch_intrin_read_mideleg()
{
    uint64_t x;
    asm volatile("csrr %0, mideleg" : "=r"(x));
    return x;
}

static inline void pz_arch_intrin_write_mideleg(uint64_t x)
{
    asm volatile("csrw mideleg, %0" : : "r"(x));
}

static inline void w_stvec(uint64_t x)
{
    asm volatile("csrw stvec, %0" : : "r"(x));
}

static inline uint64_t pz_arch_intrin_read_stvec()
{
    uint64_t x;
    asm volatile("csrr %0, stvec" : "=r"(x));
    return x;
}

static inline void pz_arch_intrin_write_mtvec(uint64_t x)
{
    asm volatile("csrw mtvec, %0" : : "r"(x));
}

static inline void pz_arch_intrin_write_pmpcfg0(uint64_t x)
{
    asm volatile("csrw pmpcfg0, %0" : : "r"(x));
}

static inline void pz_arch_intrin_write_pmpaddr0(uint64_t x)
{
    asm volatile("csrw pmpaddr0, %0" : : "r"(x));
}

#define PZ_ARCH_INTRIN_SATP_SV39 (8L << 60)
#define PZ_ARCH_INTRIN_MAKE_SATP(pagetable) \
    (PZ_ARCH_INTRIN_SATP_SV39 | (((uint64_t)pagetable) >> 12))

static inline void pz_arch_intrin_write_satp(uint64_t x)
{
    asm volatile("csrw satp, %0" : : "r"(x));
}

static inline uint64_t pz_arch_intrin_read_satp()
{
    uint64_t x;
    asm volatile("csrr %0, satp" : "=r"(x));
    return x;
}

static inline void pz_arch_intrin_write_mscratch(uint64_t x)
{
    asm volatile("csrw mscratch, %0" : : "r"(x));
}

static inline uint64_t pz_arch_intrin_read_scause()
{
    uint64_t x;
    asm volatile("csrr %0, scause" : "=r"(x));
    return x;
}

static inline uint64_t pz_arch_intrin_read_stval()
{
    uint64_t x;
    asm volatile("csrr %0, stval" : "=r"(x));
    return x;
}

static inline void pz_arch_intrin_write_mcounteren(uint64_t x)
{
    asm volatile("csrw mcounteren, %0" : : "r"(x));
}

static inline uint64_t pz_arch_intrin_read_mcounteren()
{
    uint64_t x;
    asm volatile("csrr %0, mcounteren" : "=r"(x));
    return x;
}

static inline uint64_t pz_arch_intrin_read_time()
{
    uint64_t x;
    asm volatile("csrr %0, time" : "=r"(x));
    return x;
}

static inline void pz_arch_intrin_intr_on()
{
    uint64_t s = pz_arch_intrin_read_sstatus() | PZ_ARCH_INTRIN_SSTATUS_SIE;
    pz_arch_intrin_write_sstatus(s);
}

static inline void pz_arch_intrin_read_intr_off()
{
    uint64_t s = pz_arch_intrin_read_sstatus() & ~PZ_ARCH_INTRIN_SSTATUS_SIE;
    pz_arch_intrin_write_sstatus(s);
}

static inline int pz_arch_intrin_intr_get()
{
    uint64_t x = pz_arch_intrin_read_sstatus();
    return (x & PZ_ARCH_INTRIN_SSTATUS_SIE) != 0;
}

static inline uint64_t pz_arch_intrin_read_sp()
{
    uint64_t x;
    asm volatile("mv %0, sp" : "=r"(x));
    return x;
}

static inline uint64_t pz_arch_intrin_read_tp()
{
    uint64_t x;
    asm volatile("mv %0, tp" : "=r"(x));
    return x;
}

static inline void pz_arch_intrin_write_tp(uint64_t x)
{
    asm volatile("mv tp, %0" : : "r"(x));
}

static inline uint64_t pz_arch_intrin_read_ra()
{
    uint64_t x;
    asm volatile("mv %0, ra" : "=r"(x));
    return x;
}

static inline void pz_arch_intrin_sfence_vma()
{
    // Zero means flush all TLB entries.
    asm volatile("sfence.vma zero, zero");
}

PZ_ATTR_NORETURN static inline void pz_arch_intrin_mret()
{
    asm volatile("mret");
    __builtin_unreachable();
}

#endif // !ARCH_RISCV64_RISCV_INTRIN_H
