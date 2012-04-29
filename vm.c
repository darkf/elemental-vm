/* elemental vm
   copyright (c) 2012 darkf
   MIT licensed */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memset */
#include "vm.h"
#include "op.h"

/* global stuff */
int *mem;
int regs[32+3]; /* 32 GP + FLAG + SP + IP */

#define MEM_LENGTH 1024*1024*32 /* 32 mb */

int vm_init()
{
	mem = (int *) malloc(MEM_LENGTH); 
	memset(mem, 0, MEM_LENGTH);
	memset(regs, 0, sizeof(regs));

	return 1;
}

int vm_begin()
{
	return 0;
}