// ### -------------------------------------------------------------- ###
// # files      : ff_execute.h, ff_execute.cpp                          #
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


#ifndef _FF_DECODE
#define _FF_DECODE

#include <systemc.h>
#include "constants.h"

SC_MODULE(ff_decode)
{
  sc_in<bool> CK_SX;                  // internal clock

  sc_in<sc_uint<32>> MUX_IR_SD;      // instruction register
  sc_out<sc_uint<32>> IR_RD;         // res instruction register

  sc_in<sc_uint<25>> MUX_I_TYPE_SD;  // instruction type
  sc_out<sc_uint<25>> I_TYPE_RD;     // res instruction type

  sc_in<sc_uint<8>> MUX_OPCOD_SD;    // operation code
  sc_out<sc_uint<8>> OPCOD_RD;       // res operation code

  sc_in<bool> MUX_BDSLOT_SD;          // branch delayed slot
  sc_out<bool> BDSLOT_RD;             // res branch delayed slot

// ########################## deprecated ##########
  sc_in<bool> MUX_SWAP_SD;      
  sc_out<bool> SWAP_RD;
// ########################## deprecated ##########

  sc_in<sc_uint<5>> MUX_RS_SD;       // source reg number reg.
  sc_out<sc_uint<5>> RS_RD;          // res source reg number reg.

  sc_in<sc_uint<5>> MUX_RT_SD;       // source reg number reg.
  sc_out<sc_uint<5>> RT_RD;          // res source reg number reg.

  sc_in<sc_uint<5>> MUX_RD_SD;       // source reg number reg.
  sc_out<sc_uint<5>> RD_RD;          // res source reg number reg.

  sc_in<sc_uint<5>> MUX_COP0D_SD;    // coprocessor 0 destination reg. number
  sc_out<sc_uint<5>> COP0D_RD;       // res coprocessor 0 destination reg. number

  sc_in<bool> MUX_ILLGINS_SD;         // unknown instruction
  sc_out<bool> ILLGINS_RD;            // res unknown instruction

  sc_in<bool> MUX_C0UNUSE_SD;         // coprocessor 0 unusable
  sc_out<bool> C0UNUSE_RD;            // res coprocessor 0 unusable

  sc_in<sc_uint<32>> MUX_NEXTPC_SD;  // next instruction @
  sc_out<sc_uint<32>> NEXTPC_RD;     // res next instruction @

  sc_in<sc_uint<32>> MUX_PC_SD;      // instruction @
  sc_out<sc_uint<32>> PC_RD;         // res instruction @

  sc_in<sc_uint<32>> MUX_SR_SD;      // status register
  sc_out<sc_uint<32>> SR_RD;         // res status register

  sc_in<sc_uint<32>> MUX_SOPER_SD;   // effective s operand register
  sc_out<sc_uint<32>> SOPER_RD;      // res effective s operand register

  sc_in<sc_uint<32>> MUX_TOPER_SD;   // effective t operand register
  sc_out<sc_uint<32>> TOPER_RD;      // res effective t operand register

  sc_in<sc_uint<32>> MUX_IOPER_SD;   // effective immediate operand
  sc_out<sc_uint<32>> IOPER_RD;      // res effective immediate operand

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
        sensitive 
			<< CK_SX.pos();
      }
  
  void processClock();
};
#endif

