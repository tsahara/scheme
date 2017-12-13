#include <string.h>

#include "scm.h"

struct scm_string {
	uint64_t len;
	unsigned char bytes[0];
};

scm_obj_t
scm_string_length(scm_obj_t scm_str)
{
	return 0;
}
