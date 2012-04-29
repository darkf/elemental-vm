#ifndef BIOS_H_INCLUDED
#define BIOS_H_INCLUDED

#include "vm.h"

void bios_handle_interrupt(vm_t *); /* int 0 */

#endif