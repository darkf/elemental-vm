#include "vm.h"
#include "cpu.h"
#include "op.h"
#include "flags.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

#define ONE_OPERAND (oprA=mem_read_u32(vm))
#define TWO_OPERANDS (oprA=mem_read_u32(vm),oprB=mem_read_u32(vm))

/*
	Flags and conditional branching are yet another
	semi-x86-ripoff. For reference, see
	- http://en.wikibooks.org/wiki/X86_Assembly/X86_Architecture#EFLAGS_Register
	- http://en.wikibooks.org/wiki/X86_Assembly/Control_Flow
	as well as the elemental-vm spec, SPEC.txt.
*/

void set_flag(vm_t *vm, int flag, int value) {
	vm->reg[FLAG] &= ~(1 << flag);
	vm->reg[FLAG] |= (value << flag);
}

ubyte get_flag(vm_t* vm, int flag) {
	return (vm->reg[FLAG] >> flag) & 1;
}

unsigned long* opptr(vm_t *vm, int optype, ubyte* dat)
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
	unsigned long long temp;
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
				*opptr(vm, typA, oprA) = *opptr(vm, typB, oprB);
				break;
			/* x86 ADD, SUB, etc. set flags. I have
			   tried to properly implement what
			   the Intel x86 spec says on
			   flag-setting. TODO: test this */
			case OP_ADD:	TWO_OPERANDS;
				temp = *opptr(vm, typA, oprA)
					+ *opptr(vm, typB, oprB);
				set_flag(vm, ZF, temp == 0);
				set_flag(vm, OF, temp > ULONG_MAX);
				set_flag(vm, CF, temp > ULONG_MAX);
				set_flag(vm, SF, temp > (unsigned
					long long)LONG_MAX);
				*opptr(vm, typA, oprA) = temp;
				break;
			case OP_SUB:	TWO_OPERANDS;
				temp = *opptr(vm, typA, oprA)
					- *opptr(vm, typB, oprB);
				set_flag(vm, ZF, temp == 0);
				set_flag(vm, OF, *opptr(vm, typA, oprA)
					> *opptr(vm, typB, oprB));
				set_flag(vm, CF, *opptr(vm, typA, oprA)
					> *opptr(vm, typB, oprB));
				set_flag(vm, SF, temp > (unsigned
					long long)INT_MAX);
				*opptr(vm, typA, oprA) = temp;
				break;
			case OP_MUL:	TWO_OPERANDS;
				*opptr(vm, typA, oprA) *= *opptr(vm, typB, oprB);
				set_flag(vm, CF, *opptr(vm, typA, oprA) & 0xffff0000 == 0);
				set_flag(vm, OF, *opptr(vm, typA, oprA) & 0xffff0000 == 0);
				break;
			/* according to Intel, DIV has "undefined" effects on
			   flags */
			case OP_DIV:	TWO_OPERANDS;
				*opptr(vm, typA, oprA) /= *opptr(vm, typB, oprB);
				break;
			/* I don't think MOD is an actual Intel x86
			   instruction */
			case OP_MOD:	TWO_OPERANDS;
				*opptr(vm, typA, oprA) %= *opptr(vm, typB, oprB);
				break;
			/* TODO: flags for the operations below */
			case OP_SHL:	TWO_OPERANDS;
				*opptr(vm, typA, oprA) <<= *opptr(vm, typB, oprB);
				break;
			case OP_SHR:	TWO_OPERANDS;
				*opptr(vm, typA, oprA) >>= *opptr(vm, typB, oprB);
				break;
			case OP_AND:	TWO_OPERANDS;
				*opptr(vm, typA, oprA) &= *opptr(vm, typB, oprB);
				break;
			case OP_OR:	TWO_OPERANDS;
				*opptr(vm, typA, oprA) |= *opptr(vm, typB, oprB);
				break;
			case OP_NOT:	ONE_OPERAND;
				*opptr(vm, typA, oprA) = ~*opptr(vm, typA, oprA);
				break;
			case OP_XOR:	TWO_OPERANDS;
				*opptr(vm, typA, oprA) ^= *opptr(vm, typB, oprB);
				break;
			case OP_ZERO:	ONE_OPERAND;
				*opptr(vm, typA, oprA) = 0;
				break;
			case OP_CMP:	TWO_OPERANDS;
				/* CMP instruction based on the one
				   in the x86 book linked above */
				temp = *opptr(vm, typA, oprA) - *opptr(vm, typB, oprB);
				set_flag(vm, ZF,
					*opptr(vm, typA, oprA) ==
					 *opptr(vm, typB, oprB));
				set_flag(vm, OF, temp > ULONG_MAX &&
					(*opptr(vm, typA, oprA) >
					 *opptr(vm, typB, oprB)));
				set_flag(vm, CF,
					*opptr(vm, typB, oprB) >
					 *opptr(vm, typA, oprA));
				set_flag(vm, SF,
					*opptr(vm, typB, oprB) >
					 *opptr(vm, typA, oprA));
				break;
			case OP_B: ONE_OPERAND;
				do_branch:
				vm->reg[IP] = *opptr(vm, typA, oprA);
				break;

			/* the flag conditions are taken
			   from the conditional branching
			   page of the x86 book linked
			   above */

			case OP_BZ: ONE_OPERAND;
				if(get_flag(vm, ZF))	goto do_branch;
				break;
			case OP_BNZ: ONE_OPERAND;
				if(!get_flag(vm, ZF))	goto do_branch;
				break;
			case OP_BLT: ONE_OPERAND;
				if(get_flag(vm, SF) !=
				   get_flag(vm, OF))	goto do_branch;
				break;
			case OP_BGT: ONE_OPERAND;
				if(!get_flag(vm, ZF) &&
					get_flag(vm, SF) == get_flag(vm, OF))
					goto do_branch;
				break;
			case OP_BE: ONE_OPERAND;
				if(get_flag(vm, ZF))	goto do_branch;
				break;
			case OP_BNE: ONE_OPERAND;
				if(!get_flag(vm, ZF))	goto do_branch;
				break;
			default:
				printf("Unknown instruction 0x%x (%s)",
					oper, op_tostr(oper));
				printf(" -- halting VM\n");
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

