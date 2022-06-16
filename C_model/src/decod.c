#include <stdio.h>
#include <stdlib.h>

#include "decod.h"

/* Chaines pour affichage */

static const char *mnem_ext[] = {"eq",
                          "ne",
                          "hs",
                          "lo",
                          "mi",
                          "pl",
                          "vs",
                          "vc",
                          "hi",
                          "ls",
                          "ge",
                          "lt",
                          "gt",
                          "le",
                          "", // ou "al"
                          "-"};

static const char *mnemoniq[] = {"and",
                          "eor",
                          "sub",
                          "rsb",
                          "add",
                          "adc",
                          "sbc",
                          "rsc",
                          "tst",
                          "teq",
                          "cmp",
                          "cmn",
                          "orr",
                          "mov",
                          "bic",
                          "mvn",
                          "mul",
                          "mla",
                          "swp",
                          "swpb",
                          "ldr",
                          "ldrb",
                          "str",
                          "strb",
                          "ldm",
                          "stm",
                          "b",
                          "bl",
                          "ldc",
                          "stc",
                          "cdp",
                          "mcr",
                          "mrc",
                          "swi"};

static const char *registres[] = {" r0"," r1"," r2"," r3"," r4"," r5"," r6"," r7",
											" r8"," r9"," r10"," fp"," r12"," sp"," lr"," pc"};

static const char *mnem_shift[] = {"lsl", "lsr", "asr", "ror"};

static const char *mnem_ldm[2][4] = {{"ia", "ib", "da", "db"}, {"fd", "ed", "fa", "ea"}};
static const char *mnem_stm[2][4] = {{"ia", "ib", "da", "db"}, {"ea", "fa", "ed", "fd"}};

