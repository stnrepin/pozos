#include "kern/mm.h"

#include "kern/arch/mm.h"

#include "kern/printf.h"
#include "kern/uart.h"
#include "kern/kalloc.h"
#include "kern/string.h"
#include "kern/sysdef.h"

#ifndef PZ_MEM_START
#warning "PZ_MEM_START is not defined. Use default value (0x80000000)"
#define PZ_MEM_START 0x80000000
#endif

#define KVA_START ((uintptr_t)(-1) << (VA_BITS - 1))

#define PTE_R PZ_ARCH_MM_PAGE_R
#define PTE_W PZ_ARCH_MM_PAGE_W
#define PTE_X PZ_ARCH_MM_PAGE_X
#define PTE_V PZ_ARCH_MM_PAGE_V

typedef uint64_t *pagetbl_ptr_t;
typedef uint64_t pte_t;

struct kvm_info {
    pagetbl_ptr_t ptbl;
};

extern char _text_end[];

static struct kvm_info g_kvm;

static void kmap(uint64_t va, uint64_t pa, uint64_t sz, int perm);
static int map(pagetbl_ptr_t tbl, uint64_t va, uint64_t pa, uint64_t sz,
               int perm);
pte_t *walk(pagetbl_ptr_t tbl, uint64_t va, int alloc);

static inline void panic(const char *mes)
{
    pz_sys_uart_puts(mes);
}

void pz_mm_init(void)
{
    g_kvm.ptbl = (pagetbl_ptr_t)pz_alloc_pnew();

    memset((void *)g_kvm.ptbl, 0, PZ_PAGESZ);

    kmap(pz_sys_uart_base(), pz_sys_uart_base(), PZ_PAGESZ, PTE_R | PTE_W);

    kmap((uint64_t)PZ_MEM_START, PZ_MEM_START,
         (uint64_t)_text_end - PZ_MEM_START, PTE_R | PTE_X);

    kmap((uint64_t)_text_end, (uint64_t)_text_end,
         (PZ_MEM_START + PZ_MEM_SIZE) - (uint64_t)_text_end, PTE_R | PTE_W);
}

void pz_mm_init_cpu(void)
{
    pz_arch_mm_set_root_pg_tbl((void *)g_kvm.ptbl);
}

static void kmap(uint64_t va, uint64_t pa, uint64_t sz, int perm)
{
    if (map(g_kvm.ptbl, va, pa, sz, perm) != 0) {
        panic("vm panic: map failed\n");
    }
}

static int map(pagetbl_ptr_t tbl, uint64_t va, uint64_t pa, uint64_t sz,
               int perm)
{
    int i;
    uint64_t var, last;
    pte_t *pte;

    if (sz == 0) {
        panic("vm panic: map: size == 0\n");
        return -1;
    }

    var = PZ_ROUNDUP(va, PZ_PAGESZ);
    last = PZ_ROUNDDOWN(va + sz - 1, PZ_PAGESZ);

    i = 0;
    while (1) {
        pte = walk(tbl, var, 1);
        if (pte == NULL) {
            return -2;
        }
        if ((*pte & PTE_V) != 0) {
            printf("i=%d, pa=%lx, var=%lx\n", i, pa, var);
            panic("vm panic: map: already created PTE\n");
            return -3;
        }
        *pte = PZ_ARCH_MM_PA2PTE(pa) | perm | PTE_V;
        if (var == last) {
            break;
        }

        var += PZ_PAGESZ;
        pa += PZ_PAGESZ;
        i++;
    }

    return 0;
}

pte_t *walk(pagetbl_ptr_t tbl, uint64_t va, int alloc)
{
    int lvl;
    pte_t *pte;

    if (va >= PZ_ARCH_MM_MAX_VA) {
        panic("vm panic: walk: va too large\n");
    }

    for (lvl = 2; lvl > 0; lvl--) {
        pte = &tbl[PZ_ARCH_MM_GET_LVL(lvl, va)];
        if (*pte & PTE_V) {
            tbl = (pagetbl_ptr_t)PZ_ARCH_MM_PTE2PA(*pte);
        } else {
            if (!alloc) {
                return 0;
            }
            tbl = (pagetbl_ptr_t)pz_alloc_pnew();
            if (tbl == 0) {
                return 0;
            }
            memset((void *)tbl, 0, PZ_PAGESZ);
            *pte = PZ_ARCH_MM_PA2PTE(tbl) | PTE_V;
        }
    }

    return &tbl[PZ_ARCH_MM_GET_LVL(0, va)];
}
