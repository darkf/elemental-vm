#include <stdio.h>
#include <stdlib.h>
#include "vm.h"
#include "op.h"

int main(int argc, char *argv[])
{
	vm_t vm;

	/* testing code put in main to be easy to remove */
	printf("testing... OP_ADD = %s\n", op_tostr(OP_ADD));

	vm_init(&vm);
	vm_begin(&vm);
	return 0;
}
