#include "vm.h"
#include "mem.h"

sval_t
sval_cproc_new(struct vm *vm, void *proc)
{
	vaddr_t vaddr;

	vaddr = mem_allocate(vm, WORDSIZE * 2);
	mem_store(vm, vaddr, SVAL_TAG_CPROC);
	mem_store(vm, vaddr + WORDSIZE, (word_t)proc);
	return (sval_t)(vaddr + 0x06);
}
