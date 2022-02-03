// ### -------------------------------------------------------------- ###
// # file       : mux_memory.h                                          #
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

#ifndef _MUX_MEMORY
#define _MUX_MEMORY

#include <systemc.h>
#include "constants.h"

SC_MODULE(mux_memory)
{
	sc_in<bool> BUBBLE_SM;
	sc_in<bool> HOLD_SM;
	sc_in<bool> KEEP_SM;

	sc_in<sc_uint<32> > IR_RE;
	sc_in<sc_uint<32> > IR_RM;
	sc_out<sc_uint<32> > MUX_IR_SM;

	sc_in<bool> DATARED_SM;
	sc_out<bool> MUX_DATARED_SM;

	sc_in<sc_uint<25> > I_TYPE_RM;
	sc_in<sc_uint<25> > I_TYPE_RE;
	sc_out<sc_uint<25> > MUX_I_TYPE_SM;

	sc_in<sc_uint<5> > RD_RM;
	sc_in<sc_uint<5> > RD_SM;
	sc_out<sc_uint<5> > MUX_RD_SM;

	sc_in<sc_uint<8> > OPCOD_RM;
	sc_in<sc_uint<8> > OPCOD_RE;
	sc_out<sc_uint<8> > MUX_OPCOD_SM;

	sc_in<sc_uint<32> > DATA_RM;
	sc_in<sc_uint<32> > DATA_SM;
	sc_out<sc_uint<32> > MUX_DATA_SM;

	sc_in<sc_uint<32> > RES_RE;
	sc_in<sc_uint<32> > NEXTPC_RE;
	sc_in<sc_uint<32> > BADVADR_RM;
	sc_in<bool> BADDA_XM;
	sc_in<bool> BADIA_XM;
	sc_out<sc_uint<32> > MUX_BADVADR_SM;

	SC_CTOR(mux_memory):
	BUBBLE_SM("BUBBLE_SM"),
	HOLD_SM("HOLD_SM"),
	KEEP_SM("KEEP_SM"),

	IR_RE("IR_RE"),
	IR_RM("IR_RM"),
	MUX_IR_SM("MUX_IR_SM"),

	DATARED_SM("DATARED_SM"),
	MUX_DATARED_SM("MUX_DATARED_SM"),

	I_TYPE_RM("I_TYPE_RM"),
	I_TYPE_RE("I_TYPE_RE"),
	MUX_I_TYPE_SM("MUX_I_TYPE_SM"),

	RD_RM("RD_RM"),
	RD_SM("RD_SM"),
	MUX_RD_SM("MUX_RD_SM"),

	OPCOD_RM("OPCOD_RM"),
	OPCOD_RE("OPCOD_RE"),
	MUX_OPCOD_SM("MUX_OPCOD_SM"),

	DATA_RM("DATA_RM"),
	DATA_SM("DATA_SM"),
	MUX_DATA_SM("MUX_DATA_SM"),

	RES_RE("RES_RE"),
	NEXTPC_RE("NEXTPC_RE"),
	BADVADR_RM("BADVADR_RM"),
	BADDA_XM("BADDA_XM"),
	BADIA_XM("BADIA_XM"),
	MUX_BADVADR_SM("MUX_BADVADR_SM")

	{
		SC_METHOD(processMUX_IR_SM);
		sensitive << IR_RM << IR_RE << BUBBLE_SM << HOLD_SM ;
		SC_METHOD(processMUX_DATARED_SM);
		sensitive << DATARED_SM << BUBBLE_SM << HOLD_SM ;
		SC_METHOD(processMUX_I_TYPE_SM);
		sensitive << I_TYPE_RM << I_TYPE_RE << BUBBLE_SM << HOLD_SM ;
		SC_METHOD(processMUX_RD_SM);
		sensitive << RD_RM << RD_SM  << BUBBLE_SM << HOLD_SM ;
		SC_METHOD(processMUX_OPCOD_SM);
		sensitive << OPCOD_RM << OPCOD_RE << BUBBLE_SM << HOLD_SM ;
		SC_METHOD(processMUX_DATA_SM);
		sensitive << DATA_RM << DATA_SM << KEEP_SM ;
		SC_METHOD(processMUX_BADVADR_SM);
		sensitive << RES_RE << NEXTPC_RE << BADVADR_RM << BADDA_XM << BADIA_XM ;
	}

	void processMUX_IR_SM();

	void processMUX_DATARED_SM();

	void processMUX_I_TYPE_SM();

	void processMUX_RD_SM();

	void processMUX_OPCOD_SM();

	void processMUX_DATA_SM();

	void processMUX_BADVADR_SM();

};
#endif

