// ### -------------------------------------------------------------- ###
// # files      : mux_decode.h, mux_decode.cpp                          #
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


#ifndef _MUX_DECODE
#define _MUX_DECODE

#include <systemc.h>
#include "constants.h"

SC_MODULE(mux_decode)
{
  sc_in<bool> BUBBLE_SD;				// instruction bubble
  sc_in<bool> HOLD_SD;					// hold instruction
  sc_in<bool> KEEP_SD;					// keep data
  sc_in<bool> INTRQ_XX;					// sw interrupt request
  sc_in<bool> EXCRQ_XM;					// exception request
  sc_in<bool> RESET_RX;					// synchronize

  sc_in<sc_uint<32>> IR_RI;			// instruction register
  sc_in<sc_uint<32>> IR_RD;			// instruction register
  sc_out<sc_uint<32>> MUX_IR_SD;		// res instruction register

  sc_in<sc_uint<25>> I_TYPE_SD;		// instruction type
  sc_in<sc_uint<25>> I_TYPE_RD;		// instruction type
  sc_out<sc_uint<25>> MUX_I_TYPE_SD;	// res instruction type

  sc_in<sc_uint<8>> OPCOD_SD;			// operation code
  sc_in<sc_uint<8>> OPCOD_RD;			// operation code 
  sc_out<sc_uint<8>> MUX_OPCOD_SD;		// res operation code

  sc_in<bool> BDSLOT_RI;				// delayed slot from ifetch
  sc_in<bool> BDSLOT_RD;				// delayed slot from decod
  sc_out<bool> MUX_BDSLOT_SD;			// res delayed slot

// ####################### deprecated #########
  sc_in<bool> SWAP_SD;
  sc_in<bool> SWAP_RD;
  sc_out<bool> MUX_SWAP_SD;
// ####################### deprecated #########

  sc_in<sc_uint<5>> RS_SD;				// source register number
  sc_in<sc_uint<5>> RS_RD;				// source register number
  sc_out<sc_uint<5>> MUX_RS_SD;		// res source register number

  sc_in<sc_uint<5>> RT_SD;				// source register number
  sc_in<sc_uint<5>> RT_RD;				// source register number
  sc_out<sc_uint<5>> MUX_RT_SD;		// res source register number

  sc_in<sc_uint<5>> RD_SD;				// destination register number
  sc_in<sc_uint<5>> RD_RD;				// destination register number
  sc_out<sc_uint<5>> MUX_RD_SD;		// res destination register number

  sc_in<sc_uint<5>> COP0D_SD;			// comprocessor 0 destination resgiter number
  sc_in<sc_uint<5>> COP0D_RD;			// comprocessor 0 destination resgiter number
  sc_out<sc_uint<5>> MUX_COP0D_SD;		// res comprocessor 0 destination resgiter number

  sc_in<bool> ILLGINS_XD;				// unknown instruction exception
  sc_in<bool> ILLGINS_RD;				// unknown instruction from register
  sc_out<bool> MUX_ILLGINS_SD;			// res unknown instruction

  sc_in<bool> C0UNUSE_XD;				// comprocessor 0 unusable exception
  sc_in<bool> C0UNUSE_RD;				// comprocessor 0 unusable from register
  sc_out<bool> MUX_C0UNUSE_SD;			// res comprocessor 0 unusable

  sc_in<sc_uint<32>> NEXTPC_XX;		// next instruction @ (hardware)
  sc_in<sc_uint<32>> NEXTPC_SD;		// next instruction @
  sc_in<sc_uint<32>> NEXTPC_RD;		// next instruction @
  sc_out<sc_uint<32>> MUX_NEXTPC_SD;	// res next instruction @

  sc_in<sc_uint<32>> PC_RI;			// instruction @
  sc_in<sc_uint<32>> PC_RD;			// instruction @
  sc_out<sc_uint<32>> MUX_PC_SD;		// res instruction @

  sc_in<sc_uint<32>> SR_RI;			// status register
  sc_in<sc_uint<32>> SR_RD;			// status register
  sc_out<sc_uint<32>> MUX_SR_SD;		// res status register

  sc_in<sc_uint<32>> X_SE;				// X operand
  sc_in<sc_uint<32>> SOPER_SD;			// effective s operand
  sc_out<sc_uint<32>> MUX_SOPER_SD;	// res operand between soper and X

  sc_in<sc_uint<32>> Y_SE;				// Y operand
  sc_in<sc_uint<32>> TOPER_SD;			// effective t operand
  sc_out<sc_uint<32>> MUX_TOPER_SD;	// res operand between toper and Y

  sc_in<sc_uint<32>> IOPER_SD;			// effective immediate operand
  sc_in<sc_uint<32>> IOPER_RD;			// effective immediate operand
  sc_out<sc_uint<32>> MUX_IOPER_SD;	// res effective immediate operand

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
		sensitive 
			<< IR_RI 
			<< IR_RD 
			<< BUBBLE_SD 
			<< HOLD_SD ;
		SC_METHOD(processMUX_I_TYPE_SD);
		sensitive 
			<< I_TYPE_RD 
			<< I_TYPE_SD 
			<< BUBBLE_SD 
			<< HOLD_SD ;
		SC_METHOD(processMUX_OPCOD_SD);
		sensitive 
			<< OPCOD_RD 
			<< OPCOD_SD 
			<< BUBBLE_SD 
			<< HOLD_SD ;
		SC_METHOD(processMUX_BDSLOT_SD);
		sensitive 
			<< BDSLOT_RD 
			<< BDSLOT_RI 
			<< BUBBLE_SD 
			<< HOLD_SD ;
		SC_METHOD(processMUX_SWAP_SD);
		sensitive 
			<< SWAP_RD 
			<< SWAP_SD 
			<< BUBBLE_SD 
			<< HOLD_SD ;
		SC_METHOD(processMUX_RS_SD);
		sensitive 
			<< RS_RD 
			<< RS_SD 
			<< BUBBLE_SD 
			<< HOLD_SD ;
		SC_METHOD(processMUX_RT_SD);
		sensitive 
			<< RT_RD 
			<< RT_SD 
			<< BUBBLE_SD 
			<< HOLD_SD ;
		SC_METHOD(processMUX_RD_SD);
		sensitive 
			<< RD_RD 
			<< RD_SD 
			<< BUBBLE_SD 
			<< HOLD_SD ;
		SC_METHOD(processMUX_COP0D_SD);
		sensitive 
			<< COP0D_RD 
			<< COP0D_SD 
			<< BUBBLE_SD 
			<< HOLD_SD ;
		SC_METHOD(processMUX_ILLGINS_SD);
		sensitive 
			<< ILLGINS_RD 
			<< ILLGINS_XD 
			<< BUBBLE_SD 
			<< HOLD_SD ;
		SC_METHOD(processMUX_NEXTPC_SD);
		sensitive 
			<< INTRQ_XX 
			<< EXCRQ_XM 
			<< RESET_RX 
			<< NEXTPC_XX 
			<< NEXTPC_RD 
			<< NEXTPC_SD 
			<< KEEP_SD ;
		SC_METHOD(processMUX_PC_SD);
		sensitive 
			<< PC_RD 
			<< PC_RI 
			<< HOLD_SD ;
		SC_METHOD(processMUX_SR_SD);
		sensitive 
			<< SR_RD 
			<< SR_RI 
			<< HOLD_SD ;
		SC_METHOD(processMUX_SOPER_SD);
		sensitive 
			<< X_SE 
			<< SOPER_SD 
			<< KEEP_SD ;
		SC_METHOD(processMUX_TOPER_SD);
		sensitive 
			<< Y_SE 
			<< TOPER_SD 
			<< KEEP_SD ;
		SC_METHOD(processMUX_IOPER_SD);
		sensitive 
			<< IOPER_RD 
			<< IOPER_SD 
			<< KEEP_SD ;
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

