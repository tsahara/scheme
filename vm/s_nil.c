#include <err.h>

#include "vm.h"

int
sval_nil_p(struct vm *vm, sval_t sval)
{
	return gen_sval_type(vm, sval) == SVAL_TYPE_NIL;
}
