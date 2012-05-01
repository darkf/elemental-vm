#include <stdio.h>
#include <stdlib.h>
#include "vm.h"
#include "op.h"

int main(int argc, char *argv[])
{
	vm_t vm;
	vm_init(&vm);
	vm_begin(&vm);
	vm_quit(&vm);
	return 0;
}
