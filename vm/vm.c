#include <err.h>

#include <stdlib.h>

#include "vm.h"

struct vm {
	struct vcpu *vcpu;
	struct mem *mem;
};

struct vm *
vm_new(void)
{
	struct vm *vm;

	vm = calloc(1, sizeof(struct vm));
	if (vm == NULL)
		return NULL;

	vm->mem = mem_new();
	if (vm->mem == NULL)
		goto eret;

	vm->vcpu = vcpu_new(vm->mem);
	if (vm->vcpu == NULL)
		goto eret;

	return vm;
eret:
	if (vm->vcpu != NULL)
		vcpu_free(vm->vcpu);
	if (vm->mem != NULL)
		mem_free(vm->mem);
	free(vm);
	return NULL;
}

void
vm_free(struct vm *vm)
{
	vcpu_free(vm->vcpu);
	mem_free(vm->mem);
	free(vm);
}

int
vm_run(struct vm *vm, const char *filename)
{
	sval_t sbc, spair;
	vaddr_t addr, addr2;

	mem_load(vm->mem, filename, &addr);
	spair = sval_ptr_to_pair(vm->mem, addr);
	sbc = sval_pair_car(vm->mem, spair);

	addr2 = sval_bytevector_vaddr(vm->mem, sbc);
	vcpu_run(vm->vcpu, addr2);
	return 0;
}
