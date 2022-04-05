// ### -------------------------------------------------------------- ###
// # file       : regfile.h                                             #
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

#ifndef _REGFILE
#define _REGFILE
//#include "systemc.h"
#include <systemc.h>

SC_MODULE(regfile)
{
	sc_in<sc_uint<5>> RRA;
	sc_in<sc_uint<5>> RRB;
	sc_in<sc_uint<5>> WRC;
	sc_in<sc_uint<32>> WDATA;
	sc_in<bool> CK_SX;
	sc_out<sc_uint<32>> DA;
	sc_out<sc_uint<32>> DB;

	sc_signal<sc_uint<32>> registerFile[32];

	SC_CTOR(regfile)
	{
		SC_METHOD(mReadRegs);
		sensitive 
			<< RRA 
			<< RRB ;
		SC_METHOD(mWriteReg);
		sensitive_pos(CK_SX) ;
	}

	void mReadRegs()
	{
		int numregA=(int)RRA.read();
		int numregB=(int)RRB.read();
		if (numregA!=0)
			DA.write(registerFile[numregA].read());
		else
			DA.write(0);
		if (numregB!=0)
			DB.write(registerFile[numregB].read());
		else
			DB.write(0);
	}

	void mWriteReg()
	{
		int numreg=(int)WRC.read();
		if (numreg!=0)
			registerFile[numreg].write(WDATA.read());
	}
};
#endif
