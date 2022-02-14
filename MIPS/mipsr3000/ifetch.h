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

#include <systemc.h>
#include "constants.h"

SC_MODULE(ifetch)
{
	sc_in<bool> 		I_BRNCH_SD;		//branch instruction
	sc_in<sc_uint<32>>	NEXTSR_RX;		//next instruction sts
	sc_in<sc_uint<8>> 	OPCOD_SD;		//opcode from decod signal
	sc_in<sc_uint<8>> 	OPCOD_RD;		//opcode from decod register
	sc_in<sc_uint<8>> 	OPCOD_RE;		//opcode from exec register

	sc_out<sc_uint<32>>	SR_SI;			// Status register
	sc_out<bool>		BDSLOT_XI;   	// Branch delayed slot

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
		sensitive << I_BRNCH_SD;
		SC_METHOD(processSR_SI);
		sensitive << NEXTSR_RX << OPCOD_SD << OPCOD_RD << OPCOD_RE;
	}

  // ### ------------------------------------------------------ ###
  // #   the incoming instruction is a branch delayed slot	#
  // ### ------------------------------------------------------ ###

	void processBDSLOT_XI();
    
  // ### ------------------------------------------------------ ###
  // #   define the value of the status register for the incoming	#
  // # instruction (bypass the status register when an rfe is	#
  // # being executed in the pipe)				#
  // ### ------------------------------------------------------ ###
    
	void processSR_SI();
};
#endif
