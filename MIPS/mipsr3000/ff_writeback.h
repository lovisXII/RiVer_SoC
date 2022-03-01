// ### -------------------------------------------------------------- ###
// # files      : ff_writeback.h, ff_writeback.cpp                      #
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

#ifndef _FF_WRITEBACK
#define _FF_WRITEBACK

#include <systemc.h>
#include "constants.h"

SC_MODULE(ff_writeback)
{
  sc_in<bool> CK_SX;              // internal clock

  sc_in<sc_uint<32> > MUX_LO_SW;  // low register
  sc_out<sc_uint<32> > LO_RW;     // res low register
  sc_in<sc_uint<32> > MUX_HI_SW;  // high register
  sc_out<sc_uint<32> > HI_RW;     // res high register

  SC_CTOR(ff_writeback)
  {
    SC_METHOD(processClock);
    sensitive << CK_SX.pos();
  }

  void processClock();
};
#endif

