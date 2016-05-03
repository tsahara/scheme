#include <err.h>

#include <stdio.h>
#include <stdlib.h>

#include "vm.h"
#include "mem.h"

#if 0
static sval_t
relocate_file_to_vm(struct vm *vm, vaddr_t fileptr, word_t fileobj)
{
	sval_t len;
	const char *cp;
	warnx("ptr=0x%x, obj=0x%x", fileptr, fileobj);
	if (sval_symbol_p(vm, fileptr + fileobj)) {
		len = mem_sval_fetch(vm, fileptr + fileobj, 0);
		cp = mem_sval_to_ptr(vm, fileptr + fileobj, WORDSIZE);
		return sval_symbol(vm, cp, len);
	} else {
		errx(1, "relocate_file_to_vm: notyet");
	}
}
#endif

static void
relocate(struct vm *vm, vaddr_t fileptr, sval_t env)
{
	vaddr_t a;
	word_t w;

	a = fileptr + WORDSIZE;
	w = mem_fetch(vm, a);
	warnx("w=0x%x", w);

#if 0
	closure = sval_pair_cdr(vm, fileptr);
	bytecode = sval_pair_car(vm, fileptr + closure);
	reftab = sval_pair_car(vm, sval_pair_cdr(vm, sval_pair_cdr(vm, closure)));

	for (l = reftab; sval_pair_p(vm, l); l = sval_pair_cdr(vm, l)) {

		/* entry = (symptr addr0 addr1 ...) */
		entry = sval_pair_car(vm, l);
		printf("entry:");
		gen_write(vm, entry);

		symptr = sval_pair_car(vm, entry);
		sym = relocate_file_to_vm(vm, fileptr, symptr);

		addrl = sval_pair_cdr(vm, entry);
		printf("sym:");
		gen_write(vm, sym);
		addr = sval_pair_car(vm, addrl);
		printf(" @%x\n", addr);

		//env_lookup(vm, env, entry)
		printf("\n");
	}
#endif
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
	sval_t env;
	vaddr_t addr;

	env = env_global(vm);
	env = env_add(vm, env,
		      sval_symbol_cstr(vm, "write"),
		      sval_cproc_new(vm, gen_write));
	env = env_add(vm, env,
		      sval_symbol_cstr(vm, "exit"),
		      sval_cproc_new(vm, gen_exit));

	mem_load(vm, filename, &addr);
	relocate(vm, addr, env);

//	addr2 = sval_bytevector_vaddr(vm, stoplevel);
	vcpu_run(vm, addr);
	return 0;
}
