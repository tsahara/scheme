#ifndef __VM_VM_H__
#define __VM_VM_H__

#include <stdint.h>

typedef uint32_t word_t;
typedef word_t sval_t;		/* scheme value */
typedef word_t vaddr_t;		/* address in memory */

#define WORDSIZE	4
#define WORDWRAP(x)	(((x) + 3) & ~(WORDSIZE - 1))

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

enum {
	SVAL_TAG_CPROC = 1,
	SVAL_TAG_BYTEVECTOR,
	SVAL_TAG_UNKNOWN
};
#define SVAL_TAG_MAX SVAL_TAG_UNKNOWN


struct mem;
struct vcpu;

struct vm {
	struct vcpu *vcpu;
	struct mem *mem;
};

/* closure */
sval_t closure_new(struct vm *, void *);

/* environment */
sval_t env_add(struct vm *, sval_t, sval_t, sval_t);
sval_t env_global(struct vm *);
sval_t env_lookup(struct vm *, sval_t, sval_t);

struct vcpu *vcpu_new(struct vm *);
void vcpu_free(struct vcpu *);
void vcpu_run(struct vm *, vaddr_t);

struct vm *vm_new(void);
int vm_run(struct vm *, const char *);


word_t sval_tag_get(struct vm *, sval_t);

/* cproc */
sval_t sval_cproc_new(struct vm *, void *);

word_t sval_fixnum_to_num(struct vm *, sval_t);

sval_t sval_nil(struct vm *);
int sval_nil_p(struct vm *, vaddr_t);

sval_t sval_pair(struct vm *, sval_t, sval_t);
sval_t sval_pair_car(struct vm *, sval_t);
sval_t sval_pair_cdr(struct vm *, sval_t);
int sval_pair_p(struct vm *, sval_t);
sval_t sval_ptr_to_pair(struct vm *, vaddr_t);
void sval_pair_set_car_b(struct vm *, sval_t, sval_t);
void sval_pair_set_cdr_b(struct vm *, sval_t, sval_t);

vaddr_t sval_bytevector_vaddr(struct vm *, sval_t);

/* symbol */
sval_t sval_symbol(struct vm *, const char *, word_t);
sval_t sval_symbol_cstr(struct vm *, const char *);
int sval_symbol_p(struct vm *, sval_t);

void gen_exit(struct vm *, sval_t);
int gen_sval_type(struct vm *, sval_t);
void gen_write(struct vm *, sval_t);

#endif /* __VM_VM_H__ */
