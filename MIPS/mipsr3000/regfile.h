// ### -------------------------------------------------------------- ###
// # file       : regfile.h                                             #
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

#ifndef _REGFILE
#define _REGFILE

#include <systemc.h>

SC_MODULE(regfile)
{
	sc_in<sc_uint<5>> RRA;
	sc_in<sc_uint<5>> RRB;
	sc_in<sc_uint<5>> WRC;
	sc_in<sc_uint<32>> WDATA;
	sc_in_clk CK_SX;
	sc_out<sc_uint<32>> DA;
	sc_out<sc_uint<32>> DB;

	sc_signal<sc_uint<32>> registerFile[32];

	SC_CTOR(regfile)
	{
		SC_METHOD(mReadRegs);
		sensitive << RRA << RRB;
		SC_METHOD(mWriteReg);
		sensitive << CK_SX.pos();
	}

	void mReadRegs();

	void mWriteReg();
};
#endif
