#include "kern/arch/init.h"

#include "kern/arch/sysdef.h"
#include "kern/arch/mm.h"
#include "kern/arch/intrin.h"

#define PZ_MAXCPU 4

__attribute__((aligned(16))) char g_stacks[PZ_PAGESZ * PZ_MAXCPU];

void pz_arch_start_kern(start_kern_f sk_f)
{
    uint64_t mstatus, sie;
    // set M Previous Privilege mode to Supervisor, for mret.
    mstatus = pz_arch_intrin_read_mstatus();
    mstatus &= ~PZ_ARCH_INTRIN_MSTATUS_MPP_MASK;
    mstatus |= PZ_ARCH_INTRIN_MSTATUS_MPP_S;
    pz_arch_intrin_write_mstatus(mstatus);

    // set M Exception Program Counter to main, for mret.
    // requires gcc -mcmodel=medany
    pz_arch_intrin_write_mepc((uint64_t)sk_f);

    // disable paging for now.
    pz_arch_intrin_write_satp(0);

    // delegate all interrupts and exceptions to supervisor mode.
    pz_arch_intrin_write_medeleg(0xffff);
    pz_arch_intrin_write_mideleg(0xffff);
    sie = pz_arch_intrin_read_sie();
    sie = sie | PZ_ARCH_INTRIN_SIE_SEIE | PZ_ARCH_INTRIN_SIE_STIE |
          PZ_ARCH_INTRIN_SIE_SSIE;
    pz_arch_intrin_write_sie(sie);

    // configure Physical Memory Protection to give supervisor mode
    // access to all of physical memory.
    pz_arch_intrin_write_pmpaddr0(0x3fffffffffffffull);
    pz_arch_intrin_write_pmpcfg0(0xf);

    // ask for clock interrupts.
    //timerinit();

    // keep each CPU's hartid in its tp register, for cpuid().
    int id = pz_arch_intrin_read_mhartid();
    pz_arch_intrin_write_tp(id);

    // switch to supervisor mode and jump to main().
    pz_arch_intrin_mret();
}
