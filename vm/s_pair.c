#include <err.h>

#include "vm.h"

sval_t
sval_pair_car(struct vm *vm, sval_t val)
{
	vaddr_t addr = mem_sval_to_vaddr(vm, val, 0);
	return mem_fetch(vm, addr);
}

sval_t
sval_pair_cdr(struct vm *vm, sval_t val)
{
	vaddr_t addr = mem_sval_to_vaddr(vm, val, WORDSIZE);
	return mem_fetch(vm, addr);
}

void sval_set_car_b(struct vm *, sval_t, sval_t);
void sval_set_cdr_b(struct vm *, sval_t, sval_t);

sval_t
sval_ptr_to_pair(struct vm *vm, vaddr_t addr)
{
	return addr + 1;
}

