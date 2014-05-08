#include "vm.h"

vaddr_t
sval_bytevector_vaddr(struct vm *vm, sval_t sval)
{
	return mem_sval_to_vaddr(vm, sval, WORDSIZE * 2);
}
