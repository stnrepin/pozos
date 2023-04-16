#ifndef ARCH_RISCV64_SYSDEF_H
#define ARCH_RISCV64_SYSDEF_H

typedef unsigned long long uint64_t;
typedef volatile uint64_t vaddr_t;
typedef uint64_t uintptr_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef uint64_t size_t;

#define asm __asm__

static inline void pz_write32(vaddr_t addr, uint32_t value)
{
    *((volatile uint32_t *)(addr)) = value;
}

static inline uint32_t pz_read32(const vaddr_t addr)
{
    return *((volatile uint32_t *)(addr));
}

static inline uint64_t pz_sys_ticks(void)
{
    uint64_t cnt;
    __asm__ volatile("csrr %0, time\n" : "=r"(cnt)::"memory");
    return cnt;
}

static inline void pz_sys_delay(uint64_t us)
{
    uint64_t t1 = pz_sys_ticks();
    uint64_t t2 = t1 + us * 24;
    do {
        t1 = pz_sys_ticks();
    } while (t2 >= t1);
}

#endif // !ARCH_RISCV64_SYSDEF_H