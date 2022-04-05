// ### -------------------------------------------------------------- ###
// # file       : mipsr3000.h                                           #
// # date       : Oct 28 2003                                           #
// # version    : v2.0                                                  #
// #                                                                    #
// # origin     : this description has been developed by CAO-VLSI team  #
// #              at MASI laboratory, University Pierre et Marie Curie  #
// #              4 Place Jussieu 75252 Paris Cedex 05 - France         #
// #                                                                    #
// # descr.     : data flow description of a five stage pipelined Mips  #
// #              R3000 processor                                       #
// #                                                                    #
// # Original                                                           #
// # authors    : D. Hommais, P. Bazargan Sabet                         #
// #                                                                    #
// # System-C                                                           #
// # rewriting  : F. Pecheux                                            #
// ### -------------------------------------------------------------- ###

#ifndef _MIPSR3000
#define _MIPSR3000

#include <systemc.h>

// IFETCH Stage
#include "ifetch.h"
#include "mux_ifetch.h"
#include "ff_ifetch.h"

// DECODE Stage
#include "decode.h"
#include "decode_rom.h"
#include "mux_decode.h"
#include "ff_decode.h"

// EXECUTE Stage
#include "execute.h"
#include "mux_execute.h"
#include "ff_execute.h"

// MEMORY Stage
#include "memory.h"
#include "mux_memory.h"
#include "ff_memory.h"

// WRITEBACK Stage
#include "writeback.h"
#include "mux_writeback.h"
#include "ff_writeback.h"

// Registers and logic outside pipeline 
#include "misc.h"
#include "mux_misc.h"
#include "ff_misc.h"

// Other resources
#include "alu.h"
#include "regfile.h"
#include "forwardunit.h"
#include "pipeunit.h"

