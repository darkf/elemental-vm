#include "vm.h"

void cpu_run(vm_t *vm);
unsigned long* opptr(vm_t *vm, int optype, ubyte* dat);

inline ubyte* mem_read_u8(vm_t *vm);
inline ubyte* mem_read_u32(vm_t *vm);
