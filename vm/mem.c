#include <err.h>

#include <stdio.h>
#include <stdlib.h>

#include "vm.h"

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
mem_allocate(struct mem *mem, word_t size)
{
	vaddr_t addr;

	addr = mem->allcated;
	mem->allcated += addr_roundup(size);
	return addr;
}

word_t
mem_fetch(struct mem *mem, vaddr_t addr)
{
	word_t w;
	unsigned char *cp;

	if ((addr % WORDSIZE) != 0)
		err(1, "unaligned access: %0*x", WORDSIZE * 2, addr);
	cp = mem->base + addr;
	w = cp[0];
	w = (w << 8) + cp[1];
	w = (w << 8) + cp[2];
	w = (w << 8) + cp[3];
	return w;
}

int
mem_load(struct mem *mem, const char *filename, vaddr_t *addr)
{
	FILE *fp;
	size_t n;

	fp = fopen(filename, "r");
	if (fp == NULL)
		err(1, "%s", filename);
	n = fread(mem->base, 1, mem->size, fp);
	if (ferror(fp))
		err(1, "fread");
	fclose(fp);

	mem->allocated = addr_roundup(n);

	*addr = 0;
	return 0;
}

void *
mem_sval_to_ptr(struct mem *mem, sval_t val, unsigned int offset)
{
	vaddr_t addr = mem_sval_to_vaddr(mem, val, offset);
	return &mem->base[addr];
}

vaddr_t
mem_sval_to_vaddr(struct mem *mem, sval_t val, unsigned int offset)
{
	return  (val & ~(WORDSIZE * 2 - 1)) + offset;
}
