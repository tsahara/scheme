#include <err.h>

#include "vm.h"

sval_t
sval_nil(struct vm *vm)
{
	return 0x0f;
}

int
sval_nil_p(struct vm *vm, sval_t sval)
{
	return gen_sval_type(vm, sval) == SVAL_TYPE_NIL;
}
