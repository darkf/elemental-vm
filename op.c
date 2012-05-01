#include "op.h"
#include <stdlib.h>

const char *op_tostr(signed int op)
{
	if(op > OP_COUNT) return NULL;
	return opnames[op];
}
