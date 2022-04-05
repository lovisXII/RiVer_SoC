// ### -------------------------------------------------------------- ###
// # file       : ifetch.h                                              #
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

#ifndef _IFETCH
#define _IFETCH
//#include "systemc.h"
#include "constants.h"
#include <systemc.h>

SC_MODULE(ifetch)
{
	sc_in<bool> 		I_BRNCH_SD;
	sc_in<sc_uint<32>> 	NEXTSR_RX;
	sc_in<sc_uint<8>> 	OPCOD_SD;
	sc_in<sc_uint<8>> 	OPCOD_RD;
	sc_in<sc_uint<8>> 	OPCOD_RE;

	sc_out<sc_uint<32>> 	SR_SI;
	sc_out<bool>		BDSLOT_XI;

	SC_CTOR(ifetch):
		I_BRNCH_SD("I_BRNCH_SD"),
		NEXTSR_RX("NEXTSR_RX"),
		OPCOD_SD("OPCOD_SD"),
		OPCOD_RD("OPCOD_RD"),
		OPCOD_RE("OPCOD_RE"),
		SR_SI("SR_SI"),
		BDSLOT_XI("BDSLOT_XI")
	{
		SC_METHOD(processBDSLOT_XI);
		sensitive 
			<< I_BRNCH_SD;
		SC_METHOD(processSR_SI);
		sensitive 
			<< NEXTSR_RX 
			<< OPCOD_SD 
			<< OPCOD_RD 
			<< OPCOD_RE;
	}

		// ### ------------------------------------------------------ ###
		// #   the incoming instruction is a branch delayed slot	#
		// ### ------------------------------------------------------ ###

	void processBDSLOT_XI()
	{
		BDSLOT_XI.write(I_BRNCH_SD.read());
	}

		// ### ------------------------------------------------------ ###
		// #   define the value of the status register for the incoming	#
		// # instruction (bypass the status register when an rfe is	#
		// # being executed in the pipe)				#
		// ### ------------------------------------------------------ ###

	void processSR_SI()
	{
		sc_uint<32> nextsr_rx=NEXTSR_RX.read();

		if (((int)OPCOD_SD.read()==rfe_i)||
			((int)OPCOD_RD.read()==rfe_i)||
			((int)OPCOD_RE.read()==rfe_i))
			SR_SI.write(nextsr_rx.range(31,4) 
			<< 4 | nextsr_rx.range(5,2));
		else
			SR_SI.write(nextsr_rx);
	}
};
#endif
