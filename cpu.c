#include "vm.h"
#include "cpu.h"
#include "op.h"
#include <stdlib.h>
#include <stdio.h>

#define READ vm->mem[pc++]
#define READ_32 (vm->mem[pc+=4, pc-4])
#define TWO_OPERANDS (oprA=&READ_32, oprB=&READ_32)
#define PEEK vm->mem[pc]

unsigned long* operand_pointer(vm_t *vm, int optype, unsigned long* dat)
{
	static unsigned long be32;
	be32 = (*dat << 24) | (dat[1] << 16) | (dat[2] << 8) | dat[3];

	switch(optype) {
		case REGISTER:	return &vm->reg[be32];
		case MEMORY:	return &vm->mem[be32];
		case DEREF:	return &vm->mem[vm->mem[be32]];
		case LITERAL:	return &be32;
		default:	printf("Illegal operand type %d\n", optype); return NULL;
	}
}

void cpu_run(vm_t *vm, int pc)
{
	int oper, typf, typA, typB;
	unsigned long *oprA, *oprB;

	while(1) {
		/* read in the instruction code */
		oper = READ;

		/* read in the two operand-type nibbles */
		typf = READ;
		typA = (typf & 0xf0) >> 4;
		typB = (typf & 0xf);

		switch(oper) {
			case OP_HALT:	return;
			case OP_MOV32:
				TWO_OPERANDS;
				*operand_pointer(vm, typA, oprA) =
				 *operand_pointer(vm, typB, oprB);
				break;
			default:	printf("Sorry, instruction 0x%x is not in "
					       "my repertoire at the moment.\n", oper);
					return;
		}
	}
}
