// ### -------------------------------------------------------------- ###
// # file       : misc.h                                                #
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

#ifndef _MISC
#define _MISC

#include <systemc.h>
#include "constants.h"

SC_MODULE(misc)
{
	sc_in<bool> 			CK;
	sc_out<bool> 			CK_SX;
	sc_in<sc_uint<6>> 		IT_N;
	sc_out<sc_uint<6>> 	IT_XX;
	sc_in<sc_uint<32>> 	CAUSE_RX;
	sc_in<bool> 			BDSLOT_RD;
	sc_in<sc_uint<2>> 		COPERR_XM;
	sc_in<sc_uint<2>> 		SWINT_XM;
	sc_in<sc_uint<2>> 		SWINTRQ_XM;
	sc_in<sc_uint<4>> 		EXCCODE_XM;
	sc_in<sc_uint<32>> 	RES_RE;
	sc_in<sc_uint<32>> 	NEXTSR_RX;
	sc_out<sc_uint<32>> 	CAUSE_SX;
	sc_out<sc_uint<32>> 	NEXTSR_XX;
	sc_out<bool> 			WSR_XX;
	sc_in<sc_uint<32>> 	PC_RD;
	sc_in<sc_uint<32>> 	REDOPC_RE;
	sc_out<sc_uint<32>> 	EPC_XX;
	sc_in<bool> 			RESET_RX;
	sc_out<bool> 			WEPC_XX;
	sc_out<bool> 			INTRQ_XX;
	sc_out<sc_uint<32>> 	D_A;
	sc_in<sc_uint<32>> 	NEXTPC_RD;
	sc_out<sc_uint<32>> 	I_A;
	sc_in<sc_uint<32>> 	WDATA_RE;
	sc_out<sc_uint<32>> 	D_OUT;
	sc_in<bool> 			DATARED_RM;
	sc_out<bool> 			D_ACK;
	sc_in<sc_uint<8>> 		OPCOD_RE;
	sc_out<bool> 			I_FTCH;
	sc_in<bool> 			INSTRED_RI;
	sc_out<bool> 			I_ACK;
	sc_in<bool>				I_WORD_SM;
	sc_in<bool>				I_HALF_SM;
	sc_in<bool>				I_BYTE_SM;
	sc_in<bool>				WRITE_SM;
	sc_out<sc_uint<2>> 	D_ATYPE;
	sc_in<bool>				EARLYEX_XM;
	sc_in<bool>				DACCESS_SM;
	sc_out<bool>			D_RQ;
	sc_in<bool>				DLOCK_SM;
	sc_out<bool> 			D_LOCK;
	sc_in<bool> 			EXCRQ_XM;
	sc_out<sc_uint<32>> 	NEXTPC_XX;

	sc_signal<bool> 		BOOTEV_XX;
	sc_signal<sc_uint<6>> 	HWINTRQ_XX;
	sc_signal<bool> 		GLBMSK_XX;
	sc_signal<sc_uint<6>> 	ITMASK_XX;
	sc_signal<sc_uint<32>> 	CAUSE_XX;

	SC_CTOR(misc)
		/*:
		CK("CK"),
		CK_SX("CK_SX"),
		IT_N("IT_N"),
		IT_XX("IT_XX"),
		CAUSE_RX("CAUSE_RX"),
		BDSLOT_RD("BDSLOT_RD"),
		COPERR_XM("COPERR_XM"),
		SWINT_XM("SWINT_XM"),
		EXCCODE_XM("EXCCODE_XM"),
		RES_RE("RES_RE"),
		NEXTSR_RX("NEXTSR_RX"),
		CAUSE_SX("CAUSE_SX"),
		NEXTSR_XX("NEXTSR_XX"),
		WSR_XX("WSR_XX"),
		PC_RD("PC_RD"),
		REDOPC_RE("REDOPC_RE"),
		EPC_XX("EPC_XX"),
		INTRQ_XX("INTRQ_XX"),
		RESET_RX("RESET_RX"),
		WEPC_XX("WEPC_XX"),
		D_A("D_A"),
		NEXTPC_RD("NEXTPC_RD"),
		I_A("I_A"),
		WDATA_RE("WDATA_RE"),
		D_OUT("D_OUT"),
		DATARED_RM("DATARED_RM"),
		D_ACK("D_ACK"),
		OPCOD_RE("OPCOD_RE"),
		I_FTCH("I_FTCH"),
		INSTRED_RI("INSTRED_RI"),
		I_ACK("I_ACK"),
		I_WORD_SM("I_WORD_SM"),
		I_HALF_SM("I_HALF_SM"),
		I_BYTE_SM("I_BYTE_SM"),
		WRITE_SM("WRITE_SM"),
		D_ATYPE("D_ATYPE"),
		EARLYEX_XM("EARLYEX_XM"),
		DACCESS_SM("DACCESS_SM"),
		D_RQ("D_RQ"),
		DLOCK_SM("DLOCK_SM"),
		D_LOCK("D_LOCK"),

		BOOTEV_XX("BOOTEV_XX"),
		HWINTRQ_XX("HWINTRQ_XX"),
		GLBMSK_XX("GLBMSK_XX"),
		ITMASK_XX("ITMASK_XX")
		*/
	{
		SC_METHOD(processCK_SX);
		sensitive 
			<< CK;
		SC_METHOD(processIT_XX);
		sensitive 
			<< IT_N;
		SC_METHOD(processCAUSE_XX);
		sensitive 
			<< CAUSE_RX 
			<< BDSLOT_RD 
			<< COPERR_XM 
			<< IT_XX 
			<< SWINT_XM 
			<< EXCCODE_XM;
		SC_METHOD(processCAUSE_SX);
		sensitive 
			<< RES_RE 
			<< CAUSE_RX 
			<< IT_XX;
		SC_METHOD(processBOOTEV_XX);
		sensitive 
			<< NEXTSR_RX;
		SC_METHOD(processNEXTPC_XX);
		sensitive 
			<< NEXTSR_RX 
			<< BOOTEV_XX;
		SC_METHOD(processNEXTSR_XX);
		sensitive 
			<< NEXTSR_RX 
			<< RESET_RX;
		SC_METHOD(processWSR_XX);
		sensitive 
			<< EXCRQ_XM 
			<< INTRQ_XX 
			<< RESET_RX;
		SC_METHOD(processEPC_XX);
		sensitive 
			<< BDSLOT_RD 
			<< PC_RD 
			<< REDOPC_RE;
		SC_METHOD(processWEPC_XX);
		sensitive 
			<< INTRQ_XX 
			<< RESET_RX;
		SC_METHOD(processHWINTRQ_XX);
		sensitive 
			<< CAUSE_RX;
		SC_METHOD(processGLBMSK_XX);
		sensitive 
			<< NEXTSR_RX;
		SC_METHOD(processITMASK_XX);
		sensitive 
			<< NEXTSR_RX;
		SC_METHOD(processINTRQ_XX);
		sensitive 
			<< ITMASK_XX 
			<< HWINTRQ_XX 
			<< GLBMSK_XX;
		SC_METHOD(processD_A);
		sensitive 
			<< RES_RE;
		SC_METHOD(processI_A);
		sensitive 
			<< NEXTPC_RD;
		SC_METHOD(processD_OUT);
		sensitive 
			<< WDATA_RE;
		SC_METHOD(processD_ACK);
		sensitive 
			<< DATARED_RM;
		SC_METHOD(processI_FTCH);
		sensitive 
			<< OPCOD_RE;
		SC_METHOD(processI_ACK);
		sensitive 
			<< INSTRED_RI;
		SC_METHOD(processD_ATYPE);
		sensitive 
			<< I_WORD_SM 
			<< WRITE_SM 
			<< I_HALF_SM 
			<< I_BYTE_SM;
		SC_METHOD(processD_RQ);
		sensitive 
			<< EARLYEX_XM 
			<< RESET_RX 
			<< DACCESS_SM;
		SC_METHOD(processD_LOCK);
		sensitive 
			<< DLOCK_SM;
	}

	void processCK_SX();

	void processIT_XX();

	void processCAUSE_XX();

  // ### ---------------------------------------------------- ###
  // #   prepare the data to be written into the cause register:	#
  // #								#
  // #     - at each cycle, the Hardware Interrupt Pending is	#
  // #       saved						#
  // ### ---------------------------------------------------- ###

	void processCAUSE_SX();

  // ### ----------------------------------------------------// ###
  // #   compute the next instruction address:			#
  // #								#
  // #    - in case of reset					#
  // #    - in case of interrupt or exception during the bootstrap#
  // #    - in case of interrupt or exception			#
  // ### ----------------------------------------------------// ###

	void processBOOTEV_XX();

	void processNEXTPC_XX();

  // ### ----------------------------------------------------// ###
  // #   prepare the data to be written into the status register:	#
  // #								#
  // #     - reset:						#
  // #         initialize the status register (set the Bootstrap	#
  // #         Exception Vector, reset the global Interrupt Mask	#
  // #         and put the processor in kernel mode		#
  // #     - exception and interrupt:				#
  // #         the status register is saved.			#
  // ### ----------------------------------------------------// ###

	void processNEXTSR_XX();

	void processWSR_XX();

  // ### ----------------------------------------------------// ###
  // #   prepare the data to be written into the Exception	#
  // # Program Counter register:					#
  // #								#
  // #     - interrupt:						#
  // #         the address of the first unexecuted instruction is	#
  // #         saved unless the first unexecuted instruction is	#
  // #         in the delayed slot of a branch instruction in	#
  // #         which case the address of the branch instruction	#
  // #         is saved.						#
  // #     - exception:						#
  // #         the address of the faulty instruction is saved	#
  // #         unless the faulty instruction is in the delayed	#
  // #         slot of a branch instruction in which case the	#
  // #         address of the branch instruction is saved.	#
  // ### ----------------------------------------------------// ###

	void processEPC_XX();
    
  // ### ----------------------------------------------------// ###
  // #   Exception Program Counter register is modified in case	#
  // # of reset, interrupts or exceptions				#
  // ### ----------------------------------------------------// ###

	void processWEPC_XX();

	void processHWINTRQ_XX();

	void processGLBMSK_XX();

	void processITMASK_XX();

	void processINTRQ_XX();

  // ### ------------------------------------------------------ ###
  // #   assign outputs (data and instruction address):		#
  // ### ------------------------------------------------------ ###

	void processD_A();
	void processI_A();

  // ### ------------------------------------------------------ ###
  // #   assign outputs (data output buffers)			#
  // ### ------------------------------------------------------ ###

	void processD_OUT();

  // ### ------------------------------------------------------ ###
  // #   assign outputs (data read acknowledge)			#
  // #								#
  // #     - the acknowledge signals to the cache that a data	#
  // #       has been read from the memory has been saved into	#
  // #       the data register					#
  // ### ------------------------------------------------------ ###

	void processD_ACK();

  // ### ------------------------------------------------------ ###
  // #   assign outputs (instruction memory access):		#
  // #								#
  // #     - disable the memory access for the instruction fetch	#
  // #       when the instruction in Execute stage is a Sleep.	#
  // #       This is to avoid the discharge of the memory lines	#
  // #       in the instruction cache. Since I_FTCH is a critical	#
  // #       signal, it does not depend on the instruction in	#
  // #       Instruction Decode stage. The decode phase is used	#
  // #       to detect the Sleep instruction and will disable the	#
  // #       memory access in the next cycle.			#
  // ### ------------------------------------------------------ ###

	void processI_FTCH();

  // ### ------------------------------------------------------ ###
  // #   assign outputs (instruction fetch acknowledge):		#
  // #								#
  // #     - the acknowledge signals to the cache that the new	#
  // #       instruction fetched from the memory has been saved	#
  // #       into the instruction register			#
  // ### ------------------------------------------------------ ###

	void processI_ACK();

  // ### ------------------------------------------------------ ###
  // #   assign outputs (data memory access):			#
  // ### ------------------------------------------------------ ###

	void processD_ATYPE();

  // ### ------------------------------------------------------ ###
  // #   assign outputs (data memory access):			#
  // #     - disable the data memory access in case of early	#
  // #       execption or when a reset has occured.		#
  // ### ------------------------------------------------------ ###

	void processD_RQ();

  // ### ------------------------------------------------------ ###
  // #   assign outputs (miscellaneous):				#
  // ### ------------------------------------------------------ ###

	void processD_LOCK();


};
#endif

