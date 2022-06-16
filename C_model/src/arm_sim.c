#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arm3.h"
#include "mem.h"
#include "ElfObj.h"

typedef enum _etat {STOP, NEXT, BREAK, CONT, RUN} Etat;

int main(int argc, char *argv[])
	{
	Arm3 *proc;
	Etat etat = NEXT;
	int cpt=1;
	int retV = 0;


	if ((argc < 2) || (argc > 3))
		{
		fprintf(stderr, "Usage : arm_sim elf_file [-run]\n");
		return 1;
		}

	if (argc == 3)
		{
		if (strcmp(argv[2], "-run") != 0)
			{
			fprintf(stderr, "Usage : arm_sim elf_file [-run]\n");
			return 1;
			}
		etat = RUN;
		}

	// Chargement de la memoire
	init_mem(argv[1]);

	// Demarage du proc
	proc = create_arm3();

	do
		{
		uint32_t inst;
		InstDec *pins = NULL;
		int charlu;

		fprintf(stdout, "-------------- Instruction %d -----------\n", cpt++);
		print_arm3(proc, stdout);
		inst = mem_lw(proc->pc);
		pins = decod(inst);
		fprintf(stdout, "IR = 0x%08x\t", inst);
		print_inst(stdout, pins);
		free(pins);

		if (proc->pc == GoodAdr)
			{
			fprintf(stderr, "Test GOOD !!!!\n");
			etat = STOP;
			}
		if (proc->pc == BadAdr)
			{
			fprintf(stderr, "Test BAD !!!!\n");
			retV = 1;
			etat = STOP;
			}
		
		if ((etat != RUN) && (etat != STOP))
			{
			fprintf(stdout, "Choix : ");
			if ((charlu = fgetc(stdin)) == '\n') charlu = 'n';
			else while (fgetc(stdin) != '\n');
			//fprintf(stdout, "%c");
			switch(charlu)
				{
				case 'n' :
					etat = NEXT;
					break;
				case 'c' :
					etat = CONT;
					break;
				case 'r' :
					etat = RUN;
					break;
				case 'q' :
					etat = STOP;
					break;
				case 's' :
					etat = STOP;
					break;
				default :
					fprintf(stderr, "Choix non reconnu\n");
				}
			}
		if (etat != STOP) arm3_step(proc);
		}
	while (etat != STOP);

	mem_free();
	del_arm3(proc);
	return retV;
	}
