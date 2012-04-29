#ifndef VM_H_INCLUDED
#define VM_H_INCLUDED

#define MEM_LENGTH 1024*1024*32 /* 32 mb */

typedef struct {
	unsigned long *mem;
	unsigned long reg[32+3]; /* 32 GP + FLAG + SP + IP */
} vm_t;

int vm_init(vm_t *);
int vm_begin(vm_t *);

#endif
