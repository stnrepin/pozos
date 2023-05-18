#include "kern/kalloc.h"

#include "kern/arch/mm.h"

#include "kern/mm.h"
#include "kern/printf.h"
#include "kern/string.h"

struct alloc_cb {
    struct alloc_cb *next;
};

struct alloc_mem {
    struct alloc_cb *freelist;
};

static struct alloc_mem g_alloc_mem;

// The end of the kernel data. Set by linker.
extern char _mem_end[];

static void free_range(void *pa_start, void *pa_end);

void pz_alloc_init()
{
    free_range(_mem_end, _mem_end + PZ_MEM_SIZE);
}

void *pz_alloc_pnew()
{
    struct alloc_cb *cb;

    // @todo locking
    cb = g_alloc_mem.freelist;
    if (cb) {
        g_alloc_mem.freelist = cb->next;
        memset((void *)cb, 0xee, PZ_PAGESZ);
    }

    return cb;
}

void pz_alloc_pfree(void *pa)
{
    struct alloc_cb *cb;
    //assert((uintptr_t)pa % PZ_PAGESZ) == 0 && (char *)pa >= g_end &&
    //    (uintptr_t)pa < PZ_MEM_SIZE)

    memset(pa, 0xbb, PZ_PAGESZ);

    // @todo locking
    cb = (struct alloc_cb *)pa;
    cb->next = g_alloc_mem.freelist;
    g_alloc_mem.freelist = cb;
}

static void free_range(void *pa_start, void *pa_end)
{
    char *p = (char *)PZ_ROUNDUP((uint64_t)pa_start, PZ_PAGESZ);
    while (p + PZ_PAGESZ <= (char *)pa_end) {
        pz_alloc_pfree(p);
        p += PZ_PAGESZ;
    }
}
