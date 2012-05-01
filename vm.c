/* elemental vm
   copyright (c) 2012 darkf
   MIT licensed */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memset */
#include "vm.h"
#include "op.h"
#include "cpu.h"

#define STACK_SIZE (1024)

void load_program(vm_t *vm, long ptr)
{
	/* this little program sets register 0 to literal value 123 */
	vm->reg[IP] = ptr;			/* program is at 0x0 */
	vm->mem[ptr++] = OP_MOV32;
	vm->mem[ptr++] = (REGISTER << 4) | LITERAL;
	vm->mem[ptr++] = 0;			/* dest: big-endian 32-bit 0 */
	vm->mem[ptr++] = 0;
	vm->mem[ptr++] = 0;
	vm->mem[ptr++] = 0;
	vm->mem[ptr++] = 0;			/* src: big-endian 32-bit 123 */
	vm->mem[ptr++] = 0;
	vm->mem[ptr++] = 0;
	vm->mem[ptr++] = 123;
	vm->mem[ptr++] = OP_HALT;
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
	load_program(vm, base);

	return 1;
}

int vm_quit(vm_t *vm)
{
	free(vm->mem);
	return 1;
}

int vm_begin(vm_t *vm)
{
	/* run the program */
	cpu_run(vm);

	/* pure debug code here -- check if the
	   program worked */
	printf("register 0 = %ld\n", vm->reg[0]);

	return 0;
}
