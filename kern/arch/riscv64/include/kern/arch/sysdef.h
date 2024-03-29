#ifndef ARCH_RISCV64_SYSDEF_H
#define ARCH_RISCV64_SYSDEF_H

#include <stddef.h>
#include <stdint.h>

#ifndef __INT64_TYPE__
typedef long long int uint64_t;
#endif
typedef volatile uint64_t vaddr_t;

#define asm __asm__

#define PZ_ATTR_NORETURN __attribute__((noreturn))
#define PZ_ATTR_UNREACHABLE __builtin_unreachable

static inline void pz_write32(uintptr_t addr, uint32_t value)
{
    *((volatile uint32_t *)(addr)) = value;
}

static inline uint32_t pz_read32(const uintptr_t addr)
{
    return *((volatile uint32_t *)(addr));
}

#endif // !ARCH_RISCV64_SYSDEF_H
