#include "vm.h"

vaddr_t
sval_bytevector_vaddr(struct mem *mem, sval_t sval)
{
	return mem_sval_to_vaddr(mem, sval, WORDSIZE * 2);
}
