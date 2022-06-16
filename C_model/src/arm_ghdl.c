#include <stdio.h>
#include "mem.h"

extern int ghdl_main(int argc, char *argv[]);

int main(int argc, char *argv[])
	{
	int retv;

	// Chargement de la memoire
	init_mem(argv[1]);

	// Demarage de ghdl
	retv = ghdl_main(argc - 1, argv + 1);

	mem_free();
	return retv;
	}
