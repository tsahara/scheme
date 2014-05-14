#include <err.h>

#include <stdlib.h>

#include "vm.h"

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

	vm->vcpu = vcpu_new(vm);
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
	sval_t env, sbc, spair;
	vaddr_t addr, addr2;

	env = env_global(vm);

	mem_load(vm->mem, filename, &addr);
	spair = sval_ptr_to_pair(vm, addr);
	sbc = sval_pair_car(vm, spair);

	gen_write(vm, spair);

	addr2 = sval_bytevector_vaddr(vm, sbc);
	vcpu_run(vm, addr2);
	return 0;
}
