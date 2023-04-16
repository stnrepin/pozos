#include "kern/mm.h"

#include "kern/arch/mm.h"
#include "kern/arch/init.h"

#include "kern/sysdef.h"

#define MEGAPAGE_SIZE ((uintptr_t)(PAGESZ << PAGE_SHIFT))

#define VA_BITS 39
#define KVA_START ((uintptr_t)(-1) << (VA_BITS - 1))

#ifndef PZ_MEM_START
#warning "PZ_MEM_START is not defined. Use default value (0x80000000)"
#define PZ_MEM_START 0x80000000
#endif

struct freepg_list {
    size_t size;
    struct freepg_list_node *head;
};

struct freepg_list_node {
    uintptr_t addr;
};

struct kern_vm_info {
    uintptr_t kva_to_pa_off;
    uint64_t mem_sz;
    uintptr_t first_free_page;
    uint64_t nb_free_pages;
    size_t next_free_page;

    struct freepg_list freepgs;
};

static struct kern_vm_info g_kvm_info;

static inline uintptr_t pa_to_kva(uintptr_t phys_addr);

static void init_alloc_early(void);
static void *alloc_early(size_t n);

static uintptr_t get_free_page_addr(size_t idx);

void pz_mm_init_and_run(pz_mm_kern_init_vm_f *start_f)
{
    void *start_f_va;
    uintptr_t kstack_top_va;

    kstack_top_va = pa_to_kva(pz_mm_init());
    start_f_va = (void *)pa_to_kva((uintptr_t)start_f);

    pz_arch_enter_privilege(start_f_va, kstack_top_va);
}

static void *alloc_early_align(size_t n, size_t align);

uintptr_t pz_mm_init(void)
{
    uintptr_t kstack_top;

    // XXX: Device dependent
    g_kvm_info.mem_sz = 16 * 1024 * 1024; // 16 MB;

    init_alloc_early();

    g_kvm_info.freepgs.size = g_kvm_info.mem_sz / PZ_PAGESZ;
    g_kvm_info.freepgs.head = alloc_early(g_kvm_info.freepgs.size *
                                          sizeof(struct freepg_list_node));

    root_page_table = (void *)__page_alloc_assert();
    __map_kernel_range(KVA_START, MEM_START, mem_size,
                       PROT_READ | PROT_WRITE | PROT_EXEC);

    flush_tlb();
    write_csr(satp,
              ((uintptr_t)root_page_table >> RISCV_PGSHIFT) | SATP_MODE_CHOICE);

    uintptr_t kernel_stack_top = __page_alloc_assert() + RISCV_PGSIZE;

    // relocate
    g_kvm_info.kva_to_pa_off = KVA_START - PZ_MEM_START;
    g_kvm_info.freepgs.head = (void *)pa_to_kva((uintptr_t)g_kvm_info.freepgs.head);
    //root_page_table = (void *)pa2kva(root_page_table);

    return kstack_top;
}

inline uintptr_t pa_to_kva(uintptr_t phys_addr)
{
    return phys_addr + g_kvm_info.kva_to_pa_off;
}

static void init_alloc_early(void)
{
    uintptr_t user_sz;

    user_sz = -KVA_START;
    g_kvm_info.mem_sz = PZ_MIN(g_kvm_info.mem_sz, user_sz);

    // From linker script.
    // _lkr_kdata_end
    extern char _kernel_data_end;

    uintptr_t last_static_addr = (uintptr_t)&_kernel_data_end;
    g_kvm_info.first_free_page = PZ_ROUNDUP(last_static_addr, PZ_PAGESZ);
    g_kvm_info.nb_free_pages =
            (g_kvm_info.mem_sz - (g_kvm_info.first_free_page - PZ_MEM_START)) /
            PZ_PAGESZ;
}

static void *alloc_early(size_t n)
{
    size_t nb_pages = PZ_ROUNDUP(n, PZ_PAGESZ) / PZ_PAGESZ;
    return alloc_early_align(nb_pages, 1);
}

static void *alloc_early_align(size_t n, size_t align)
{
    size_t skip_pages;
    size_t next_free_pg;
    void *addr;

    next_free_pg = g_kvm_info.next_free_page;

    skip_pages = (align - 1) & -(get_free_page_addr(next_free_pg) / PZ_PAGESZ);
    n += skip_pages;

    if (n + next_free_pg < n || n + next_free_pg > g_kvm_info.nb_free_pages)
        return NULL;

    addr = (void *)get_free_page_addr(next_free_pg + skip_pages);
    next_free_pg += n;
    g_kvm_info.next_free_page = next_free_pg;
    return addr;
}

static uintptr_t get_free_page_addr(size_t idx)
{
    return g_kvm_info.first_free_page + idx * PZ_PAGESZ;
}
