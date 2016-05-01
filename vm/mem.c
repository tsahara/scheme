#include <err.h>

#include <stdio.h>
#include <stdlib.h>

#include "vm.h"
#include "mem.h"


#define ALLOC_SIZE	(256*1024)

struct mem {
	unsigned char *base;
	unsigned int size;
	vaddr_t allocated;
};

static vaddr_t addr_roundup(vaddr_t);

static vaddr_t
addr_roundup(vaddr_t addr)
{
	return (addr + (WORDSIZE * 2 - 1)) & ~(WORDSIZE * 2 - 1);
}

struct mem *
mem_new(void)
{
	struct mem *mem;

	mem = calloc(1, sizeof(struct mem));
	if (mem == NULL)
		return NULL;
	mem->base = calloc(1, ALLOC_SIZE);
	if (mem->base == NULL)
		goto eret;
	mem->size = ALLOC_SIZE;
	mem->allocated = 0;
	return mem;
eret:
	free(mem);
	return NULL;
}

void
mem_free(struct mem *mem)
{
	free(mem->base);
	free(mem);
}

vaddr_t
mem_allocate(struct vm *vm, word_t size)
{
	vaddr_t addr;

	addr = vm->mem->allocated;
	vm->mem->allocated += addr_roundup(size);
	return addr;
}

word_t
mem_fetch(struct vm *vm, vaddr_t addr)
{
	word_t w;
	unsigned char *cp;

	if ((addr % WORDSIZE) != 0)
		err(1, "unaligned access: %0*x", WORDSIZE * 2, addr);
	cp = vm->mem->base + addr;
	w = cp[0];
	w = (w << 8) + cp[1];
	w = (w << 8) + cp[2];
	w = (w << 8) + cp[3];
	return w;
}

int
mem_load(struct vm *vm, const char *filename, vaddr_t *addrp)
{
	FILE *fp;
	size_t n;
	vaddr_t addr;
	long filesize;

	fp = fopen(filename, "r");
	if (fp == NULL)
		err(1, "%s", filename);

	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp);
	rewind(fp);

	addr = mem_allocate(vm, filesize);
	n = fread(vm->mem->base + addr, 1, filesize, fp);
	if (ferror(fp))
		err(1, "fread");
	fclose(fp);

	*addrp = addr;
	return 0;
}

void
mem_store(struct vm *vm, vaddr_t addr, word_t val)
{
	unsigned char *cp;

	if ((addr % WORDSIZE) != 0) {
		warnx("unaligned access: %0*x", WORDSIZE * 2, addr);
		abort();
	}
	cp = vm->mem->base + addr;
	cp[0] = val >> 24;
	cp[1] = val >> 16;
	cp[2] = val >> 8;
	cp[3] = val;
}

word_t
mem_sval_fetch(struct vm *vm, sval_t sval, unsigned int offset)
{
	return mem_fetch(vm, mem_sval_to_vaddr(vm, sval, offset));
}

void *
mem_sval_to_ptr(struct vm *vm, sval_t val, unsigned int offset)
{
	vaddr_t addr = mem_sval_to_vaddr(vm, val, offset);
	return &vm->mem->base[addr];
}

vaddr_t
mem_sval_to_vaddr(struct vm *vm, sval_t val, unsigned int offset)
{
	return  (val & ~(WORDSIZE * 2 - 1)) + offset;
}

void *
mem_vaddr_to_ptr(struct vm *vm, vaddr_t vaddr, unsigned int offset)
{
	return &vm->mem->base[vaddr + offset];
}
