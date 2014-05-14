#include "vm.h"

sval_t
env_global(struct vm *vm)
{
	sval_t env;

	env = sval_nil(vm);
	return env;
}
