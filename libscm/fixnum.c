#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#include "scm.h"

static int64_t
scm_fixnum_to_int64(scm_obj_t s_num)
{
	return (int64_t)s_num / 0x10;
}

scm_obj_t
scm_fixnum_from_int(int num)
{
	// if (num > 0x...)
	return (scm_obj_t)(num << 4);
}

scm_obj_t
scm_write_fixnum(scm_obj_t s_num)
{
	printf("%"PRId64"\n", scm_fixnum_to_int64(s_num));
	return SCM_OBJ_UNSPECIFIED;
}
