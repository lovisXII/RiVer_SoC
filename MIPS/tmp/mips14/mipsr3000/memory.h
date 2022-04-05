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
//#include "systemc.h"
#include <systemc.h>

SC_MODULE(memory)
{
	sc_in<sc_uint<32>> 	RES_RE;
	sc_in<sc_uint<32>> 	D_IN;
	sc_in<sc_uint<8>> 	OPCOD_RE;
	sc_in<bool> 		D_BERR_N;
	sc_in<sc_uint<32>> 	SR_RE;
	sc_in<bool> 		IASVIOL_RE;
	sc_in<bool> 		IAMALGN_RE;
	sc_in<bool> 		INTRQ_XX;
	sc_in<bool> 		IABUSER_RE;
	sc_in<bool> 		SYSCALL_RE;
	sc_in<bool> 		BREAK_RE;
	sc_in<bool> 		ILLGINS_RE;
	sc_in<bool> 		C0UNUSE_RE;
	sc_in<bool> 		BDSLOT_RE;
	sc_in<sc_uint<6>> 	IT_XX;
	sc_in<sc_uint<32>> 	CAUSE_RX;
	sc_in<sc_uint<5>> 	COP0D_RE;
	sc_in<bool> 		KEEP_SM;
	sc_in<sc_uint<32>> 	PC_RE;
	sc_in<sc_uint<32>> 	REDOPC_RE;
	sc_in<bool> 		OVF_RE;

	sc_in<sc_uint<25>> I_TYPE_RE;
	sc_in<bool> SWAP_RE;
	sc_in<bool> COPYCAP_RE;
	sc_in<sc_uint<5>> RD_RE;

	sc_out<bool> 	I_MPDC_SM;
	sc_out<bool> 	I_WRITE_SM;
	sc_signal<bool> 	I_LOAD_SM;
	sc_signal<bool> 	I_STOR_SM;
	sc_out<bool> 	I_BYTE_SM;
	sc_out<bool> 	I_HALF_SM;
	sc_out<bool> 	I_WORD_SM;
	sc_signal<bool> 	FSTSWAP_SM;
	sc_out<bool> 		DACCESS_SM;
	sc_out<bool> 	WRITE_SM;
	sc_out<bool> 		DLOCK_SM;
	sc_out<bool> 	DATARED_SM;
	sc_out<sc_uint<5>> 	RD_SM;
	sc_out<sc_uint<4>> 	BYTSEL_SM;
	sc_out<sc_uint<32>> 	REDDAT_SM;
	sc_signal<sc_uint<24>> BSEXT_SM;
	sc_signal<sc_uint<16>> HSEXT_SM;
	sc_out<sc_uint<32>> 	DATA_SM;
	sc_signal<bool> 	DABUSER_XM;
	sc_signal<bool> 	LAMALGN_XM;
	sc_signal<bool> 	SAMALGN_XM;
	sc_signal<bool> 	LASVIOL_XM;
	sc_signal<bool> 	SASVIOL_XM;
	sc_signal<bool> 	BADDA_XM;
	sc_signal<bool> 	BADIA_XM;
	sc_out<sc_uint<2>> 		COPERR_XM;
	sc_out<sc_uint<4>> 	EXCCODE_XM;
	sc_out<sc_uint<32>> 	CAUSE_XM;
	sc_out<sc_uint<2>> 	SWINT_XM;
	sc_out<bool> 		WCAUSE_SM;
	sc_out<sc_uint<32>> 	CAUSE_SM;
	sc_signal<sc_uint<32>> RSTORSR_SM;
	sc_out<sc_uint<32>> 	NEXTSR_SM;
	sc_out<bool> 		WSR_SM;
	sc_out<sc_uint<32>> 	EPC_XM;
	sc_out<bool> 		LATEEX_XM;
	sc_out<bool> 		EARLYEX_XM;
	sc_out<bool> 		EXCRQ_XM;
	sc_out<sc_uint<2>> 	SWINTRQ_XM;
	sc_out<bool> 		WEPC_XM;

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
		sensitive 
			<< I_TYPE_RE;
		SC_METHOD(processI_WRITE_SM);
		sensitive 
			<< I_TYPE_RE;
		SC_METHOD(processI_LOAD_SM);
		sensitive 
			<< I_TYPE_RE;
		SC_METHOD(processI_STOR_SM);
		sensitive 
			<< I_TYPE_RE;
		SC_METHOD(processI_BYTE_SM);
		sensitive 
			<< I_TYPE_RE;
		SC_METHOD(processI_HALF_SM);
		sensitive 
			<< I_TYPE_RE;
		SC_METHOD(processI_WORD_SM);
		sensitive 
			<< I_TYPE_RE;
		SC_METHOD(processFSTSWAP_SM);
		sensitive 
			<< SWAP_RE 
			<< COPYCAP_RE;
		SC_METHOD(processDACCESS_SM);
		sensitive 
			<< I_STOR_SM 
			<< I_LOAD_SM;
		SC_METHOD(processWRITE_SM);
		sensitive 
			<< I_STOR_SM 
			<< FSTSWAP_SM;
		SC_METHOD(processDLOCK_SM);
		sensitive 
			<< FSTSWAP_SM;
		SC_METHOD(processDATARED_SM);
		sensitive 
			<< DACCESS_SM 
			<< WRITE_SM;
		SC_METHOD(processRD_SM);
		sensitive 
			<< SWAP_RE 
			<< COPYCAP_RE 
			<< RD_RE;
		SC_METHOD(processBYTSEL_SM);
		sensitive 
			<< RES_RE 
			<< I_BYTE_SM 
			<< I_HALF_SM 
			<< I_WORD_SM;
		SC_METHOD(processREDDAT_SM);
		sensitive 
			<< D_IN 
			<< BYTSEL_SM;
		SC_METHOD(processBSEXT_SM);
		sensitive 
			<< REDDAT_SM 
			<< OPCOD_RE;
		SC_METHOD(processHSEXT_SM);
		sensitive 
			<< REDDAT_SM 
			<< OPCOD_RE;
		SC_METHOD(processDATA_SM);
		sensitive 
			<< REDDAT_SM 
			<< OPCOD_RE 
			<< BSEXT_SM 
			<< HSEXT_SM 
			<< RES_RE;
		SC_METHOD(processDABUSER_XM);
		sensitive 
			<< D_BERR_N;
		SC_METHOD(processLAMALGN_XM);
		sensitive 
			<< RES_RE 
			<< I_WORD_SM 
			<< I_LOAD_SM 
			<< I_HALF_SM 
			<< I_LOAD_SM;
		SC_METHOD(processSAMALGN_XM);
		sensitive 
			<< RES_RE 
			<< I_WORD_SM 
			<< I_STOR_SM 
			<< I_HALF_SM;
		SC_METHOD(processLASVIOL_XM);
		sensitive 
			<< RES_RE 
			<< SR_RE;
		SC_METHOD(processSASVIOL_XM);
		sensitive 
			<< RES_RE 
			<< SR_RE;
		SC_METHOD(processBADDA_XM);
		sensitive 
			<< SASVIOL_XM 
			<< LASVIOL_XM 
			<< LAMALGN_XM 
			<< SAMALGN_XM;
		SC_METHOD(processBADIA_XM);
		sensitive 
			<< IASVIOL_RE 
			<< IAMALGN_RE;
		SC_METHOD(processCOPERR_XM);
		sensitive 
			<< I_TYPE_RE;
		SC_METHOD(processEXCCODE_XM);
		sensitive 
			<< INTRQ_XX 
			<< LAMALGN_XM 
			<< LASVIOL_XM 
			<< IAMALGN_RE
		  
			<< IASVIOL_RE 
			<< SAMALGN_XM 
			<< SASVIOL_XM 
			<< IABUSER_RE 
			<< DABUSER_XM 
			<< SYSCALL_RE
		  
			<< BREAK_RE 
			<< ILLGINS_RE 
			<< C0UNUSE_RE;
		SC_METHOD(processCAUSE_XM);
		sensitive 
			<< BDSLOT_RE 
			<< COPERR_XM 
			<< IT_XX 
			<< CAUSE_RX 
			<< EXCCODE_XM;
		SC_METHOD(processSWINT_XM);
		sensitive 
			<< RES_RE 
			<< CAUSE_RX 
			<< OPCOD_RE 
			<< COP0D_RE 
			<< KEEP_SM;
		SC_METHOD(processWCAUSE_SM);
		sensitive 
			<< OPCOD_RE 
			<< COP0D_RE;
		SC_METHOD(processCAUSE_SM);
		sensitive 
			<< RES_RE 
			<< CAUSE_RX;
		SC_METHOD(processRSTORSR_SM);
		sensitive 
			<< CAUSE_RX;
		SC_METHOD(processNEXTSR_SM);
		sensitive 
			<< OPCOD_RE 
			<< RSTORSR_SM 
			<< RES_RE;
		SC_METHOD(processWSR_SM);
		sensitive 
			<< OPCOD_RE 
			<< COP0D_RE;
	}

	void processI_MPDC_SM()
	{
		sc_uint<25> i_type_re=I_TYPE_RE.read();

		I_MPDC_SM.write(i_type_re[8]);
	}

	void processI_WRITE_SM()
	{
		sc_uint<25> i_type_re=I_TYPE_RE.read();

		I_WRITE_SM.write(i_type_re[8] | i_type_re[7]);
	}

	void processI_LOAD_SM()
	{
		sc_uint<25> i_type_re=I_TYPE_RE.read();

		I_LOAD_SM.write(i_type_re[4]);
	}

	void processI_STOR_SM()
	{
		sc_uint<25> i_type_re=I_TYPE_RE.read();

		I_STOR_SM.write(i_type_re[3]);
	}

	void processI_BYTE_SM()
	{
		sc_uint<25> i_type_re=I_TYPE_RE.read();

		I_BYTE_SM.write(i_type_re[2]);
	}

	void processI_HALF_SM()
	{
		sc_uint<25> i_type_re=I_TYPE_RE.read();

		I_HALF_SM.write(i_type_re[1]);
	}

	void processI_WORD_SM()
	{
		sc_uint<25> i_type_re=I_TYPE_RE.read();

		I_WORD_SM.write(i_type_re[0]);
	}

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

	void processFSTSWAP_SM()
	{
		FSTSWAP_SM.write(SWAP_RE.read() & COPYCAP_RE.read());
	}

	void processDACCESS_SM()
	{
		DACCESS_SM.write(I_STOR_SM.read() | I_LOAD_SM.read()) ;
	}

	void processWRITE_SM()
	{
		WRITE_SM.write(I_STOR_SM.read()  & !FSTSWAP_SM.read());
	}

	void processDLOCK_SM()
	{
		DLOCK_SM.write(FSTSWAP_SM.read());
	}

		// ### ------------------------------------------------------ ###
		// #   a data has been read from the memory			#
		// ### ------------------------------------------------------ ###

	void processDATARED_SM()
	{
		DATARED_SM.write(DACCESS_SM.read() & (!WRITE_SM.read()));
	}
		// ### ------------------------------------------------------ ###
		// #   define the destination register number in the register	#
		// # file							#
		// ### ------------------------------------------------------ ###

	void processRD_SM()
	{
		RD_SM.write(((SWAP_RE.read() & !COPYCAP_RE.read()) == 1) ? (sc_uint<5>)(0x0) : RD_RE.read()) ;
	}

		// ### ------------------------------------------------------ ###
		// #   select bytes that are to be read from or written into	#
		// # memory							#
		// ### ------------------------------------------------------ ###

	void processBYTSEL_SM()
	{
		sc_uint<32> res_re=RES_RE.read();
		sc_uint<5> temp=(I_BYTE_SM.read() 
			<< 4) | (I_HALF_SM.read() 
			<< 3) | (I_WORD_SM.read() 
			<< 2) | res_re.range(1,0);

		if ((int)temp==0x10)
			BYTSEL_SM.write(0x1);
		else if ((int)temp==0x11)
			BYTSEL_SM.write(0x2);
		else if ((int)temp==0x12)
			BYTSEL_SM.write(0x4);
		else if ((int)temp==0x13)
			BYTSEL_SM.write(0x8);
		else if ((int)temp==0x08)
			BYTSEL_SM.write(0x3);
		else if ((int)temp==0x0A)
			BYTSEL_SM.write(0xC);
		else if ((int)temp==0x04)
			BYTSEL_SM.write(0xF);
		else
			BYTSEL_SM.write(0x0);
	}

		// ### ------------------------------------------------------ ###
		// #   align data read from storage				#
		// ### ------------------------------------------------------ ###

	void processREDDAT_SM()
	{
		sc_uint<32> bytsel_sm=BYTSEL_SM.read();
		sc_uint<32> d_in=D_IN.read();

		if ((int)bytsel_sm[0]==1)
			REDDAT_SM.write(d_in.range(31,0));
		else if ((int)bytsel_sm[1]==1)
			REDDAT_SM.write(d_in.range(31,8));
		else if ((int)bytsel_sm[2]==1)
			REDDAT_SM.write(d_in.range(31,16));
		else
			REDDAT_SM.write(d_in.range(31,24));
	}
		// ### ------------------------------------------------------ ###
		// #   extend the sign when loading a byte or a half word in	#
		// # signed mode						#
		// ### ------------------------------------------------------ ###

	void processBSEXT_SM()
	{
		sc_uint<32> reddat_sm=REDDAT_SM.read();

		if ((reddat_sm[7]==1)&&(OPCOD_RE.read()==lb_i))
			BSEXT_SM.write(0xFFFFFF);
		else
			BSEXT_SM.write(0x0);
	}

	void processHSEXT_SM()
	{
		sc_uint<32> reddat_sm=REDDAT_SM.read();

		if ((reddat_sm[15]==1)&&(OPCOD_RE.read()==lh_i))
			HSEXT_SM.write(0xFFFF);
		else
			HSEXT_SM.write(0x0);
	}

	void processDATA_SM()
	{
		sc_uint<32> reddat_sm=REDDAT_SM.read();

		if ((OPCOD_RE.read()==lw_i)||
			(OPCOD_RE.read()==swap_i))
			DATA_SM.write(REDDAT_SM.read());
		else if ((OPCOD_RE.read()==lb_i)||
			(OPCOD_RE.read()==lbu_i))
			DATA_SM.write(BSEXT_SM.read() 
			<< 8 | reddat_sm.range(7,0));
		else if ((OPCOD_RE.read()==lh_i)||
			(OPCOD_RE.read()==lhu_i))
			DATA_SM.write(HSEXT_SM.read() 
			<< 16 | reddat_sm.range(15,0));
		else
			DATA_SM.write(RES_RE.read());
	}

		// ### ----------------------------------------------------// ###
		// #   exceptions detected during the Memory Access cycle:	#
		// #								#
		// #     - data  address bus error				#
		// #     - load  address miss aligned				#
		// #     - store address miss aligned				#
		// #     - load  address violating system space			#
		// #     - store address violating system space			#
		// ### ----------------------------------------------------// ###

	void processDABUSER_XM()
	{
		DABUSER_XM.write(!D_BERR_N.read());
	}

	void processLAMALGN_XM()
	{
		sc_uint<32> res_re=RES_RE.read();

		if ((I_WORD_SM.read()==1)&&(I_LOAD_SM.read()==1))
			LAMALGN_XM.write(res_re[1] | res_re[0]);
		else if ((I_HALF_SM.read()==1)&&(I_LOAD_SM.read()==1))
			LAMALGN_XM.write(res_re[0]);
		else
			LAMALGN_XM.write(0);
	}

	void processSAMALGN_XM()
	{
		sc_uint<32> res_re=RES_RE.read();

		if ((I_WORD_SM.read()==1)&&(I_STOR_SM.read()==1))
			SAMALGN_XM.write(res_re[1] | res_re[0]);
		else if ((I_HALF_SM.read()==1)&&(I_STOR_SM.read()==1))
			SAMALGN_XM.write(res_re[0]);
		else
			SAMALGN_XM.write(0);
	}

	void processLASVIOL_XM()
	{
		sc_uint<32> res_re=RES_RE.read();
		sc_uint<32> sr_re=SR_RE.read();

		if (I_LOAD_SM.read()==1)
			LASVIOL_XM.write(res_re[31] & sr_re[1]);
		else
			LASVIOL_XM.write(0);
	}

	void processSASVIOL_XM()
	{
		sc_uint<32> res_re=RES_RE.read();
		sc_uint<32> sr_re=SR_RE.read();

		if (I_STOR_SM.read()==1)
			SASVIOL_XM.write(res_re[31] & sr_re[1]);
		else
			SASVIOL_XM.write(0);
	}

		// ### ------------------------------------------------------ ###
		// #   exceptions relative to data address (in such a case the	#
		// # data address is saved into the Bad Virtual Address		#
		// # register)							#
		// ### ------------------------------------------------------ ###

	void processBADDA_XM()
	{
		BADDA_XM.write(SASVIOL_XM.read() | LASVIOL_XM.read() | LAMALGN_XM.read() | SAMALGN_XM.read());
	}

		// ### ------------------------------------------------------ ###
		// #   exceptions relative to instruction address (in such a	#
		// # case the instruction address is saved into the Bad Virtual	#
		// # Address register)						#
		// ### ------------------------------------------------------ ###

	void processBADIA_XM()
	{
		BADIA_XM.write(IASVIOL_RE.read() | IAMALGN_RE.read());
	}

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

	void processCOPERR_XM()
	{
		COPERR_XM.write(0x0);
	}

	void processEXCCODE_XM()
	{
		if (INTRQ_XX.read()==1)
			EXCCODE_XM.write(0);
		else if ((LAMALGN_XM.read() | LASVIOL_XM.read() | IAMALGN_RE.read() | IASVIOL_RE.read())==1)
			EXCCODE_XM.write(4);
		else if ((SAMALGN_XM.read() | SASVIOL_XM.read())==1)
			EXCCODE_XM.write(5);
		else if (IABUSER_RE.read()==1)
			EXCCODE_XM.write(6);
		else if (DABUSER_XM.read()==1)
			EXCCODE_XM.write(7);
		else if (SYSCALL_RE.read()==1)
			EXCCODE_XM.write(8);
		else if (BREAK_RE.read()==1)
			EXCCODE_XM.write(9);
		else if (ILLGINS_RE.read()==1)
			EXCCODE_XM.write(0xA);
		else if (C0UNUSE_RE.read()==1)
			EXCCODE_XM.write(0xB);
		else
			EXCCODE_XM.write(0xC);
	}

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

	void processCAUSE_XM()
	{
		sc_uint<32> cause_rx=CAUSE_RX.read();
		sc_uint<32> cause_xm=0x0;

		cause_xm[31]=BDSLOT_RE.read();
		cause_xm.range(29,28)=COPERR_XM.read();
		cause_xm.range(15,10)=IT_XX.read();
		cause_xm.range(9,8)=cause_rx.range(9,8);
		cause_xm.range(5,2)=EXCCODE_XM.read();
		CAUSE_XM.write(cause_xm);
	}

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

	void processSWINT_XM()
	{
		sc_uint<32> res_re=RES_RE.read();
		sc_uint<32> cause_rx=CAUSE_RX.read();

		if (((int)OPCOD_RE.read() == mtc0_i ) && ((int)COP0D_RE.read() == cause_s) && (KEEP_SM.read()== 0))
			SWINT_XM.write(res_re.range(9,8));
		else
			SWINT_XM.write(cause_rx.range(9,8));
	}

		// ### ---------------------------------------------------- ###
		// #   prepare the data to be written into the cause register:	#
		// #								#
		// #     - the execution of a mtc0 instruction updates the	#
		// #       Software Interrupt Pending bits (this case hapens	#
		// #       when software interrupts are masked) (the Hardware	#
		// #       Interrupt Pending is updated at each cycle).		#
		// ### ---------------------------------------------------- ###

	void processWCAUSE_SM()
	{
		if (((int)OPCOD_RE.read() == mtc0_i ) && ((int)COP0D_RE.read() == cause_s))
			WCAUSE_SM.write(1);
		else
			WCAUSE_SM.write(0);
	}

	void processCAUSE_SM()
	{
		sc_uint<32> res_re=RES_RE.read();
		sc_uint<32> cause_rx=CAUSE_RX.read();

		CAUSE_SM.write(cause_rx.range(31,16) 
			<< 16 | IT_XX.read() 
			<< 10 | res_re.range(9,8) 
			<< 8 | cause_rx.range(7,0));
	}

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

	void processRSTORSR_SM()
	{
		sc_uint<32> nextsr_rx=CAUSE_RX.read();

		RSTORSR_SM.write(nextsr_rx.range(31,4) 
			<< 4 | nextsr_rx.range(5,2));
	}

	void processNEXTSR_SM()
	{
		if ((int)OPCOD_RE.read()==rfe_i)
			NEXTSR_SM.write(RSTORSR_SM);
		else
			NEXTSR_SM.write(RES_RE);
	}

	void processWSR_SM()
	{
		if (((int)OPCOD_RE.read()==rfe_i)||(((int)OPCOD_RE.read() == mtc0_i)&&((int)COP0D_RE.read() == status_s)))
			WSR_SM.write(1);
		else
			WSR_SM.write(0);
	}

	void processEPC_XM()
	{
		if (BDSLOT_RE.read()==0)
			EPC_XM.write(PC_RE.read());
		else
			EPC_XM.write(REDOPC_RE.read());
	}

	// ### ----------------------------------------------------// ###
	// #   exception request					#
	// #     - check early exceptions (those who unvalidate the	#
	// #       memory access during the Memory Access cycle)	#
	// #     - check late exceptions (these one has no effect on	#
	// #       the current memory access)				#
	// ### ----------------------------------------------------// ###

	void processLATEEX_XM()
	{
		LATEEX_XM.write(DABUSER_XM.read()) ;
	}

	void processEARLYEX_XM()
	{
		EARLYEX_XM.write(ILLGINS_RE.read() | C0UNUSE_RE.read() |
              IAMALGN_RE.read() | IASVIOL_RE.read() | IABUSER_RE.read() |
              OVF_RE.read()     | BREAK_RE.read()   | SYSCALL_RE.read() |
              LAMALGN_XM.read() | LASVIOL_XM.read() |
              SAMALGN_XM.read() | SASVIOL_XM.read());
	}

	void processEXCRQ_XM()
	{
		EXCRQ_XM.write(EARLYEX_XM.read() | LATEEX_XM.read());
	}

	// ### ----------------------------------------------------// ###
	// #   interrupt request					#
	// #     - check hadrware and software requests and interrupt	#
	// #       mask							#
	// ### ----------------------------------------------------// ###

	void processSWINTRQ_XM()
	{
		sc_uint<32> cause_rx=CAUSE_RX.read();
		SWINTRQ_XM.write(cause_rx.range(9,8));
	}

	void processWEPC_XM()
	{
		WEPC_XM.write(EXCRQ_XM.read());
	}


};
#endif

