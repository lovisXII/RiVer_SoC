#ifndef _I_CACHE
#define _I_CACHE

#include <systemc.h>

SC_MODULE(i_cache)
{
	sc_in<bool>					CK;// external clock
	sc_in<bool>					RESET_N;// reset

	sc_in<sc_uint<32> >			I_A;// address
	sc_in<bool>					I_FTCH;// fetch request
	sc_in<bool>					I_ACK;// fetch acknowledge
	sc_out<sc_uint<32> >		I;// instruction
	sc_out<bool>				I_FRZ;// unable to answer
	sc_out<bool>				I_BERR_N;// bus error

	sc_inout<sc_uint<32> >		PI_A;
	sc_inout<sc_uint<4> >		PI_OPC;
	sc_inout<bool>			PI_READ;
	sc_inout<bool>			PI_LOCK;
	sc_out<bool>			PI_IREQ;
	sc_in<bool>			PI_GNT;
	sc_in<sc_uint<3> >		PI_ACK;
	sc_in<sc_uint<32> >		PI_D;
	sc_in<bool>			PI_TOUT;

	sc_signal<bool>				CK_SX;// internal clk

	sc_signal<sc_biguint<128> >	BLOC0_RX;// data bloc 0
	sc_signal<sc_biguint<128> >	BLOC1_RX;// data bloc 1
	sc_signal<sc_biguint<128> >	BLOC2_RX;// data bloc 2
	sc_signal<sc_biguint<128> >	BLOC3_RX;// data bloc 3
	sc_signal<sc_biguint<128> >	BLOC_SX;// selected bloc

	sc_signal<sc_uint<32> >		DATA_SX;// selected data

	sc_signal<sc_uint<26> >		TAG0_RX;// tag  bloc 0
	sc_signal<sc_uint<26> >		TAG1_RX;// tag  bloc 1
	sc_signal<sc_uint<26> >		TAG2_RX;// tag  bloc 2
	sc_signal<sc_uint<26> >		TAG3_RX;// tag  bloc 3
	sc_signal<sc_uint<26> >		TAG_SX;// tag

	sc_signal<bool>				PRES0_RX;// present bloc 0
	sc_signal<bool>				PRES1_RX;// present bloc 1
	sc_signal<bool>				PRES2_RX;// present bloc 2
	sc_signal<bool>				PRES3_RX;// present bloc 3
	sc_signal<bool>				PRES_SX;// present

	sc_signal<sc_uint<26> >		ADRTAG_SX;// proc. adr's tag
	sc_signal<sc_uint<2> >		BLCNBR_SX;// proc. bloc number
	sc_signal<sc_uint<2> >		BLCOFS_SX;// proc. bloc offset

	sc_signal<sc_uint<26> >		CMPTAGS_SX;// tags comparison
	sc_signal<bool>				EQLTAGS_SX;// tags equal

	sc_signal<sc_uint<2> >		WRTBNBR_SX;// replaced bloc #
	sc_signal<sc_uint<26> >		WRTBTAG_SX;// replaced bloc tag
	sc_signal<bool>				WRTBLOC_SX;// update repl. bloc
	sc_signal<sc_biguint<128> >	BLOCDIN_SX;// value to wrt in blc

	sc_signal<sc_uint<32> >		CMPUADR_SX;// comp. uncach. adr
	sc_signal<bool>				EQLUADR_SX;// equal uncach. adr

	sc_signal<sc_uint<32> >		MSK0ADR_SX;// uncach. masked adr

	sc_signal<bool>			 	UNCMISS_SX;// uncachable miss
	sc_signal<bool>				DIRMISS_SX;// directory  miss
	sc_signal<bool>				MISS_SX;// global     miss

	sc_signal<bool>				RESET_RX;// reset register
	sc_signal<sc_uint<2> >		RSTCNT_SX;// reset counter
	sc_signal<sc_uint<2> >		RSTCNT_RX;// reset counter
	sc_signal<sc_uint<3> >		RSTCRY_SX;// reset counter carry
	sc_signal<bool>				ENDRST_SX;// end of reset phase

	sc_signal<bool>				RSTPRS0_SX;// reset present bit 0
	sc_signal<bool>				RSTPRS1_SX;// reset present bit 1
	sc_signal<bool>				RSTPRS2_SX;// reset present bit 2
	sc_signal<bool>				RSTPRS3_SX;// reset present bit 3

	sc_signal<sc_uint<26> >		SNPBTAG_SX;// bloc tags for snoop
	sc_signal<sc_uint<26> >		SNPCMPT_SX;// comp tags for snoop
	sc_signal<sc_uint<2> >		SNPBLC_SX;// bloc #    for snoop
	sc_signal<bool>				SNPINVL_SX;// invalidate condition

	sc_signal<bool>				INVPRS0_SX;// invalid pres bit 0
	sc_signal<bool>				INVPRS1_SX;// invalid pres bit 1
	sc_signal<bool>				INVPRS2_SX;// invalid pres bit 2
	sc_signal<bool>				INVPRS3_SX;// invalid pres bit 3

	sc_signal<sc_uint<3> >		C_NXTS_SX;// next cache state
	sc_signal<sc_uint<3> >		C_STAT_RX;// cache state reg.

	sc_signal<sc_uint<32> >		UCACADR_RX;// uncach. data adr
	sc_signal<sc_uint<32> >		UCACDAT_RX;// uncach. data
	sc_signal<bool>				UCACVLD_RX;// uncach. data valid
	sc_signal<bool>				WRTUCAC_SX;// uncach. wrt enable

	sc_signal<sc_uint<32> >		RBLOCAD_RX;// read bloc adr
	sc_signal<sc_uint<32> >		REDBLC0_RX;// read bloc word 0
	sc_signal<sc_uint<32> >		REDBLC1_RX;// read bloc word 1
	sc_signal<sc_uint<32> >		REDBLC2_RX;// read bloc word 2
	sc_signal<sc_uint<32> >		REDBLC3_RX;// read bloc word 3

	sc_signal<bool>				WRTADR_SX;// read blc adr   wen
	sc_signal<bool>				WRTDATA_SX;// read blc dat   wen
	sc_signal<bool>				WRTDAT0_SX;// read blc dat 0 wen
	sc_signal<bool>				WRTDAT1_SX;// read blc dat 1 wen
	sc_signal<bool>				WRTDAT2_SX;// read blc dat 2 wen
	sc_signal<bool>				WRTDAT3_SX;// read blc dat 3 wen

	sc_signal<bool>				R_NXTS_SX;// rqst fifo nxt state
	sc_signal<bool>				R_STAT_RX;// rqst fifo state

	sc_signal<sc_uint<39> >		RQSDIN_SX;// request fifo in
	sc_signal<bool>				RQSPSH_SX;// request fifo push
	sc_signal<bool>				RQSPOP_SX;// request fifo pull
	sc_signal<bool>				RQSEMTY_SX;// request fifo empty
	sc_signal<bool>				RQSFULL_SX;// request fifo full
	sc_signal<sc_uint<39> >		RQSREG0_RX;// request fifo reg 0

	sc_signal<bool>				WRTRQS0_SX;// rqst reg 0 wen

	sc_signal<sc_uint<32> >		RQSADR_SX;// request adr
	sc_signal<sc_uint<4> >		RQSACSC_SX;// request access #

	sc_signal<sc_uint<2> >		A_NXTS_SX;// answer fifo nxt st
	sc_signal<sc_uint<2> >		A_STAT_RX;// answer fifo state

	sc_signal<sc_biguint<65> >	ANSDIN_SX;// answer  fifo in
	sc_signal<bool>				ANSPSH_SX;// answer  fifo push
	sc_signal<bool>				ANSPOP_SX;// answer  fifo pull
	sc_signal<bool>				ANSEMTY_SX;// answer  fifo empty
	sc_signal<bool>				ANSFULL_SX;// answer  fifo full
	sc_signal<sc_biguint<65> >	ANSREG0_RX;// answer  fifo reg 0
	sc_signal<sc_biguint<65> >	ANSREG1_RX;// answer  fifo reg 1

	sc_signal<bool>				WRTANS0_SX;// answer reg 0 wen
	sc_signal<bool>				WRTANS1_SX;// answer reg 0 wen

	sc_signal<sc_uint<32> >		ANSDATA_SX;// answer  data
	sc_signal<sc_uint<32> >		ANSADR_SX;// answer  adr
	sc_signal<bool>				ANSFLAG_SX;// answer  flag
	sc_signal<bool>				SELANS0_SX;// select reg 0 input

	sc_signal<bool>				INITACS_SX;// init access cntr

	sc_signal<sc_uint<4> >		ACSCNT_RX;// access  countr
	sc_signal<sc_uint<4> >		ACSCNT_SX;// access  countr
	sc_signal<sc_uint<4> >		ACSCRY_SX;// access  countr cry
	sc_signal<bool>				MOREADR_SX;// more accesses to do

	sc_signal<sc_uint<32> >		ADRCNT_RX;// address countr
	sc_signal<sc_uint<32> >		ADRCNT_SX;// address countr
	sc_signal<sc_uint<32> >		ADRCRY_SX;// address countr cry

	sc_signal<sc_uint<32> >		SENDADR_RX;// address send on Pi

	sc_signal<sc_uint<3> >		P_NXTS_SX;// next pi bus state
	sc_signal<sc_uint<3> >		P_STAT_RX;// pi bus state reg.

	sc_signal<bool>				PILINEN_SX;// Pi line enable
	sc_signal<bool>				DEFLTEN_SX;// default Pi enable

	sc_signal<bool>				DATARDY_SX;// data ready on Pi

	sc_uint<3>			c_redy;
	sc_uint<3>			c_uwat;
	sc_uint<3>			c_mwat;
	sc_uint<3>			c_mwrt;
	sc_uint<3>			c_rset;

	bool 				r_emty;
	bool 				r_full;

	bool 				r_red;
	bool 				r_wrt;

	sc_uint<2>			r_wrd;
	sc_uint<2>			r_hlf;
	sc_uint<2>			r_byt;

	sc_uint<2>			a_emty;
	sc_uint<2>			a_half;
	sc_uint<2>			a_full;

	bool 				a_sft;
	bool 				a_new;

	bool 				a_cnt;
	bool 				a_end;

	sc_uint<3>			p_idle;
	sc_uint<3>			p_rqst;
	sc_uint<3>			p_fadr;
	sc_uint<3>			p_nadr;
	sc_uint<3>			p_ldat;
	sc_uint<3>			p_watr;
	
	sc_uint<3>			p_ack_rdy;
	sc_uint<3>			p_ack_rdm;

	sc_uint<4>			p_opc_nop;
	sc_uint<4>			p_opc_wdu;

	bool 				p_red_red;
	bool 				p_red_wrt;

	sc_uint<32>			u0_adr;
	sc_uint<32>			u0_msk;

 sc_signal<sc_lv<65> > essai;

	SC_CTOR(i_cache)
	{
		c_redy=0x0;
		c_uwat=0x1;
		c_mwat=0x2;
		c_mwrt=0x6;
		c_rset=0x7;

		r_emty=0;
		r_full=1;

		r_red=1;
		r_wrt=0;

		r_wrd=0x0;
		r_hlf=0x1;
		r_byt=0x3;

		a_emty=0x0;
		a_half=0x1;
		a_full=0x3;

		a_sft=1;
		a_new=0;

		a_cnt=0;
		a_end=1;

		p_idle=0x0;
		p_rqst=0x1;
		p_fadr=0x2;
		p_nadr=0x3;
		p_ldat=0x4;
		p_watr=0x5;
	
		p_ack_rdy=0x3;
		p_ack_rdm=0x1;

		p_opc_nop=0x0;
		p_opc_wdu=0x2;

		p_red_red=1;
		p_red_wrt=0;

		u0_adr=0xFFFFFFFF;
		u0_msk=0x0;

		SC_METHOD(processCK_SX);
		sensitive << CK;
		SC_METHOD(processADRTAG_SX);
		sensitive << I_A;
		SC_METHOD(processBLCNBR_SX);
		sensitive << I_A;
		SC_METHOD(processBLCOFS_SX);
		sensitive << I_A;
		SC_METHOD(processTAG_SX);
		sensitive << BLCNBR_SX << TAG0_RX << TAG1_RX << TAG2_RX << TAG3_RX;
		SC_METHOD(processPRES_SX);
		sensitive << BLCNBR_SX << PRES0_RX << PRES1_RX << PRES2_RX << PRES3_RX;
		SC_METHOD(processBLOC_SX);
		sensitive << BLCNBR_SX << BLOC0_RX << BLOC1_RX << BLOC2_RX << BLOC3_RX;
		SC_METHOD(processDATA_SX);
		sensitive << BLCOFS_SX << BLOC_SX;
		SC_METHOD(processCMPTAGS_SX);
		sensitive << ADRTAG_SX << TAG_SX;
		SC_METHOD(processEQLTAGS_SX);
		sensitive << CMPTAGS_SX;
		SC_METHOD(processDIRMISS_SX);
		sensitive << PRES_SX << EQLTAGS_SX << I_FTCH;
		SC_METHOD(processCMPUADR_SX);
		sensitive << I_A << UCACADR_RX;
		SC_METHOD(processEQLUADR_SX);
		sensitive << CMPUADR_SX << UCACVLD_RX;
		SC_METHOD(processMSK0ADR_SX);
		sensitive << I_A;
		SC_METHOD(processUNCMISS_SX);
		sensitive << MSK0ADR_SX << EQLUADR_SX << I_FTCH;
		SC_METHOD(processMISS_SX);
		sensitive << UNCMISS_SX << DIRMISS_SX;
		SC_METHOD(processRSTCNT_SX);
		sensitive << RSTCNT_RX << RSTCRY_SX;
		SC_METHOD(processRSTCRY_SX);
		sensitive << RSTCNT_RX << RSTCRY_SX;
		SC_METHOD(processENDRST_SX);
		sensitive << RSTCRY_SX;
		SC_METHOD(processRSTPRS);
		sensitive << C_STAT_RX << RSTCNT_RX;
		SC_METHOD(processC_NXTS_SX);
		sensitive << RESET_RX << C_STAT_RX << ENDRST_SX << MISS_SX << ANSEMTY_SX << UNCMISS_SX << DIRMISS_SX
			<< ANSFLAG_SX;
		SC_METHOD(processANSPOP_SX);
		sensitive << C_STAT_RX << ANSEMTY_SX;
		SC_METHOD(processWRTUCAC_SX);
		sensitive << C_STAT_RX << ANSEMTY_SX;
		SC_METHOD(processWRTDATA_SX);
		sensitive << C_STAT_RX << ANSEMTY_SX;
		SC_METHOD(processWRTDAT_SX);
		sensitive << ANSADR_SX << WRTDATA_SX;
		SC_METHOD(processWRTADR_SX);
		sensitive << C_STAT_RX << ANSEMTY_SX << ANSFLAG_SX;
		SC_METHOD(processWRTBLOC_SX);
		sensitive << C_STAT_RX;
		SC_METHOD(processWRTBNBR_SX);
		sensitive << RBLOCAD_RX;
		SC_METHOD(processWRTBTAG_SX);
		sensitive << RBLOCAD_RX;
		SC_METHOD(processBLOCDIN_SX);
		sensitive << REDBLC0_RX << REDBLC1_RX << REDBLC2_RX << REDBLC3_RX;
		SC_METHOD(processRQSPSH_SX);
		sensitive << C_STAT_RX << MISS_SX;
		SC_METHOD(processRQSDIN_SX);
		sensitive << I_A << UNCMISS_SX;
		SC_METHOD(processR_NXTS_SX);
		sensitive << RESET_RX << R_STAT_RX << RQSPSH_SX << RQSPOP_SX;
		SC_METHOD(processRQSEMTY_SX);
		sensitive << R_STAT_RX;
		SC_METHOD(processRQSFULL_SX);
		sensitive << R_STAT_RX;
		SC_METHOD(processRQSADR_SX);
		sensitive << RQSREG0_RX;
		SC_METHOD(processRQSACSC_SX);
		sensitive << RQSREG0_RX;
		SC_METHOD(processWRTRQS0_SX);
		sensitive << R_STAT_RX << RQSPSH_SX;
		SC_METHOD(processA_NXTS_SX);
		sensitive << RESET_RX << A_STAT_RX << ANSPSH_SX << ANSPOP_SX;
		SC_METHOD(processANSEMTY_SX);
		sensitive << A_STAT_RX;
		SC_METHOD(processANSFULL_SX);
		sensitive << A_STAT_RX;
		SC_METHOD(processANSDATA_SX);
		sensitive << ANSREG0_RX;
		SC_METHOD(processANSADR_SX);
		sensitive << ANSREG0_RX;
		SC_METHOD(processANSFLAG_SX);
		sensitive << ANSREG0_RX;
		SC_METHOD(processWRTANS0_SX);
		sensitive << A_STAT_RX << ANSPSH_SX << ANSPOP_SX;
		SC_METHOD(processWRTANS1_SX);
		sensitive << A_STAT_RX << ANSPSH_SX;
		SC_METHOD(processSELANS0_SX);
		sensitive << A_STAT_RX << ANSPOP_SX;
		SC_METHOD(processP_NXTS_SX);
		sensitive << RESET_RX << P_STAT_RX << RQSEMTY_SX << DATARDY_SX << PI_GNT << MOREADR_SX;
		SC_METHOD(processDEFLTEN_SX);
		sensitive << PI_GNT;
		SC_METHOD(processPILINEN_SX);
		sensitive << P_STAT_RX << DEFLTEN_SX;
		SC_METHOD(processANSPSH_SX);
		sensitive << P_STAT_RX << DATARDY_SX;
		SC_METHOD(processANSDIN_SX);
		sensitive << PI_D << SENDADR_RX << P_STAT_RX;
		SC_METHOD(processINITACS_SX);
		sensitive << P_STAT_RX << PI_GNT << P_STAT_RX;
		SC_METHOD(processRQSPOP_SX);
		sensitive << INITACS_SX;
		SC_METHOD(processADRCNT_SX);
		sensitive << ADRCNT_RX << ADRCRY_SX;
		SC_METHOD(processADRCRY_SX);
		sensitive << ADRCNT_RX << DATARDY_SX << ANSFULL_SX;
		SC_METHOD(processMOREADR_SX);
		sensitive << ACSCNT_RX;
		SC_METHOD(processDATARDY_SX);
		sensitive << PI_ACK;
		SC_METHOD(processSNPBLC_SX);
		sensitive << PI_A;
		SC_METHOD(processSNPBTAG_SX);
		sensitive << SNPBLC_SX << TAG0_RX << TAG1_RX << TAG2_RX << TAG3_RX;
		SC_METHOD(processSNPCMPT_SX);
		sensitive << PI_A << SNPBTAG_SX;
		SC_METHOD(processSNPINVL_SX);
		sensitive << SNPCMPT_SX << PI_READ;
		SC_METHOD(processINVPRS_SX);
		sensitive << SNPBLC_SX << SNPINVL_SX;
		SC_METHOD(processI_FRZ);
		sensitive << C_STAT_RX << MISS_SX;
		SC_METHOD(processI);
		sensitive << UCACVLD_RX << UCACDAT_RX << DATA_SX;
		SC_METHOD(processPI_IREQ);
		sensitive << P_STAT_RX;
		SC_METHOD(processPI_LINES);
		sensitive << CK_SX << ADRCNT_RX << PILINEN_SX << DEFLTEN_SX << P_STAT_RX << MOREADR_SX << ANSFULL_SX;
		SC_METHOD(processCLOCK);
		sensitive_pos << CK_SX;
		SC_METHOD(processI_BERR_N);
		SC_METHOD(processACSCNT_SX);
		sensitive << ACSCNT_RX << P_STAT_RX << DATARDY_SX << ANSFULL_SX;
	}

	// ### ------------------------------------------------------ ###
	// #   internal clock						#
	// ### ------------------------------------------------------ ###

	void processCK_SX();

	// ### ------------------------------------------------------ ###
	// #   extract from the instruction address:			#
	// #     - the tag						#
	// #     - the bloc number					#
	// #     - the offset inside the bloc				#
	// ### ------------------------------------------------------ ###

	void processADRTAG_SX();

	void processBLCNBR_SX();

	void processBLCOFS_SX();

	// ### ------------------------------------------------------ ###
	// #   read the tag and the present bit from the directory	#
	// ### ------------------------------------------------------ ###

	void processTAG_SX();
    
	void processPRES_SX();

	// ### ------------------------------------------------------ ###
	// #   read the data bloc					#
	// ### ------------------------------------------------------ ###

	void processBLOC_SX();

	// ### ------------------------------------------------------ ###
	// #   extract the data from the bloc				#
	// ### ------------------------------------------------------ ###

	void processDATA_SX();

	// ### ------------------------------------------------------ ###
	// #   compare the bloc's tag and the address's tag and		#
	// # determine if the the required bloc is present in the cache	#
	// ### ------------------------------------------------------ ###

	void processCMPTAGS_SX();

	void processEQLTAGS_SX();

	void processDIRMISS_SX();

	// ### ------------------------------------------------------ ###
	// #   the required data may be an uncachable data recently	#
	// # fetched from the external memory. Compare the address with	#
	// # the address of the last uncachable word read from the	#
	// # external memory and check its validity.			#
	// ### ------------------------------------------------------ ###

	void processCMPUADR_SX();

	void processEQLUADR_SX();

	// ### ------------------------------------------------------ ###
	// #   compare the address with the uncachable addresses:	#
	// #								#
	// #   there is a miss if the address is an uncachable address	#
	// # and if the data is not the last uncachable read data	#
	// ### ------------------------------------------------------ ###

	void processMSK0ADR_SX();

	void processUNCMISS_SX();

	// ### ------------------------------------------------------ ###
	// #   global miss						#
	// ### ------------------------------------------------------ ###

	void processMISS_SX();

	// ### ------------------------------------------------------ ###
	// #   in case of reset, invalidate all the present bits in the	#
	// # directory. A counter is used to generate bloc numbers. One	#
	// # bit is reseted at each cycle. The cache stays in reset	#
	// # state until all the present bits are reseted.		#
	// ### ------------------------------------------------------ ###

	void processRSTCNT_SX();

	void processRSTCRY_SX();

	void processENDRST_SX();

	void processRSTPRS();

	// ### ----------------------------------------------------// ###
	// #   the cache controller finite state machine includes 5	#
	// # states:							#
	// #								#
	// #     - Ready (c_redy): the cache is ready to answer to any	#
	// #         request from the processor. If the data is in the	#
	// #         cache, it is send back to the processor and the	#
	// #         cache remains in ready state.			#
	// #     - Wait Uncachable (c_uwat): the cache goes to this	#
	// #         state when an uncachable data has been requested	#
	// #         by the processor. The cache remains in this state	#
	// #         until the data is fetched from the external memory.#
	// #         Then, the data is transfered into a special	#
	// #         register and is made available for the processor.	#
	// #     - Wait on Directory Miss (c_mwat): the cache goes to	#
	// #         this state if a miss has been generated from the	#
	// #         directory. It remains in this state until the data	#
	// #         are fetched from the memory. Each time a data (a	#
	// #         word) is received from the memory, it is saved as	#
	// #         part of a bloc in a special register.		#
	// #     - Write Missing Bloc (c_mwrt): once a whole bloc has	#
	// #         been fetched, it is written in one of the blocs of	#
	// #         the cache. The tag of the bloc and the present bit	#
	// #         are updated.					#
	// #     - Reset (c_rset): in case of reset the caches goes to	#
	// #         the reset state and remains in this state until	#
	// #         all the present bits have been reseted.		#
	// ### ----------------------------------------------------// ###

	// ### ----------------------------------------------------// ###
	// #   next cache state:					#
	// #								#
	// #     - in case of reset go to the Reset state (c_rset). The	#
	// #       cache remains in the Reset state until all the	#
	// #       present bits are reseted.				#
	// #								#
	// #     - othewise the cache is in Ready state (c_redy) and	#
	// #       answers to instruction fetch requests until a	#
	// #       required data is not found in the cache (directory	#
	// #       miss or uncachable miss). In such a case the cache	#
	// #       goes either to the Wait Uncachable state or to the	#
	// #       Wait on Directory Miss. In the same time, the request#
	// #       context is send to the Request Fifo. In the next	#
	// #       cycle the fifo's empty line goes to false sending	#
	// #       a request to the bus controller state machine.	#
	// #								#
	// #     - in Wait Unchachable (c_uwat) the cache waits for the	#
	// #       end of the transfer on the bus, which makes the data	#
	// #       becoming available in the Answer Fifo (Answer Fifo	#
	// #       not empty). When the non empty signal is received	#
	// #       the uncachable data is extracted from the Answer	#
	// #       Fifo and written into a special register. In the	#
	// #       next cycle, the contoller goes to the Ready state	#
	// #       (c_redy).						#
	// #								#
	// #     - the cache goes to the Wait on Directory Miss (c_mwat)#
	// #       whenever a requested data is not found in the cache.	#
	// #       Each time a data is fetched from the memory and	#
	// #       becomes available in the Answer Fifo (Answer Fifo	#
	// #       not empty), it is extracted from the fifo and	#
	// #       written as a part of a cache bloc in a special	#
	// #       register. Once a complete bloc has been fetched from	#
	// #       the memory, (tha last data has its End flag set in	#
	// #       the Answer Fifo) the cache goes to the Write Missing	#
	// #       Bloc state (c_mwrt).					#
	// #								#
	// #     - in the Write Missing Bloc (c_mwrt) the cache writes	#
	// #       the bloc extracted from the Answer Fifo into the	#
	// #       correct bloc emplacement in the cache. In the same	#
	// #       cycle, the tag and the present bit of the bloc are	#
	// #       updated. Then, the cache returns to the Ready state	#
	// #       (c_redy).						#
	// ### ----------------------------------------------------// ###

	void processC_NXTS_SX();

	// ### ------------------------------------------------------ ###
	// #   extract a data from the Answer Fifo when waiting for an	#
	// # answer and the fifo is not empty				#
	// ### ------------------------------------------------------ ###

	void processANSPOP_SX();
	// ### ------------------------------------------------------ ###
	// #   when an uncachable data has been fetched, extract it	#
	// # from the Answer Fifo and write it in a special register.	#
	// ### ------------------------------------------------------ ###

	void processWRTUCAC_SX();

	// ### ------------------------------------------------------ ###
	// #   when waiting for a missing bloc, each time a word has	#
	// # been read, extract it from the Answer Fifo and write it	#
	// # into a special register					#
	// ### ------------------------------------------------------ ###

	void processWRTDATA_SX();

	void processWRTDAT_SX();

	void processWRTADR_SX();

	// ### ------------------------------------------------------ ###
	// #   when a complete bloc has been fetched, transfer it from	#
	// # a special register into the cache				#
	// ### ------------------------------------------------------ ###

	void processWRTBLOC_SX();

	void processWRTBNBR_SX();

	void processWRTBTAG_SX();

	// ### ------------------------------------------------------ ###
	// #   define the value to be saved into the cache bloc		#
	// ### ------------------------------------------------------ ###

	void processBLOCDIN_SX();

	// ### ------------------------------------------------------ ###
	// #   in case of miss (uncachable or directory miss) push the	#
	// # context on the Request Fifo:				#
	// #   The context contains the address of the missing data,	#
	// # the type of the access (read or write - here always read)	#
	// # the type of the data (word, half-word or byte - here	#
	// # always word) and the number of data requierd (in power of	#
	// # 2).							#
	// ### ------------------------------------------------------ ###

	void processRQSPSH_SX();

	void processRQSDIN_SX();

	// ### ------------------------------------------------------ ###
	// #   the Request Fifo has only one place and its controller	#
	// # has two states: Empty (r_emty) and Full (r_full).		#
	// ### ------------------------------------------------------ ###

	void processR_NXTS_SX();

	// ### ------------------------------------------------------ ###
	// #   define the Request Fifo's flag				#
	// ### ------------------------------------------------------ ###

	void processRQSEMTY_SX();

	void processRQSFULL_SX();

    // ### ------------------------------------------------------ ###
	// #   define the Request Fifo's output				#
	// ### ------------------------------------------------------ ###

	void processRQSADR_SX();

	void processRQSACSC_SX();

	// ### ------------------------------------------------------ ###
	// #   define the Request Fifo's write condition		#
	// ### ------------------------------------------------------ ###

	void processWRTRQS0_SX();

	// ### ------------------------------------------------------ ###
	// #   the Answer Fifo has two places and its controller has	#
	// # 3 states: Empty (a_emty), Half-full (a_half) and Full	#
	// # (a_full).							#
	// ### ------------------------------------------------------ ###

	void processA_NXTS_SX();

	// ### ------------------------------------------------------ ###
	// #   define the Answer Fifo's flag				#
	// ### ------------------------------------------------------ ###

	void processANSEMTY_SX();

	void processANSFULL_SX();
    
	// ### ------------------------------------------------------ ###
	// #   define the Answer Fifo's output				#
	// ### ------------------------------------------------------ ###

	void processANSDATA_SX();

	void processANSADR_SX();

	void processANSFLAG_SX();

	// ### ------------------------------------------------------ ###
	// #   define the Answer Fifo's write condition			#
	// ### ------------------------------------------------------ ###

	void processWRTANS0_SX();

	void processWRTANS1_SX();

	void processSELANS0_SX();

	// ### ----------------------------------------------------// ###
	// #   the Pi-bus interface state machine includes 5 states:	#
	// #								#
	// #     - Idle (p_idle): the master is idle. No transaction is	#
	// #       on going.						#
	// #     - Request (p_rqst): A request is send on the Pi-bus.	#
	// #     - First Address (p_fadr): a first address is send	#
	// #     - Next Addresses (p_nadr): when multiple accesses have	#
	// #       to be performed, for all other addresses but the	#
	// #       first the Pi-bus controller remains in this state.	#
	// #     - Last Data (p_ldat): reading the last data from the	#
	// #       bus.							#
	// ### ----------------------------------------------------// ###

	// ### ----------------------------------------------------// ###
	// #   Pi-bus interface state machine - next state:		#
	// #								#
	// #     - in case of reset go to the Idle state. Remain in the	#
	// #       idle state unitl a request is received from the	#
	// #       Request Fifo (not empty).				#
	// #								#
	// #     - when a request is received go to the Request state	#
	// #       if the default master is not granted. A request is	#
	// #       posted for the Pi-bus. The machine remains in the	#
	// #       request state until the bus is granted for the cache	#
	// #       then, go to the First Address state.			#
	// #								#
	// #     - when a cache request is received, if the default	#
	// #       master is granted, go immediatly to First Address	#
	// #       state. Also, when after a request the bus is	granted,#
	// #       the next state is First Address. The machine remains	#
	// #       only one cycle in this state. The first address is	#
	// #       sent on the bus. Then, the machine goes either to	#
	// #       Next Addresses if other accesses are needed or to	#
	// #       Last Data of only one data is to be transfered	#
	// #								#
	// #     - the machine stays in Next Address state until the	#
	// #       last address is sent. Then, it goes to Last Data.	#
	// #								#
	// #     - in Last Data the last data is transfered on the bus	#
	// #       and the cache stops driving the Pi-bus lines. In the	#
	// #       next cycle the controller returns to the Idle state.	#
	// #								#
	// ### ----------------------------------------------------// ###

	void processP_NXTS_SX();

	// ### ------------------------------------------------------ ###
	// #   Pi-bus lines' output enable :				#
	// #     enable output lines when the bus has been granted or	#
	// #     if the cache is the default master			#
	// ### ------------------------------------------------------ ###

	void processDEFLTEN_SX();

	void processPILINEN_SX();

	// ### ------------------------------------------------------ ###
	// #   when a data has been read on the bus, push it into the	#
	// # Answer Fifo.						#
	// #   define the value pushed into the Answer Fifo		#
	// ### ------------------------------------------------------ ###

	void processANSPSH_SX();

	void processANSDIN_SX();

	// ### ------------------------------------------------------ ###
	// #   initialize the address and the access counters when a	#
	// # request has been accepted. Extract the request from the	#
	// # Request Fifo.						#
	// ### ------------------------------------------------------ ###

	void processINITACS_SX();

	void processRQSPOP_SX();

	// ### ------------------------------------------------------ ###
	// #   address counter :					#
	// #     - the address is incremented only on its 4 least	#
	// #       significant bits since each bloc of the cache	#
	// #       contains 16 bytes					#
	// #     - do not increment the address if the previous access	#
	// #       has not been completed (data not ready or if the	#
	// #       present access cannot be acheived (Answer Fifo is	#
	// #       full)						#
	// ### ------------------------------------------------------ ###

    void processADRCNT_SX();

        void processADRCRY_SX();

        void processACSCNT_SX();

	/*
	void processACSCRY_SX()
	{
		sc_uint<4> acscnt_rx=ACSCNT_RX.read();
		sc_uint<4> acscry_sx=ACSCRY_SX.read();

		ACSCNT_SX.write(~acscnt_rx.range(3,0) ^ acscry_sx.range(3,0));
	}
	*/

	void processMOREADR_SX();

	// ### ------------------------------------------------------ ###
	// #   data transfered						#
	// ### ------------------------------------------------------ ###

	void processDATARDY_SX();

	// ### ------------------------------------------------------ ###
	// #   extract the bloc number from the address transmitted on	#
	// # the Pi_bus for the snoop-invalidate function		#
	// ### ------------------------------------------------------ ###

	void processSNPBLC_SX();

	// ### ------------------------------------------------------ ###
	// #   read the tag of blocs present in the cache for the snoop	#
	// # invalidate function.					#
	// ### ------------------------------------------------------ ###

	void processSNPBTAG_SX();

	// ### ------------------------------------------------------ ###
	// #   if the tag of the address transmitted on the bus matches	#
	// # the tag of the bloc present in the cache and if a write	#
	// # operation is undergoing on the bus, invalidate the present	#
	// # bit of the bloc in the cache.				#
	// #								#
	// #   Note that if a conflict rises between the invalidate	#
	// # and the update of the present bit of a newly fetched bloc	#
	// # from the memory, the invalidate has a greater priority.	#
	// ### ------------------------------------------------------ ###

	void processSNPCMPT_SX();

	void processSNPINVL_SX();

	void processINVPRS_SX();


	// ### ------------------------------------------------------ ###
	// #   assign output:						#
	// #     - processor's interface				#
	// ### ------------------------------------------------------ ###

	void processI_FRZ();

	void processI();

	// ### ------------------------------------------------------ ###
	// #   assign output:						#
	// #     - Pi-bus's interface					#
	// ### ------------------------------------------------------ ###

	void processPI_IREQ();

	void processPI_LINES();


	void processCLOCK();

	void processI_BERR_N();

};
#endif

