#ifndef DECOD
#define DECOD

#include <stdint.h>

typedef enum _inst_t {REGOP_T = 1,
                      MULT_T,
                      SWAP_T,
                      TRANS_T,
                      MTRANS_T,
                      BRANCH_T,
                      CODTRANS_T,
                      COREGOP_T,
                      CORTRANS_T,
                      SWI_T} Inst_t;

#define REGOP_MSK    0x0c000000
#define MULT_MSK     0x0fc000f0
#define SWAP_MSK     0x0fb00ff0
#define TRANS_MSK    0x0c000000
#define MTRANS_MSK   0x0e000000
#define BRANCH_MSK   0x0e000000
#define CODTRANS_MSK 0x0e000000
#define COREGOP_MSK  0x0f000010
#define CORTRANS_MSK 0x0f000010
#define SWI_MSK      0x0f000000

#define REGOP_VAL    0x00000000
#define MULT_VAL     0x00000090
#define SWAP_VAL     0x01000090
#define TRANS_VAL    0x04000000
#define MTRANS_VAL   0x08000000
#define BRANCH_VAL   0x0a000000
#define CODTRANS_VAL 0x0c000000
#define COREGOP_VAL  0x0e000000
#define CORTRANS_VAL 0x0e000010
#define SWI_VAL      0x0f000000

typedef enum _cond {Z_SET = 0,
                    Z_CLEAR = 1,
						  C_SET = 2,
						  C_CLEAR = 3,
						  N_SET = 4,
						  N_CLEAR = 5,
						  V_SET = 6,
						  V_CLEAR = 7,
						  C_NZ = 8,
						  NC_Z = 9,
						  N_eq_V = 10,
						  N_ne_V = 11,
						  NZ_N_eq_V = 12,
						  Z_or_N_ne_V = 13,
						  ALWAYS = 14,
						  INVAL = 15} Cond;

#define COND(code) ((code) >> 28)

typedef enum _inst {AND = 0,
                    EOR = 1,
						  SUB = 2,
						  RSB = 3,
						  ADD = 4,
						  ADC = 5,
						  SBC = 6,
						  RSC = 7,
						  TST = 8,
						  TEQ = 9,
						  CMP = 10,
						  CMN = 11,
						  ORR = 12,
						  MOV = 13,
						  BIC = 14,
						  MVN = 15,
						  MUL,
						  MLA,
						  SWP,
						  SWPB,
						  LDR,
						  LDRB,
						  STR,
						  STRB,
						  LDM,
						  STM,
						  B,
						  BL,
						  LDC,
						  STC,
						  CDP,
						  MCR,
						  MRC,
						  SWI} Inst;

#define OPCODE(code) (((code) & 0x01e00000) >> 21)
#define A21(code) ((code) & 0x00200000)
#define B22(code) ((code) & 0x00400000)
#define L20(code) ((code) & 0x00100000)
#define L24(code) ((code) & 0x01000000)

typedef enum _reg {R0 = 0,R1,R2,R3,R4,R5,R6,R7,R8,R9,R10,FP,R12,SP,LR,PC,RU} Reg;

#define S20(code) ((code) & 0x00100000)
#define U23(code) ((code) & 0x00800000)
#define I25(code) ((code) & 0x02000000)
#define R19_16(code) (((code) & 0x000f0000) >> 16)
#define R15_12(code) (((code) & 0x0000f000) >> 12)
#define R11_8(code) (((code) & 0x00000f00) >> 8)
#define R3_0(code) ((code) & 0x0000000f)
#define P24(code) ((code) & 0x01000000)
#define W21(code) ((code) & 0x00200000)
#define SHOP_R(code) (((code) & 0x00000010) >> 4)
#define SHOP_SH(code) (((code) & 0x00000060) >> 5)
#define SHOP_SHIMM(code) (((code) & 0x00000f80) >> 7)

#define IMM_8(code) ((code) & 0x000000ff)
#define IMM_CD(code) (((code) & 0x00000f00) >> 8)

#define OFF_12(code) ((code) & 0x00000fff)
#define OFF_24(code) ((code) & 0x00ffffff)
#define OFF_24S(code) ((code) & 0x00800000)

typedef enum _flag {CLEAR = 0, SET} Flag;

typedef enum _regop_op2 {IMM32 , SHIFT, REG_SHIFT} Regop_op2;

typedef enum _shift {LSL, LSR, ASR, ROR} Shift;

typedef enum _trans_offset {IMM_OFF , IMMPRE, IMMPOST, REG_OFF, REGPRE, REGPOST, SCAL_OFF, SCALPRE, SCALPOST} Trans_offset;

typedef enum _mtrans_mode {INC_AFTER , INC_BEFORE, DEC_AFTER, DEC_BEFORE} Mtrans_mode;

typedef struct _regop
	{
	Reg Rn;
	Reg Rd;
	Reg Rs;
	Reg Rm;
	Regop_op2 op2_t;
	uint32_t imm32;
	Shift shift;
	uint8_t shift_imm;
	Flag s;
	} Regop;

typedef struct _mult
	{
	Reg Rd;
	Reg Rn;
	Reg Rm;
	Reg Rs;
	Flag s;
	} Mult;

typedef struct _swap
	{
	Reg Rn;
	Reg Rd;
	Reg Rm;
	} Swap;

typedef struct _trans
	{
	Reg Rn;
	Reg Rd;
	Reg Rm;
	Trans_offset offset;
	uint16_t off12;
	Shift shift;
	uint8_t shimm5;
	Flag u;
	} Trans;

typedef struct _mtrans
	{
	Reg Rn;
	Mtrans_mode mode;
	uint16_t reg_list;
	Flag w;
	} Mtrans;

typedef struct _branch
	{
	int32_t offset;
	} Branch;

typedef struct _inst_dec
	{
	Inst_t type;
	Inst inst;
	Cond cond;
	union op_
		{
		Regop regop;
		Mult mult;
		Swap swap;
		Trans trans;
		Mtrans mtrans;
		Branch branch;
		}op;
	} InstDec;


InstDec *decod(uint32_t code);
void print_inst(FILE *f, InstDec *pins);

#endif
