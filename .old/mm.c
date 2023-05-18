#include "kern/mm.h"

#include "kern/arch/init.h"
#include "kern/arch/mm.h"
#include "kern/printf.h"

#include "kern/sysdef.h"

#define VA_BITS 39
#define KVA_START ((uintptr_t)(-1) << (VA_BITS - 1))

#define PAGELVLS ((VA_BITS - PZ_PAGE_SHIFT) / PZ_PAGELVL_BITS)

#ifndef PZ_MEM_START
#warning "PZ_MEM_START is not defined. Use default value (0x80000000)"
#define PZ_MEM_START 0x80000000
#endif

#define PROT_NONE 0
#define PROT_READ 1
#define PROT_WRITE 2
#define PROT_EXEC 4

#define PTE_PPN_SHIFT 10

#define PTE_V 0x001 /* Valid */
#define PTE_R 0x002 /* Read */
#define PTE_W 0x004 /* Write */
#define PTE_X 0x008 /* Execute */
#define PTE_U 0x010 /* User */
#define PTE_G 0x020 /* Global */
#define PTE_A 0x040 /* Accessed */
#define PTE_D 0x080 /* Dirty */
#define PTE_SOFT 0x300 /* Reserved for Software */
#define PTE_RSVD 0x1FC0000000000000 /* Reserved for future standard use */
#define PTE_PBMT 0x6000000000000000 /* Svpbmt: Page-based memory types */
#define PTE_N 0x8000000000000000 /* Svnapot: NAPOT translation contiguity */
#define PTE_ATTR 0xFFC0000000000000 /* All attributes and reserved bits */

typedef uintptr_t pte_t;

struct freepg_list {
    size_t sz;
    size_t total_sz;
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

    void *root_pg_tbl;
};

static struct kern_vm_info g_kvm_info;

static inline uintptr_t pa_to_kva(uintptr_t phys_addr);

static void init_alloc_early(void);
static uintptr_t alloc_early(size_t n);
static uintptr_t alloc_early_align(size_t n, size_t align);

static uintptr_t get_free_page_addr(size_t idx);
static uintptr_t page_alloc(void);
static uintptr_t page_alloc_assert(void);

static void map_kernel_page(uintptr_t vaddr, uintptr_t paddr, int level,
                            int prot);

static pte_t *walk_internal(pte_t *root_pg_tbl, uintptr_t vaddr, int create,
                            int level);

static inline pte_t pte_create(uintptr_t ppn, int type)
{
    return (ppn << PTE_PPN_SHIFT) | PTE_V | type;
}

static inline pte_t ptd_create(uintptr_t ppn)
{
    return pte_create(ppn, PTE_V);
}

static inline pte_t prot_to_type(int prot, int user)
{
    pte_t pte = 0;
    if (prot & PROT_READ)
        pte |= PTE_R | PTE_A;
    if (prot & PROT_WRITE)
        pte |= PTE_W | PTE_D;
    if (prot & PROT_EXEC)
        pte |= PTE_X | PTE_A;
    if (pte == 0)
        pte = PTE_R;
    if (user)
        pte |= PTE_U;
    return pte;
}

static inline int freepgs_empty(void)
{
    return g_kvm_info.freepgs.sz == 0;
}

static inline void freepgs_insert(const struct freepg_list_node *node)
{
    // XXX: randomize mapping
    g_kvm_info.freepgs.head[g_kvm_info.freepgs.sz++] = *node;
}

static inline struct freepg_list_node freepgs_remove(void)
{
    return g_kvm_info.freepgs.head[--g_kvm_info.freepgs.sz];
}

static inline int freepgs_augment(void)
{
    struct freepg_list_node node;

    uintptr_t page = alloc_early(PZ_PAGESZ);
    if (page != 0) {
        node.addr = page;
        freepgs_insert(&node);
    }
    return page;
}

void pz_mm_init_and_run(pz_mm_kern_init_vm_f *start_f)
{
    void *start_f_va;
    uintptr_t kstack_top_va;

    kstack_top_va = pa_to_kva(pz_mm_init()) + PZ_PAGESZ;
    start_f_va = (void *)pa_to_kva((uintptr_t)start_f);

    extern uintptr_t pz_arch_trap_entry;
    pz_arch_set_trap(pa_to_kva(pz_arch_trap_entry));

    printf("VM entering privilege (kstack_top_va=%lx, start_f_va=%lx)...\n",
           kstack_top_va, start_f_va);
    //map_kernel_page(0x02500000, 0x02500000, 1, PROT_READ | PROT_WRITE);
    //pz_arch_enter_privilege(start_f_va, kstack_top_va);
}

