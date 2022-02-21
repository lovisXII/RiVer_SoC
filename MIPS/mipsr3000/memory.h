// ### -------------------------------------------------------------- ###
// # file       : memory.h                                              #
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

#ifndef _MEMORY
#define _MEMORY

#include <systemc.h>
#include "constants.h"

SC_MODULE(memory)
{
	sc_in<sc_uint<32> > 	RES_RE;		// result out of alu
	sc_in<sc_uint<32> > 	D_IN;		// data bus (input)
	sc_in<sc_uint<8> > 	OPCOD_RE;		// operation code
	sc_in<bool> 		D_BERR_N;		// data bus error
	sc_in<sc_uint<32> > 	SR_RE;		// status register
	sc_in<bool> 		IASVIOL_RE;		// instruction @ segmentation violation
	sc_in<bool> 		IAMALGN_RE;		// instruction @ miss alignment
	sc_in<bool> 		INTRQ_XX;		// interrupt request
	sc_in<bool> 		IABUSER_RE;		// instruction @ bus error
	sc_in<bool> 		SYSCALL_RE;		// syscall exception
	sc_in<bool> 		BREAK_RE;		// break exception
	sc_in<bool> 		ILLGINS_RE;		// unknown instruction
	sc_in<bool> 		C0UNUSE_RE;		// coprocessor 0 unusable
	sc_in<bool> 		BDSLOT_RE;		// branch delayed slot
	sc_in<sc_uint<6> > 	IT_XX;			// external interrupts
	sc_in<sc_uint<32> > 	CAUSE_RX;	// cause register
	sc_in<sc_uint<5> > 	COP0D_RE;		// coprocessor 0 destination register number
	sc_in<bool> 		KEEP_SM;		// keep data signal
	sc_in<sc_uint<32> > 	PC_RE;		// instruction @
	sc_in<sc_uint<32> > 	REDOPC_RE;	// old instruction @
	sc_in<bool> 		OVF_RE;			// arithmetique overflow

	sc_in<sc_uint<25> > I_TYPE_RE;		// instruction type

	// ############### deprecated #################
	sc_in<bool> SWAP_RE;
	// ############### deprecated #################

	sc_in<bool> COPYCAP_RE;				// copying capability
	sc_in<sc_uint<5> > RD_RE;			// destination register number

	sc_out<bool> 			I_MPDC_SM;	// mem produces res
	sc_out<bool> 			I_WRITE_SM;	// write into register
	sc_signal<bool> 		I_LOAD_SM;	// read from memory
	sc_signal<bool> 		I_STOR_SM;	// write from memory
	sc_out<bool> 			I_BYTE_SM;	// access type (byte)
	sc_out<bool> 			I_HALF_SM;	// access type (half word)
	sc_out<bool> 			I_WORD_SM;	// access type (word)
	sc_signal<bool> 		FSTSWAP_SM;	// first swap access
	sc_out<bool> 			DACCESS_SM;	// data memory access
	sc_out<bool> 			WRITE_SM;	// write into storage
	sc_out<bool> 			DLOCK_SM;	// lock data access
	sc_out<bool> 			DATARED_SM;	// read access
	sc_out<sc_uint<5> > 	RD_SM;		// destination register
	sc_out<sc_uint<4> > 	BYTSEL_SM;	// byte select for rw
	sc_out<sc_uint<32> > 	REDDAT_SM;	// aligned data
	sc_signal<sc_uint<24> > BSEXT_SM;	// data sign ext byte
	sc_signal<sc_uint<16> > HSEXT_SM;	// data sign ext half word
	sc_out<sc_uint<32> > 	DATA_SM;	// data bus / res
	sc_signal<bool> 		DABUSER_XM;	// data @ bus error
	sc_signal<bool> 		LAMALGN_XM;	// load @ miss alignment
	sc_signal<bool> 		SAMALGN_XM;	// store @ miss alignment
	sc_signal<bool> 		LASVIOL_XM;	// load @ segmentation violation
	sc_signal<bool> 		SASVIOL_XM;	// store @ segmentation violation
	sc_signal<bool> 		BADDA_XM;	// bad data @
	sc_signal<bool> 		BADIA_XM;	// bad instruction @
	sc_out<sc_uint<2> > 	COPERR_XM;	// coprocessor number
	sc_out<sc_uint<4> > 	EXCCODE_XM;	// exception code
	sc_out<sc_uint<32> > 	CAUSE_XM;	// exception cause (exp)
	sc_out<sc_uint<2> > 	SWINT_XM;	// sw interrupt (mtc0)
	sc_out<bool> 			WCAUSE_SM;	// excp cause wen (sw)
	sc_out<sc_uint<32> > 	CAUSE_SM;	// exception cause (sw)
	sc_signal<sc_uint<32> > RSTORSR_SM;	// next instruction sts (rfe)
	sc_out<sc_uint<32> > 	NEXTSR_SM;	// next instruction sts (sw)
	sc_out<bool> 			WSR_SM;		// next st write enable 
	sc_out<sc_uint<32> > 	EPC_XM;		// exc pg counter
	sc_out<bool> 			LATEEX_XM;	// late exceptions
	sc_out<bool> 			EARLYEX_XM;	// early exceptions
	sc_out<bool> 			EXCRQ_XM;	// exception request
	sc_out<sc_uint<2> > 	SWINTRQ_XM;	// sw interrupt request
	sc_out<bool> 			WEPC_XM;	// exc pg cntr write enable

	SC_CTOR(memory):
		I_TYPE_RE("I_TYPE_RE"),
		SWAP_RE("SWAP_RE"),
		COPYCAP_RE("COPYCAP_RE"),
		RD_RE("RD_RE"),
		I_MPDC_SM("I_MPDC_SM"),
		I_WRITE_SM("I_WRITE_SM"),
		I_LOAD_SM("I_LOAD_SM"),
		I_STOR_SM("I_STOR_SM"),
		I_BYTE_SM("I_BYTE_SM"),
		I_HALF_SM("I_HALF_SM"),
		I_WORD_SM("I_WORD_SM"),
		FSTSWAP_SM("FSTSWAP_SM"),
		DACCESS_SM("DACCESS_SM"),
		WRITE_SM("WRITE_SM"),
		DLOCK_SM("DLOCK_SM"),
		DATARED_SM("DATARED_SM"),
		RD_SM("RD_SM"),
		BYTSEL_SM("BYTSEL_SM"),
		REDDAT_SM("REDDAT_SM"),
		BSEXT_SM("BSEXT_SM"),
		HSEXT_SM("HSEXT_SM"),
		DATA_SM("DATA_SM"),
		DABUSER_XM("DABUSER_XM"),
		LAMALGN_XM("LAMALGN_XM"),
		SAMALGN_XM("SAMALGN_XM"),
		LASVIOL_XM("LASVIOL_XM"),
		SASVIOL_XM("SASVIOL_XM"),
		BADDA_XM("BADDA_XM"),
		BADIA_XM("BADIA_XM"),
		COPERR_XM("COPERR_XM"),
		EXCCODE_XM("EXCCODE_XM"),
		CAUSE_XM("CAUSE_XM"),
		SWINT_XM("SWINT_XM"),
		WCAUSE_SM("WCAUSE_SM"),
		CAUSE_SM("CAUSE_SM"),
		RSTORSR_SM("RSTORSR_SM"),
		NEXTSR_SM("NEXTSR_SM"),
		WSR_SM("WSR_SM"),
		EPC_XM("EPC_XM"),
		LATEEX_XM("LATEEX_XM"),
		EARLYEX_XM("EARLYEX_XM"),
		EXCRQ_XM("EXCRQ_XM"),
		SWINTRQ_XM("SWINTRQ_XM"),
		WEPC_XM("WEPC_XM")
	{
		SC_METHOD(processI_MPDC_SM);
		sensitive << I_TYPE_RE;
		SC_METHOD(processI_WRITE_SM);
		sensitive << I_TYPE_RE;
		SC_METHOD(processI_LOAD_SM);
		sensitive << I_TYPE_RE;
		SC_METHOD(processI_STOR_SM);
		sensitive << I_TYPE_RE;
		SC_METHOD(processI_BYTE_SM);
		sensitive << I_TYPE_RE;
		SC_METHOD(processI_HALF_SM);
		sensitive << I_TYPE_RE;
		SC_METHOD(processI_WORD_SM);
		sensitive << I_TYPE_RE;
		SC_METHOD(processFSTSWAP_SM);
		sensitive << SWAP_RE << COPYCAP_RE;
		SC_METHOD(processDACCESS_SM);
		sensitive << I_STOR_SM << I_LOAD_SM;
		SC_METHOD(processWRITE_SM);
		sensitive << I_STOR_SM << FSTSWAP_SM;
		SC_METHOD(processDLOCK_SM);
		sensitive << FSTSWAP_SM;
		SC_METHOD(processDATARED_SM);
		sensitive << DACCESS_SM << WRITE_SM;
		SC_METHOD(processRD_SM);
		sensitive << SWAP_RE << COPYCAP_RE << RD_RE;
		SC_METHOD(processBYTSEL_SM);
		sensitive << RES_RE << I_BYTE_SM << I_HALF_SM << I_WORD_SM;
		SC_METHOD(processREDDAT_SM);
		sensitive << D_IN << BYTSEL_SM;
		SC_METHOD(processBSEXT_SM);
		sensitive << REDDAT_SM << OPCOD_RE;
		SC_METHOD(processHSEXT_SM);
		sensitive << REDDAT_SM << OPCOD_RE;
		SC_METHOD(processDATA_SM);
		sensitive << REDDAT_SM << OPCOD_RE << BSEXT_SM << HSEXT_SM << RES_RE;
		SC_METHOD(processDABUSER_XM);
		sensitive << D_BERR_N;
		SC_METHOD(processLAMALGN_XM);
		sensitive << RES_RE << I_WORD_SM << I_LOAD_SM << I_HALF_SM << I_LOAD_SM;
		SC_METHOD(processSAMALGN_XM);
		sensitive << RES_RE << I_WORD_SM << I_STOR_SM << I_HALF_SM;
		SC_METHOD(processLASVIOL_XM);
		sensitive << RES_RE << SR_RE;
		SC_METHOD(processSASVIOL_XM);
		sensitive << RES_RE << SR_RE;
		SC_METHOD(processBADDA_XM);
		sensitive << SASVIOL_XM << LASVIOL_XM << LAMALGN_XM << SAMALGN_XM;
		SC_METHOD(processBADIA_XM);
		sensitive << IASVIOL_RE << IAMALGN_RE;
		SC_METHOD(processCOPERR_XM);
		sensitive << I_TYPE_RE;
		SC_METHOD(processEXCCODE_XM);
		sensitive << INTRQ_XX << LAMALGN_XM << LASVIOL_XM << IAMALGN_RE
		  << IASVIOL_RE << SAMALGN_XM << SASVIOL_XM << IABUSER_RE << DABUSER_XM << SYSCALL_RE
		  << BREAK_RE << ILLGINS_RE << C0UNUSE_RE;
		SC_METHOD(processCAUSE_XM);
		sensitive << BDSLOT_RE << COPERR_XM << IT_XX << CAUSE_RX << EXCCODE_XM;
		SC_METHOD(processSWINT_XM);
		sensitive << RES_RE << CAUSE_RX << OPCOD_RE << COP0D_RE << KEEP_SM;
		SC_METHOD(processWCAUSE_SM);
		sensitive << OPCOD_RE << COP0D_RE;
		SC_METHOD(processCAUSE_SM);
		sensitive << RES_RE << CAUSE_RX;
		SC_METHOD(processRSTORSR_SM);
		sensitive << CAUSE_RX;
		SC_METHOD(processNEXTSR_SM);
		sensitive << OPCOD_RE << RSTORSR_SM << RES_RE;
		SC_METHOD(processWSR_SM);
		sensitive << OPCOD_RE << COP0D_RE;
	}

	void processI_MPDC_SM();

	void processI_WRITE_SM();

	void processI_LOAD_SM();

	void processI_STOR_SM();

	void processI_BYTE_SM();

	void processI_HALF_SM();

	void processI_WORD_SM();

  // ### ----------------------------------------------------// ###
  // #   data memory access (lock and write)			#
  // #								#
  // #       L  : I_LOAD_SM					#
  // #       S  : I_STOR_SM					#
  // #       F  : FSTSWAP_SM					#
  // #       W  : WRITE_SM					#
  // #       A  : DACCESS_SM					#
  // #       LK : D_LOCK						#
  // #								#
  // #       L   S   F         operation              W   A  LK	#
  // #     +---+---+---+----------------------------+---+---+---+	#
  // #     | 1 | 0 | 0 | load  instruction          | 0 | 1 | 0 |	#
  // #     | 0 | 1 | 0 | store instruction          | 1 | 1 | 0 |	#
  // #     | 1 | 1 | 1 | first  swap access (load)  | 0 | 1 | 1 |	#
  // #     | 1 | 1 | 0 | second swap access (store) | 1 | 1 | 0 |	#
  // #     | 0 | 0 | 0 | no access                  | 0 | 0 | 0 |	#
  // #								#
  // ### ----------------------------------------------------// ###

	void processFSTSWAP_SM();

	void processDACCESS_SM();

	void processWRITE_SM();

	void processDLOCK_SM();

  // ### ------------------------------------------------------ ###
  // #   a data has been read from the memory			#
  // ### ------------------------------------------------------ ###

	void processDATARED_SM();
  // ### ------------------------------------------------------ ###
  // #   define the destination register number in the register	#
  // # file							#
  // ### ------------------------------------------------------ ###

	void processRD_SM();

  // ### ------------------------------------------------------ ###
  // #   select bytes that are to be read from or written into	#
  // # memory							#
  // ### ------------------------------------------------------ ###

	void processBYTSEL_SM();

  // ### ------------------------------------------------------ ###
  // #   align data read from storage				#
  // ### ------------------------------------------------------ ###

	void processREDDAT_SM();
  // ### ------------------------------------------------------ ###
  // #   extend the sign when loading a byte or a half word in	#
  // # signed mode						#
  // ### ------------------------------------------------------ ###

	void processBSEXT_SM();

	void processHSEXT_SM();

	void processDATA_SM();

  // ### ----------------------------------------------------// ###
  // #   exceptions detected during the Memory Access cycle:	#
  // #								#
  // #     - data  address bus error				#
  // #     - load  address miss aligned				#
  // #     - store address miss aligned				#
  // #     - load  address violating system space			#
  // #     - store address violating system space			#
  // ### ----------------------------------------------------// ###

	void processDABUSER_XM();

	void processLAMALGN_XM();

	void processSAMALGN_XM();

	void processLASVIOL_XM();

	void processSASVIOL_XM();

  // ### ------------------------------------------------------ ###
  // #   exceptions relative to data address (in such a case the	#
  // # data address is saved into the Bad Virtual Address		#
  // # register)							#
  // ### ------------------------------------------------------ ###

	void processBADDA_XM();

  // ### ------------------------------------------------------ ###
  // #   exceptions relative to instruction address (in such a	#
  // # case the instruction address is saved into the Bad Virtual	#
  // # Address register)						#
  // ### ------------------------------------------------------ ###

	void processBADIA_XM();

  // ### ------------------------------------------------------ ###
  // #   prepare the data to be written into the cause register:	#
  // #								#
  // #     the cause register is written in the following cases	#
  // #   (decreasing order of priority):				#
  // #								#
  // #     BD   : Branch Delay					#
  // #     CE   : Coprocessor Error				#
  // #     HWIP : Hardware Interrupt Pending			#
  // #     SWIP : Software Interrupt Pending			#
  // #     EXC  : Exception Code					#
  // #								#
  // #                       BD    CE   HWIP   SWIP   EXC		#
  // #   -----------------+-----+-----+------+------+-----+	#
  // #   exception        | New | New | New  | Old  | New |	#
  // #   interrupt        | New | New | New  | New  | New |	#
  // #   mtc0 instruction | Old | Old | New  | New  | Old |	#
  // #   at each cycle    | Old | Old | New  | Old  | Old |	#
  // #								#
  // ### ------------------------------------------------------ ###

	void processCOPERR_XM();

	void processEXCCODE_XM();

  // ### ------------------------------------------------------ ###
  // #   prepare the data to be written into the cause register:	#
  // #								#
  // #     - in case of exception almost all the fileds are	#
  // #       updated. The Branch Delayed Slot, the Coprocessor	#
  // #       Error and the Exception Code are modified to report	#
  // #       the processor's state. The Software Interrupt	#
  // #       Pending remains unchanged (the Hardware Interrupts	#
  // #       Pending is updated at each cycle).			#
  // ### ------------------------------------------------------ ###

	void processCAUSE_XM();

  // ### ------------------------------------------------------ ###
  // #   prepare the data to be written into the cause register:	#
  // #								#
  // #     - in case of interrupt all the fileds are updated.	#
  // #       The Branch Delayed Slot, the Coprocessor Error and	#
  // #       the Exception Code are modified to report the	#
  // #       processor's state. The Software Interrupt Pending	#
  // #       are modified if the instruction in Memory Access	#
  // #       stage is a mtc0 (ststus). (the Hardware Interrupts	#
  // #       Pending is updated at each cycle).			#
  // #								#
  // #       NB: writing the Software Interrupt Pending bits is	#
  // #           conditioned by the executability of the mtc0	#
  // #           instruction (KEEP_SM = '0'). In fact, testing	#
  // #           the executability is unnecessary because there	#
  // #           is no case where this particular instruction is	#
  // #           unexecutable.					#
  // ### ----------------------------------------------------// ###

	void processSWINT_XM();

  // ### ---------------------------------------------------- ###
  // #   prepare the data to be written into the cause register:	#
  // #								#
  // #     - the execution of a mtc0 instruction updates the	#
  // #       Software Interrupt Pending bits (this case hapens	#
  // #       when software interrupts are masked) (the Hardware	#
  // #       Interrupt Pending is updated at each cycle).		#
  // ### ---------------------------------------------------- ###

	void processWCAUSE_SM();

	void processCAUSE_SM();

  // ### ---------------------------------------------------- ###
  // #   prepare the data to be written into the status register:	#
  // #								#
  // #     - rfe instruction:					#
  // #         the old status restored into the register		#
  // #     - mtc0 instruction:					#
  // #         the status register is loaded by the value of an	#
  // #         integer register					#
  // #								#
  // #   write enable into the status register:			#
  // #     - the status register is modified when executing an	#
  // #       rfe or an mtc0 (move into status register).		#
  // ### ---------------------------------------------------- ###

	void processRSTORSR_SM();

	void processNEXTSR_SM();

	void processWSR_SM();

	void processEPC_XM();

  // ### ----------------------------------------------------// ###
  // #   exception request					#
  // #     - check early exceptions (those who unvalidate the	#
  // #       memory access during the Memory Access cycle)	#
  // #     - check late exceptions (these one has no effect on	#
  // #       the current memory access)				#
  // ### ----------------------------------------------------// ###

	void processLATEEX_XM();

	void processEARLYEX_XM();

	void processEXCRQ_XM();

  // ### ----------------------------------------------------// ###
  // #   interrupt request					#
  // #     - check hadrware and software requests and interrupt	#
  // #       mask							#
  // ### ----------------------------------------------------// ###

	void processSWINTRQ_XM();

	void processWEPC_XM();


};
#endif

