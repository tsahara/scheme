#include <stdio.h>
#include <stdlib.h>

#include "vm.h"

int
gen_sval_type(struct vm *vm, sval_t sval)
{
	sval_t tag;
	int t;
	
	t = sval & 7;
	if (t < 6)
		return t;
	else if (t == 6) {
		tag = mem_fetch(vm, mem_sval_to_vaddr(vm, sval, 0));
		return SVAL_TYPE_TAGGED;
	} else if ((sval & 0x0f) == 0x07) {
		return SVAL_TYPE_CHAR;
	}

	switch (sval) {
	case 0x0f: return SVAL_TYPE_NIL;
	case 0x1f: return SVAL_TYPE_TRUE;
	case 0x2f: return SVAL_TYPE_FALSE;
	case 0x3f: return SVAL_TYPE_UNSPECIFIED;
	}

	return SVAL_TYPE_UNKNOWN;
}

const char *
gen_type_string(int type)
{
	static const char *typestring[] = {
		"fixnum",
		"pair",
		"string",
		"symbol",
		"vector",
		"closure",
		"tagged",
		"char",
		"nil",
		"true",
		"false",
		"unspecified",
		"cproc",
		"bytevector",
		NULL
	};

	if (type < SVAL_TYPE_MAX)
		return typestring[type];
	else
		return "(unknown)";
}

static void
gen_write_string(const char *str)
{
	printf("%s", str);
}

void
gen_write(struct vm *vm, sval_t sval)
{
	char buf[80];

	switch (gen_sval_type(vm, sval)) {
	case SVAL_TYPE_PAIR:
		gen_write_string("(");
		gen_write(vm, sval_pair_car(vm, sval));
		gen_write_string(" . ");
		gen_write(vm, sval_pair_cdr(vm, sval));
		gen_write_string(")");
		break;
	case SVAL_TYPE_VECTOR:
		gen_write_string("#(");
		gen_write_string("...");
		gen_write_string(")");
		break;
	case SVAL_TYPE_NIL:
		gen_write_string("()");
		break;
	default:
		snprintf(buf, sizeof(buf), "<type=%s,sval=%x>",
				gen_type_string(sval), sval);
		printf("%s", buf);
		break;
	}
}
