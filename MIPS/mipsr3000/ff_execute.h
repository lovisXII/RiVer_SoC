// ### -------------------------------------------------------------- ###
// # file       : ff_execute.h, ff_execute.cpp                          #
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


#ifndef _FF_EXECUTE
#define _FF_EXECUTE

#include <systemc.h>
#include "constants.h"

SC_MODULE(ff_execute)
{
  sc_in<bool> CK_SX;

  sc_in<sc_uint<32>> MUX_IR_SE;      // instruction register
  sc_out<sc_uint<32>> IR_RE;         // res instruction register

  sc_in<sc_uint<25>> MUX_I_TYPE_SE;  // instruction type
  sc_out<sc_uint<25>> I_TYPE_RE;     // res instruction type

  sc_in<sc_uint<8>> MUX_OPCOD_SE;    // operation code
  sc_out<sc_uint<8>> OPCOD_RE;       // res operation code

  sc_in<bool> MUX_BDSLOT_SE;          // branch delayed slot
  sc_out<bool> BDSLOT_RE;             // res branch delayed slot

  sc_in<sc_uint<5>> MUX_RD_SE;       // destination register number
  sc_out<sc_uint<5>> RD_RE;          // res destination register number

  sc_in<sc_uint<5>> MUX_COP0D_SE;    // coprocessor 0 destination register number
  sc_out<sc_uint<5>> COP0D_RE;       // res coprocessor 0 destination register number

  sc_in<bool> MUX_OVF_SE;             // arithmetique overflow
  sc_out<bool> OVF_RE;                // res arithmetique overflow

  sc_in<bool> MUX_IAMALGN_SE;         // instruction @ miss alignement
  sc_out<bool> IAMALGN_RE;            // res instruction @ miss alignement

  sc_in<bool> MUX_IASVIOL_SE;         // instruction @ segmentation violation
  sc_out<bool> IASVIOL_RE;            // res instruction @ segmentation violation

  sc_in<bool> MUX_IABUSER_SE;         // instruction @ bus error
  sc_out<bool> IABUSER_RE;            // res instruction @ bus error

  sc_in<bool> MUX_BREAK_SE;           // break exception
  sc_out<bool> BREAK_RE;              // res break exception

  sc_in<bool> MUX_SYSCALL_SE;         // syscall exception
  sc_out<bool> SYSCALL_RE;            // res syscall exception

  sc_in<bool> MUX_ILLGINS_SE;         // unknown instruction
  sc_out<bool> ILLGINS_RE;            // res unknown instruction

  sc_in<bool> MUX_C0UNUSE_SE;         // coprocessor 0 unusable
  sc_out<bool> C0UNUSE_RE;            // res coprocessor 0 unusable

  // ############################# deprecated ##############
  sc_in<bool> MUX_SWAP_SE;            
  sc_out<bool> SWAP_RE;
  // ############################# deprecated ##############

  sc_in<sc_uint<32>> MUX_PC_SE;      // instruction @
  sc_out<sc_uint<32>> PC_RE;         // res instruction @

  sc_in<sc_uint<32>> MUX_SR_SE;      // status register
  sc_out<sc_uint<32>> SR_RE;         // res status register

  sc_in<sc_uint<32>> MUX_NEXTPC_SE;  // next instruction @
  sc_out<sc_uint<32>> NEXTPC_RE;     // res next instruction @

  sc_in<sc_uint<32>> MUX_RES_SE;     // result out of alu
  sc_out<sc_uint<32>> RES_RE;        // res of the result out of alu

  sc_in<sc_uint<32>> MUX_WDATA_SE;   // data bus output register
  sc_out<sc_uint<32>> WDATA_RE;      // res data bus output register

  sc_in<bool> MUX_COPYCAP_SE;         // copying capability
  sc_out<bool> COPYCAP_RE;            // res copying capability

  sc_in<sc_uint<32>> MUX_REDOPC_SE;  // old instruction @
  sc_out<sc_uint<32>> REDOPC_RE;     // res old instruction @

  SC_CTOR(ff_execute):
    MUX_IR_SE("MUX_IR_SE"),
    IR_RE("IR_RE"),
    MUX_I_TYPE_SE("MUX_I_TYPE_SE"),
    I_TYPE_RE("I_TYPE_RE"),
    MUX_OPCOD_SE("MUX_OPCOD_SE"),
    OPCOD_RE("OPCOD_RE"),
    MUX_BDSLOT_SE("MUX_BDSLOT_SE"),
    BDSLOT_RE("BDSLOT_RE"),
    MUX_RD_SE("MUX_RD_SE"),
    RD_RE("RD_RE"),
    MUX_COP0D_SE("COP0D_SE"),
    COP0D_RE("COP0D_RE"),
    MUX_OVF_SE("MUX_OVF_SE"),
    OVF_RE("OVF_RE"),
    MUX_IAMALGN_SE("MUX_IAMALGN_SE"),
    IAMALGN_RE("IAMALGN_RE"),
    MUX_IASVIOL_SE("MUX_IASVIOL_SE"),
    IASVIOL_RE("IASVIOL_RE"),
    MUX_IABUSER_SE("MUX_IABUSER_SE"),
    IABUSER_RE("IABUSER_RE"),
    MUX_BREAK_SE("MUX_BREAK_SE"),
    BREAK_RE("BREAK_RE"),
    MUX_SYSCALL_SE("MUX_SYSCALL_SE"),
    SYSCALL_RE("SYSCALL_RE"),
    MUX_ILLGINS_SE("MUX_ILLGINS_SE"),
    ILLGINS_RE("ILLGINS_RE"),
    MUX_C0UNUSE_SE("MUX_C0UNUSE_SE"),
    C0UNUSE_RE("C0UNUSE_RE"),
    MUX_SWAP_SE("MUX_SWAP_SE"),
    SWAP_RE("SWAP_RE"),
    MUX_PC_SE("MUX_PC_SE"),
    PC_RE("PC_RE"),
    MUX_SR_SE("SR_SE"),
    SR_RE("SR_RE"),
    MUX_NEXTPC_SE("MUX_NEXTPC_SE"),
    NEXTPC_RE("NEXTPC_RE"),
    MUX_RES_SE("MUX_RES_SE"),
    RES_RE("RES_RE"),
    MUX_WDATA_SE("MUX_WDATA_SE"),
    WDATA_RE("WDATA_RE"),
    MUX_COPYCAP_SE("MUX_COPYCAP_SE"),
    COPYCAP_RE("COPYCAP_RE"),
    MUX_REDOPC_SE("MUX_REDOPC_SE"),
    REDOPC_RE("REDOPC_RE")
      {
		SC_METHOD(processClock);
		sensitive << CK_SX.pos();
      }

  void processClock();
};
#endif

