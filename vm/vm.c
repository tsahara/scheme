#include <err.h>

#include <stdio.h>
#include <stdlib.h>

#include "vm.h"

static void
relocate(struct vm *vm, vaddr_t fileptr, sval_t closure, sval_t env)
{
	sval_t addr, addrl, sym, entry, l, reftab, bytecode;

	bytecode = sval_pair_car(vm, closure);
	reftab = sval_pair_car(vm, sval_pair_cdr(vm, sval_pair_cdr(vm, closure)));

	for (l = reftab; sval_pair_p(vm, l); l = sval_pair_cdr(vm, l)) {
		entry = sval_pair_car(vm, l);
		sym = sval_pair_car(vm, entry);
		addrl = sval_pair_cdr(vm, entry);
		gen_write(vm, sym);
		addr = sval_pair_car(vm, addrl);
		printf(" @%x\n", addr);
		
		//env_lookup(vm, env, entry)
		printf("\n");
	}
}

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
	sval_t env, spair, stoplevel;
	vaddr_t addr, addr2;

	env = env_global(vm);
	env = env_add(vm, env,
		      sval_symbol(vm, "write"),
		      sval_cproc_new(vm, gen_write));
	gen_write(vm, env);

	mem_load(vm->mem, filename, &addr);
	spair = sval_ptr_to_pair(vm, addr);
	stoplevel = sval_pair_cdr(vm, spair);

	relocate(vm, addr, stoplevel, env);

	addr2 = sval_bytevector_vaddr(vm, stoplevel);
//	vcpu_run(vm, addr2);
	return 0;
}