SC_MODULE(mipsr3000)
{
  sc_in<bool>			CK;		    // external clock
    sc_in<bool>			RESET_N;	// external reset
    sc_in<sc_uint<6>>		IT_N;	// external interrupts

    sc_out<sc_uint<32>>	I_A;	// instruction address
    sc_out<bool>		I_FTCH;		// instruction fetch
    sc_out<bool>		I_ACK;		// inst. fetch acknow.
    sc_in<bool>			I_BERR_N;	// inst. bus error
    sc_in<bool>			I_FRZ;		// inst. fetch pending
    sc_in<sc_uint<32>>		I;		// instruction bus

    sc_out<sc_uint<32>>	D_A;	// data address
    sc_out<bool>		D_RQ;		// data access request
    sc_out<bool>		D_LOCK;		// lock memory access
    sc_out<sc_uint<2>>		D_ATYPE;// data access type
    sc_out<bool>		D_ACK;		// data fetch acknow.
    sc_in<bool>			D_BERR_N;	// data bus error
    sc_in<bool>			D_FRZ;		// data access pending
    sc_in<sc_uint<32>>		D_IN;	// data bus (input )
    sc_out<sc_uint<32>>	D_OUT;	// data bus (output)

	// list of signals

	sc_signal<bool> CK_SX;  				// internal clock
	sc_signal<sc_uint<25>> I_TYPE_SD;  	// instruction type
	sc_signal<sc_uint<25>> MUX_I_TYPE_SD;  // instruction type
	sc_signal<sc_uint<25>> I_TYPE_RD;  	// instruction type
	sc_signal<sc_uint<25>> MUX_I_TYPE_SE;  // instruction type
	sc_signal<sc_uint<25>> I_TYPE_RE;  	// instruction type
	sc_signal<sc_uint<25>> MUX_I_TYPE_SM;  // instruction type
	sc_signal<sc_uint<25>> I_TYPE_RM;  	// instruction type
	sc_signal<bool> I_RFMT_SD;  			// r format
	sc_signal<bool> I_IFMT_SD; 				// i format
	sc_signal<bool> I_JFMT_SD;  			// j format
	sc_signal<bool> I_ILLG_SD;  			// illegal instruction
	sc_signal<bool> I_IFMT_SE;  			// i format
	sc_signal<bool> I_READS_SD;  			// ins. uses s oper
	sc_signal<bool> I_READT_SD;  			// ins. uses t oper
	sc_signal<bool> I_READS_SE;  			// ins. uses s oper
	sc_signal<bool> I_READT_SE;  			// ins. uses t oper
	sc_signal<bool> I_DUSE_SD;  			// dec uses operands
	sc_signal<bool> I_EUSE_SE;  			// exe uses operands
	sc_signal<bool> I_EPDC_SE;  			// exe produces res
	sc_signal<bool> I_MPDC_SE;  			// mem produces res
	sc_signal<bool> I_MPDC_SM;  			// mem produces res
	sc_signal<bool> I_OSGND_SD; 	 		// signed operands
	sc_signal<bool> I_RSGND_SE;  			// signed result
	sc_signal<sc_uint<3>> I_OPER_SE;  		// alu operation
	sc_signal<sc_uint<2>> I_LOGIC_SE;  	// logic operation
	sc_signal<bool> I_SUB_SE;  				// subtract
	sc_signal<bool> I_RIGHT_SE;  			// shift right
	sc_signal<bool> I_OVRF_SE;  			// detect overflow
	sc_signal<bool> I_LOAD_SM;  			// read  from memory
	sc_signal<bool> I_STOR_SM;  			// write into memory
	sc_signal<bool> I_BYTE_SM;  			// access type (byte)
	sc_signal<bool> I_HALF_SM;  			// access type (h-word)
	sc_signal<bool> I_WORD_SM;  			// access type (word)
	sc_signal<bool> I_BRNCH_SD;  			// branch instruction
	sc_signal<bool> I_BRNCH_SE;  			// branch instruction
	sc_signal<bool> I_WRT31_SD;  			// write into r31
	sc_signal<bool> I_WRITE_SD;  			// write into reg.
	sc_signal<bool> I_WRITE_SE;  			// write into reg.
	sc_signal<bool> I_WRITE_SM;  			// write into reg.
	sc_signal<bool> I_WRITE_SW;  			// write into reg.
	sc_signal<sc_uint<8>> COP0_SD;  		// cop0 extension
	sc_signal<sc_uint<8>> OPCOD_SD;  		// operation code
	sc_signal<sc_uint<8>> MUX_OPCOD_SD;  	// operation code
	sc_signal<sc_uint<8>> OPCOD_RD;  		// operation code
	sc_signal<sc_uint<8>> MUX_OPCOD_SE;  	// operation code
	sc_signal<sc_uint<8>> OPCOD_RE;  		// operation code
	sc_signal<sc_uint<8>> MUX_OPCOD_SM;  	// operation code
	sc_signal<sc_uint<8>> OPCOD_RM;  		// operation code

	//the use of swap signal are deprecated so need to be removed
	sc_signal<bool> SWAP_SD;  				// swap instruction
	sc_signal<bool> MUX_SWAP_SD;  			// swap instruction
	sc_signal<bool> SWAP_RD;  				// swap instruction
	sc_signal<bool> MUX_SWAP_SE;  			// swap instruction
	sc_signal<bool> SWAP_RE;  				// swap instruction

	//when instruction need more than 1 cycle for execute
	//the result of the exec stage is gonna get in again
	//on the next cycle
	//ex.: division state machine
	sc_signal<bool> COPYCAP_SE;  			// copying capability
	sc_signal<bool> MUX_COPYCAP_SE;  		// copying capability
	sc_signal<bool> COPYCAP_RE;  			// copying capability

	sc_signal<bool> FSTSWAP_SM;  			// first swap access

	// source register number
	// ex.: add r5, r4, r3 => source register number are 4 and 3 (r4 and r3)
	sc_signal<sc_uint<5>> RS_SD;  			// source reg nbr
	sc_signal<sc_uint<5>> RT_SD;  			// source reg nbr
	sc_signal<sc_uint<5>> MUX_RT_SD;  		// source reg nbr reg.
	sc_signal<sc_uint<5>> RT_RD;  			// source reg nbr reg.
	sc_signal<sc_uint<5>> MUX_RS_SD;  		// source reg nbr reg.
	sc_signal<sc_uint<5>> RS_RD;  			// source reg nbr reg.

	sc_signal<sc_uint<32>> MUX_IR_SI;  	// instruction register
	sc_signal<sc_uint<32>> IR_RI;  		// instruction register
	sc_signal<sc_uint<32>> MUX_IR_SD;  	// instruction register
	sc_signal<sc_uint<32>> IR_RD;  		// instruction register
	sc_signal<sc_uint<32>> MUX_IR_SE;  	// instruction register
	sc_signal<sc_uint<32>> IR_RE;  		// instruction register
	sc_signal<sc_uint<32>> MUX_IR_SM;  	// instruction register
	sc_signal<sc_uint<32>> IR_RM;  		// instruction register
	sc_signal<bool> INSTRED_RI;  			// new inst. fetched
	sc_signal<bool> MUX_INSTRED_SI;  		// new inst. fetched
	sc_signal<sc_uint<32>> NEXTPC_SD;  	// next instr. address
	sc_signal<sc_uint<32>> NEXTPC_XX;  	// next instr. adr (hw)
	sc_signal<sc_uint<32>> MUX_NEXTPC_SD;  // next instr. address
	sc_signal<sc_uint<32>> NEXTPC_RD;  	// next instr. address
	sc_signal<sc_uint<32>> MUX_NEXTPC_SE;  // next instr. address
	sc_signal<sc_uint<32>> NEXTPC_RE;  	// next instr. address
	sc_signal<sc_uint<32>> MUX_PC_SI;  	// instruction address
	sc_signal<sc_uint<32>> PC_RI;  		// instruction address
	sc_signal<sc_uint<32>> MUX_PC_SD;  	// instruction address
	sc_signal<sc_uint<32>> PC_RD;  		// instruction address
	sc_signal<sc_uint<32>> MUX_PC_SE;  	// instruction address
	sc_signal<sc_uint<32>> PC_RE;  		// instruction address

	//redopc is the addresse to return when syscall
	sc_signal<sc_uint<32>> MUX_REDOPC_SE;  // old inst. address
	sc_signal<sc_uint<32>> REDOPC_RE;  	// old inst. address

	sc_signal<bool> EXCRQ_XM;  				// exception request 
	sc_signal<sc_uint<2>> COPERR_XM;  		// coprocessor's #
	sc_signal<sc_uint<4>> EXCCODE_XM;  	// exception code
	sc_signal<sc_uint<32>> CAUSE_XM;  		// exception cause (exp)
	sc_signal<sc_uint<32>> CAUSE_XX;  		// exception cause (int)
	sc_signal<sc_uint<32>> CAUSE_SM;  		// exception cause (sw)
	sc_signal<sc_uint<32>> CAUSE_SX;  		// exception cause (hw)
	sc_signal<sc_uint<32>> CAUSE_RX;  		// cause register
	sc_signal<bool> WCAUSE_SM;  			// excp cause wen (sw)

	// (mips1) RFE => restore user status(used for restore status before return to the main processus)
	// (mips32) this instruction is called Eret
	sc_signal<sc_uint<32>> RSTORSR_SM;  	// next instruction status (rfe instruction)

	sc_signal<sc_uint<32>> NEXTSR_SM;  	// next instruction status (software)
	sc_signal<sc_uint<32>> NEXTSR_XX;  	// next instruction status (hardware it)
	sc_signal<sc_uint<32>> NEXTSR_RX;  	// next instruction status register

	//coprocessor status register (data like mode user or system)
	sc_signal<sc_uint<32>> SR_SI;  		// status register
	sc_signal<sc_uint<32>> MUX_SR_SI; 		// status register
	sc_signal<sc_uint<32>> SR_RI; 			// status register
	sc_signal<sc_uint<32>> MUX_SR_SD;  	// status register
	sc_signal<sc_uint<32>> SR_RD;  		// status register
	sc_signal<sc_uint<32>> MUX_SR_SE;  	// status register
	sc_signal<sc_uint<32>> SR_RE;  		// status register

	sc_signal<bool> WSR_SM;  				// nextsr's write en.
	sc_signal<bool> WSR_XX;  				// nextsr's write en.
	sc_signal<sc_uint<32>> S_SD;  			// s from reg. bank
	sc_signal<sc_uint<32>> SOPER_SD;  		// effective s oper.
	sc_signal<sc_uint<32>> MUX_SOPER_SD;  	// eff. s oper. reg.
	sc_signal<sc_uint<32>> SOPER_RD;  		// eff. s oper. reg.
	sc_signal<sc_uint<32>> X_SE;  			// x operand
	sc_signal<sc_uint<32>> XARITH_SE;  	// x oper. for arithm.
	sc_signal<sc_uint<32>> T_SD;  			// t from reg. bank
	sc_signal<sc_uint<32>> TOPER_SD;  		// effective t oper.
	sc_signal<sc_uint<32>> MUX_TOPER_SD;  	// eff. t oper. reg.
	sc_signal<sc_uint<32>> TOPER_RD;  		// eff. t oper. reg.
	sc_signal<sc_uint<32>> Y_SE;  			// y operand
	sc_signal<sc_uint<32>> YARITH_SE;  	// y oper. for arithm.
	sc_signal<sc_uint<32>> COP0OP_SD;  	// cop 0 source opr.
	sc_signal<sc_uint<5>> COP0D_SD;  		// cop 0 dest. reg. nbr
	sc_signal<sc_uint<5>> MUX_COP0D_SD;  	// cop 0 dest. reg. nbr
	sc_signal<sc_uint<5>> COP0D_RD;  		// cop 0 dest. reg. nbr
	sc_signal<sc_uint<5>> MUX_COP0D_SE;  	// cop 0 dest. reg. nbr
	sc_signal<sc_uint<5>> COP0D_RE;  		// cop 0 dest. reg. nbr

	sc_signal<sc_uint<32>> IOPER_SD;  		// effective immediate operand
	sc_signal<sc_uint<32>> MUX_IOPER_SD;  	// effective immediate operand
	sc_signal<sc_uint<32>> IOPER_RD;  		// effective immediate operand
	
	sc_signal<bool> SLEEP_SD;  				// sleep inst. stall
	sc_signal<bool> HAZARDS_SD;  			// hazards (signal bypass()si ou no bypass)
	sc_signal<bool> HAZARDS_SE;  			// hazards (signal bypass)

	// IFETCH instruction controller signals
	sc_signal<bool> KILL_SI;  				// kill the instruction
	sc_signal<bool> STALL_SI; 	 			// freeze the inst.
	sc_signal<bool> COPY_SI;  				// duplicate the inst.
	sc_signal<bool> EXEC_SI;  				// execute the inst.
	sc_signal<bool> BUBBLE_SI;  			// introduce bubble
	sc_signal<bool> HOLD_SI;  				// hold the instruction
	sc_signal<bool> SHIFT_SI;  				// shift new inst.
	sc_signal<bool> KEEP_SI;  				// keep the data
	sc_signal<bool> LOAD_SI;  				// load a new data

	// DECOD instruction controller signals
	sc_signal<bool> KILL_SD;  				// kill the instruction
	sc_signal<bool> STALL_SD;  				// freeze the inst.
	sc_signal<bool> COPY_SD;  				// duplicate the inst.
	sc_signal<bool> EXEC_SD;  				// execute the inst.
	sc_signal<bool> BUBBLE_SD;  			// introduce bubble
	sc_signal<bool> HOLD_SD;  				// hold the instruction
	sc_signal<bool> SHIFT_SD;  				// shift new inst.
	sc_signal<bool> KEEP_SD;  				// keep the data
	sc_signal<bool> LOAD_SD;  				// load a new data

	// EXEC instruction controller signals
	sc_signal<bool> KILL_SE;  				// kill the instruction
	sc_signal<bool> STALL_SE;  				// freeze the inst.
	sc_signal<bool> COPY_SE;  				// duplicate the inst.
	sc_signal<bool> EXEC_SE;  				// execute the inst.
	sc_signal<bool> BUBBLE_SE;  			// introduce bubble
	sc_signal<bool> HOLD_SE;  				// hold the instruction
	sc_signal<bool> SHIFT_SE;  				// shift new inst.
	sc_signal<bool> KEEP_SE;  				// keep the data
	sc_signal<bool> LOAD_SE;  				// load a new data

	// MEMORY instruction controller signals
	sc_signal<bool> KILL_SM;  				// kill the instruction
	sc_signal<bool> STALL_SM; 	 			// freeze the inst.
	sc_signal<bool> COPY_SM;  				// duplicate the inst.
	sc_signal<bool> EXEC_SM;  				// execute the inst.
	sc_signal<bool> BUBBLE_SM; 	 			// introduce bubble
	sc_signal<bool> HOLD_SM;  				// hold the instruction
	sc_signal<bool> SHIFT_SM;  				// shift new inst.
	sc_signal<bool> KEEP_SM;  				// keep the data
	sc_signal<bool> LOAD_SM;  				// load a new data

	// WRITE BACK instruction controller signals
	sc_signal<bool> KILL_SW;  				// kill the instruction
	sc_signal<bool> STALL_SW;  				// freeze the inst.
	sc_signal<bool> COPY_SW;  				// duplicate the inst.
	sc_signal<bool> EXEC_SW;  				// execute the inst.
	sc_signal<bool> BUBBLE_SW;  			// introduce bubble 
	sc_signal<bool> HOLD_SW;  				// hold the instruction 
	sc_signal<bool> SHIFT_SW;  				// shift new inst 
	sc_signal<bool> LOAD_SW;  				// load a new data 
	sc_signal<bool> KEEP_SW;  				// keep the data 

	sc_signal<sc_uint<32>> WREG_SW;  		// integer reg wen

	// low and high register are used for hardware or software 
	// operations like multiplication or division with 
	sc_signal<bool> WLO_SW;  				// low reg's write en
	sc_signal<sc_uint<32>> MUX_LO_SW;  	// low register
	sc_signal<sc_uint<32>> LO_RW;  		// low register
	sc_signal<bool> WHI_SW;  				// high reg's write en
	sc_signal<sc_uint<32>> MUX_HI_SW;  	// high register
	sc_signal<sc_uint<32>> HI_RW;  		// high register

	// destination register number
	// ex.: add r5, r4, r3 => destination register number is 5 (r5)
	sc_signal<sc_uint<5>> RD_SD;  			// destination reg #
	sc_signal<sc_uint<5>> MUX_RD_SD;  		// destination reg #
	sc_signal<sc_uint<5>> RD_RD;  			// destination reg #
	sc_signal<sc_uint<5>> MUX_RD_SE;  		// destination reg #
	sc_signal<sc_uint<5>> RD_RE;  			// destination reg #
	sc_signal<sc_uint<5>> RD_SM;  			// destination reg #
	sc_signal<sc_uint<5>> MUX_RD_SM;  		// destination reg #
	sc_signal<sc_uint<5>> RD_RM;  			// destination reg #

	sc_signal<sc_uint<32>> XOPER_SE;  		// effective x operand
	sc_signal<sc_uint<32>> YOPER_SE;  		// effective y operand

	sc_signal<sc_uint<32>> RES_SE;  		// result out of alu 
	sc_signal<sc_uint<32>> MUX_RES_SE;  	// result out of alu 
	sc_signal<sc_uint<32>> RES_RE;  		// result out of alu 
	sc_signal<bool> OVERFLW_SE;  			// overflow out of alu

	sc_signal<sc_uint<32>> MUX_WDATA_SE;  	// data bus output reg.
	sc_signal<sc_uint<32>> WDATA_RE;  		// data bus output reg.
	sc_signal<bool> DACCESS_SM;  			// data memory access
	sc_signal<bool> DATARED_SM;  			// read access
	sc_signal<bool> MUX_DATARED_SM;  		// read access
	sc_signal<bool> DATARED_RM;  			// read access
	sc_signal<bool> WRITE_SM;  				// write into storage
	sc_signal<bool> DLOCK_SM;  				// lock data access
	sc_signal<sc_uint<4>> BYTSEL_SM;  		// byte select for rw
	sc_signal<sc_uint<24>> BSEXT_SM;  		// data sign ext byte
	sc_signal<sc_uint<16>> HSEXT_SM;  		// data sign ext half w
	sc_signal<sc_uint<32>> REDDAT_SM;  	// aligned data
	sc_signal<sc_uint<32>> DATA_SM;  		// data bus / res
	sc_signal<sc_uint<32>> MUX_DATA_SM;  	// data bus input reg.
	sc_signal<sc_uint<32>> DATA_RM;  		// data bus input reg.
	sc_signal<sc_uint<32>> MUX_BADVADR_SM; // bad virtual adr reg
	sc_signal<sc_uint<32>> BADVADR_RM;  	// bad virtual adr reg
	sc_signal<bool> BADIA_XM;  				// bad inst adr
	sc_signal<bool> BADDA_XM;  				// bad data adr
	
	sc_signal<bool> GLBMSK_XX;  			// global    it mask
	sc_signal<sc_uint<8>> ITMASK_XX;  		// individual it mask

	sc_signal<sc_uint<2>> SWINT_XM;  		// sw interrupt (mtc0)
	sc_signal<sc_uint<2>> SWINTRQ_XM;  	// sw interrupt request
	sc_signal<sc_uint<6>> IT_XX;  			// external interrupts
	sc_signal<sc_uint<6>> HWINTRQ_XX;  	// hw interrupt request
	sc_signal<bool> INTRQ_XX;  				// interrupt request

	sc_signal<bool> BDSLOT_XI;  			// branch delayed slot
	sc_signal<bool> MUX_BDSLOT_SI;  		// branch delayed slot
	sc_signal<bool> BDSLOT_RI;  			// branch delayed slot
	sc_signal<bool> MUX_BDSLOT_SD; 			// branch delayed slot
	sc_signal<bool> BDSLOT_RD; 			 	// branch delayed slot
	sc_signal<bool> MUX_BDSLOT_SE;  		// branch delayed slot
	sc_signal<bool> BDSLOT_RE;  			// branch delayed slot
	sc_signal<bool> WREDOPC_SE;  			// redopc write enable
	sc_signal<bool> ILLGINS_XD;  			// unknown instruction
	sc_signal<bool> MUX_ILLGINS_SD;  		// unknown instruction
	sc_signal<bool> ILLGINS_RD;  			// unknown instruction
	sc_signal<bool> MUX_ILLGINS_SE;  		// unknown instruction
	sc_signal<bool> ILLGINS_RE;  			// unknown instruction
	sc_signal<bool> C0UNUSE_XD;  			// copro. 0 unusable
	sc_signal<bool> MUX_C0UNUSE_SD;  		// copro. 0 unusable
	sc_signal<bool> C0UNUSE_RD;  			// copro. 0 unusable
	sc_signal<bool> MUX_C0UNUSE_SE;  		// copro. 0 unusable
	sc_signal<bool> C0UNUSE_RE;  			// copro. 0 unusable
	sc_signal<bool> IAMALGN_XE;  			// inst adr miss algn
	sc_signal<bool> MUX_IAMALGN_SE;  		// inst adr miss algn
	sc_signal<bool> IAMALGN_RE;  			// inst adr miss algn
	sc_signal<bool> IASVIOL_XE;  			// inst adr segm viol
	sc_signal<bool> MUX_IASVIOL_SE;  		// inst adr segm viol
	sc_signal<bool> IASVIOL_RE;  			// inst adr segm viol
	sc_signal<bool> IABUSER_XE;  			// inst adr bus error
	sc_signal<bool> MUX_IABUSER_SE;  		// inst adr bus error
	sc_signal<bool> IABUSER_RE;  			// inst adr bus error
	sc_signal<bool> OVF_XE;  				// arithmetic overflow
	sc_signal<bool> MUX_OVF_SE;  			// arithmetic overflow
	sc_signal<bool> OVF_RE;  				// arithmetic overflow
	sc_signal<bool> SYSCALL_XE;  			// sw exc. (syscall)
	sc_signal<bool> MUX_SYSCALL_SE;  		// sw exc. (syscall)
	sc_signal<bool> SYSCALL_RE;  			// sw exc. (syscall)

	//break is syscall like but is used for debug
	sc_signal<bool> BREAK_XE;  				// sw exc. (break  )
	sc_signal<bool> MUX_BREAK_SE;  			// sw exc. (break  )
	sc_signal<bool> BREAK_RE;  				// sw exc. (break  )
	
	sc_signal<bool> LAMALGN_XM;  			// load  adr miss algn
	sc_signal<bool> LASVIOL_XM;  			// load  adr segm viol
	sc_signal<bool> SAMALGN_XM;  			// store adr miss algn
	sc_signal<bool> SASVIOL_XM;  			// store adr segm viol
	sc_signal<bool> DABUSER_XM;  			// data adr bus error
	sc_signal<bool> EARLYEX_XM;  			// early exceptions
	sc_signal<bool> LATEEX_XM;  			// late  exceptions

	sc_signal<sc_uint<32>> MUX_CAUSE_SX;  	//
	sc_signal<sc_uint<32>> MUX_EPC_SX;  	//
	sc_signal<sc_uint<32>> MUX_NEXTSR_SX;  //
	sc_signal<bool> MUX_RESET_SX;  			//
	sc_signal<bool> BOOTEV_XX;  			// bootstrap exc vect
	sc_signal<sc_uint<32>> EPC_XX;  		// exc pg counter
	sc_signal<sc_uint<32>> EPC_XM;  		// exc pg counter
	sc_signal<bool> WEPC_XX;  				// exc pg cntr write en
	sc_signal<bool> WEPC_XM;  				// exc pg cntr write en
	sc_signal<sc_uint<32>> EPC_RX;  		// exc pg counter reg
	sc_signal<bool> RESET_RX;  				// synchronize

    
	ifetch *ifetch1;
	mux_ifetch *mux_ifetch1;
	ff_ifetch *ff_ifetch1;
    
	decode *decode1;
	decode_rom *decode_rom1;
	mux_decode *mux_decode1;
	ff_decode *ff_decode1;
    
	execute *execute1;
	mux_execute *mux_execute1;
	ff_execute *ff_execute1;
    
	memory *memory1;
	mux_memory *mux_memory1;
	ff_memory *ff_memory1;
    
	writeback *writeback1;
	mux_writeback *mux_writeback1;
	ff_writeback *ff_writeback1;
    
	misc *misc1;
	mux_misc *mux_misc1;
	ff_misc *ff_misc1;
    
	alu *alu1;
	regfile *regfile1;
	forwardunit *forwardunit1;
	pipeunit *pipeunit1;

	SC_CTOR(mipsr3000):
		CK("CK"),
		RESET_N("RESET_N"),
		IT_N("IT_N"),

		I_A("I_A"),
		I_FTCH("I_FTCH"),
		I_ACK("I_ACK"),
		I_BERR_N("I_BERR_N"),
		I_FRZ("I_FRZ"),
		I("I"),

		D_A("D_A"),
		D_RQ("D_RQ"),
		D_LOCK("D_LOCK"),
		D_ATYPE("D_ATYPE"),
		D_ACK("D_ACK"),
		D_BERR_N("D_BERR_N"),
		D_FRZ("D_FRZ"),
		D_IN("D_IN"),
		D_OUT("D_OUT")
	{
		ifetch1=new ifetch("ifetch1");

		ifetch1->I_BRNCH_SD(I_BRNCH_SD);
		ifetch1->NEXTSR_RX(NEXTSR_RX);
		ifetch1->OPCOD_SD(OPCOD_SD);
		ifetch1->OPCOD_RD(OPCOD_RD);
		ifetch1->OPCOD_RE(OPCOD_RE);
		ifetch1->SR_SI(SR_SI);
		ifetch1->BDSLOT_XI(BDSLOT_XI);

		mux_ifetch1=new mux_ifetch("mux_ifetch1");

		mux_ifetch1->BUBBLE_SI(BUBBLE_SI);
		mux_ifetch1->HOLD_SI(HOLD_SI);
		mux_ifetch1->KEEP_SI(KEEP_SI);
		mux_ifetch1->IR_RI(IR_RI);
		mux_ifetch1->I(I);
		mux_ifetch1->MUX_IR_SI(MUX_IR_SI);
		mux_ifetch1->MUX_INSTRED_SI(MUX_INSTRED_SI);
		mux_ifetch1->BDSLOT_XI(BDSLOT_XI);
		mux_ifetch1->BDSLOT_RI(BDSLOT_RI);
		mux_ifetch1->MUX_BDSLOT_SI(MUX_BDSLOT_SI);
		mux_ifetch1->PC_RI(PC_RI);
		mux_ifetch1->NEXTPC_RD(NEXTPC_RD);
		mux_ifetch1->MUX_PC_SI(MUX_PC_SI);
		mux_ifetch1->SR_RI(SR_RI);
		mux_ifetch1->SR_SI(SR_SI);
		mux_ifetch1->MUX_SR_SI(MUX_SR_SI);

		ff_ifetch1=new ff_ifetch("ff_ifetch1");

		ff_ifetch1->CK_SX(CK_SX);
		ff_ifetch1->MUX_IR_SI(MUX_IR_SI);
		ff_ifetch1->IR_RI(IR_RI);
		ff_ifetch1->MUX_INSTRED_SI(MUX_INSTRED_SI);
		ff_ifetch1->INSTRED_RI(INSTRED_RI);
		ff_ifetch1->MUX_BDSLOT_SI(MUX_BDSLOT_SI);
		ff_ifetch1->BDSLOT_RI(BDSLOT_RI);
		ff_ifetch1->MUX_PC_SI(MUX_PC_SI);
		ff_ifetch1->PC_RI(PC_RI);
		ff_ifetch1->MUX_SR_SI(MUX_SR_SI);
		ff_ifetch1->SR_RI(SR_RI);

		decode1=new decode("decode1");

		decode1->IR_RI(IR_RI);
		decode1->OPCOD_SD(OPCOD_SD);
		decode1->I_TYPE_SD(I_TYPE_SD);
		decode1->I_DUSE_SD(I_DUSE_SD);
		decode1->I_READS_SD(I_READS_SD);
		decode1->I_READT_SD(I_READT_SD);
		decode1->I_BRNCH_SD(I_BRNCH_SD);
		decode1->SWAP_SD(SWAP_SD);
		decode1->COP0D_SD(COP0D_SD);
		decode1->RS_SD(RS_SD);
		decode1->RT_SD(RT_SD);
		decode1->RD_SD(RD_SD);
		decode1->IOPER_SD(IOPER_SD);
		decode1->SLEEP_SD(SLEEP_SD);
		decode1->NEXTPC_SD(NEXTPC_SD);
		decode1->ILLGINS_XD(ILLGINS_XD);
		decode1->C0UNUSE_XD(C0UNUSE_XD);
		decode1->BADVADR_RM(BADVADR_RM);
		decode1->NEXTSR_RX(NEXTSR_RX);
		decode1->EPC_RX(EPC_RX);
		decode1->CAUSE_RX(CAUSE_RX);
		decode1->LO_RW(LO_RW);
		decode1->HI_RW(HI_RW);
		decode1->SOPER_SD(SOPER_SD);
		decode1->TOPER_SD(TOPER_SD);
		decode1->SR_RI(SR_RI);
		decode1->NEXTPC_RD(NEXTPC_RD);

		decode_rom1=new decode_rom("decode_rom1");

		decode_rom1->OPCOD_SD(OPCOD_SD);
		decode_rom1->I_TYPE_SD(I_TYPE_SD);

		mux_decode1=new mux_decode("mux_decode1");

		mux_decode1->BUBBLE_SD(BUBBLE_SD);
		mux_decode1->HOLD_SD(HOLD_SD);
		mux_decode1->KEEP_SD(KEEP_SD);
		mux_decode1->INTRQ_XX(INTRQ_XX);
		mux_decode1->EXCRQ_XM(EXCRQ_XM);
		mux_decode1->RESET_RX(RESET_RX);
		mux_decode1->IR_RI(IR_RI);
		mux_decode1->IR_RD(IR_RD);
		mux_decode1->MUX_IR_SD(MUX_IR_SD);
		mux_decode1->I_TYPE_SD(I_TYPE_SD);
		mux_decode1->I_TYPE_RD(I_TYPE_RD);
		mux_decode1->MUX_I_TYPE_SD(MUX_I_TYPE_SD);
		mux_decode1->OPCOD_SD(OPCOD_SD);
		mux_decode1->OPCOD_RD(OPCOD_RD);
		mux_decode1->MUX_OPCOD_SD(MUX_OPCOD_SD);
		mux_decode1->BDSLOT_RI(BDSLOT_RI);
		mux_decode1->BDSLOT_RD(BDSLOT_RD);
		mux_decode1->MUX_BDSLOT_SD(MUX_BDSLOT_SD);
		mux_decode1->SWAP_SD(SWAP_SD);
		mux_decode1->SWAP_RD(SWAP_RD);
		mux_decode1->MUX_SWAP_SD(MUX_SWAP_SD);
		mux_decode1->RS_SD(RS_SD);
		mux_decode1->RS_RD(RS_RD);
		mux_decode1->MUX_RS_SD(MUX_RS_SD);
		mux_decode1->RT_SD(RT_SD);
		mux_decode1->RT_RD(RT_RD);
		mux_decode1->MUX_RT_SD(MUX_RT_SD);
		mux_decode1->RD_SD(RD_SD);
		mux_decode1->RD_RD(RD_RD);
		mux_decode1->MUX_RD_SD(MUX_RD_SD);
		mux_decode1->COP0D_SD(COP0D_SD);
		mux_decode1->COP0D_RD(COP0D_RD);
		mux_decode1->MUX_COP0D_SD(MUX_COP0D_SD);
		mux_decode1->ILLGINS_XD(ILLGINS_XD);
		mux_decode1->ILLGINS_RD(ILLGINS_RD);
		mux_decode1->MUX_ILLGINS_SD(MUX_ILLGINS_SD);
		mux_decode1->C0UNUSE_XD(C0UNUSE_XD);
		mux_decode1->C0UNUSE_RD(C0UNUSE_RD);
		mux_decode1->MUX_C0UNUSE_SD(MUX_C0UNUSE_SD);
		mux_decode1->NEXTPC_XX(NEXTPC_XX);
		mux_decode1->NEXTPC_SD(NEXTPC_SD);
		mux_decode1->NEXTPC_RD(NEXTPC_RD);
		mux_decode1->MUX_NEXTPC_SD(MUX_NEXTPC_SD);
		mux_decode1->PC_RI(PC_RI);
		mux_decode1->PC_RD(PC_RD);
		mux_decode1->MUX_PC_SD(MUX_PC_SD);
		mux_decode1->SR_RI(SR_RI);
		mux_decode1->SR_RD(SR_RD);
		mux_decode1->MUX_SR_SD(MUX_SR_SD);
		mux_decode1->X_SE(X_SE);
		mux_decode1->SOPER_SD(SOPER_SD);
		mux_decode1->MUX_SOPER_SD(MUX_SOPER_SD);
		mux_decode1->Y_SE(Y_SE);
		mux_decode1->TOPER_SD(TOPER_SD);
		mux_decode1->MUX_TOPER_SD(MUX_TOPER_SD);
		mux_decode1->IOPER_SD(IOPER_SD);
		mux_decode1->IOPER_RD(IOPER_RD);
		mux_decode1->MUX_IOPER_SD(MUX_IOPER_SD);

		ff_decode1=new ff_decode("ff_decode1");

		ff_decode1->CK_SX(CK_SX);
		ff_decode1->MUX_IR_SD(MUX_IR_SD);
		ff_decode1->IR_RD(IR_RD);
		ff_decode1->MUX_I_TYPE_SD(MUX_I_TYPE_SD);
		ff_decode1->I_TYPE_RD(I_TYPE_RD);
		ff_decode1->MUX_OPCOD_SD(MUX_OPCOD_SD);
		ff_decode1->OPCOD_RD(OPCOD_RD);
		ff_decode1->MUX_BDSLOT_SD(MUX_BDSLOT_SD);
		ff_decode1->BDSLOT_RD(BDSLOT_RD);
		ff_decode1->MUX_SWAP_SD(MUX_SWAP_SD);
		ff_decode1->SWAP_RD(SWAP_RD);
		ff_decode1->MUX_RS_SD(MUX_RS_SD);
		ff_decode1->RS_RD(RS_RD);
		ff_decode1->MUX_RT_SD(MUX_RT_SD);
		ff_decode1->RT_RD(RT_RD);
		ff_decode1->MUX_RD_SD(MUX_RD_SD);
		ff_decode1->RD_RD(RD_RD);
		ff_decode1->MUX_COP0D_SD(MUX_COP0D_SD);
		ff_decode1->COP0D_RD(COP0D_RD);
		ff_decode1->MUX_ILLGINS_SD(MUX_ILLGINS_SD);
		ff_decode1->ILLGINS_RD(ILLGINS_RD);
		ff_decode1->MUX_C0UNUSE_SD(MUX_C0UNUSE_SD);
		ff_decode1->C0UNUSE_RD(C0UNUSE_RD);
		ff_decode1->MUX_NEXTPC_SD(MUX_NEXTPC_SD);
		ff_decode1->NEXTPC_RD(NEXTPC_RD);
		ff_decode1->MUX_PC_SD(MUX_PC_SD);
		ff_decode1->PC_RD(PC_RD);
		ff_decode1->MUX_SR_SD(MUX_SR_SD);
		ff_decode1->SR_RD(SR_RD);
		ff_decode1->MUX_SOPER_SD(MUX_SOPER_SD);
		ff_decode1->SOPER_RD(SOPER_RD);
		ff_decode1->MUX_TOPER_SD(MUX_TOPER_SD);
		ff_decode1->TOPER_RD(TOPER_RD);
		ff_decode1->MUX_IOPER_SD(MUX_IOPER_SD);
		ff_decode1->IOPER_RD(IOPER_RD);

		execute1=new execute("execute1");

		execute1->SR_RI(SR_RI);
		execute1->I_TYPE_RD(I_TYPE_RD);
		execute1->OPCOD_RD(OPCOD_RD);
		execute1->IOPER_RD(IOPER_RD);
		execute1->NEXTSR_RX(NEXTSR_RX);
		execute1->NEXTPC_RD(NEXTPC_RD);
		execute1->SWAP_RD(SWAP_RD);
		execute1->I_BERR_N(I_BERR_N);
		execute1->I_IFMT_SE(I_IFMT_SE);
		execute1->I_EUSE_SE(I_EUSE_SE);
		execute1->I_READS_SE(I_READS_SE);
		execute1->I_READT_SE(I_READT_SE);
		execute1->I_OPER_SE(I_OPER_SE);
		execute1->I_LOGIC_SE(I_LOGIC_SE);
		execute1->I_SUB_SE(I_SUB_SE);
		execute1->I_RIGHT_SE(I_RIGHT_SE);
		execute1->I_RSGND_SE(I_RSGND_SE);
		execute1->I_OVRF_SE(I_OVRF_SE);
		execute1->I_MPDC_SE(I_MPDC_SE);
		execute1->I_EPDC_SE(I_EPDC_SE);
		execute1->I_WRITE_SE(I_WRITE_SE);
		execute1->X_SE(X_SE);
		execute1->Y_SE(Y_SE);
		execute1->OVERFLW_SE(OVERFLW_SE);
		execute1->XOPER_SE(XOPER_SE);
		execute1->YOPER_SE(YOPER_SE);
		execute1->WREDOPC_SE(WREDOPC_SE);
		execute1->IABUSER_XE(IABUSER_XE);
		execute1->BREAK_XE(BREAK_XE);
		execute1->SYSCALL_XE(SYSCALL_XE);
		execute1->OVF_XE(OVF_XE);
		execute1->IAMALGN_XE(IAMALGN_XE);
		execute1->IASVIOL_XE(IASVIOL_XE);
		execute1->COPYCAP_SE(COPYCAP_SE);

		mux_execute1=new mux_execute("mux_execute1");

		mux_execute1->BUBBLE_SE(BUBBLE_SE);
		mux_execute1->HOLD_SE(HOLD_SE);
		mux_execute1->KEEP_SE(KEEP_SE);
		mux_execute1->STALL_SM(STALL_SM);
		mux_execute1->COPY_SM(COPY_SM);
		mux_execute1->IR_RD(IR_RD);
		mux_execute1->IR_RE(IR_RE);
		mux_execute1->MUX_IR_SE(MUX_IR_SE);
		mux_execute1->I_TYPE_RD(I_TYPE_RD);
		mux_execute1->I_TYPE_RE(I_TYPE_RE);
		mux_execute1->MUX_I_TYPE_SE(MUX_I_TYPE_SE);
		mux_execute1->OPCOD_RD(OPCOD_RD);
		mux_execute1->OPCOD_RE(OPCOD_RE);
		mux_execute1->MUX_OPCOD_SE(MUX_OPCOD_SE);
		mux_execute1->BDSLOT_RD(BDSLOT_RD);
		mux_execute1->BDSLOT_RE(BDSLOT_RE);
		mux_execute1->MUX_BDSLOT_SE(MUX_BDSLOT_SE);
		mux_execute1->RD_RD(RD_RD);
		mux_execute1->RD_RE(RD_RE);
		mux_execute1->MUX_RD_SE(MUX_RD_SE);
		mux_execute1->COP0D_RD(COP0D_RD);
		mux_execute1->COP0D_RE(COP0D_RE);
		mux_execute1->MUX_COP0D_SE(MUX_COP0D_SE);
		mux_execute1->OVF_RE(OVF_RE);
		mux_execute1->OVF_XE(OVF_XE);
		mux_execute1->MUX_OVF_SE(MUX_OVF_SE);
		mux_execute1->IAMALGN_RE(IAMALGN_RE);
		mux_execute1->IAMALGN_XE(IAMALGN_XE);
		mux_execute1->MUX_IAMALGN_SE(MUX_IAMALGN_SE);
		mux_execute1->IASVIOL_RE(IASVIOL_RE);
		mux_execute1->IASVIOL_XE(IASVIOL_XE);
		mux_execute1->MUX_IASVIOL_SE(MUX_IASVIOL_SE);
		mux_execute1->IABUSER_RE(IABUSER_RE);
		mux_execute1->IABUSER_XE(IABUSER_XE);
		mux_execute1->MUX_IABUSER_SE(MUX_IABUSER_SE);
		mux_execute1->BREAK_RE(BREAK_RE);
		mux_execute1->BREAK_XE(BREAK_XE);
		mux_execute1->MUX_BREAK_SE(MUX_BREAK_SE);
		mux_execute1->SYSCALL_RE(SYSCALL_RE);
		mux_execute1->SYSCALL_XE(SYSCALL_XE);
		mux_execute1->MUX_SYSCALL_SE(MUX_SYSCALL_SE);
		mux_execute1->ILLGINS_RD(ILLGINS_RD);
		mux_execute1->ILLGINS_RE(ILLGINS_RE);
		mux_execute1->MUX_ILLGINS_SE(MUX_ILLGINS_SE);
		mux_execute1->C0UNUSE_RE(C0UNUSE_RE);
		mux_execute1->C0UNUSE_RD(C0UNUSE_RD);
		mux_execute1->MUX_C0UNUSE_SE(MUX_C0UNUSE_SE);
		mux_execute1->SWAP_RE(SWAP_RE);
		mux_execute1->SWAP_RD(SWAP_RD);
		mux_execute1->MUX_SWAP_SE(MUX_SWAP_SE);
		mux_execute1->PC_RD(PC_RD);
		mux_execute1->PC_RE(PC_RE);
		mux_execute1->MUX_PC_SE(MUX_PC_SE);
		mux_execute1->SR_RD(SR_RD);
		mux_execute1->SR_RE(SR_RE);
		mux_execute1->MUX_SR_SE(MUX_SR_SE);
		mux_execute1->NEXTPC_RD(NEXTPC_RD);
		mux_execute1->NEXTPC_RE(NEXTPC_RE);
		mux_execute1->MUX_NEXTPC_SE(MUX_NEXTPC_SE);
		mux_execute1->RES_SE(RES_SE);
		mux_execute1->RES_RE(RES_RE);
		mux_execute1->MUX_RES_SE(MUX_RES_SE);
		mux_execute1->Y_SE(Y_SE);
		mux_execute1->WDATA_RE(WDATA_RE);
		mux_execute1->MUX_WDATA_SE(MUX_WDATA_SE);
		mux_execute1->COPYCAP_SE(COPYCAP_SE);
		mux_execute1->COPYCAP_RE(COPYCAP_RE);
		mux_execute1->MUX_COPYCAP_SE(MUX_COPYCAP_SE);
		mux_execute1->REDOPC_RE(REDOPC_RE);
		mux_execute1->WREDOPC_SE(WREDOPC_SE);
		mux_execute1->MUX_REDOPC_SE(MUX_REDOPC_SE);

		ff_execute1=new ff_execute("ff_execute1");

		ff_execute1->CK_SX(CK_SX);
		ff_execute1->MUX_IR_SE(MUX_IR_SE);
		ff_execute1->IR_RE(IR_RE);
		ff_execute1->MUX_I_TYPE_SE(MUX_I_TYPE_SE);
		ff_execute1->I_TYPE_RE(I_TYPE_RE);
		ff_execute1->MUX_OPCOD_SE(MUX_OPCOD_SE);
		ff_execute1->OPCOD_RE(OPCOD_RE);
		ff_execute1->MUX_BDSLOT_SE(MUX_BDSLOT_SE);
		ff_execute1->BDSLOT_RE(BDSLOT_RE);
		ff_execute1->MUX_RD_SE(MUX_RD_SE);
		ff_execute1->RD_RE(RD_RE);
		ff_execute1->MUX_COP0D_SE(MUX_COP0D_SE);
		ff_execute1->COP0D_RE(COP0D_RE);
		ff_execute1->MUX_OVF_SE(MUX_OVF_SE);
		ff_execute1->OVF_RE(OVF_RE);
		ff_execute1->MUX_IAMALGN_SE(MUX_IAMALGN_SE);
		ff_execute1->IAMALGN_RE(IAMALGN_RE);
		ff_execute1->MUX_IASVIOL_SE(MUX_IASVIOL_SE);
		ff_execute1->IASVIOL_RE(IASVIOL_RE);
		ff_execute1->MUX_IABUSER_SE(MUX_IABUSER_SE);
		ff_execute1->IABUSER_RE(IABUSER_RE);
		ff_execute1->MUX_BREAK_SE(MUX_BREAK_SE);
		ff_execute1->BREAK_RE(BREAK_RE);
		ff_execute1->MUX_SYSCALL_SE(MUX_SYSCALL_SE);
		ff_execute1->SYSCALL_RE(SYSCALL_RE);
		ff_execute1->MUX_ILLGINS_SE(MUX_ILLGINS_SE);
		ff_execute1->ILLGINS_RE(ILLGINS_RE);
		ff_execute1->MUX_C0UNUSE_SE(MUX_C0UNUSE_SE);
		ff_execute1->C0UNUSE_RE(C0UNUSE_RE);
		ff_execute1->MUX_SWAP_SE(MUX_SWAP_SE);
		ff_execute1->SWAP_RE(SWAP_RE);
		ff_execute1->MUX_PC_SE(MUX_PC_SE);
		ff_execute1->PC_RE(PC_RE);
		ff_execute1->MUX_SR_SE(MUX_SR_SE);
		ff_execute1->SR_RE(SR_RE);
		ff_execute1->MUX_NEXTPC_SE(MUX_NEXTPC_SE);
		ff_execute1->NEXTPC_RE(NEXTPC_RE);
		ff_execute1->MUX_RES_SE(MUX_RES_SE);
		ff_execute1->RES_RE(RES_RE);
		ff_execute1->MUX_WDATA_SE(MUX_WDATA_SE);
		ff_execute1->WDATA_RE(WDATA_RE);
		ff_execute1->MUX_COPYCAP_SE(MUX_COPYCAP_SE);
		ff_execute1->COPYCAP_RE(COPYCAP_RE);
		ff_execute1->MUX_REDOPC_SE(MUX_REDOPC_SE);
		ff_execute1->REDOPC_RE(REDOPC_RE);

		memory1=new memory("memory1");

		memory1->RES_RE(RES_RE);
		memory1->D_IN(D_IN);
		memory1->OPCOD_RE(OPCOD_RE);
		memory1->D_BERR_N(D_BERR_N);
		memory1->SR_RE(SR_RE);
		memory1->IASVIOL_RE(IASVIOL_RE);
		memory1->IAMALGN_RE(IAMALGN_RE);
		memory1->INTRQ_XX(INTRQ_XX);
		memory1->IABUSER_RE(IABUSER_RE);
		memory1->SYSCALL_RE(SYSCALL_RE);
		memory1->BREAK_RE(BREAK_RE);
		memory1->ILLGINS_RE(ILLGINS_RE);
		memory1->C0UNUSE_RE(C0UNUSE_RE);
		memory1->BDSLOT_RE(BDSLOT_RE);
		memory1->IT_XX(IT_XX);
		memory1->CAUSE_RX(CAUSE_RX);
		memory1->COP0D_RE(COP0D_RE);
		memory1->KEEP_SM(KEEP_SM);
		memory1->PC_RE(PC_RE);
		memory1->REDOPC_RE(REDOPC_RE);
		memory1->OVF_RE(OVF_RE);
		memory1->I_TYPE_RE(I_TYPE_RE);
		memory1->SWAP_RE(SWAP_RE);
		memory1->COPYCAP_RE(COPYCAP_RE);
		memory1->RD_RE(RD_RE);
		memory1->I_WRITE_SM(I_WRITE_SM);
		memory1->WRITE_SM(WRITE_SM);
		memory1->DACCESS_SM(DACCESS_SM);
		memory1->DLOCK_SM(DLOCK_SM);
		memory1->DATARED_SM(DATARED_SM);
		memory1->RD_SM(RD_SM);
		memory1->BYTSEL_SM(BYTSEL_SM);
		memory1->REDDAT_SM(REDDAT_SM);
		memory1->DATA_SM(DATA_SM);
		memory1->COPERR_XM(COPERR_XM);
		memory1->EXCCODE_XM(EXCCODE_XM);
		memory1->CAUSE_XM(CAUSE_XM);
		memory1->SWINT_XM(SWINT_XM);
		memory1->WCAUSE_SM(WCAUSE_SM);
		memory1->CAUSE_SM(CAUSE_SM);
		memory1->NEXTSR_SM(NEXTSR_SM);
		memory1->WSR_SM(WSR_SM);
		memory1->EPC_XM(EPC_XM);
		memory1->LATEEX_XM(LATEEX_XM);
		memory1->EARLYEX_XM(EARLYEX_XM);
		memory1->EXCRQ_XM(EXCRQ_XM);
		memory1->SWINTRQ_XM(SWINTRQ_XM);
		memory1->WEPC_XM(WEPC_XM);
		memory1->I_WORD_SM(I_WORD_SM);
		memory1->I_HALF_SM(I_HALF_SM);
		memory1->I_BYTE_SM(I_BYTE_SM);
		memory1->I_MPDC_SM(I_MPDC_SM);

		mux_memory1=new mux_memory("mux_memory1");

		mux_memory1->BUBBLE_SM(BUBBLE_SM);
		mux_memory1->HOLD_SM(HOLD_SM);
		mux_memory1->KEEP_SM(KEEP_SM);
		mux_memory1->IR_RE(IR_RE);
		mux_memory1->IR_RM(IR_RM);
		mux_memory1->MUX_IR_SM(MUX_IR_SM);
		mux_memory1->DATARED_SM(DATARED_SM);
		mux_memory1->MUX_DATARED_SM(MUX_DATARED_SM);
		mux_memory1->I_TYPE_RM(I_TYPE_RM);
		mux_memory1->I_TYPE_RE(I_TYPE_RE);
		mux_memory1->MUX_I_TYPE_SM(MUX_I_TYPE_SM);
		mux_memory1->RD_RM(RD_RM);
		mux_memory1->RD_SM(RD_SM);
		mux_memory1->MUX_RD_SM(MUX_RD_SM);
		mux_memory1->OPCOD_RM(OPCOD_RM);
		mux_memory1->OPCOD_RE(OPCOD_RE);
		mux_memory1->MUX_OPCOD_SM(MUX_OPCOD_SM);
		mux_memory1->DATA_RM(DATA_RM);
		mux_memory1->DATA_SM(DATA_SM);
		mux_memory1->MUX_DATA_SM(MUX_DATA_SM);
		mux_memory1->RES_RE(RES_RE);
		mux_memory1->NEXTPC_RE(NEXTPC_RE);
		mux_memory1->BADVADR_RM(BADVADR_RM);
		mux_memory1->BADDA_XM(BADDA_XM);
		mux_memory1->BADIA_XM(BADIA_XM);
		mux_memory1->MUX_BADVADR_SM(MUX_BADVADR_SM);

		ff_memory1=new ff_memory("ff_memory1");

		ff_memory1->CK_SX(CK_SX);
		ff_memory1->MUX_IR_SM(MUX_IR_SM);
		ff_memory1->IR_RM(IR_RM);
		ff_memory1->MUX_DATARED_SM(MUX_DATARED_SM);
		ff_memory1->DATARED_RM(DATARED_RM);
		ff_memory1->MUX_I_TYPE_SM(MUX_I_TYPE_SM);
		ff_memory1->I_TYPE_RM(I_TYPE_RM);
		ff_memory1->MUX_RD_SM(MUX_RD_SM);
		ff_memory1->RD_RM(RD_RM);
		ff_memory1->MUX_OPCOD_SM(MUX_OPCOD_SM);
		ff_memory1->OPCOD_RM(OPCOD_RM);
		ff_memory1->MUX_DATA_SM(MUX_DATA_SM);
		ff_memory1->DATA_RM(DATA_RM);
		ff_memory1->MUX_BADVADR_SM(MUX_BADVADR_SM);
		ff_memory1->BADVADR_RM(BADVADR_RM);

		writeback1=new writeback("writeback1");

		writeback1->I_TYPE_RM(I_TYPE_RM);
		writeback1->OPCOD_RM(OPCOD_RM);
		writeback1->I_WRITE_SW(I_WRITE_SW);
		writeback1->WLO_SW(WLO_SW);
		writeback1->WHI_SW(WHI_SW);

		mux_writeback1=new mux_writeback("mux_writeback1");

		mux_writeback1->WLO_SW(WLO_SW);
		mux_writeback1->WHI_SW(WHI_SW);
		mux_writeback1->DATA_RM(DATA_RM);
		mux_writeback1->LO_RW(LO_RW);
		mux_writeback1->MUX_LO_SW(MUX_LO_SW);
		mux_writeback1->HI_RW(HI_RW);
		mux_writeback1->MUX_HI_SW(MUX_HI_SW);

		ff_writeback1=new ff_writeback("ff_writeback1");

		ff_writeback1->CK_SX(CK_SX);
		ff_writeback1->MUX_LO_SW(MUX_LO_SW);
		ff_writeback1->LO_RW(LO_RW);
		ff_writeback1->MUX_HI_SW(MUX_HI_SW);
		ff_writeback1->HI_RW(HI_RW);

		misc1=new misc("misc1");

        	misc1->CK(CK);
        	misc1->CK_SX(CK_SX);
        	misc1->IT_N(IT_N);
        	misc1->IT_XX(IT_XX);
        	misc1->CAUSE_RX(CAUSE_RX);
        	misc1->BDSLOT_RD(BDSLOT_RD);
        	misc1->COPERR_XM(COPERR_XM);
        	misc1->SWINT_XM(SWINT_XM);
        	misc1->SWINTRQ_XM(SWINTRQ_XM);
        	misc1->EXCCODE_XM(EXCCODE_XM);
        	misc1->RES_RE(RES_RE);
        	misc1->NEXTSR_RX(NEXTSR_RX);
        	misc1->CAUSE_SX(CAUSE_SX);
        	misc1->NEXTSR_XX(NEXTSR_XX);
        	misc1->WSR_XX(WSR_XX);
        	misc1->PC_RD(PC_RD);
        	misc1->REDOPC_RE(REDOPC_RE);
        	misc1->EPC_XX(EPC_XX);
        	misc1->RESET_RX(RESET_RX);
        	misc1->WEPC_XX(WEPC_XX);
        	misc1->INTRQ_XX(INTRQ_XX);
        	misc1->D_A(D_A);
        	misc1->NEXTPC_RD(NEXTPC_RD);
        	misc1->I_A(I_A);
        	misc1->WDATA_RE(WDATA_RE);
        	misc1->D_OUT(D_OUT);
        	misc1->DATARED_RM(DATARED_RM);
        	misc1->D_ACK(D_ACK);
        	misc1->OPCOD_RE(OPCOD_RE);
        	misc1->I_FTCH(I_FTCH);
        	misc1->INSTRED_RI(INSTRED_RI);
        	misc1->I_ACK(I_ACK);
        	misc1->I_WORD_SM(I_WORD_SM);
        	misc1->I_HALF_SM(I_HALF_SM);
        	misc1->I_BYTE_SM(I_BYTE_SM);
        	misc1->WRITE_SM(WRITE_SM);
        	misc1->D_ATYPE(D_ATYPE);
        	misc1->EARLYEX_XM(EARLYEX_XM);
        	misc1->DACCESS_SM(DACCESS_SM);
        	misc1->D_RQ(D_RQ);
        	misc1->DLOCK_SM(DLOCK_SM);
        	misc1->D_LOCK(D_LOCK);
        	misc1->EXCRQ_XM(EXCRQ_XM);
        	misc1->NEXTPC_XX(NEXTPC_XX);

		mux_misc1=new mux_misc("mux_misc1");

        	mux_misc1->CAUSE_XM(CAUSE_XM);
        	mux_misc1->EXCRQ_XM(EXCRQ_XM);
        	mux_misc1->INTRQ_XX(INTRQ_XX);
        	mux_misc1->CAUSE_XX(CAUSE_XX);
        	mux_misc1->WCAUSE_SM(WCAUSE_SM);
        	mux_misc1->KEEP_SM(KEEP_SM);
        	mux_misc1->CAUSE_SM(CAUSE_SM);
        	mux_misc1->CAUSE_SX(CAUSE_SX);
        	mux_misc1->MUX_CAUSE_SX(MUX_CAUSE_SX);
        	mux_misc1->WEPC_XM(WEPC_XM);
       		mux_misc1->EPC_XM(EPC_XM);
        	mux_misc1->WEPC_XX(WEPC_XX);
        	mux_misc1->EPC_XX(EPC_XX);
        	mux_misc1->EPC_RX(EPC_RX);
        	mux_misc1->MUX_EPC_SX(MUX_EPC_SX);
        	mux_misc1->WSR_XX(WSR_XX);
        	mux_misc1->WSR_SM(WSR_SM);
        	mux_misc1->NEXTSR_SM(NEXTSR_SM);
        	mux_misc1->NEXTSR_RX(NEXTSR_RX);
        	mux_misc1->NEXTSR_XX(NEXTSR_XX);
        	mux_misc1->MUX_NEXTSR_SX(MUX_NEXTSR_SX);
        	mux_misc1->RESET_N(RESET_N);
        	mux_misc1->MUX_RESET_SX(MUX_RESET_SX);
		
		ff_misc1=new ff_misc("ff_misc1");

        	ff_misc1->CK_SX(CK_SX);
        	ff_misc1->MUX_CAUSE_SX(MUX_CAUSE_SX);
        	ff_misc1->CAUSE_RX(CAUSE_RX);
        	ff_misc1->MUX_EPC_SX(MUX_EPC_SX);
        	ff_misc1->EPC_RX(EPC_RX);
        	ff_misc1->MUX_NEXTSR_SX(MUX_NEXTSR_SX);
        	ff_misc1->NEXTSR_RX(NEXTSR_RX);
        	ff_misc1->MUX_RESET_SX(MUX_RESET_SX);
        	ff_misc1->RESET_RX(RESET_RX);

		alu1=new alu("alu1");

        	alu1->XOPER_SE(XOPER_SE);
        	alu1->YOPER_SE(YOPER_SE);
        	alu1->I_SUB_SE(I_SUB_SE);
        	alu1->I_OPER_SE(I_OPER_SE);
        	alu1->I_RSGND_SE(I_RSGND_SE);
        	alu1->I_RIGHT_SE(I_RIGHT_SE);
        	alu1->I_LOGIC_SE(I_LOGIC_SE);
        	alu1->IOPER_RD(IOPER_RD);
        	alu1->Y_SE(Y_SE);
        	alu1->X_SE(X_SE);
        	alu1->OPCOD_RD(OPCOD_RD);
        	alu1->RES_SE(RES_SE);

		regfile1=new regfile("regfile1");

        	regfile1->RRA(RS_SD);
        	regfile1->RRB(RT_SD);
        	regfile1->WRC(RD_RM);
        	regfile1->WDATA(DATA_RM);
        	regfile1->CK_SX(CK_SX);
        	regfile1->DA(S_SD);
        	regfile1->DB(T_SD);

		forwardunit1=new forwardunit("forwardunit1");

		forwardunit1->RS_SD(RS_SD);
		forwardunit1->RS_RD(RS_RD);
		forwardunit1->RT_SD(RT_SD);
		forwardunit1->RT_RD(RT_RD);
		forwardunit1->RD_RD(RD_RD);
		forwardunit1->RD_RE(RD_RE);
		forwardunit1->RD_RM(RD_RM);
		forwardunit1->I_READS_SD(I_READS_SD);
		forwardunit1->I_READS_SE(I_READS_SE);
		forwardunit1->I_READT_SD(I_READT_SD);
		forwardunit1->I_READT_SE(I_READT_SE);
		forwardunit1->I_WRITE_SE(I_WRITE_SE);
		forwardunit1->I_WRITE_SM(I_WRITE_SM);
		forwardunit1->I_WRITE_SW(I_WRITE_SW);
		forwardunit1->I_DUSE_SD(I_DUSE_SD);
		forwardunit1->I_EPDC_SE(I_EPDC_SE);
		forwardunit1->I_MPDC_SE(I_MPDC_SE);
		forwardunit1->I_MPDC_SM(I_MPDC_SM);
		forwardunit1->I_EUSE_SE(I_EUSE_SE);
		forwardunit1->RES_RE(RES_RE);
		forwardunit1->DATA_RM(DATA_RM);
		forwardunit1->S_SD(S_SD);
		forwardunit1->T_SD(T_SD);
		forwardunit1->SOPER_RD(SOPER_RD);
		forwardunit1->TOPER_RD(TOPER_RD);
		forwardunit1->HAZARDS_SD(HAZARDS_SD);
		forwardunit1->HAZARDS_SE(HAZARDS_SE);
		forwardunit1->SOPER_SD(SOPER_SD);
		forwardunit1->TOPER_SD(TOPER_SD);
		forwardunit1->X_SE(X_SE);
		forwardunit1->Y_SE(Y_SE);

		pipeunit1=new pipeunit("pipeunit1");

		pipeunit1->KILL_SI(KILL_SI);
		pipeunit1->STALL_SI(STALL_SI);
		pipeunit1->COPY_SI(COPY_SI);
		pipeunit1->EXEC_SI(EXEC_SI);
		pipeunit1->BUBBLE_SI(BUBBLE_SI);
		pipeunit1->HOLD_SI(HOLD_SI);
		pipeunit1->SHIFT_SI(SHIFT_SI);
		pipeunit1->LOAD_SI(LOAD_SI);
		pipeunit1->KEEP_SI(KEEP_SI);

		pipeunit1->KILL_SD(KILL_SD);
		pipeunit1->STALL_SD(STALL_SD);
		pipeunit1->COPY_SD(COPY_SD);
		pipeunit1->EXEC_SD(EXEC_SD);
		pipeunit1->BUBBLE_SD(BUBBLE_SD);
		pipeunit1->HOLD_SD(HOLD_SD);
		pipeunit1->SHIFT_SD(SHIFT_SD);
		pipeunit1->LOAD_SD(LOAD_SD);
		pipeunit1->KEEP_SD(KEEP_SD);

		pipeunit1->KILL_SE(KILL_SE);
		pipeunit1->STALL_SE(STALL_SE);
		pipeunit1->COPY_SE(COPY_SE);
		pipeunit1->EXEC_SE(EXEC_SE);
		pipeunit1->BUBBLE_SE(BUBBLE_SE);
		pipeunit1->HOLD_SE(HOLD_SE);
		pipeunit1->SHIFT_SE(SHIFT_SE);
		pipeunit1->LOAD_SE(LOAD_SE);
		pipeunit1->KEEP_SE(KEEP_SE);

		pipeunit1->KILL_SM(KILL_SM);
		pipeunit1->STALL_SM(STALL_SM);
		pipeunit1->COPY_SM(COPY_SM);
		pipeunit1->EXEC_SM(EXEC_SM);
		pipeunit1->BUBBLE_SM(BUBBLE_SM);
		pipeunit1->HOLD_SM(HOLD_SM);
		pipeunit1->SHIFT_SM(SHIFT_SM);
		pipeunit1->LOAD_SM(LOAD_SM);
		pipeunit1->KEEP_SM(KEEP_SM);

		pipeunit1->KILL_SW(KILL_SW);
		pipeunit1->STALL_SW(STALL_SW);
		pipeunit1->COPY_SW(COPY_SW);
		pipeunit1->EXEC_SW(EXEC_SW);
		pipeunit1->BUBBLE_SW(BUBBLE_SW);
		pipeunit1->HOLD_SW(HOLD_SW);
		pipeunit1->SHIFT_SW(SHIFT_SW);
		pipeunit1->LOAD_SW(LOAD_SW);
		pipeunit1->KEEP_SW(KEEP_SW);

		pipeunit1->EXCRQ_XM(EXCRQ_XM);
		pipeunit1->INTRQ_XX(INTRQ_XX);
		pipeunit1->RESET_RX(RESET_RX);
		pipeunit1->SLEEP_SD(SLEEP_SD);
		pipeunit1->I_FRZ(I_FRZ);
		pipeunit1->HAZARDS_SD(HAZARDS_SD);
		pipeunit1->HAZARDS_SE(HAZARDS_SE);
		pipeunit1->D_FRZ(D_FRZ);
		pipeunit1->COPYCAP_RE(COPYCAP_RE);
	}

};
#endif

