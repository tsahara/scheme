#include <err.h>

#include "vm.h"

sval_t
sval_pair_car(struct mem *mem, sval_t val)
{
	vaddr_t addr = mem_sval_to_vaddr(mem, val, 0);
	return mem_fetch(mem, addr);
}

sval_t
sval_pair_cdr(struct mem *mem, sval_t val)
{
	word_t *p =  mem_sval_to_ptr(mem, val, WORDSIZE);
	return *p;
}

void sval_set_car_b(struct mem *, sval_t, sval_t);
void sval_set_cdr_b(struct mem *, sval_t, sval_t);

sval_t
sval_ptr_to_pair(struct mem *mem, vaddr_t addr)
{
	return addr + 1;
}

