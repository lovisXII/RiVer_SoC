// ### -------------------------------------------------------------- ###
// # files      : decode_rom.h, decode_rom.h                            #
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


#ifndef _DECODE_ROM
#define _DECODE_ROM

#include <systemc.h>
#include "constants.h"

typedef struct
{
	sc_uint<4> instf; 	// Instruction format
	sc_uint<4> sou;		// Source operand usage
	bool suso;			// signed/unsigned source operands
	sc_uint<3> ot;		// operation type
	sc_uint<2> o;		// operation
	bool sur;			// signed/unsigned result
	bool od;			// overflow detection
	sc_uint<3> wspr;	// which stage produces the result
	bool toi;			// type of instruction (branch)
	sc_uint<5> toma;	// type of memory access
} rom_out;

SC_MODULE(decode_rom)
{
	sc_in<sc_uint<8> > OPCOD_SD;
	sc_out<sc_uint<25> > I_TYPE_SD;

	SC_CTOR(decode_rom):
		OPCOD_SD("OPCOD_SD"),
		I_TYPE_SD("I_TYPE_SD")
	{
		SC_METHOD(processROM);
		sensitive << OPCOD_SD;
	}

	void processROM();

};
#endif

