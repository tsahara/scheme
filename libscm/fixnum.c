#include "scm.h"

scm_obj_t
scm_fixnum_from_int(int num)
{
	// if (num > 0x...)
	return (scm_obj_t)(num << 4);
}