void print_inst(FILE *f, InstDec *pins)
	{
	int i;

	fputs(mnemoniq[pins->inst], f);
	fputs(mnem_ext[pins->cond], f);

	switch (pins->type)
		{
		case REGOP_T :
			if (pins->op.regop.s)
				if ((pins->inst != CMN) && (pins->inst != CMP) &&
						(pins->inst != TEQ) && (pins->inst != TST))  fputs("s", f);

			switch (pins->inst)
				{
				case MOV :
				case MVN :
					fputs(registres[pins->op.regop.Rd], f);
					fputc(',', f);
					break;

				case ADC :
				case ADD :
				case AND :
				case BIC :
				case EOR :
				case ORR :
				case RSB :
				case RSC :
				case SBC :
				case SUB :
					fputs(registres[pins->op.regop.Rd], f);
					fputc(',', f);

				default :
				/*case CMN :
				case CMP :
				case TEQ :
				case TST :*/
					fputs(registres[pins->op.regop.Rn], f);
					fputc(',', f);
					break;
				}
			switch (pins->op.regop.op2_t)
				{
				// shifter op2
				case IMM32 :
					fprintf(f, " #%u\n", pins->op.regop.imm32);
					return;

				case SHIFT :
					fputs(registres[pins->op.regop.Rm], f);
					if ((pins->op.regop.shift == LSL) && (pins->op.regop.shift_imm == 0))
						{
						fputc('\n', f);
						return;
						}
					if ((pins->op.regop.shift == ROR) && (pins->op.regop.shift_imm == 0))
						{
						fputs( ", rrx\n", f);
						return;
						}
					fputc(',', f);
					fprintf(f, " %s#%u", mnem_shift[pins->op.regop.shift], pins->op.regop.shift_imm);
					break;

				case REG_SHIFT :
					fputs(registres[pins->op.regop.Rm], f);
					fputc(',', f);
					fputs(mnem_shift[pins->op.regop.shift], f);
					fputs(registres[pins->op.regop.Rs], f);
				}
			break;

		case MULT_T :
			if (pins->op.mult.s) fputs("s", f);
			fputs(registres[pins->op.mult.Rd], f);
			fputc(',', f);
			fputs(registres[pins->op.mult.Rm], f);
			fputc(',', f);
			fputs(registres[pins->op.mult.Rs], f);
			if (pins->inst == MLA) fprintf(f, ",%s", registres[pins->op.mult.Rn]);
			break;

		case SWAP_T :
			fprintf(f, ",%s,%s,[%s]", registres[pins->op.swap.Rd], registres[pins->op.swap.Rd],
												registres[pins->op.swap.Rd] + 1);
			break;

		case TRANS_T :
			fputs(registres[pins->op.regop.Rd], f);
			fputc(',', f);

			switch (pins->op.trans.offset)
				{
				case IMM_OFF : 
					fprintf(f, " [%s, #%s%u]", registres[pins->op.trans.Rn] + 1,
									  				pins->op.trans.u ? "" : "-", pins->op.trans.off12);
					break;

				case IMMPRE : 
					fprintf(f, " [%s, #%s%u]!", registres[pins->op.trans.Rn] + 1,
									  				pins->op.trans.u ? "" : "-", pins->op.trans.off12);
					break;

				case IMMPOST : 
					fprintf(f, " [%s], #%s%u", registres[pins->op.trans.Rn] + 1,
									  				pins->op.trans.u ? "" : "-", pins->op.trans.off12);
					break;

				case REG_OFF : 
					fprintf(f, " [%s, #%s%s]", registres[pins->op.trans.Rn] + 1,
									  				pins->op.trans.u ? "" : "-",
									  				registres[pins->op.trans.Rm] + 1);
					break;

				case REGPRE : 
					fprintf(f, " [%s, #%s%s]!", registres[pins->op.trans.Rn] + 1,
									  				pins->op.trans.u ? "" : "-",
									  				registres[pins->op.trans.Rm] + 1);
					break;

				case REGPOST : 
					fprintf(f, " [%s], #%s%s", registres[pins->op.trans.Rn] + 1,
									  				pins->op.trans.u ? "" : "-",
									  				registres[pins->op.trans.Rm] + 1);
					break;

				case SCAL_OFF :
					fprintf(f, " [%s, #%s%s, %s #%u]", registres[pins->op.trans.Rn] + 1,
									  				pins->op.trans.u ? "" : "-",
									  				registres[pins->op.trans.Rm] + 1,
									  				mnem_shift[pins->op.trans.shift],
									  				pins->op.trans.shimm5);
					break;

				case SCALPRE : 
					fprintf(f, " [%s, #%s%s, %s #%u]!", registres[pins->op.trans.Rn] + 1,
									  				pins->op.trans.u ? "" : "-",
									  				registres[pins->op.trans.Rm] + 1,
									  				mnem_shift[pins->op.trans.shift],
									  				pins->op.trans.shimm5);
					break;

				case SCALPOST :
					fprintf(f, " [%s], #%s%s, %s #%u", registres[pins->op.trans.Rn] + 1,
														pins->op.trans.u ? "" : "-",
														registres[pins->op.trans.Rm] + 1,
														mnem_shift[pins->op.trans.shift],
														pins->op.trans.shimm5);
				}
			break;

		case MTRANS_T :
			if (pins->inst == LDM)
				fputs(mnem_ldm[(pins->op.mtrans.Rn == SP) ? 1: 0][pins->op.mtrans.mode], f);
			else
				fputs(mnem_stm[(pins->op.mtrans.Rn == SP) ? 1: 0][pins->op.mtrans.mode], f);
			fputs(registres[pins->op.mtrans.Rn], f);
			if (pins->op.mtrans.w) fputs("!", f);
			fputs(", {", f);
			for (i=R0; i<=PC; i++)
				if (pins->op.mtrans.reg_list & ( 1 << i)) 
					{
					fputs(registres[i] +1, f);
					break;
					}
			for (i++; i<=PC; i++)
				if (pins->op.mtrans.reg_list & ( 1 << i)) fprintf(f, ",%s", registres[i]);
			fputc('}', f);

			break;
		case BRANCH_T :
			fprintf (f, " <PC + 0x%x>", (pins->op.branch.offset << 2) + 8);
			break;
		default :
			fputs("Type instruction inconnue!\n", stderr);
			exit(1);
		}

	fputc('\n', f);
	}

InstDec *create_instdec(Inst_t type, Inst inst, Cond cond)
	{
	InstDec *pins = malloc(sizeof(InstDec));

	if (pins == NULL)
		{
		fprintf(stderr, "Erreur allocation InstDec -> NULL\n");
		exit(1);
		}
	
	pins->type = type;
	pins->inst = inst;
	pins->cond = cond;

	return pins;
	}

