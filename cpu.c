#include "vm.h"
#include "cpu.h"
#include "op.h"
#include <stdlib.h>
#include <stdio.h>

#define ONE_OPERAND (oprA=mem_read_u32(vm))
#define TWO_OPERANDS (oprA=mem_read_u32(vm),oprB=mem_read_u32(vm))

unsigned long* operand_pointer(vm_t *vm, int optype, ubyte* dat)
{
	static unsigned long be32;
	be32 = (*dat << 24) | (dat[1] << 16) | (dat[2] << 8) | dat[3];

	switch(optype) {
		case REGISTER:	return (unsigned long *)&vm->reg[be32];
		case MEMORY:	return (unsigned long *)&vm->mem[be32];
		case DEREF:	return (unsigned long *)&vm->mem[vm->mem[be32]];
		case LITERAL:	return &be32;
		default:	printf("Illegal operand type %d\n", optype); return NULL;
	}
}

void cpu_run(vm_t *vm)
{
	int oper, typf, typA, typB;
	ubyte *oprA, *oprB;

	while(1) {
		/* read in the instruction code */
		oper = *mem_read_u8(vm);

		/* read in the two operand-type nibbles */
		typf = *mem_read_u8(vm);
		typA = (typf & 0xf0) >> 4;
		typB = (typf & 0xf);

		switch(oper) {
			case OP_HALT: return;

			case OP_MOV32:	TWO_OPERANDS;
				*operand_pointer(vm, typA, oprA) = *operand_pointer(vm, typB, oprB);
				break;
			case OP_ADD:	TWO_OPERANDS;
				*operand_pointer(vm, typA, oprA) += *operand_pointer(vm, typB, oprB);
				break;
			case OP_SUB:	TWO_OPERANDS;
				*operand_pointer(vm, typA, oprA) -= *operand_pointer(vm, typB, oprB);
				break;
			case OP_MUL:	TWO_OPERANDS;
				*operand_pointer(vm, typA, oprA) *= *operand_pointer(vm, typB, oprB);
				break;
			case OP_DIV:	TWO_OPERANDS;
				*operand_pointer(vm, typA, oprA) /= *operand_pointer(vm, typB, oprB);
				break;
			case OP_MOD:	TWO_OPERANDS;
				*operand_pointer(vm, typA, oprA) %= *operand_pointer(vm, typB, oprB);
				break;
			case OP_SHL:	TWO_OPERANDS;
				*operand_pointer(vm, typA, oprA) <<= *operand_pointer(vm, typB, oprB);
				break;
			case OP_SHR:	TWO_OPERANDS;
				*operand_pointer(vm, typA, oprA) >>= *operand_pointer(vm, typB, oprB);
				break;
			case OP_AND:	TWO_OPERANDS;
				*operand_pointer(vm, typA, oprA) &= *operand_pointer(vm, typB, oprB);
				break;
			case OP_OR:	TWO_OPERANDS;
				*operand_pointer(vm, typA, oprA) |= *operand_pointer(vm, typB, oprB);
				break;
			case OP_NOT:	ONE_OPERAND;
				*operand_pointer(vm, typA, oprA) = ~*operand_pointer(vm, typA, oprA);
				break;
			case OP_XOR:	TWO_OPERANDS;
				*operand_pointer(vm, typA, oprA) ^= *operand_pointer(vm, typB, oprB);
				break;
			case OP_ZERO:	ONE_OPERAND;
				*operand_pointer(vm, typA, oprA) = 0;
				break;
			default:
				printf("Unknown instruction 0x%x\n", oper);
				return;
		}
	}
}

inline ubyte* mem_read_u8(vm_t *vm)
{	
	return (ubyte *)&vm->mem[vm->reg[IP]++];
}

inline ubyte* mem_read_u32(vm_t *vm)
{	
	ubyte* ptr = (ubyte *)&vm->mem[vm->reg[IP]];
	vm->reg[IP] += 4;
	return ptr;
}
