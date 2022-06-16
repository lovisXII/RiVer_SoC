#ifndef ARM3
#define ARM3

#include "decod.h"

#define NEG(v) ((v) >> 31) 

typedef enum _testr {U, GOOD, BAD, TIMEOUT} TestR;

//extern TestR Sim_res;

typedef struct _arm3
	{
	uint32_t r[11];
	uint32_t fp;
	uint32_t r12;
	uint32_t sp;
	uint32_t lr;
	uint32_t pc;
	Flag Z;
	Flag N;
	Flag C;
	Flag V;
	} Arm3;

Arm3 *create_arm3();
void del_arm3(Arm3 *proc);
void print_arm3(Arm3 *proc, FILE *f);

void arm3_step(Arm3 *proc);
void arm3_run(Arm3 *proc, int nbcycles);

#endif
