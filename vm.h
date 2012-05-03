#ifndef VM_H_INCLUDED
#define VM_H_INCLUDED

#include <stdint.h>

#define MEM_LENGTH (1024*1024*32) /* 32 mb */
#define STACK_SIZE (1024)

/* the registers */
#define FLAG	(32+1-1)
#define SP	(32+2-1)
#define IP	(32+3-1)

typedef uint8_t ubyte;

typedef struct {
	ubyte *mem;		 /* byte indexing */
	unsigned long reg[32+3]; /* 32 GP + FLAG + SP + IP */
	unsigned long base_addr;
} vm_t;

int vm_init(vm_t *);
int vm_begin(vm_t *);
int vm_quit(vm_t *);

#endif
