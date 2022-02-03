// ### -------------------------------------------------------------- ###
// # file       : ff_ifetch.h                                           #
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

#ifndef _FF_IFETCH
#define _FF_IFETCH
//#include "systemc.h"
#include "constants.h"
#include <systemc.h>

SC_MODULE(ff_ifetch)
{
	sc_in<bool> CK_SX;

	sc_in<sc_uint<32> > MUX_IR_SI;
	sc_out<sc_uint<32> > IR_RI;

	sc_in<bool> MUX_INSTRED_SI;
	sc_out<bool> INSTRED_RI;

	sc_in<bool> MUX_BDSLOT_SI;
	sc_out<bool> BDSLOT_RI;

	sc_in<sc_uint<32> > MUX_PC_SI;
	sc_out<sc_uint<32> > PC_RI;

	sc_in<sc_uint<32> > MUX_SR_SI;
	sc_out<sc_uint<32> > SR_RI;

	SC_CTOR(ff_ifetch):
		CK_SX("CK_SX"),
		MUX_IR_SI("MUX_IR_SI"),
		IR_RI("IR_RI"),
		MUX_INSTRED_SI("MUX_INSTRED_SI"),
		INSTRED_RI("INSTRED_RI"),
		MUX_BDSLOT_SI("MUX_BDSLOT_SI"),
		BDSLOT_RI("BDSLOT_RI"),
		MUX_PC_SI("MUX_PC_SI"),
		PC_RI("PC_RI"),
		MUX_SR_SI("MUX_SR_SI"),
		SR_RI("SR_RI")
	{
		SC_METHOD(processClock);
		sensitive_pos << CK_SX;
	}

	void processClock()
	{
		IR_RI.write(MUX_IR_SI.read());
		INSTRED_RI.write(MUX_INSTRED_SI.read());
		BDSLOT_RI.write(MUX_BDSLOT_SI.read());
		PC_RI.write(MUX_PC_SI.read());
		SR_RI.write(MUX_SR_SI.read());
	}
};
#endif

