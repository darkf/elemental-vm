#include <stdio.h>
#include <stdlib.h>
#include "vm.h"

int main(int argc, char *argv[])
{
	vm_t vm;
	vm_init(&vm);
	vm_begin(&vm);
	return 0;
}
