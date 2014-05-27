#include "vm.h"

word_t
sval_fixnum_to_num(struct vm *vm, sval_t sval)
{
	return sval >> 3;
}
