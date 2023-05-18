#ifndef SYS_KALLOC_H
#define SYS_KALLOC_H

void pz_alloc_init(void);
void *pz_alloc_pnew(void);
void pz_alloc_pfree(void *pa);

#endif // !SYS_KALLOC_H
