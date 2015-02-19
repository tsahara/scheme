#include <err.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include "vm.h"

static void usage(void);

static void
usage(void)
{
	printf("usage: vm <objfile>\n");
}

int
main(int argc, char **argv)
{
	struct vm *vm; 
	int ch;

	while ((ch = getopt(argc, argv, "")) != -1) {
		switch (ch) {
		case '?':
		default:
			usage();
			exit(0);
		}
	}
	argc -= optind;
	argv += optind;

	if (argc != 1) {
		usage();
		exit(1);
	}
	vm = vm_new();
	if (vm == NULL)
		err(1, "cannot create an address space");
	vm_run(vm, argv[0]);
	exit(0);
}
