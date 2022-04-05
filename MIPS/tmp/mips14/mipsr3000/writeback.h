// ### -------------------------------------------------------------- ###
// # file       : writeback.h                                           #
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

#ifndef _WRITEBACK
#define _WRITEBACK
//#include "systemc.h"
#include <systemc.h>

SC_MODULE(writeback)
{
	sc_in<sc_uint<25>> 		I_TYPE_RM;
	sc_in<sc_uint<8>> 		OPCOD_RM;
	sc_out<bool>			I_WRITE_SW;
	sc_out<bool>			WLO_SW;
	sc_out<bool>			WHI_SW;

	SC_CTOR(writeback):
		I_WRITE_SW("I_WRITE_SW")
	{
		SC_METHOD(processI_WRITE_SW);
		sensitive 
			<< I_TYPE_RM;
		SC_METHOD(processWLO_SW);
		sensitive 
			<< OPCOD_RM;
		SC_METHOD(processWHI_SW);
		sensitive 
			<< OPCOD_RM;
	}

	void processI_WRITE_SW()
	{
		sc_uint<25> i_type_rm=I_TYPE_RM.read();

		I_WRITE_SW.write(i_type_rm[8] | i_type_rm[7]);
	}

		// ### ----------------------------------------------------// ###
		// #   low and high registers' write enable:			#
		// #     - theses registers are modified when a Move into LO or	#
		// #       a Move into HI instruction is executed		#
		// ### ----------------------------------------------------// ###

	void processWLO_SW()
	{
		if ((int)OPCOD_RM.read()==mtlo_i)
			WLO_SW.write(1);
		else
			WLO_SW.write(0);
	}

	void processWHI_SW()
	{
		if ((int)OPCOD_RM.read()==mthi_i)
			WHI_SW.write(1);
		else
			WHI_SW.write(0);
	}
};
#endif
