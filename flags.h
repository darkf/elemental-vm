/*
        Flags and conditional branching are yet another
        semi-x86-ripoff. For reference, see
        - http://en.wikibooks.org/wiki/X86_Assembly/X86_Architecture#EFLAGS_Register
        - http://en.wikibooks.org/wiki/X86_Assembly/Control_Flow
*/

/* see the spec for details */
enum {
	ZF = 0,
	OF,
	CF,
	EF,
	SF
};
