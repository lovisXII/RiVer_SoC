// ### -------------------------------------------------------------- ###
// # file       : ff_decode.h                                           #
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

#ifndef _FF_DECODE
#define _FF_DECODE
//#include "systemc.h"
#include "constants.h"
#include <systemc.h>

SC_MODULE(ff_decode)
{
	sc_in<bool> CK_SX;

	sc_in<sc_uint<32>> MUX_IR_SD;
	sc_out<sc_uint<32>> IR_RD;

	sc_in<sc_uint<25>> MUX_I_TYPE_SD;
	sc_out<sc_uint<25>> I_TYPE_RD;

	sc_in<sc_uint<8>> MUX_OPCOD_SD;
	sc_out<sc_uint<8>> OPCOD_RD;

	sc_in<bool> MUX_BDSLOT_SD;
	sc_out<bool> BDSLOT_RD;

	sc_in<bool> MUX_SWAP_SD;
	sc_out<bool> SWAP_RD;

	sc_in<sc_uint<5>> MUX_RS_SD;
	sc_out<sc_uint<5>> RS_RD;

	sc_in<sc_uint<5>> MUX_RT_SD;
	sc_out<sc_uint<5>> RT_RD;

	sc_in<sc_uint<5>> MUX_RD_SD;
	sc_out<sc_uint<5>> RD_RD;

	sc_in<sc_uint<5>> MUX_COP0D_SD;
	sc_out<sc_uint<5>> COP0D_RD;

	sc_in<bool> MUX_ILLGINS_SD;
	sc_out<bool> ILLGINS_RD;

	sc_in<bool> MUX_C0UNUSE_SD;
	sc_out<bool> C0UNUSE_RD;

	sc_in<sc_uint<32>> MUX_NEXTPC_SD;
	sc_out<sc_uint<32>> NEXTPC_RD;

	sc_in<sc_uint<32>> MUX_PC_SD;
	sc_out<sc_uint<32>> PC_RD;

	sc_in<sc_uint<32>> MUX_SR_SD;
	sc_out<sc_uint<32>> SR_RD;

	sc_in<sc_uint<32>> MUX_SOPER_SD;
	sc_out<sc_uint<32>> SOPER_RD;

	sc_in<sc_uint<32>> MUX_TOPER_SD;
	sc_out<sc_uint<32>> TOPER_RD;

	sc_in<sc_uint<32>> MUX_IOPER_SD;
	sc_out<sc_uint<32>> IOPER_RD;

	SC_CTOR(ff_decode):
		CK_SX("CK_SX"),
		MUX_IR_SD("MUX_IR_SD"),
		IR_RD("IR_RD"),
		MUX_I_TYPE_SD("MUX_I_TYPE_SD"),
		I_TYPE_RD("I_TYPE_RD"),
		MUX_OPCOD_SD("MUX_OPCOD_SD"),
		OPCOD_RD("OPCOD_RD"),
		MUX_BDSLOT_SD("MUX_BDSLOT_SD"),
		BDSLOT_RD("BDSLOT_RD"),
		MUX_SWAP_SD("MUX_SWAP_SD"),
		SWAP_RD("SWAP_RD"),
		MUX_RS_SD("MUX_RS_SD"),
		RS_RD("RS_RD"),
		MUX_RT_SD("MUX_RT_SD"),
		RT_RD("RT_RD"),
		MUX_RD_SD("MUX_RD_SD"),
		RD_RD("RD_RD"),
		MUX_COP0D_SD("MUX_COP0D_SD"),
		COP0D_RD("COP0D_RD"),
		MUX_ILLGINS_SD("MUX_ILLGINS_SD"),
		ILLGINS_RD("ILLGINS_RD"),
		MUX_C0UNUSE_SD("MUX_C0UNUSE_SD"),
		C0UNUSE_RD("C0UNUSE_RD"),
		MUX_NEXTPC_SD("MUX_NEXTPC_SD"),
		NEXTPC_RD("NEXTPC_RD"),
		MUX_PC_SD("MUX_PC_SD"),
		PC_RD("PC_RD"),
		MUX_SR_SD("MUX_SR_SD"),
		SR_RD("SR_RD"),
		MUX_SOPER_SD("MUX_SOPER_SD"),
		SOPER_RD("SOPER_RD"),
		MUX_TOPER_SD("MUX_TOPER_SD"),
		TOPER_RD("TOPER_RD"),
		MUX_IOPER_SD("MUX_IOPER_SD"),
		IOPER_RD("IOPER_RD")
	{
		SC_METHOD(processClock);
		sensitive_pos 
			<< CK_SX;
	}

	void processClock()
	{
		IR_RD.write(MUX_IR_SD.read());
		I_TYPE_RD.write(MUX_I_TYPE_SD.read());
		OPCOD_RD.write(MUX_OPCOD_SD.read());
		BDSLOT_RD.write(MUX_BDSLOT_SD.read());
		SWAP_RD.write(MUX_SWAP_SD.read());
		RS_RD.write(MUX_RS_SD.read());
		RT_RD.write(MUX_RT_SD.read());
		RD_RD.write(MUX_RD_SD.read());
		COP0D_RD.write(MUX_COP0D_SD.read());
		ILLGINS_RD.write(MUX_ILLGINS_SD.read());
		C0UNUSE_RD.write(MUX_C0UNUSE_SD.read());
		NEXTPC_RD.write(MUX_NEXTPC_SD.read());
		PC_RD.write(MUX_PC_SD.read());
		SR_RD.write(MUX_SR_SD.read());
		SOPER_RD.write(MUX_SOPER_SD.read());
		TOPER_RD.write(MUX_TOPER_SD.read());
		IOPER_RD.write(MUX_IOPER_SD.read());
	}
};
#endif

