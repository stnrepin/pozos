#include "kern/arch/init.h"

#include "kern/arch/sysdef.h"
#include "encoding.h"

#define INSERT_FIELD(val, which, bitval) \
    (((val) & ~(which)) | ((bitval) * ((which) & ~((which)-1))))

void pz_arch_enter_privilege(void (*f)(uintptr_t), uintptr_t arg0)
{
    uintptr_t mstatus = read_csr(mstatus);
    mstatus = INSERT_FIELD(mstatus, MSTATUS_MPP, PRV_S);
    mstatus = INSERT_FIELD(mstatus, MSTATUS_MPIE, 0);
    write_csr(mstatus, mstatus);
    //write_csr(mscratch, MACHINE_STACK_TOP() - MENTRY_FRAME_SIZE);
    write_csr(mepc, f);

    register uintptr_t a0 __asm__("a0") = arg0;
    __asm__ volatile("mret" : : "r"(a0));

    __builtin_unreachable();
}
