#include <err.h>
#include <string.h>

#include "vm.h"
#include "mem.h"

sval_t allsymbols[1000];

word_t
sval_symbol_length(struct vm *vm, sval_t sym)
{
	return mem_sval_fetch(vm, sym, 0);
}

sval_t
sval_symbol(struct vm *vm, const char *name, word_t namelen)
{
	sval_t sym;
	vaddr_t vaddr;
	word_t len;
	int i;
	char *cp;

	for (i = 0; allsymbols[i] != 0; i++) {
		sym = allsymbols[i];
		len = sval_symbol_length(vm, sym);
		cp = mem_sval_to_ptr(vm, sym, WORDSIZE);
		if (namelen == len && memcmp(name, cp, len) == 0)
			return sym;
	}
	len = WORDSIZE + WORDWRAP(namelen);
	vaddr = mem_allocate(vm, len);
	mem_store(vm, vaddr, namelen);
	memcpy(mem_vaddr_to_ptr(vm, vaddr, WORDSIZE), name, namelen);
	sym = vaddr | 0x03;
	allsymbols[i] = sym;
	return sym;
}


sval_t
sval_symbol_cstr(struct vm *vm, const char *name)
{
	return sval_symbol(vm, name, strlen(name));
}

int
sval_symbol_p(struct vm *vm, sval_t sval)
{
	return gen_sval_type(vm, sval) == SVAL_TYPE_SYMBOL;
}
