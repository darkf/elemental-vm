const char *op_tostr(int op)
{
	const char *str = (const char *) malloc(6 /* len("mov.32") */ + 1);
	switch(op) {
		default: strcpy(str, "unknown");
	}
	return str;
}