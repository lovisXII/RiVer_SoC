// ### -------------------------------------------------------------- ###
// # files      : mux_execute.h, mux_execute.cpp                        #
// # date       : Mar 01 2022                                           #
// # version    : v2.1                                                  #
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
// #                                                                    #
// # updating   : K. Lastra                                             #
// ### -------------------------------------------------------------- ###


#ifndef _MUX_EXECUTE
#define _MUX_EXECUTE

#include <systemc.h>
#include "constants.h"

SC_MODULE(mux_execute)
{
	sc_in<bool> BUBBLE_SE;				// introduce bubble
	sc_in<bool> HOLD_SE;				// hold instruction
	sc_in<bool> KEEP_SE;				// keep the data
	
	sc_in<bool> STALL_SM;				// sleep instruction stall
	sc_in<bool> COPY_SM;				// duplicate instruction

	sc_in<sc_uint<32> > IR_RD;			// instruction register from decod
	sc_in<sc_uint<32> > IR_RE;			// instruction register from exec
	sc_out<sc_uint<32> > MUX_IR_SE;		// instruction register output

	sc_in<sc_uint<25> > I_TYPE_RD;		// instruction type from decod
	sc_in<sc_uint<25> > I_TYPE_RE;		// instruction type from exec
	sc_out<sc_uint<25> > MUX_I_TYPE_SE;	// instruction type output

	sc_in<sc_uint<8> > OPCOD_RD;		// operation code from decod	
	sc_in<sc_uint<8> > OPCOD_RE;		// operation code from exec
	sc_out<sc_uint<8> > MUX_OPCOD_SE;	// operation code output

	sc_in<bool> BDSLOT_RD;				// delayed slot from decod
	sc_in<bool> BDSLOT_RE;				// delayed slot from exec
	sc_out<bool> MUX_BDSLOT_SE;			// delayed slot output

	sc_in<sc_uint<5> > RD_RD;			// register destination @ from decod
	sc_in<sc_uint<5> > RD_RE;			// register destination @ from exec
	sc_out<sc_uint<5> > MUX_RD_SE;		// register destination @ output

	sc_in<sc_uint<5> > COP0D_RD;		// coprocesseur 0 register destination from decod
	sc_in<sc_uint<5> > COP0D_RE;		// coprocesseur 0 register destination from exec
	sc_out<sc_uint<5> > MUX_COP0D_SE;	// coprocesseur 0 register destination output

	sc_in<bool> OVF_RE;					// arithmetique overflow
	sc_in<bool> OVF_XE;					// arithmetique overflow exception
	sc_out<bool> MUX_OVF_SE;			// arithmetique overflow output

	sc_in<bool> IAMALGN_RE;				// instruction @ miss alignement
	sc_in<bool> IAMALGN_XE;				// instruction @ miss alignement
	sc_out<bool> MUX_IAMALGN_SE;		// instruction @ miss alignement output

	sc_in<bool> IASVIOL_RE;				// instruction @ segmentation violation
	sc_in<bool> IASVIOL_XE;				// instruction @ segmentation violation
	sc_out<bool> MUX_IASVIOL_SE;		// instruction @ segmentation violation output

	sc_in<bool> IABUSER_RE;				// instruction @ bus error
	sc_in<bool> IABUSER_XE;				// instruction @ bus error
	sc_out<bool> MUX_IABUSER_SE;		// instruction @ bus error output

	sc_in<bool> BREAK_RE;				// sw exec (break)
	sc_in<bool> BREAK_XE;				// sw exec (break)
	sc_out<bool> MUX_BREAK_SE;			// sw exec (break) output

	sc_in<bool> SYSCALL_RE;				// sw exec (syscall)
	sc_in<bool> SYSCALL_XE;				// sw exec (syscall)
	sc_out<bool> MUX_SYSCALL_SE;		// sw exec (syscall) output

	sc_in<bool> ILLGINS_RD;				// unknown instruction signal from decod
	sc_in<bool> ILLGINS_RE;				// unknown instruction signal from exec
	sc_out<bool> MUX_ILLGINS_SE;		// unknown instruction signal output

	sc_in<bool> C0UNUSE_RE;				// coprocesseur 0 unusable from exec	
	sc_in<bool> C0UNUSE_RD;				// coprocesseur 0 unusable from decod
	sc_out<bool> MUX_C0UNUSE_SE;		// coprocesseur 0 unusable output

	sc_in<bool> SWAP_RE;				// swap instruction from exec
	sc_in<bool> SWAP_RD;				// swap instruction from decod
	sc_out<bool> MUX_SWAP_SE;			// swap instruction output

	sc_in<sc_uint<32> > PC_RD;			// instruction @ from decod						
	sc_in<sc_uint<32> > PC_RE;			// instruction @ from exec
	sc_out<sc_uint<32> > MUX_PC_SE;		// instruction @ output

	sc_in<sc_uint<32> > SR_RD;			// status register from decod
	sc_in<sc_uint<32> > SR_RE;			// status register from exec
	sc_out<sc_uint<32> > MUX_SR_SE;		// status register output

	sc_in<sc_uint<32> > NEXTPC_RD;		// next instruction @ from decod
	sc_in<sc_uint<32> > NEXTPC_RE;		// next instruction @ from exec
	sc_out<sc_uint<32> > MUX_NEXTPC_SE;	// next instruction @ output

	sc_in<sc_uint<32> > RES_SE;			// result of alu
	sc_in<sc_uint<32> > RES_RE;			// result of alu
	sc_out<sc_uint<32> > MUX_RES_SE;	// result of alu output

	sc_in<sc_uint<32> > Y_SE;			// Y operand
	sc_in<sc_uint<32> > WDATA_RE;		// data bus output register
	sc_out<sc_uint<32> > MUX_WDATA_SE;	// data bus output

	sc_in<bool> COPYCAP_SE;				// copying capability
	sc_in<bool> COPYCAP_RE;				// copying capability
	sc_out<bool> MUX_COPYCAP_SE;		// copying capability output

	sc_in<sc_uint<32> > REDOPC_RE;		// old instruction @
	sc_in<bool> WREDOPC_SE;				// redopc write enabled
	sc_out<sc_uint<32> > MUX_REDOPC_SE;	// redopc output

	SC_CTOR(mux_execute):
	BUBBLE_SE("BUBBLE_SE"),
	HOLD_SE("HOLD_SE"),
	KEEP_SE("KEEP_SE"),
	STALL_SM("STALL_SM"),
	COPY_SM("COPY_SM"),

	IR_RD("IR_RD"),
	IR_RE("IR_RE"),
	MUX_IR_SE("MUX_IR_SE"),

	I_TYPE_RD("I_TYPE_RD"),
	I_TYPE_RE("I_TYPE_RE"),
	MUX_I_TYPE_SE("MUX_I_TYPE_SE"),

	OPCOD_RD("OPCOD_RD"),
	OPCOD_RE("OPCOD_RE"),
	MUX_OPCOD_SE("MUX_OPCOD_SE"),

	BDSLOT_RD("BDSLOT_RD"),
	BDSLOT_RE("BDSLOT_RE"),
	MUX_BDSLOT_SE("MUX_BDSLOT_SE"),

	RD_RD("RD_RD"),
	RD_RE("RD_RE"),
	MUX_RD_SE("MUX_RD_SE"),

	COP0D_RD("COP0D_RD"),
	COP0D_RE("COP0D_RE"),
	MUX_COP0D_SE("MUX_COP0D_SE"),

	OVF_RE("OVF_RE"),
	OVF_XE("OVF_XE"),
	MUX_OVF_SE("MUX_OVF_SE"),

	IAMALGN_RE("IAMALGN_RE"),
	IAMALGN_XE("IAMALGN_XE"),
	MUX_IAMALGN_SE("MUX_IAMALGN_SE"),

	IASVIOL_RE("IASVIOL_RE"),
	IASVIOL_XE("IASVIOL_XE"),
	MUX_IASVIOL_SE("MUX_IASVIOL_SE"),

	IABUSER_RE("IABUSER_RE"),
	IABUSER_XE("IABUSER_XE"),
	MUX_IABUSER_SE("MUX_IABUSER_SE"),

	BREAK_RE("BREAK_RE"),
	BREAK_XE("BREAK_XE"),
	MUX_BREAK_SE("MUX_BREAK_SE"),

	SYSCALL_RE("SYSCALL_RE"),
	SYSCALL_XE("SYSCALL_XE"),
	MUX_SYSCALL_SE("MUX_SYSCALL_SE"),

	ILLGINS_RD("ILLGINS_RD"),
	ILLGINS_RE("ILLGINS_RE"),
	MUX_ILLGINS_SE("MUX_ILLGINS_SE"),

	C0UNUSE_RE("C0UNUSE_RE"),
	C0UNUSE_RD("C0UNUSE_RD"),
	MUX_C0UNUSE_SE("MUX_C0UNUSE_SE"),

	SWAP_RE("SWAP_RE"),
	SWAP_RD("SWAP_RD"),
	MUX_SWAP_SE("MUX_SWAP_SE"),

	PC_RD("PC_RD"),
	PC_RE("PC_RE"),
	MUX_PC_SE("MUX_PC_SE"),

	SR_RD("SR_RD"),
	SR_RE("SR_RE"),
	MUX_SR_SE("MUX_SR_SE"),

	NEXTPC_RD("NEXTPC_RD"),
	NEXTPC_RE("NEXTPC_RE"),
	MUX_NEXTPC_SE("MUX_NEXTPC_SE"),

	RES_SE("RES_SE"),
	RES_RE("RES_RE"),
	MUX_RES_SE("MUX_RES_SE"),

	Y_SE("Y_SE"),
	WDATA_RE("WDATA_RE"),
	MUX_WDATA_SE("MUX_WDATA_SE"),

	COPYCAP_SE("COPYCAP_SE"),
	COPYCAP_RE("COPYCAP_RE"),
	MUX_COPYCAP_SE("MUX_COPYCAP_SE"),

	REDOPC_RE("REDOPC_RE"),
	WREDOPC_SE("WREDOPC_SE"),
	MUX_REDOPC_SE("MUX_REDOPC_SE")

	{
		SC_METHOD(processMUX_IR_SE);
		sensitive << BUBBLE_SE << HOLD_SE << IR_RE << IR_RD ;
		SC_METHOD(processMUX_I_TYPE_SE);
		sensitive << BUBBLE_SE << HOLD_SE << I_TYPE_RE << I_TYPE_RD;
		SC_METHOD(processMUX_OPCOD_SE);
		sensitive << BUBBLE_SE << HOLD_SE << OPCOD_RE << OPCOD_RD;
		SC_METHOD(processMUX_BDSLOT_SE);
		sensitive << BUBBLE_SE << HOLD_SE << BDSLOT_RE << BDSLOT_RD;
		SC_METHOD(processMUX_RD_SE);
		sensitive << BUBBLE_SE << HOLD_SE << RD_RE << RD_RD;
		SC_METHOD(processMUX_COP0D_SE);
		sensitive << BUBBLE_SE << HOLD_SE << COP0D_RE << COP0D_RD;
		SC_METHOD(processMUX_OVF_SE);
		sensitive << BUBBLE_SE << HOLD_SE << OVF_RE << OVF_XE;
		SC_METHOD(processMUX_IALMALGN_SE);
		sensitive << BUBBLE_SE << HOLD_SE << IAMALGN_RE << IAMALGN_XE;
		SC_METHOD(processMUX_IASVIOL_SE);
		sensitive << BUBBLE_SE << HOLD_SE << IASVIOL_RE << IASVIOL_XE;
		SC_METHOD(processMUX_IABUSER_SE);
		sensitive << BUBBLE_SE << HOLD_SE << IABUSER_RE << IABUSER_XE;
		SC_METHOD(processMUX_BREAK_SE);
		sensitive << BUBBLE_SE << HOLD_SE << BREAK_RE << BREAK_XE;
		SC_METHOD(processMUX_SYSCALL_SE);
		sensitive << BUBBLE_SE << HOLD_SE << SYSCALL_RE << SYSCALL_XE;
		SC_METHOD(processMUX_ILLGINS_SE);
		sensitive << BUBBLE_SE << HOLD_SE << ILLGINS_RE << ILLGINS_RD;
		SC_METHOD(processMUX_C0UNUSE_SE);
		sensitive << BUBBLE_SE << HOLD_SE << C0UNUSE_RE << C0UNUSE_RD;
		SC_METHOD(processMUX_SWAP_SE);
		sensitive << BUBBLE_SE << HOLD_SE << SWAP_RE << SWAP_RD ;
		SC_METHOD(processMUX_PC_SE);
		sensitive << HOLD_SE << PC_RE << PC_RD ;
		SC_METHOD(processMUX_SR_SE);
		sensitive << HOLD_SE << SR_RE << SR_RD ;
		SC_METHOD(processMUX_NEXTPC_SE);
		sensitive << KEEP_SE << NEXTPC_RE << NEXTPC_RD ;
		SC_METHOD(processMUX_RES_SE);
		sensitive << KEEP_SE << RES_RE << RES_SE ;
		SC_METHOD(processMUX_WDATA_SE);
		sensitive << KEEP_SE << WDATA_RE << Y_SE ;
		SC_METHOD(processMUX_COPYCAP_SE);
		sensitive << BUBBLE_SE << STALL_SM << COPY_SM << COPYCAP_RE << COPYCAP_SE ;
		SC_METHOD(processMUX_REDOPC_SE);
		sensitive << KEEP_SE << WREDOPC_SE << REDOPC_RE << PC_RD ;
	}

	void processMUX_IR_SE();

	void processMUX_I_TYPE_SE();

	void processMUX_OPCOD_SE();

	void processMUX_BDSLOT_SE();

	void processMUX_RD_SE();

	void processMUX_COP0D_SE();

	void processMUX_OVF_SE();

	void processMUX_IALMALGN_SE();

	void processMUX_IASVIOL_SE();

	void processMUX_IABUSER_SE();

	void processMUX_BREAK_SE();

	void processMUX_SYSCALL_SE();

	void processMUX_ILLGINS_SE();

	void processMUX_C0UNUSE_SE();

	void processMUX_SWAP_SE();

	void processMUX_PC_SE();

	void processMUX_SR_SE();

	void processMUX_NEXTPC_SE();

	void processMUX_RES_SE();

	void processMUX_WDATA_SE();

	void processMUX_COPYCAP_SE();

	void processMUX_REDOPC_SE();

};
#endif

