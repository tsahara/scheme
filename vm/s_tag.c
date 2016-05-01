#include "vm.h"
#include "mem.h"


word_t
sval_tag_get(struct vm *vm, sval_t sval)
{
	word_t vaddr = mem_sval_to_vaddr(vm, sval, 0);
	return mem_fetch(vm, vaddr);
}
