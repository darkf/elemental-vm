/* elemental vm
   copyright (c) 2012 darkf
   MIT licensed */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memset */
#include "vm.h"
#include "op.h"
#include "cpu.h"

int vm_init(vm_t *vm)
{
	vm->mem = malloc(MEM_LENGTH);
	memset(vm->mem, 0, MEM_LENGTH);
	memset(vm->reg, 0, sizeof(vm->reg));

	return 1;
}

int vm_begin(vm_t *vm)
{
	/* sets register 0 to literal value 123 */
	vm->mem[0x0] = OP_MOV32;
	vm->mem[0x1] = (0x0 << 4) | (0x3);	/* register, literal */
	vm->mem[0x2] = 0;	/* big-endian 32-bit 0 */
	vm->mem[0x3] = 0;
	vm->mem[0x4] = 0;
	vm->mem[0x5] = 0;
	vm->mem[0x6] = 0;	/* big-endian 32-bit 123 */
	vm->mem[0x7] = 0;
	vm->mem[0x8] = 0;
	vm->mem[0x9] = 123;
	vm->mem[0xA] = OP_HALT;

	/* run this program we stored at address 0x0 */
	cpu_run(vm, 0x0);

	/* pure debug code here */
	printf("register 0 = %ld\n", vm->reg[0]);

	return 0;
}
