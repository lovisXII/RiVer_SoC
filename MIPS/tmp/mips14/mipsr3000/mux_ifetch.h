// ### -------------------------------------------------------------- ###
// # file       : mux_ifetch.h                                          #
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

#ifndef _MUX_IFETCH
#define _MUX_IFETCH
//#include "systemc.h"
#include "constants.h"
#include <systemc.h>

SC_MODULE(mux_ifetch)
{
	sc_in<bool> BUBBLE_SI;
	sc_in<bool> HOLD_SI;
	sc_in<bool> KEEP_SI;

	sc_in<sc_uint<32>> IR_RI;
	sc_in<sc_uint<32>> I;
	sc_out<sc_uint<32>> MUX_IR_SI;

	sc_out<bool> MUX_INSTRED_SI;

	sc_in<bool> BDSLOT_XI;
	sc_in<bool> BDSLOT_RI;
	sc_out<bool> MUX_BDSLOT_SI;

	sc_in<sc_uint<32>> PC_RI;
	sc_in<sc_uint<32>> NEXTPC_RD;
	sc_out<sc_uint<32>> MUX_PC_SI;

	sc_in<sc_uint<32>> SR_RI;
	sc_in<sc_uint<32>> SR_SI;
	sc_out<sc_uint<32>> MUX_SR_SI;

	SC_CTOR(mux_ifetch):
		BUBBLE_SI("BUBBLE_SI"),
		HOLD_SI("HOLD_SI"),
		KEEP_SI("KEEP_SI"),
		IR_RI("IR_RI"),
		I("I"),
		MUX_IR_SI("MUX_IR_SI"),
		MUX_INSTRED_SI("MUX_INSTRED_SI"),
		BDSLOT_XI("BDSLOT_XI"),
		BDSLOT_RI("BDSLOT_RI"),
		MUX_BDSLOT_SI("MUX_BDSLOT_SI"),
		PC_RI("PC_RI"),
		NEXTPC_RD("NEXTPC_RD"),
		MUX_PC_SI("MUX_PC_SI"),
		SR_RI("SR_RI"),
		SR_SI("SR_SI"),
		MUX_SR_SI("MUX_SR_SI")

	{
		SC_METHOD(processMUX_IR_SI);
		sensitive << BUBBLE_SI << HOLD_SI << IR_RI << I ;
		SC_METHOD(processMUX_INSTRED_SI);
		sensitive << BUBBLE_SI << HOLD_SI ;
		SC_METHOD(processMUX_BDSLOT_SI);
		sensitive << BUBBLE_SI << HOLD_SI << BDSLOT_XI << BDSLOT_RI ;
		SC_METHOD(processMUX_PC_SI);
		sensitive << KEEP_SI << PC_RI << NEXTPC_RD ;
		SC_METHOD(processMUX_SR_SI);
		sensitive << KEEP_SI << SR_RI << SR_SI ;
	}

	void processMUX_IR_SI()
	{
		if (BUBBLE_SI.read())
			MUX_IR_SI.write(nop_i);
		else if (HOLD_SI.read())
			MUX_IR_SI.write(IR_RI.read());
		else
			MUX_IR_SI.write(I.read());
	}

	void processMUX_INSTRED_SI()
	{
		if (BUBBLE_SI.read())
			MUX_INSTRED_SI.write(0);
		else if (HOLD_SI.read())
			MUX_INSTRED_SI.write(0);
		else
			MUX_INSTRED_SI.write(1);
	}

	void processMUX_BDSLOT_SI()
	{
		if (BUBBLE_SI.read())
			MUX_BDSLOT_SI.write(BDSLOT_XI.read());
		else if (HOLD_SI.read())
			MUX_BDSLOT_SI.write(BDSLOT_RI.read());
		else
			MUX_BDSLOT_SI.write(BDSLOT_XI.read());
	}

	void processMUX_PC_SI()
	{
		if (KEEP_SI.read())
			MUX_PC_SI.write(PC_RI.read());
		else
			MUX_PC_SI.write(NEXTPC_RD.read());
	}

	void processMUX_SR_SI()
	{
		if (KEEP_SI.read())
			MUX_SR_SI.write(SR_RI.read());
		else
			MUX_SR_SI.write(SR_SI.read());
	}

};
#endif

