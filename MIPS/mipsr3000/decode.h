// ### -------------------------------------------------------------- ###
// # files      : decode.h, decode.cpp                                  #
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


#ifndef _DECODE
#define _DECODE

#include <systemc.h>
#include "constants.h"

SC_MODULE(decode)
{
	sc_in<sc_uint<32>> 	IR_RI;			// instruction register
	sc_out<sc_uint<8>> 	OPCOD_SD;		// operation code
	sc_in<sc_uint<25>> 	I_TYPE_SD;		// instruction type

	sc_signal<sc_uint<8>> 	COP0_SD;		// coprocesseur 0 signal
	sc_signal<bool> 		I_ILLG_SD;		// illegal instruction signal
	sc_signal<bool> 		I_JFMT_SD;		// instruction J format
	sc_signal<bool> 		I_IFMT_SD;		// instruction I format
	sc_signal<bool> 		I_RFMT_SD;		// instruction R format
	
	sc_out<bool> 			I_DUSE_SD;		// uses operands signal
	sc_out<bool> 			I_READS_SD;		// instruction uses S operands
	sc_out<bool> 			I_READT_SD;		// instruction uses T operands
	
	sc_signal<bool> 		I_OSGND_SD;		// signed operation
	sc_signal<bool> 		I_WRT31_SD;		// write into r31
	sc_signal<bool> 		I_WRITE_SD;		// write into reg
	
	sc_out<bool> 			I_BRNCH_SD;		// branch instruction
// #
	sc_out<bool> 			SWAP_SD;		//  <========= # deprecated #
// #
	sc_out<sc_uint<5>> 	COP0D_SD;		// coprocessor  destination register number
	sc_out<sc_uint<5>> 	RS_SD;			// source register number
	sc_out<sc_uint<5>> 	RT_SD;			// source register number
	sc_out<sc_uint<5>> 	RD_SD;			// destination register number

	sc_signal<sc_uint<32>> COP0OP_SD;		// coprocessesor 0 source operator
	sc_signal<bool> 		IMDSGN_SD;		// ?
	sc_signal<sc_uint<16>>	IMDSEX_SD;		// ?
	sc_out<sc_uint<32>>	IOPER_SD;		// effective immediate operand
	sc_signal<sc_uint<32>>	OFFSET_SD;		// pc offset

	//branch condition
	sc_signal<sc_uint<5>>	S_CMP_T_SD;		// compare condition
	sc_signal<bool>			S_EQ_T_SD;		// equal condition
	sc_signal<bool>			S_LT_Z_SD;		// less than condition
	sc_signal<bool>			S_LE_Z_SD;		// less or equal condition

	sc_out<bool> 			SLEEP_SD;		// sleep instruction stall
	sc_out<sc_uint<32>>		NEXTPC_SD;		// next PC output
	sc_out<bool> 			ILLGINS_XD;		// unknown instruction
	sc_out<bool> 			C0UNUSE_XD;		// comprocesor 0 unusable

	sc_signal<sc_uint<32>> JMPADR_SD;		// jump @ 
	sc_signal<sc_uint<32>> BRAADR_SD;		// branch @  
	sc_signal<sc_uint<32>> SEQADR_SD;		// seq @ ?

	sc_in<sc_uint<32>> 	BADVADR_RM;		// bad virtual @ register
	sc_in<sc_uint<32>> 	NEXTSR_RX;		// next instruction sts register (sts)?

	// @ of d'instrution to return when jump for example to kernel code
	sc_in<sc_uint<32>> 	EPC_RX;		
		
	sc_in<sc_uint<32>> 	CAUSE_RX;		// cause(exception) register
	sc_in<sc_uint<32>> 	LO_RW;			// low register
	sc_in<sc_uint<32>> 	HI_RW;			// high register
	sc_in<sc_uint<32>> 	SOPER_SD;		// effective s operator
	sc_in<sc_uint<32>> 	TOPER_SD;		// effective t operator
	sc_in<sc_uint<32>> 	SR_RI;			// status register
	sc_in<sc_uint<32>> 	NEXTPC_RD;		// next PC input

	SC_CTOR(decode):
	IR_RI("IR_RI"),
	OPCOD_SD("OPCOD_SD"),
	I_TYPE_SD("I_TYPE_SD"),

	COP0_SD("COP0_SD"),
	I_ILLG_SD("I_ILLG_SD"),
	I_JFMT_SD("I_JFMT_SD"),
	I_IFMT_SD("I_IFMT_SD"),
	I_RFMT_SD("I_RFMT_SD"),

	I_DUSE_SD("I_DUSE_SD"),
	I_READS_SD("I_READS_SD"),
	I_READT_SD("I_READT_SD"),

	I_OSGND_SD("I_OSGND_SD"),
	I_WRT31_SD("I_WRT31_SD"),
	I_WRITE_SD("I_WRITE_SD"),

	I_BRNCH_SD("I_BRNCH_SD"),
	SWAP_SD("SWAP_SD"),
	COP0D_SD("COP0D_SD"),
	RS_SD("RS_SD"),
	RT_SD("RT_SD"),
	RD_SD("RD_SD"),

	COP0OP_SD("COP0OP_SD"),
	IMDSGN_SD("IMDSGN_SD"),
	IMDSEX_SD("IMDSEX_SD"),
	IOPER_SD("IOPER_SD"),
	OFFSET_SD("OFFSET_SD"),
	S_CMP_T_SD("S_CMP_T_SD"),
	S_EQ_T_SD("S_EQ_T_SD"),
	S_LT_Z_SD("S_LT_Z_SD"),
	S_LE_Z_SD("S_LE_Z_SD"),
	
	SLEEP_SD("SLEEP_SD"),
	NEXTPC_SD("NEXTPC_SD"),
	ILLGINS_XD("ILLGINS_XD"),
	C0UNUSE_XD("C0UNUSE_XD"),

	JMPADR_SD("JMPADR_SD"),
	BRAADR_SD("BRAADR_SD"),
	SEQADR_SD("SEQADR_SD"),

	BADVADR_RM("BADVADR_RM"),
	NEXTSR_RX("NEXTSR_RX"),
	EPC_RX("EPC_RX"),
	CAUSE_RX("CAUSE_RX"),
	LO_RW("LO_RW"),
	HI_RW("HI_RW"),
	SOPER_SD("SOPER_SD"),
	TOPER_SD("TOPER_SD"),
	SR_RI("SR_RI"),
	NEXTPC_RD("NEXTPC_RD")
	{
		SC_METHOD(processCOP0_SD);
		sensitive << IR_RI;
		SC_METHOD(processOPCOD_SD);
		sensitive << IR_RI << COP0_SD;
		SC_METHOD(processI_ILLG_SD);
		sensitive << I_TYPE_SD;
		SC_METHOD(processI_JFMT_SD);
		sensitive << I_TYPE_SD;
		SC_METHOD(processI_IFMT_SD);
		sensitive << I_TYPE_SD;
		SC_METHOD(processI_RFMT_SD);
		sensitive << I_TYPE_SD;
		SC_METHOD(processI_DUSE_SD);
		sensitive << I_TYPE_SD;
		SC_METHOD(processI_READS_SD);
		sensitive << I_TYPE_SD;
		SC_METHOD(processI_READT_SD);
		sensitive << I_TYPE_SD;
		SC_METHOD(processI_OSGND_SD);
		sensitive << I_TYPE_SD;
		SC_METHOD(processI_WRT31_SD);
		sensitive << I_TYPE_SD;
		SC_METHOD(processI_WRITE_SD);
		sensitive << I_TYPE_SD;
		SC_METHOD(processI_BRNCH_SD);
		sensitive << I_TYPE_SD;
		SC_METHOD(processSWAP_SD);
		sensitive << OPCOD_SD;
		SC_METHOD(processCOP0D_SD);
		sensitive << COP0D_SD;
		SC_METHOD(processRS_SD);
		sensitive << IR_RI;
		SC_METHOD(processRT_SD);
		sensitive << IR_RI;
		SC_METHOD(processRD_SD);
		sensitive << IR_RI << I_WRT31_SD << I_WRITE_SD << I_RFMT_SD << I_IFMT_SD;
		SC_METHOD(processCOP0OP_SD);
		sensitive << IR_RI << BADVADR_RM << NEXTSR_RX << EPC_RX << CAUSE_RX;
		SC_METHOD(processIMDSGN_SD);
		sensitive << IR_RI;
		SC_METHOD(processIMDSEX_SD);
		sensitive << IR_RI << IMDSGN_SD << I_OSGND_SD;
		SC_METHOD(processIOPER_SD);
		sensitive << IR_RI << OPCOD_SD << SEQADR_SD << LO_RW << HI_RW << IMDSEX_SD;
		SC_METHOD(processOFFSET_SD);
		sensitive << IR_RI << IMDSEX_SD;
		SC_METHOD(processS_CMP_T_SD);
		sensitive << SOPER_SD << TOPER_SD;
		SC_METHOD(processS_EQ_T_SD);
		sensitive << S_CMP_T_SD;
		SC_METHOD(processS_LT_Z_SD);
		sensitive << SOPER_SD;
		SC_METHOD(processS_LE_Z_SD);
		sensitive << SOPER_SD;
		SC_METHOD(processSLEEP_SD);
		sensitive << OPCOD_SD;
		SC_METHOD(processNEXTPC_SD);
		sensitive << SOPER_SD << JMPADR_SD << S_EQ_T_SD << S_LT_Z_SD << S_LE_Z_SD << BRAADR_SD << SEQADR_SD;
		SC_METHOD(processILLGINS_XD);
		sensitive << I_ILLG_SD;
		SC_METHOD(processC0UNUSE_XD);
		sensitive << SR_RI << OPCOD_SD;
		SC_METHOD(processJMPADR_SD);
		sensitive << NEXTPC_RD << IR_RI;
		SC_METHOD(processBRAADR_SD);
		sensitive << NEXTPC_RD << OFFSET_SD;
		SC_METHOD(processSEQADR_SD);
		sensitive << NEXTPC_RD;
	}

  // ### ------------------------------------------------------ ###
  // #   extract the opcode from instruction register		#
  // ### ------------------------------------------------------ ###

	void processCOP0_SD();

  // ### -------------------------------------------------- ###
  // #   decode the instruction's opcode				        #
  // #								                        #
  // #     - instruction format              4 bits (24 - 21)	#
  // #     - source operands usage           4 bits (20 - 17)	#
  // #     - signed/unsigned source operands 1 bit  (     16)	#
  // #     - operation type                  3 bits (15 - 13)	#
  // #     - operation                       2 bits (12 - 11)	#
  // #     - signed/unsigned result          1 bit  (     10)	#
  // #     - overflow detection              1 bit  (      9)	#
  // #     - which stage produces the result 3 bits ( 8 -  6)	#
  // #     - type of instruction (branch)    1 bit  (      5)	#
  // #     - type of memory access           5 bits ( 4 -  0)	#
  // ### -------------------------------------------------- ###

	void processOPCOD_SD();

	void processI_ILLG_SD();

	void processI_JFMT_SD();

	void processI_IFMT_SD();

	void processI_RFMT_SD();

	void processI_DUSE_SD();

	void processI_READS_SD();

	void processI_READT_SD();

	void processI_OSGND_SD();
	void processI_WRT31_SD();
	void processI_WRITE_SD();
	void processI_BRNCH_SD();

  // ### ------------------------------------------------------ ###
  // #   swap instruction						#
  // ### ------------------------------------------------------ ###


	void processSWAP_SD();

  // ### ------------------------------------------------------ ###
  // #   extract special destination registers number from the	#
  // # instruction.						#
  // #								#
  // #   extract source and destination registers number from the	#
  // # instruction. Destination register number is set to r0 when	#
  // # not used.							#
  // ### ------------------------------------------------------ ###

	void processCOP0D_SD();
	void processRS_SD();

	void processRT_SD();
	void processRD_SD();

  // ### ------------------------------------------------------ ###
  // #   read the special register to be transfered into an	#
  // # integer register (mfc0 instruction)			#
  // ### ------------------------------------------------------ ###

	void processCOP0OP_SD();

  // ### ------------------------------------------------------ ###
  // #   compute immediate operand				#
  // ### ------------------------------------------------------ ###

	void processIMDSGN_SD();
	void processIMDSEX_SD();

	void processIOPER_SD();

  // ### ------------------------------------------------------ ###
  // #   compute the offset to be added to the current		#
  // # instruction address :					#
  // #   - short offset for conditional branches			#
  // ### ------------------------------------------------------ ###

	void processOFFSET_SD();

  // ### ------------------------------------------------------ ###
  // #   conditional branches' condition				#
  // ### ------------------------------------------------------ ###

	void processS_CMP_T_SD();

	void processS_EQ_T_SD();

	void processS_LT_Z_SD();

    void processS_LE_Z_SD();

  // ### ------------------------------------------------------ ###
  // #   decoding the sleep instruction. This signal is used to	#
  // # stall the pipeline.					#
  // ### ------------------------------------------------------ ###


      void processSLEEP_SD();

  // ### ------------------------------------------------------ ###
  // #   next instruction's address				#
  // ### ------------------------------------------------------ ###

	void processNEXTPC_SD();
    
  // ### ------------------------------------------------------ ###
  // #   exception detected during the Instruction Decode cycle:	#
  // #								#
  // #     - illegal instruction					#
  // #     - coprocessor zero unusable				#
  // ### ------------------------------------------------------ ###

	void processILLGINS_XD();

	void processC0UNUSE_XD();

	void processJMPADR_SD();

	void processBRAADR_SD();

	void processSEQADR_SD();

};
#endif

