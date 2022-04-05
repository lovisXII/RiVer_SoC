// ### -------------------------------------------------------------- ###
// # file       : ff_misc.h                                             #
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

#ifndef _FF_MISC
#define _FF_MISC
//#include "systemc.h"
#include <systemc.h>

SC_MODULE(ff_misc)
{
	sc_in<bool> CK_SX;

	sc_in<sc_uint<32>> 	MUX_CAUSE_SX;
	sc_out<sc_uint<32>> 	CAUSE_RX;
	sc_in<sc_uint<32>>	MUX_EPC_SX;
	sc_out<sc_uint<32>>	EPC_RX;
	sc_in<sc_uint<32>>	MUX_NEXTSR_SX;
	sc_out<sc_uint<32>>	NEXTSR_RX;
	sc_in<bool>		MUX_RESET_SX;
	sc_out<bool>		RESET_RX;

	SC_CTOR(ff_misc)
	{
		SC_METHOD(processClock);
		sensitive_pos 
			<< CK_SX;
	}

	void processClock()
	{
		// ### ------------------------------------------------------ ###
		// #   assign the the coprocessor zero's registers :		#
		// #     the Cause register					#
		// ### ------------------------------------------------------ ###

		CAUSE_RX.write(MUX_CAUSE_SX.read());

		// ### ------------------------------------------------------ ###
		// #   assign the the coprocessor zero's registers :		#
		// #     the Exception Program Counter register			#
		// ### ------------------------------------------------------ ###

		EPC_RX.write(MUX_EPC_SX.read());

		// ### ------------------------------------------------------ ###
		// #   assign the the coprocessor zero's registers :		#
		// #     the Status register					#
		// ### ------------------------------------------------------ ###

		NEXTSR_RX.write(MUX_NEXTSR_SX.read());

		// ### ------------------------------------------------------ ###
		// #   assign registers (those directly controled by hardware)	#
		// ### ------------------------------------------------------ ###

		RESET_RX.write(MUX_RESET_SX.read());
	}
};
#endif

