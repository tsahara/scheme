#include <err.h>

#include "vm.h"
#include "mem.h"


sval_t
sval_pair(struct vm *vm, sval_t car, sval_t cdr)
{
	sval_t pair;
	vaddr_t addr;

	addr = mem_allocate(vm, WORDSIZE * 2);
	pair = addr | 1;
	sval_pair_set_car_b(vm, pair, car);
	sval_pair_set_cdr_b(vm, pair, cdr);
	return pair;
}

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

void
sval_pair_set_car_b(struct vm *vm, sval_t pair, sval_t val)
{
	vaddr_t addr = mem_sval_to_vaddr(vm, pair, 0);
	mem_store(vm, addr, val);
}

void
sval_pair_set_cdr_b(struct vm *vm, sval_t pair, sval_t val)
{
	vaddr_t addr = mem_sval_to_vaddr(vm, pair, WORDSIZE);
	mem_store(vm, addr, val);
}

int
sval_pair_p(struct vm *vm, sval_t sval)
{
	return gen_sval_type(vm, sval) == SVAL_TYPE_PAIR;
}

sval_t
sval_ptr_to_pair(struct vm *vm, vaddr_t addr)
{
	return addr + 1;
}
