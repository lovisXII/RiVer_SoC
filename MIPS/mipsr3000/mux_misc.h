// ### -------------------------------------------------------------- ###
// # file       : mux_misc.h                                            #
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

#ifndef _MUX_MISC
#define _MUX_MISC

#include <systemc.h>

SC_MODULE(mux_misc)
{
	sc_in<sc_uint<32> > CAUSE_XM;
	sc_in<bool> EXCRQ_XM;
	sc_in<bool> INTRQ_XX;
	sc_in<sc_uint<32> > CAUSE_XX;
	sc_in<bool> WCAUSE_SM;
	sc_in<bool> KEEP_SM;
	sc_in<sc_uint<32> > CAUSE_SM;
	sc_in<sc_uint<32> > CAUSE_SX;
	sc_out<sc_uint<32> > MUX_CAUSE_SX;

	sc_in<bool> WEPC_XM;
	sc_in<sc_uint<32> > EPC_XM;
	sc_in<bool> WEPC_XX;
	sc_in<sc_uint<32> > EPC_XX;
	sc_in<sc_uint<32> > EPC_RX;
	sc_out<sc_uint<32> > MUX_EPC_SX;

	sc_in<bool> WSR_XX;
	sc_in<bool> WSR_SM;
	sc_in<sc_uint<32> > NEXTSR_SM;
	sc_in<sc_uint<32> > NEXTSR_RX;
	sc_in<sc_uint<32> > NEXTSR_XX;
	sc_out<sc_uint<32> > MUX_NEXTSR_SX;

	sc_in<bool> RESET_N;
	sc_out<bool> MUX_RESET_SX;

	SC_CTOR(mux_misc)
	{
		SC_METHOD(processMUX_CAUSE_SX);
		sensitive << CAUSE_XM << EXCRQ_XM << INTRQ_XX << CAUSE_XX << WCAUSE_SM << KEEP_SM << CAUSE_SM << CAUSE_SX;
		SC_METHOD(processMUX_EPC_SX);
		sensitive << WEPC_XM << EPC_XM << WEPC_XX << EPC_XX << EPC_RX;
		SC_METHOD(processMUX_NEXTSR_SX);
		sensitive << WSR_XX << NEXTSR_XX << WSR_SM << KEEP_SM << NEXTSR_SM << NEXTSR_RX;
		SC_METHOD(processMUX_RESET_SX);
		sensitive << RESET_N;
	}

	// ### ----------------------------------------------------// ###
	// #   assign the the coprocessor zero's registers :		#
	// #     the Cause register				                	#
	// ### ----------------------------------------------------// ###

	void processMUX_CAUSE_SX();

	// ### ----------------------------------------------------// ###
	// #   assign the the coprocessor zero's registers :		#
	// #     the Exception Program Counter register		     	#
	// ### ----------------------------------------------------// ###

	void processMUX_EPC_SX();

	// ### ----------------------------------------------------// ###
	// #   assign the the coprocessor zero's registers :		#
	// #     the Status register					#
	// ### ----------------------------------------------------// ###

	void processMUX_NEXTSR_SX();

	// ### ----------------------------------------------------// ###
	// #   assign registers (those directly controled by hardware)	#
	// ### ----------------------------------------------------// ###

	void processMUX_RESET_SX();

};
#endif
