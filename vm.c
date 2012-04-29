/* elemental vm
   copyright (c) 2012 darkf
   MIT licensed */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memset */
#include "vm.h"
#include "op.h"

int vm_init(vm_t *vm)
{
	vm->mem = (int *) malloc(MEM_LENGTH); 
	memset(vm->mem, 0, MEM_LENGTH);
	memset(vm->regs, 0, sizeof(vm->regs));

	return 1;
}

int vm_begin(vm_t *vm)
{
	return 0;
}