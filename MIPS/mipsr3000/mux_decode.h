// ### -------------------------------------------------------------- ###
// # file       : mux_decode.h                                          #
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

#ifndef _MUX_DECODE
#define _MUX_DECODE

#include <systemc.h>
#include "constants.h"

SC_MODULE(mux_decode)
{
  sc_in<bool> BUBBLE_SD;
  sc_in<bool> HOLD_SD;
  sc_in<bool> KEEP_SD;
  sc_in<bool> INTRQ_XX;
  sc_in<bool> EXCRQ_XM;
  sc_in<bool> RESET_RX;

  sc_in<sc_uint<32> > IR_RI;
  sc_in<sc_uint<32> > IR_RD;
  sc_out<sc_uint<32> > MUX_IR_SD;

  sc_in<sc_uint<25> > I_TYPE_SD;
  sc_in<sc_uint<25> > I_TYPE_RD;
  sc_out<sc_uint<25> > MUX_I_TYPE_SD;

  sc_in<sc_uint<8> > OPCOD_SD;
  sc_in<sc_uint<8> > OPCOD_RD;
  sc_out<sc_uint<8> > MUX_OPCOD_SD;

  sc_in<bool> BDSLOT_RI;
  sc_in<bool> BDSLOT_RD;
  sc_out<bool> MUX_BDSLOT_SD;

  sc_in<bool> SWAP_SD;
  sc_in<bool> SWAP_RD;
  sc_out<bool> MUX_SWAP_SD;

  sc_in<sc_uint<5> > RS_SD;
  sc_in<sc_uint<5> > RS_RD;
  sc_out<sc_uint<5> > MUX_RS_SD;

  sc_in<sc_uint<5> > RT_SD;
  sc_in<sc_uint<5> > RT_RD;
  sc_out<sc_uint<5> > MUX_RT_SD;

  sc_in<sc_uint<5> > RD_SD;
  sc_in<sc_uint<5> > RD_RD;
  sc_out<sc_uint<5> > MUX_RD_SD;

  sc_in<sc_uint<5> > COP0D_SD;
  sc_in<sc_uint<5> > COP0D_RD;
  sc_out<sc_uint<5> > MUX_COP0D_SD;

  sc_in<bool> ILLGINS_XD;
  sc_in<bool> ILLGINS_RD;
  sc_out<bool> MUX_ILLGINS_SD;

  sc_in<bool> C0UNUSE_XD;
  sc_in<bool> C0UNUSE_RD;
  sc_out<bool> MUX_C0UNUSE_SD;

  sc_in<sc_uint<32> > NEXTPC_XX;
  sc_in<sc_uint<32> > NEXTPC_SD;
  sc_in<sc_uint<32> > NEXTPC_RD;
  sc_out<sc_uint<32> > MUX_NEXTPC_SD;

  sc_in<sc_uint<32> > PC_RI;
  sc_in<sc_uint<32> > PC_RD;
  sc_out<sc_uint<32> > MUX_PC_SD;

  sc_in<sc_uint<32> > SR_RI;
  sc_in<sc_uint<32> > SR_RD;
  sc_out<sc_uint<32> > MUX_SR_SD;

  sc_in<sc_uint<32> > X_SE;
  sc_in<sc_uint<32> > SOPER_SD;
  sc_out<sc_uint<32> > MUX_SOPER_SD;

  sc_in<sc_uint<32> > Y_SE;
  sc_in<sc_uint<32> > TOPER_SD;
  sc_out<sc_uint<32> > MUX_TOPER_SD;

  sc_in<sc_uint<32> > IOPER_SD;
  sc_in<sc_uint<32> > IOPER_RD;
  sc_out<sc_uint<32> > MUX_IOPER_SD;

  SC_CTOR(mux_decode):
	BUBBLE_SD("BUBBLE_SD"),
	HOLD_SD("HOLD_SD"),
	KEEP_SD("KEEP_SD"),
	INTRQ_XX("INTRQ_XX"),
	EXCRQ_XM("EXCRQ_XM"),
	RESET_RX("RESET_RX"),

	IR_RI("IR_RI"),
	IR_RD("IR_RD"),
	MUX_IR_SD("MUX_IR_SD"),

	I_TYPE_SD("I_TYPE_SD"),
	I_TYPE_RD("I_TYPE_RD"),
	MUX_I_TYPE_SD("MUX_I_TYPE_SD"),

	OPCOD_SD("OPCOD_SD"),
	OPCOD_RD("OPCOD_RD"),
	MUX_OPCOD_SD("MUX_OPCOD_SD"),

	BDSLOT_RI("BDSLOT_RI"),
	BDSLOT_RD("BDSLOT_RD"),
	MUX_BDSLOT_SD("MUX_BDSLOT_SD"),

	SWAP_SD("SWAP_SD"),
	SWAP_RD("SWAP_RD"),
	MUX_SWAP_SD("MUX_SWAP_SD"),

	RS_SD("RS_SD"),
	RS_RD("RS_RD"),
	MUX_RS_SD("MUX_RS_SD"),

	RT_SD("RT_SD"),
	RT_RD("RT_RD"),
	MUX_RT_SD("MUX_RT_SD"),

	RD_SD("RD_SD"),
	RD_RD("RD_RD"),
	MUX_RD_SD("MUX_RD_SD"),

	COP0D_SD("COP0D_SD"),
	COP0D_RD("COP0D_RD"),
	MUX_COP0D_SD("MUX_COP0D_SD"),

	ILLGINS_XD("ILLGINS_XD"),
	ILLGINS_RD("ILLGINS_RD"),
	MUX_ILLGINS_SD("MUX_ILLGINS_SD"),

	C0UNUSE_XD("C0UNUSE_XD"),
	C0UNUSE_RD("C0UNUSE_RD"),
	MUX_C0UNUSE_SD("MUX_C0UNUSE_SD"),

	NEXTPC_XX("NEXTPC_XX"),
	NEXTPC_SD("NEXTPC_SD"),
	NEXTPC_RD("NEXTPC_RD"),
	MUX_NEXTPC_SD("MUX_NEXTPC_SD"),

	PC_RI("PC_RI"),
	PC_RD("PC_RD"),
	MUX_PC_SD("MUX_PC_SD"),

	SR_RI("SR_RI"),
	SR_RD("SR_RD"),
	MUX_SR_SD("MUX_SR_SD"),

	X_SE("X_SE"),
	SOPER_SD("SOPER_SD"),
	MUX_SOPER_SD("MUX_SOPER_SD"),

	Y_SE("Y_SE"),
	TOPER_SD("TOPER_SD"),
	MUX_TOPER_SD("MUX_TOPER_SD"),

	IOPER_SD("IOPER_SD"),
	IOPER_RD("IOPER_RD"),
	MUX_IOPER_SD("MUX_IOPER_SD")

      {
		SC_METHOD(processMUX_IR_SD);
		sensitive << IR_RI << IR_RD << BUBBLE_SD << HOLD_SD ;
		SC_METHOD(processMUX_I_TYPE_SD);
		sensitive << I_TYPE_RD << I_TYPE_SD << BUBBLE_SD << HOLD_SD ;
		SC_METHOD(processMUX_OPCOD_SD);
		sensitive << OPCOD_RD << OPCOD_SD << BUBBLE_SD << HOLD_SD ;
		SC_METHOD(processMUX_BDSLOT_SD);
		sensitive << BDSLOT_RD << BDSLOT_RI << BUBBLE_SD << HOLD_SD ;
		SC_METHOD(processMUX_SWAP_SD);
		sensitive << SWAP_RD << SWAP_SD << BUBBLE_SD << HOLD_SD ;
		SC_METHOD(processMUX_RS_SD);
		sensitive << RS_RD << RS_SD << BUBBLE_SD << HOLD_SD ;
		SC_METHOD(processMUX_RT_SD);
		sensitive << RT_RD << RT_SD << BUBBLE_SD << HOLD_SD ;
		SC_METHOD(processMUX_RD_SD);
		sensitive << RD_RD << RD_SD << BUBBLE_SD << HOLD_SD ;
		SC_METHOD(processMUX_COP0D_SD);
		sensitive << COP0D_RD << COP0D_SD << BUBBLE_SD << HOLD_SD ;
		SC_METHOD(processMUX_ILLGINS_SD);
		sensitive << ILLGINS_RD << ILLGINS_XD << BUBBLE_SD << HOLD_SD ;
		SC_METHOD(processMUX_NEXTPC_SD);
		sensitive << INTRQ_XX << EXCRQ_XM << RESET_RX << NEXTPC_XX << NEXTPC_RD << NEXTPC_SD << KEEP_SD ;
		SC_METHOD(processMUX_PC_SD);
		sensitive << PC_RD << PC_RI << HOLD_SD ;
		SC_METHOD(processMUX_SR_SD);
		sensitive << SR_RD << SR_RI << HOLD_SD ;
		SC_METHOD(processMUX_SOPER_SD);
		sensitive << X_SE << SOPER_SD << KEEP_SD ;
		SC_METHOD(processMUX_TOPER_SD);
		sensitive << Y_SE << TOPER_SD << KEEP_SD ;
		SC_METHOD(processMUX_IOPER_SD);
		sensitive << IOPER_RD << IOPER_SD << KEEP_SD ;
      }

  void processMUX_IR_SD();

  void processMUX_I_TYPE_SD();
  
  void processMUX_OPCOD_SD();
  
  void processMUX_BDSLOT_SD();
  
  void processMUX_SWAP_SD();
  
  void processMUX_RS_SD();
  
  void processMUX_RT_SD();
  
  void processMUX_RD_SD();
  
  void processMUX_COP0D_SD();
  
  void processMUX_ILLGINS_SD();
  
  void processMUX_C0UNUSE_SD();
  
  void processMUX_NEXTPC_SD();
  
  void processMUX_PC_SD();
  
  void processMUX_SR_SD();
  
  void processMUX_SOPER_SD();
  
  void processMUX_TOPER_SD();
  
  void processMUX_IOPER_SD();
  
};
#endif

