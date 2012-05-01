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

int vm_quit(vm_t *vm)
{
	free(vm->mem);
	return 1;
}

int vm_begin(vm_t *vm)
{
	/* this little program sets register 0 to literal value 123 */
	vm->reg[IP] = 0x0;			/* program is at 0x0 */
	vm->mem[0x0] = OP_MOV32;
	vm->mem[0x1] = (REGISTER << 4) | LITERAL;
	vm->mem[0x2] = 0;			/* dest: big-endian 32-bit 0 */
	vm->mem[0x3] = 0;
	vm->mem[0x4] = 0;
	vm->mem[0x5] = 0;
	vm->mem[0x6] = 0;			/* src: big-endian 32-bit 123 */
	vm->mem[0x7] = 0;
	vm->mem[0x8] = 0;
	vm->mem[0x9] = 123;
	vm->mem[0xA] = OP_HALT;

	/* run the program */
	cpu_run(vm);

	/* pure debug code here -- check if the
	   program worked */
	printf("register 0 = %ld\n", vm->reg[0]);

	return 0;
}
