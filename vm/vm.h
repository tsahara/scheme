#include <stdint.h>

typedef uint32_t word_t;
typedef word_t sval_t;		/* scheme value */
typedef word_t vaddr_t;		/* address in memory */

#define WORDSIZE	4

struct mem;
struct vm;

struct mem *mem_new(void);
void mem_free(struct mem *);
vaddr_t mem_allocate(struct mem *, word_t);
word_t mem_fetch(struct mem *, vaddr_t);
int mem_load(struct mem *, const char *, vaddr_t *);
void *mem_sval_to_ptr(struct mem *, sval_t, unsigned int);
vaddr_t mem_sval_to_vaddr(struct mem *, sval_t, unsigned int);

struct vcpu *vcpu_new(struct mem *);
void vcpu_free(struct vcpu *);
void vcpu_run(struct vcpu *, vaddr_t);

struct vm *vm_new(void);
int vm_run(struct vm *, const char *);

sval_t sval_ptr_to_pair(struct mem *, vaddr_t);
sval_t sval_pair_car(struct mem *, sval_t);
sval_t sval_pair_cdr(struct mem *, sval_t);
void sval_set_car_b(struct mem *, sval_t, sval_t);
void sval_set_cdr_b(struct mem *, sval_t, sval_t);

vaddr_t sval_bytevector_vaddr(struct mem *, sval_t);
