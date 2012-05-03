/* elemental vm
   copyright (c) 2012 darkf
   MIT licensed */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memset */
#include "vm.h"
#include "op.h"
#include "cpu.h"

/* mini instruction assembler for early testing */
unsigned long op_emit(vm_t *vm, long op, operand_t *a, operand_t *b, unsigned long ptr)
{
	vm->mem[ptr++] = op;
	if(a != NULL)
		if(b != NULL)	
			vm->mem[ptr++] = (a->type << 4) | b->type;
		else
			vm->mem[ptr++] = (a->type << 4);

	if(a != NULL) {
		/* 32-bit big endian encoded over four bytes */
		vm->mem[ptr++] = (a->data >> 24) & 0xff;
		vm->mem[ptr++] = (a->data >> 16) & 0xff;
		vm->mem[ptr++] = (a->data >> 8) & 0xff;
		vm->mem[ptr++] = (a->data >> 0) & 0xff;
	}
	if(b != NULL) {
		/* 32-bit big endian encoded over four bytes */
		vm->mem[ptr++] = (b->data >> 24) & 0xff;
		vm->mem[ptr++] = (b->data >> 16) & 0xff;
		vm->mem[ptr++] = (b->data >> 8) & 0xff;
		vm->mem[ptr++] = (b->data >> 0) & 0xff;
	}
	return ptr;
}

void load_program(vm_t *vm, unsigned long ptr)
{
	operand_t a, b, c, d, e;
	vm->reg[IP] = ptr;

	long_to_register(&a, 0); /* reg 0 */
	long_to_literal(&b, 0);
	long_to_literal(&c, 1);
	long_to_literal(&d, 10);

	/*
			mov $0, 0
	 	loop:	add $0, 1
			cmp $0, 10
			blt loop
	*/
	ptr = op_emit(vm, OP_MOV32, &a, &b, ptr);
	long_to_literal(&e, ptr);
	ptr = op_emit(vm, OP_ADD, &a, &c, ptr);
	ptr = op_emit(vm, OP_CMP, &a, &d, ptr);
	ptr = op_emit(vm, OP_BLT, &e, NULL, ptr);
	op_emit(vm, OP_HALT, NULL, NULL, ptr);
}

int vm_init(vm_t *vm)
{
	long base;
	vm->mem = malloc(MEM_LENGTH);
	memset(vm->mem, 0, MEM_LENGTH);
	memset(vm->reg, 0, sizeof(vm->reg));

	base = 0x00300000; /* end of VRAM */
	vm->reg[SP] = base;
	base += STACK_SIZE;

	vm->base_addr = base;
	return 1;
}

int vm_quit(vm_t *vm)
{
	free(vm->mem);
	return 1;
}

int vm_begin(vm_t *vm)
{
	load_program(vm, vm->base_addr);

	/* run the program */
	cpu_run(vm);

	/* pure debug code here -- check if the
	   program worked */
	printf("register 0 = %ld\n", vm->reg[0]);
	printf("register 1 = %ld\n", vm->reg[1]);

	return 0;
}
