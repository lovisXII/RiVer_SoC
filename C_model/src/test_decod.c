#include <stdio.h>
#include <stdlib.h>

#include "decod.h"

int main(int argc, char *argv[])
	{
	InstDec *pins = NULL;

	if (argc != 2)
		{
		fprintf(stderr, "Usage : test_decod instr_code\n");
		return 1;
		}
	
	if ((pins = decod(strtoul(argv[1], NULL, 16))) == NULL)
		{
		fprintf(stderr, "Erreur lors du decodage -> NULL\n");
		return 1;
		}
	
	print_inst(stdout, pins);

	free(pins);
	return 0;
	}