InstDec *decod(uint32_t code)
	{
	InstDec *pins = NULL;

	if ((code & REGOP_MSK) == REGOP_VAL)
		{

		if ((code & MULT_MSK) == MULT_VAL) //type = MULT_T;
			{
			pins = create_instdec(MULT_T, A21(code) ? MLA : MUL, COND(code));
			pins->op.mult.Rd = R19_16(code);
			pins->op.mult.Rn = R15_12(code);
			pins->op.mult.Rs = R11_8(code);
			pins->op.mult.Rm = R3_0(code);
			pins->op.mult.s = S20(code);
			return pins;
			}

		if ((code & SWAP_MSK) == SWAP_VAL) //type = SWAP_T;
			{
			pins = create_instdec(MULT_T, A21(code) ? MLA : MUL, COND(code));
			pins->op.swap.Rn = R19_16(code);
			pins->op.swap.Rd = R15_12(code);
			pins->op.swap.Rm = R3_0(code);
			return pins;
			}
		
		//type = REGOP_T;
		pins = create_instdec(REGOP_T, OPCODE(code), COND(code));
		pins->op.regop.Rn = R19_16(code);
		pins->op.regop.Rd = R15_12(code);
		pins->op.regop.s = S20(code);

		if (I25(code))
			{
			uint32_t imm32 = IMM_8(code);
			int imm_shift = IMM_CD(code) << 1;
			pins->op.regop.imm32 = (imm32 << (32 - imm_shift)) | (imm32 >> imm_shift);
			pins->op.regop.op2_t = IMM32;
			return pins;
			}

		pins->op.regop.shift = SHOP_SH(code);
		pins->op.regop.Rm = R3_0(code);

		if (SHOP_R(code))
			{
			pins->op.regop.op2_t = REG_SHIFT;
			pins->op.regop.Rs = R11_8(code);
			return pins;
			}
		pins->op.regop.op2_t = SHIFT;
		pins->op.regop.shift_imm = SHOP_SHIMM(code);
		return pins;
		}

	if ((code & TRANS_MSK) == TRANS_VAL) //type = TRANS_T;
		{
		Inst inst;

		if (L20(code))
			inst = (B22(code))?LDRB:LDR;
		else
			inst = (B22(code))?STRB:STR;

		pins = create_instdec(TRANS_T, inst, COND(code));
		pins->op.trans.Rn = R19_16(code);
		pins->op.trans.Rd = R15_12(code);
		pins->op.trans.u = U23(code);

		if (I25(code))
			{
			pins->op.trans.Rm = R3_0(code);
			if (code & 0xff0)
				{
				pins->op.trans.shift = SHOP_SH(code);
				pins->op.trans.shimm5 = SHOP_SHIMM(code);
				if (P24(code))
					if (W21(code))
						pins->op.trans.offset = SCALPRE;
					else
						pins->op.trans.offset = SCAL_OFF;
				else
					pins->op.trans.offset = SCALPOST;
						
				return pins;
				}

			if (P24(code))
				if (W21(code))
					pins->op.trans.offset = REGPRE;
				else
					pins->op.trans.offset = REG_OFF;
			else
				pins->op.trans.offset = REGPOST;
					
			return pins;
			}

		pins->op.trans.off12 = OFF_12(code);
		if (P24(code))
			if (W21(code))
				pins->op.trans.offset = IMMPRE;
			else
				pins->op.trans.offset = IMM_OFF;
		else
			pins->op.trans.offset = IMMPOST;
				
		return pins;
		}

	if ((code & MTRANS_MSK) == MTRANS_VAL) // type = MTRANS_T;
		{
		pins = create_instdec(MTRANS_T, L20(code)?LDM:STM, COND(code));
		pins->op.mtrans.Rn = R19_16(code);
		pins->op.mtrans.reg_list = code & 0xffff;
		pins->op.mtrans.w = W21(code);
		if (U23(code))
			if (P24(code))
				pins->op.mtrans.mode = INC_BEFORE; 
			else
				pins->op.mtrans.mode = INC_AFTER; 
		else
			if (P24(code))
				pins->op.mtrans.mode = DEC_BEFORE; 
			else
				pins->op.mtrans.mode = DEC_AFTER; 
				
		return pins;
		}

	if ((code & BRANCH_MSK) == BRANCH_VAL) // type = BRANCH_T;
		{
		pins = create_instdec(BRANCH_T, L24(code)?BL:B, COND(code));
		pins->op.branch.offset = (OFF_24S(code)) ? 0xff000000 | OFF_24(code): OFF_24(code);
		return pins;
		}

	/*
	else if ((code & CODTRANS_MSK) == CODTRANS_VAL) type = CODTRANS_T;
	else if ((code & COREGOP_MSK) == COREGOP_VAL) type = COREGOP_T;
	else if ((code & CORTRANS_MSK) == CORTRANS_VAL) type = CORTRANS_T;
	else if ((code & SWI_MSK) == SWI_VAL) type = SWI_T;
	*/

	fprintf(stderr, "Type instruction inconnue : 0x%80x\n", code);
	return NULL;
	}
