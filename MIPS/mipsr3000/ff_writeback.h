// ### -------------------------------------------------------------- ###
// # file       : ff_writeback.h                                        #
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

#ifndef _FF_WRITEBACK
#define _FF_WRITEBACK

#include <systemc.h>
#include "constants.h"

SC_MODULE(ff_writeback)
{
  sc_in<bool> CK_SX;

  sc_in<sc_uint<32> > MUX_LO_SW;
  sc_out<sc_uint<32> > LO_RW;
  sc_in<sc_uint<32> > MUX_HI_SW;
  sc_out<sc_uint<32> > HI_RW;

  SC_CTOR(ff_writeback)
  {
    SC_METHOD(processClock);
    sensitive_pos << CK_SX;
  }

  void processClock();
};
#endif

