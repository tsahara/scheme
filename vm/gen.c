#include <err.h>
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

static void
gen_write_string0(const char *str, word_t len)
{
	printf("%*s", (int)len, str);
}

void
gen_write(struct vm *vm, sval_t sval)
{
	sval_t cdr;
	char buf[80];

	switch (gen_sval_type(vm, sval)) {
	case SVAL_TYPE_FIXNUM:
		/* XXX: assumes long > intptr_t */
		snprintf(buf, sizeof(buf), "%ld",
			 (long)sval_fixnum_to_num(vm, sval));
		gen_write_string(buf);
		break;
	case SVAL_TYPE_PAIR:
		gen_write_string("(");
		gen_write(vm, sval_pair_car(vm, sval));

		cdr = sval_pair_cdr(vm, sval);
		while (sval_pair_p(vm, cdr)) {
			sval = cdr;
			gen_write_string(" ");
			gen_write(vm, sval_pair_car(vm, sval));
			cdr = sval_pair_cdr(vm, sval);
		}

		if (! sval_nil_p(vm, cdr)) {
			gen_write_string(" . ");
			gen_write(vm, cdr);
		}

		gen_write_string(")");
		break;
	case SVAL_TYPE_SYMBOL:
	{
		word_t vaddr, len;
		const char *cp;
		vaddr = mem_sval_to_vaddr(vm, sval, 0);
		len = mem_fetch(vm, vaddr);
		cp = mem_sval_to_ptr(vm, sval, WORDSIZE);
		gen_write_string0(cp, len);
	}
		break;
	case SVAL_TYPE_VECTOR:
		gen_write_string("#(");
		gen_write_string("...");
		gen_write_string(")");
		break;
	case SVAL_TYPE_TAGGED:
	{
		word_t tag = sval_tag_get(vm, sval);
		switch (tag) {
		case SVAL_TAG_CPROC:
			gen_write_string("#<an cproc>");
			break;
		case SVAL_TAG_BYTEVECTOR:
			gen_write_string("#u8(");
			gen_write_string("...");
			gen_write_string(")");
			break;
		}
	}
		break;
	case SVAL_TYPE_NIL:
		gen_write_string("()");
		break;
	case SVAL_TYPE_TRUE:
		gen_write_string("#t");
		break;
	case SVAL_TYPE_FALSE:
		gen_write_string("#f");
		break;
	default:
		snprintf(buf, sizeof(buf), "<type=%s,sval=0x%x>",
				gen_type_string(sval), sval);
		printf("%s", buf);
		break;
	}
}
