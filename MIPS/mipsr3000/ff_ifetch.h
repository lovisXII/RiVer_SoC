// ### -------------------------------------------------------------- ###
// # files      : ff_ifetch.h, ff_ifetch.cpp                            #
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


#ifndef _FF_IFETCH
#define _FF_IFETCH


#include <systemc.h>
#include "constants.h"

SC_MODULE(ff_ifetch)
{
  sc_in_clk CK_SX;                  // external clock

  sc_in<sc_uint<32>> MUX_IR_SI;    // IN (mux IR output)
  sc_out<sc_uint<32>> IR_RI;       // OUT (mux IR output)

  sc_in<bool> MUX_INSTRED_SI;       // IN (mux INSTRED output)
  sc_out<bool> INSTRED_RI;          // OUT (mux INSTRED output)

  sc_in<bool> MUX_BDSLOT_SI;        // IN (mux delayed slot output)
  sc_out<bool> BDSLOT_RI;           // OUT (mux delayed slot output)

  sc_in<sc_uint<32>> MUX_PC_SI;    // IN (mux PC output)
  sc_out<sc_uint<32>> PC_RI;       // OUT (mux PC output)

  sc_in<sc_uint<32>> MUX_SR_SI;    // IN (mux status register output)
  sc_out<sc_uint<32>> SR_RI;       // OUT (mux status register output)

  SC_CTOR(ff_ifetch):
    CK_SX("CK_SX"),
    MUX_IR_SI("MUX_IR_SI"),
    IR_RI("IR_RI"),
    MUX_INSTRED_SI("MUX_INSTRED_SI"),
    INSTRED_RI("INSTRED_RI"),
    MUX_BDSLOT_SI("MUX_BDSLOT_SI"),
    BDSLOT_RI("BDSLOT_RI"),
    MUX_PC_SI("MUX_PC_SI"),
    PC_RI("PC_RI"),
    MUX_SR_SI("MUX_SR_SI"),
    SR_RI("SR_RI")
      {
		SC_METHOD(processClock);
		sensitive 
			<< CK_SX;
      }

  void processClock();
};
#endif

