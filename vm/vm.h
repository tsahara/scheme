#include <stdint.h>

typedef uint32_t word_t;
typedef word_t sval_t;		/* scheme value */
typedef word_t vaddr_t;		/* address in memory */

#define WORDSIZE	4

enum {
	SVAL_TYPE_FIXNUM = 0,
	SVAL_TYPE_PAIR,
	SVAL_TYPE_STRING,
	SVAL_TYPE_SYMBOL,
	SVAL_TYPE_VECTOR,
	SVAL_TYPE_CLOSURE,
	SVAL_TYPE_TAGGED,
	SVAL_TYPE_CHAR,
	SVAL_TYPE_NIL,
	SVAL_TYPE_TRUE,
	SVAL_TYPE_FALSE,
	SVAL_TYPE_UNSPECIFIED,
	SVAL_TYPE_CPROC,
	SVAL_TYPE_BYTEVECTOR,
	SVAL_TYPE_UNKNOWN
};
#define SVAL_TYPE_MAX SVAL_TYPE_UNKNOWN

struct mem;
struct vcpu;

struct vm {
	struct vcpu *vcpu;
	struct mem *mem;
};

struct mem *mem_new(void);
void mem_free(struct mem *);
vaddr_t mem_allocate(struct mem *, word_t);
word_t mem_fetch(struct vm *, vaddr_t);
int mem_load(struct mem *, const char *, vaddr_t *);
void *mem_sval_to_ptr(struct vm *, sval_t, unsigned int);
vaddr_t mem_sval_to_vaddr(struct vm *, sval_t, unsigned int);

struct vcpu *vcpu_new(struct vm *);
void vcpu_free(struct vcpu *);
void vcpu_run(struct vm *, vaddr_t);

struct vm *vm_new(void);
int vm_run(struct vm *, const char *);

sval_t sval_ptr_to_pair(struct vm *, vaddr_t);
sval_t sval_pair_car(struct vm *, sval_t);
sval_t sval_pair_cdr(struct vm *, sval_t);
void sval_set_car_b(struct vm *, sval_t, sval_t);
void sval_set_cdr_b(struct vm *, sval_t, sval_t);

vaddr_t sval_bytevector_vaddr(struct vm *, sval_t);


void gen_write(struct vm *, sval_t);
