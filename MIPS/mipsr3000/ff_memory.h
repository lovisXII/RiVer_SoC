// ### -------------------------------------------------------------- ###
// # files      : ff_memory.h, ff_memory.cpp                            #
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

#ifndef _FF_MEMORY
#define _FF_MEMORY

#include <systemc.h>
#include "constants.h"

SC_MODULE(ff_memory)
{
  sc_in<bool> CK_SX;                  // internal clock

  sc_in<sc_uint<32>> MUX_IR_SM;      // instruction register
  sc_out<sc_uint<32>> IR_RM;         // res instruction register

  sc_in<bool> MUX_DATARED_SM;         // read access
  sc_out<bool> DATARED_RM;            // res read access

  sc_in<sc_uint<25>> MUX_I_TYPE_SM;  // instruction type
  sc_out<sc_uint<25>> I_TYPE_RM;     // res instruction type

  sc_in<sc_uint<5>> MUX_RD_SM;       // destination register #
  sc_out<sc_uint<5>> RD_RM;          // res destination register #

  sc_in<sc_uint<8>> MUX_OPCOD_SM;    // operation code
  sc_out<sc_uint<8>> OPCOD_RM;       // res operation code

  sc_in<sc_uint<32>> MUX_DATA_SM;    // data bus input register
  sc_out<sc_uint<32>> DATA_RM;       // res data bus input register

  sc_in<sc_uint<32>> MUX_BADVADR_SM; // bad virtual @ 
  sc_out<sc_uint<32>> BADVADR_RM;    // res bad virtual @ 

  SC_CTOR(ff_memory)
  {
    SC_METHOD(processClock);
    sensitive << CK_SX.pos();
  }

  void processClock();
};
#endif

