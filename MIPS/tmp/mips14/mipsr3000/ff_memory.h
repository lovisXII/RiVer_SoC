// ### -------------------------------------------------------------- ###
// # file       : ff_memory.h                                           #
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

#ifndef _FF_MEMORY
#define _FF_MEMORY
//#include "systemc.h"
#include "constants.h"
#include <systemc.h>

SC_MODULE(ff_memory)
{
	sc_in<bool> CK_SX;

	sc_in<sc_uint<32>> MUX_IR_SM;
	sc_out<sc_uint<32>> IR_RM;

	sc_in<bool> MUX_DATARED_SM;
	sc_out<bool> DATARED_RM;

	sc_in<sc_uint<25>> MUX_I_TYPE_SM;
	sc_out<sc_uint<25>> I_TYPE_RM;

	sc_in<sc_uint<5>> MUX_RD_SM;
	sc_out<sc_uint<5>> RD_RM;

	sc_in<sc_uint<8>> MUX_OPCOD_SM;
	sc_out<sc_uint<8>> OPCOD_RM;

	sc_in<sc_uint<32>> MUX_DATA_SM;
	sc_out<sc_uint<32>> DATA_RM;

	sc_in<sc_uint<32>> MUX_BADVADR_SM;
	sc_out<sc_uint<32>> BADVADR_RM;

	SC_CTOR(ff_memory)
	{
		SC_METHOD(processClock);
		sensitive_pos 
			<< CK_SX;
	}

	void processClock()
	{
		IR_RM.write(MUX_IR_SM.read());
		DATARED_RM.write(MUX_DATARED_SM.read());
		I_TYPE_RM.write(MUX_I_TYPE_SM.read());
		RD_RM.write(MUX_RD_SM.read());
		OPCOD_RM.write(MUX_OPCOD_SM.read());
		DATA_RM.write(MUX_DATA_SM.read());
		BADVADR_RM.write(MUX_BADVADR_SM.read());
	}
};
#endif