uintptr_t pz_mm_init(void)
{
    return 0;
    uintptr_t kstack_top;

    // XXX: Device dependent
    g_kvm_info.mem_sz = 16 * 1024 * 1024; // 16 MB;

    init_alloc_early();

    g_kvm_info.freepgs.total_sz = g_kvm_info.mem_sz / PZ_PAGESZ;
    g_kvm_info.freepgs.sz = 0;
    g_kvm_info.freepgs.head = (void *)alloc_early(
            g_kvm_info.freepgs.total_sz * sizeof(struct freepg_list_node));

    g_kvm_info.root_pg_tbl = (void *)page_alloc_assert();
    pz_mm_map_kernel_range(KVA_START, PZ_MEM_START, g_kvm_info.mem_sz,
                           PROT_READ | PROT_WRITE | PROT_EXEC);

    kstack_top = page_alloc_assert() + PZ_PAGESZ;

    pz_arch_mm_set_root_pg_tbl(g_kvm_info.root_pg_tbl);

    // relocate
    g_kvm_info.kva_to_pa_off = KVA_START - PZ_MEM_START;
    g_kvm_info.freepgs.head =
            (void *)pa_to_kva((uintptr_t)g_kvm_info.freepgs.head);
    g_kvm_info.root_pg_tbl =
            (void *)pa_to_kva((uintptr_t)g_kvm_info.root_pg_tbl);

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

static uintptr_t alloc_early(size_t n)
{
    size_t nb_pages = PZ_ROUNDUP(n, PZ_PAGESZ) / PZ_PAGESZ;
    return alloc_early_align(nb_pages, 1);
}

static uintptr_t alloc_early_align(size_t n, size_t align)
{
    size_t skip_pages;
    size_t next_free_pg;
    uintptr_t addr;

    next_free_pg = g_kvm_info.next_free_page;

    skip_pages = (align - 1) & -(get_free_page_addr(next_free_pg) / PZ_PAGESZ);
    n += skip_pages;

    if (n + next_free_pg < n || n + next_free_pg > g_kvm_info.nb_free_pages)
        return 0;

    addr = get_free_page_addr(next_free_pg + skip_pages);
    next_free_pg += n;
    g_kvm_info.next_free_page = next_free_pg;
    return addr;
}

static uintptr_t get_free_page_addr(size_t idx)
{
    return g_kvm_info.first_free_page + idx * PZ_PAGESZ;
}

void pz_panic(const char *mes)
{
    printf("PANIC: %s\n", mes);
}

static uintptr_t page_alloc(void)
{
    struct freepg_list_node node;

    if (freepgs_empty() && !freepgs_augment())
        return 0;

    node = freepgs_remove();
    //memset((void*)pa_to_kva(node.addr), 0, PZ_PAGESZ);
    return node.addr;
}

static uintptr_t page_alloc_assert(void)
{
    uintptr_t res;
    res = page_alloc();
    if (res == 0)
        pz_panic("page_alloc_assert(): out of memory");
    return res;
}

void pz_mm_map_kernel_range(uintptr_t vaddr, uintptr_t paddr, size_t len,
                            int prot)
{
    size_t megapage_sz;

    megapage_sz = PZ_PAGESZ << PZ_PAGELVL_BITS;

    // could support misaligned mappings, but no need today
    //kassert((vaddr | paddr | len) % megapage_size == 0);

    while (len > 0) {
        map_kernel_page(vaddr, paddr, 1, prot);

        len -= megapage_sz;
        vaddr += megapage_sz;
        paddr += megapage_sz;
    }
}

static void map_kernel_page(uintptr_t vaddr, uintptr_t paddr, int level,
                            int prot)
{
    pte_t *pte = walk_internal(g_kvm_info.root_pg_tbl, vaddr, 1, level);
    *pte = pte_create(paddr >> PZ_PAGE_SHIFT, prot_to_type(prot, 0));
}

static inline size_t pte_ppn(pte_t pte)
{
    return pte >> PTE_PPN_SHIFT;
}

static inline uintptr_t ppn(uintptr_t addr)
{
    return addr >> PZ_PAGE_SHIFT;
}

static inline size_t pt_idx(uintptr_t addr, int level)
{
    size_t idx = addr >> (PZ_PAGELVL_BITS * level + PZ_PAGE_SHIFT);
    return idx & ((1 << PZ_PAGELVL_BITS) - 1);
}

static pte_t *walk_internal(pte_t *root_tbl, uintptr_t addr, int create,
                            int level)
{
    int i;
    size_t idx;
    uintptr_t new_ptd;

    for (i = PAGELVLS - 1; i > level; i--) {
        idx = pt_idx(addr, i);
        if (!(root_tbl[idx] & PTE_V)) {
            if (create) {
                new_ptd = page_alloc();
                if (!new_ptd) {
                    return 0;
                }
                root_tbl[idx] = ptd_create(ppn(new_ptd));
            } else {
                return 0;
            }
        }
        root_tbl = (pte_t *)pa_to_kva(pte_ppn(root_tbl[idx]) << PZ_PAGE_SHIFT);
    }
    return &root_tbl[pt_idx(addr, level)];
}
