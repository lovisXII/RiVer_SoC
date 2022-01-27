// ### -------------------------------------------------------------- ###
// # file       : mux_execute.h                                         #
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

#ifndef _MUX_EXECUTE
#define _MUX_EXECUTE

#include <systemc.h>
#include "constants.h"

SC_MODULE(mux_execute)
{
	sc_in<bool> BUBBLE_SE;
	sc_in<bool> HOLD_SE;
	sc_in<bool> KEEP_SE;
	sc_in<bool> STALL_SM;
	sc_in<bool> COPY_SM;

	sc_in<sc_uint<32> > IR_RD;
	sc_in<sc_uint<32> > IR_RE;
	sc_out<sc_uint<32> > MUX_IR_SE;

	sc_in<sc_uint<25> > I_TYPE_RD;
	sc_in<sc_uint<25> > I_TYPE_RE;
	sc_out<sc_uint<25> > MUX_I_TYPE_SE;

	sc_in<sc_uint<8> > OPCOD_RD;
	sc_in<sc_uint<8> > OPCOD_RE;
	sc_out<sc_uint<8> > MUX_OPCOD_SE;

	sc_in<bool> BDSLOT_RD;
	sc_in<bool> BDSLOT_RE;
	sc_out<bool> MUX_BDSLOT_SE;

	sc_in<sc_uint<5> > RD_RD;
	sc_in<sc_uint<5> > RD_RE;
	sc_out<sc_uint<5> > MUX_RD_SE;

	sc_in<sc_uint<5> > COP0D_RD;
	sc_in<sc_uint<5> > COP0D_RE;
	sc_out<sc_uint<5> > MUX_COP0D_SE;

	sc_in<bool> OVF_RE;
	sc_in<bool> OVF_XE;
	sc_out<bool> MUX_OVF_SE;

	sc_in<bool> IAMALGN_RE;
	sc_in<bool> IAMALGN_XE;
	sc_out<bool> MUX_IAMALGN_SE;

	sc_in<bool> IASVIOL_RE;
	sc_in<bool> IASVIOL_XE;
	sc_out<bool> MUX_IASVIOL_SE;

	sc_in<bool> IABUSER_RE;
	sc_in<bool> IABUSER_XE;
	sc_out<bool> MUX_IABUSER_SE;

	sc_in<bool> BREAK_RE;
	sc_in<bool> BREAK_XE;
	sc_out<bool> MUX_BREAK_SE;

	sc_in<bool> SYSCALL_RE;
	sc_in<bool> SYSCALL_XE;
	sc_out<bool> MUX_SYSCALL_SE;

	sc_in<bool> ILLGINS_RD;
	sc_in<bool> ILLGINS_RE;
	sc_out<bool> MUX_ILLGINS_SE;

	sc_in<bool> C0UNUSE_RE;
	sc_in<bool> C0UNUSE_RD;
	sc_out<bool> MUX_C0UNUSE_SE;

	sc_in<bool> SWAP_RE;
	sc_in<bool> SWAP_RD;
	sc_out<bool> MUX_SWAP_SE;

	sc_in<sc_uint<32> > PC_RD;
	sc_in<sc_uint<32> > PC_RE;
	sc_out<sc_uint<32> > MUX_PC_SE;

	sc_in<sc_uint<32> > SR_RD;
	sc_in<sc_uint<32> > SR_RE;
	sc_out<sc_uint<32> > MUX_SR_SE;

	sc_in<sc_uint<32> > NEXTPC_RD;
	sc_in<sc_uint<32> > NEXTPC_RE;
	sc_out<sc_uint<32> > MUX_NEXTPC_SE;

	sc_in<sc_uint<32> > RES_SE;
	sc_in<sc_uint<32> > RES_RE;
	sc_out<sc_uint<32> > MUX_RES_SE;

	sc_in<sc_uint<32> > Y_SE;
	sc_in<sc_uint<32> > WDATA_RE;
	sc_out<sc_uint<32> > MUX_WDATA_SE;

	sc_in<bool> COPYCAP_SE;
	sc_in<bool> COPYCAP_RE;
	sc_out<bool> MUX_COPYCAP_SE;

	sc_in<sc_uint<32> > REDOPC_RE;
	sc_in<bool> WREDOPC_SE;
	sc_out<sc_uint<32> > MUX_REDOPC_SE;

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

