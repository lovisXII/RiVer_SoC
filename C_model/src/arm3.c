#include <stdio.h>
#include <stdlib.h>

#include "arm3.h"
#include "decod.h"
#include "mem.h"

Arm3 *create_arm3()
	{
	Arm3 *proc = malloc( sizeof(Arm3));

	if (proc == NULL)
		{
		fprintf(stderr, "Erreur allocation Arm3 -> NULL\n");
		exit(1);
		}
	
	proc->pc = 0x0;

	proc->Z = CLEAR;
	proc->N = CLEAR;
	proc->C = CLEAR;
	proc->V = CLEAR;

	return proc;
	}
	

void del_arm3(Arm3 *proc)
	{
	free(proc);
	}

void print_arm3(Arm3 *proc, FILE *f)
	{
	Reg r;
	fprintf(f, "PC=0x%x\t SP=0x%x\t N=%u Z=%u C=%u V=%u", proc->pc, proc->sp, proc->N, proc->Z, proc->C, proc->V);
	for (r=R0; r<PC; r++)
		{
		if ((r % 5) == 0) fputc ('\n', f);
		fprintf(f, "R%u=0x%08x ", r, proc->r[r]);
		}
	fputc('\n', f);
	}
	

void arm3_step(Arm3 *proc)
	{
	uint32_t code;
	InstDec *pins;
	Flag exec = CLEAR;

	/* FETCH */
	code = mem_lw(proc->pc);
	proc->pc += 4;

	/* DECODE */
	pins = decod(code);

	switch (pins->cond)
		{
		case Z_SET :
			if (proc->Z) exec = SET;
			break;
		case Z_CLEAR :
			if (!proc->Z) exec = SET;
			break;
		case C_SET :
			if (proc->C) exec = SET;
			break;
		case C_CLEAR :
			if (!proc->C) exec = SET;
			break;
		case N_SET :
			if (proc->N) exec = SET;
			break;
		case N_CLEAR :
			if (!proc->N) exec = SET;
			break;
		case V_SET :
			if (proc->V) exec = SET;
			break;
		case V_CLEAR :
			if (!proc->V) exec = SET;
			break;
		case C_NZ :
			if (proc->C && !proc->Z) exec = SET;
			break;
		case NC_Z :
			if (!proc->C || proc->Z) exec = SET;
			break;
		case N_eq_V :
			if (proc->N == proc->V) exec = SET;
			break;
		case N_ne_V :
			if (proc->N != proc->V) exec = SET;
			break;
		case NZ_N_eq_V :
			if (!proc->Z && (proc->N == proc->V)) exec = SET;
			break;
		case Z_or_N_ne_V :
			if (proc->Z || (proc->N != proc->V)) exec = SET;
			break;
		case ALWAYS :
			exec = SET;
			break;
		default : // INVAL :
			fprintf(stderr, "Code condition ivalide\n");
		}
	if (exec == CLEAR)
		{
		free(pins);
		return;
		}

	//proc->pc += 4; // pc + 8

	/* EXEC */
	uint32_t op2;
	Reg Rn;
	uint32_t Rnv;
	Reg Rm;
	uint32_t Rmv;
	Reg Rd;
	uint32_t Rdv;
	uint32_t Shiv;
	uint32_t signm;
	Flag newC = proc->C;
	Flag newV = proc->V;
	uint32_t toffset;
	uint32_t tadr;
	switch (pins->type)
		{

		case REGOP_T :
			//fprintf(stdout, "REGOP!\n");
			Rn = pins->op.regop.Rn;
			Rnv = (Rn == PC) ? proc->pc + 4 : proc->r[Rn];
			// shifter op2
			if (pins->op.regop.op2_t == IMM32)
				op2 = pins->op.regop.imm32;
			else
				{
				Rm = pins->op.regop.Rm;
				Rmv = (Rm == PC) ? proc->pc + 4 : proc->r[Rm];
				if (pins->op.regop.op2_t == REG_SHIFT) Shiv = proc->r[pins->op.regop.Rs];
				else Shiv = pins->op.regop.shift_imm;

				switch (pins->op.regop.shift)
					{
					case LSL :
						op2 = Rmv << Shiv;
						if (Shiv) newC = (Rmv & (1 << (32 - Shiv))) ? SET: CLEAR;
						break;
					case LSR :
						op2 = Rmv >> Shiv;
						newC = (Rmv & (1 << (Shiv - 1))) ? SET: CLEAR;
						break;
					case ASR :
						signm = (Rmv & 0x80000000) ? 0xFFFFFFFF << (32 - Shiv) : 0;
						op2 = signm | (Rmv >> Shiv);
						newC = (Rmv & (1 << (Shiv - 1))) ? SET: CLEAR;
						break;
					case ROR :
						if ((pins->op.regop.op2_t == SHIFT) && (Shiv == 0)) // RRX
							{
							op2 = (proc->C << 31) | (Rmv >> 1);
							newC = Rmv & 1;
							break;
							}
						op2 = (Rmv >> Shiv) | (Rmv << (32 - Shiv));
						newC = (Rmv & (1 << (Shiv -1))) ? SET: CLEAR;
					}
				}

			switch (pins->inst)
				{
				// Logical
				case AND :
					Rdv = Rnv & op2;
					break;
				case ORR :
					Rdv = Rnv | op2;
					break;
				case EOR :
					Rdv = Rnv ^ op2;
					break;
				case BIC :
					Rdv = Rnv & (~op2);
					break;
				case TEQ :
					Rdv = Rnv ^ op2;
					break;
				case TST :
					Rdv = Rnv & op2;
					break;

				// Arithmetic
				case ADD :
					//fprintf(stdout, "ADD op1=0x%08x op2=0x%08x\n", Rnv , op2);
					Rdv = Rnv + op2;
					newC = (Rdv < Rnv) ? SET : CLEAR;
					newV = ((NEG(Rnv) == NEG(op2)) && (NEG(Rdv) != NEG(Rnv))) ? SET : CLEAR;
					break;
				case ADC :
					Rdv = Rnv + op2 + proc->C;
					newC = (Rdv < Rnv) ? SET : CLEAR;
					newV = ((NEG(Rnv) == NEG(op2)) && (NEG(Rdv) != NEG(Rnv))) ? SET : CLEAR;
					break;
				case SUB :
					//Rdv = Rnv - op2;
					Rdv = Rnv + ~op2 + 1;
					newC = (Rdv <= Rnv) ? SET : CLEAR;
					newV = ((NEG(Rnv) != NEG(op2)) && (NEG(Rdv) != NEG(Rnv))) ? SET : CLEAR;
					break;
				case SBC :
					//Rdv = Rnv - op2 - (proc->C?0:1);
					Rdv = Rnv + ~op2 + (proc->C?1:0);
					newC = (Rdv < (Rnv + (proc->C?1:0))) ? SET : CLEAR;
					newV = ((NEG(Rnv) != NEG(op2)) && (NEG(Rdv) != NEG(Rnv))) ? SET : CLEAR;
					break;
				case RSB :
					//Rdv = op2 - Rnv;
					Rdv = op2 + ~Rnv + 1;
					newC = (Rdv <= op2) ? SET : CLEAR;
					newV = ((NEG(Rnv) != NEG(op2)) && (NEG(Rdv) != NEG(op2))) ? SET : CLEAR;
					break;
				case RSC :
					//Rdv = op2 - Rnv - (proc->C?0:1);
					Rdv = op2 + ~Rnv + (proc->C?1:0);
					newC = (Rdv < (op2 + (proc->C?1:0))) ? SET : CLEAR;
					newV = ((NEG(Rnv) != NEG(op2)) && (NEG(Rdv) != NEG(op2))) ? SET : CLEAR;
					break;
				case CMP :
					Rdv = Rnv - op2;
					newC = (Rdv <= Rnv) ? SET : CLEAR;
					newV = ((NEG(Rnv) != NEG(op2)) && (NEG(Rdv) != NEG(Rnv))) ? SET : CLEAR;
					break;
				case CMN :
					Rdv = Rnv + op2;
					newC = (Rdv < Rnv) ? SET : CLEAR;
					newV = ((NEG(Rnv) == NEG(op2)) && (NEG(Rdv) != NEG(Rnv))) ? SET : CLEAR;
					break;

				// Transfer
				case MOV :
					Rdv = op2;
					break;
				default : //case MVN :
					Rdv = ~op2;
				}
			if ((pins->inst == TST) || (pins->inst == TEQ) || (pins->inst == CMP) || (pins->inst == CMN))
				{
				proc->N = NEG(Rdv);
				proc->Z = (Rdv == 0)? SET : CLEAR;
				proc->C = newC;
				proc->V = newV;
				}
			else
				{
				proc->r[pins->op.regop.Rd] = Rdv;
				if ((pins->op.regop.Rd != PC) && pins->op.regop.s)
					{
					proc->N = NEG(Rdv);
					proc->Z = (Rdv == 0)? SET : CLEAR;
					proc->C = newC;
					proc->V = newV;
					}
				}
			break;

		case MULT_T :
			if (pins->inst == MUL)
				Rdv = proc->r[pins->op.mult.Rm] * proc->r[pins->op.mult.Rs];
			else
				Rdv = proc->r[pins->op.mult.Rm] * proc->r[pins->op.mult.Rs] + proc->r[pins->op.mult.Rn];
			proc->r[pins->op.mult.Rd] = Rdv;
			if (pins->op.mult.s)
				{
				proc->N = NEG(Rdv);
				proc->Z = (Rdv == 0)? SET : CLEAR;
				}
			break;

		case SWAP_T :
			if (pins->inst == SWP)
				{
				uint32_t temp = mem_lw(proc->r[pins->op.swap.Rn]);
				mem_sw(proc->r[pins->op.swap.Rn], proc->r[pins->op.swap.Rm]);
				proc->r[pins->op.swap.Rd] = temp;
				}
			else
				{
				uint8_t temp = mem_lb(proc->r[pins->op.swap.Rn]);
				mem_sb(proc->r[pins->op.swap.Rn], proc->r[pins->op.swap.Rm]);
				proc->r[pins->op.swap.Rd] = temp;
				}
			break;

		case TRANS_T :
			Rd = pins->op.trans.Rd;
			Rn = pins->op.trans.Rn;
			Rnv = (Rn == PC) ? proc->pc + 4 : proc->r[Rn];
			switch (pins->op.trans.offset) // Offset
				{
				case IMM_OFF : 
				case IMMPRE : 
				case IMMPOST : 
					toffset = pins->op.trans.off12;
					break;

				case REG_OFF : 
				case REGPRE : 
				case REGPOST : 
					toffset = proc->r[pins->op.trans.Rm];
					break;

				case SCAL_OFF :
				case SCALPRE : 
				case SCALPOST :
					Shiv = pins->op.trans.shimm5;
					Rmv = proc->r[pins->op.trans.Rm];
					switch (pins->op.trans.shift)
						{
						case LSL :
							toffset = Rmv << Shiv;
							break;
						case LSR :
							toffset = Rmv >> Shiv;
							break;
						case ASR :
							toffset = Rmv >> Shiv;
							break;
						case ROR :
							toffset = (Rmv >> Shiv) | (Rmv << (32 - Shiv));
						}
				}

			switch (pins->op.trans.offset) // Adr
				{
				case IMM_OFF : 
				case REG_OFF : 
				case SCAL_OFF :
					tadr = pins->op.trans.u ? Rnv + toffset : Rnv - toffset;
					break;

				case IMMPRE : 
				case REGPRE : 
				case SCALPRE :
					tadr = pins->op.trans.u ? Rnv + toffset : Rnv - toffset;
					proc->r[Rn] = tadr; 
					break;

				case IMMPOST : 
				case REGPOST : 
				case SCALPOST :
					tadr = proc->r[Rn]; 
					proc->r[Rn] = pins->op.trans.u ? proc->r[Rn] + toffset : proc->r[Rn] - toffset;

				}

			switch (pins->inst) // Mem transfert
				{
				case LDR :
					proc->r[Rd] = mem_lw(tadr);
					break;
				case LDRB :
					proc->r[Rd] = mem_lb(tadr);
					break;
				case STR :
					mem_sw(tadr, proc->r[Rd]);
					break;
				default :
					mem_sb(tadr, proc->r[Rd]);
				}
			break;

		case MTRANS_T :
			Rn = pins->op.mtrans.Rn;
			tadr = proc->r[Rn];
			if ((pins->op.mtrans.mode == INC_AFTER) || (pins->op.mtrans.mode == INC_BEFORE))
				{
				for (Rd = R0; Rd <= PC; Rd++)
					if ( pins->op.mtrans.reg_list & ( 1 << Rd))
						{
						if (pins->op.mtrans.mode == INC_AFTER)
							{
							if (pins->inst == LDM) proc->r[Rd] = mem_lw(tadr);
							else mem_sw(tadr, (Rd == PC) ? proc->r[Rd] + 8 : proc->r[Rd]);
							tadr += 4;
							}
						else
							{
							tadr += 4;
							if (pins->inst == LDM) proc->r[Rd] = mem_lw(tadr);
							else mem_sw(tadr, (Rd == PC) ? proc->r[Rd] + 8 : proc->r[Rd]);
							}
						}
				}
			else
				{
				for (Rd = PC + 1; Rd-- > R0;)
					if ( pins->op.mtrans.reg_list & ( 1 << Rd))
						{
						if (pins->op.mtrans.mode == DEC_AFTER)
							{
							if (pins->inst == LDM) proc->r[Rd] = mem_lw(tadr);
							else mem_sw(tadr, (Rd == PC) ? proc->r[Rd] + 8 : proc->r[Rd]);
							tadr -= 4;
							}
						else
							{
							tadr -= 4;
							if (pins->inst == LDM) proc->r[Rd] = mem_lw(tadr);
							else mem_sw(tadr, (Rd == PC) ? proc->r[Rd] + 8 : proc->r[Rd]);
							}
						}
				}
			if (pins->op.mtrans.w) proc->r[Rn] = tadr;
			break;

		default ://	case BRANCH_T :
			if (pins->inst == BL) proc->lr = proc->pc;
			proc->pc += (pins->op.branch.offset << 2) + 4;
		}

	//proc->pc -= 4; // pc + 4
	free(pins);
	}

void arm3_run(Arm3 *proc, int nbcycles)
	{
	while (nbcycles--) arm3_step(proc);
	}
