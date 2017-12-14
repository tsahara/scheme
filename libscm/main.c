#include "scm.h"

int
main(int argc, char **argv)
{
	scm_write_fixnum(scm_fixnum_from_int(12345));
	return 0;
}
