// ### -------------------------------------------------------------- ###
// # file       : mux_writeback.h                                       #
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

#ifndef _MUX_WRITEBACK
#define _MUX_WRITEBACK
//#include "systemc.h"
#include "constants.h"
#include <systemc.h>

SC_MODULE(mux_writeback)
{
	sc_in<bool> 		WLO_SW;
	sc_in<bool> 		WHI_SW;
	sc_in<sc_uint<32> > 	DATA_RM;
	sc_in<sc_uint<32> > 	LO_RW;
	sc_out<sc_uint<32> >	MUX_LO_SW;
	sc_in<sc_uint<32> > 	HI_RW;
	sc_out<sc_uint<32> > 	MUX_HI_SW;

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

	void processMUX_LO_SW()
	{
		if (WLO_SW.read()==1)
			MUX_LO_SW.write(DATA_RM.read());
		else
			MUX_LO_SW.write(LO_RW.read());
	}

	void processMUX_HI_SW()
	{
		if (WHI_SW.read()==1)
			MUX_HI_SW.write(DATA_RM.read());
		else
			MUX_HI_SW.write(HI_RW.read());
	}

};
#endif

