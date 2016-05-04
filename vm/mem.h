#ifndef __VM_MEM_H__
#define __VM_MEM_H__

#include "vm.h"

struct mem;
struct vm;

struct mem *mem_new(void);
void mem_free(struct mem *);
vaddr_t mem_allocate(struct vm *, word_t);
word_t mem_fetch(struct vm *, vaddr_t);
int mem_load(struct vm *, const char *, vaddr_t *);
void mem_store(struct vm *, vaddr_t, word_t);
word_t mem_sval_fetch(struct vm *, sval_t, unsigned int);
void *mem_sval_to_ptr(struct vm *, sval_t, unsigned int);
vaddr_t mem_sval_to_vaddr(struct vm *, sval_t, unsigned int);
void *mem_vaddr_to_ptr(struct vm *, vaddr_t, unsigned int);

#endif /* __VM_MEM_H__ */
