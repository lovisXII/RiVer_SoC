// ### -------------------------------------------------------------- ###
// # files      : mux_writeback.h, mux_writeback.cpp                    #
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

#ifndef _MUX_WRITEBACK
#define _MUX_WRITEBACK

#include <systemc.h>
#include "constants.h"

SC_MODULE(mux_writeback)
{
	sc_in<bool> 			WLO_SW; 	// low registers write enable
	sc_in<bool> 			WHI_SW;		// high registers write enable
	sc_in<sc_uint<32>> 	DATA_RM;	// data bus input register
	sc_in<sc_uint<32>> 	LO_RW;		// low register
	sc_out<sc_uint<32>>	MUX_LO_SW;	// res low register
	sc_in<sc_uint<32>> 	HI_RW;		// high register
	sc_out<sc_uint<32>> 	MUX_HI_SW;	// res high register

	SC_CTOR(mux_writeback):
		WLO_SW("WLO_SW"),
		WHI_SW("WHI_SW"),
		DATA_RM("DATA_RM"),
		LO_RW("LO_RW"),
		MUX_LO_SW("MUX_LO_SW"),
		HI_RW("HI_RW"),
		MUX_HI_SW("MUX_HI_SW")
	{
		SC_METHOD(processMUX_LO_SW);
		sensitive << WLO_SW << DATA_RM << LO_RW;
		SC_METHOD(processMUX_HI_SW);
		sensitive << WHI_SW << DATA_RM << HI_RW;
	}

	void processMUX_LO_SW();

	void processMUX_HI_SW();

};
#endif

