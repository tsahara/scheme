#include <stdio.h>

#include "vm.h"

sval_t
env_global(struct vm *vm)
{
	sval_t env;

	env = sval_nil(vm);
	return env;
}

sval_t
env_add(struct vm *vm, sval_t env, sval_t sym, sval_t val)
{
	sval_t e, p;

	e = sval_pair(vm, sym, val);
	p = sval_pair(vm, e, env);
	return p;
}

sval_t
env_lookup(struct vm *vm, sval_t env, sval_t sym)
{
	sval_t e, s;

	for (e = env; sval_pair_p(vm, e); e = sval_pair_cdr(vm, e)) {
		s = sval_pair_car(vm, e);
		if (s == sym)
			return sval_pair_cdr(vm, e);
	}
	return sval_nil(vm);
}
