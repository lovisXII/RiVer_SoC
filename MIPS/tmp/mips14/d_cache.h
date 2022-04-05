#ifndef _D_CACHE
#define _D_CACHE

SC_MODULE(d_cache)
{
	sc_in<bool>					CK;// external clock
	sc_in<bool>					RESET_N;// reset

	sc_in<sc_uint<32>>			D_A;// address
	sc_in<bool>					D_RQ;// data request
	sc_in<bool>					D_LOCK;
	sc_in<sc_uint<2>>				D_ATYPE;
	sc_in<bool>					D_ACK;
	sc_out<bool>				D_BERR_N;// bus error
	sc_out<bool>				D_FRZ;// unable to answer
	sc_out<sc_uint<32>>		D_OUT;
	sc_in<sc_uint<32>>		D_IN;

	sc_inout<sc_uint<32>>		PI_A;
	sc_inout<sc_uint<4>>		PI_OPC;
	sc_inout<bool>			PI_READ;
	sc_inout<bool>			PI_LOCK;
	sc_out<bool>			PI_IREQ;
	sc_in<bool>			PI_GNT;
	sc_in<sc_uint<3>>		PI_ACK;
	sc_inout<sc_uint<32>>		PI_D;
	sc_in<bool>			PI_TOUT;

	sc_signal<bool>				CK_SX;// internal clk

	sc_signal<sc_biguint<128>>	BLOC0_RX;// data bloc 0
	sc_signal<sc_biguint<128>>	BLOC1_RX;// data bloc 1
	sc_signal<sc_biguint<128>>	BLOC2_RX;// data bloc 2
	sc_signal<sc_biguint<128>>	BLOC3_RX;// data bloc 3
	sc_signal<sc_biguint<128>>	BLOC_SX;// selected bloc

	sc_signal<sc_uint<32>>		DATA_SX;// selected data

	sc_signal<sc_uint<26>>		TAG0_RX;// tag  bloc 0
	sc_signal<sc_uint<26>>		TAG1_RX;// tag  bloc 1
	sc_signal<sc_uint<26>>		TAG2_RX;// tag  bloc 2
	sc_signal<sc_uint<26>>		TAG3_RX;// tag  bloc 3
	sc_signal<sc_uint<26>>		TAG_SX;// tag

	sc_signal<bool>				PRES0_RX;// present bloc 0
	sc_signal<bool>				PRES1_RX;// present bloc 1
	sc_signal<bool>				PRES2_RX;// present bloc 2
	sc_signal<bool>				PRES3_RX;// present bloc 3
	sc_signal<bool>				PRES_SX;// present

	sc_signal<sc_uint<26>>		ADRTAG_SX;// proc. adr's tag
	sc_signal<sc_uint<2>>		BLCNBR_SX;// proc. bloc number
	sc_signal<sc_uint<2>>		BLCOFS_SX;// proc. bloc offset

	sc_signal<sc_uint<26>>		CMPTAGS_SX;// tags comparison
	sc_signal<bool>				EQLTAGS_SX;// tags equal

	sc_signal<sc_uint<2>>		WRTBNBR_SX;// replaced bloc #
	sc_signal<sc_uint<26>>		WRTBTAG_SX;// replaced bloc tag
	sc_signal<bool>				WRTBLOC_SX;// update repl. bloc
	sc_signal<sc_biguint<128>>	BLOCDIN_SX;// value to wrt in blc

	sc_signal<sc_uint<32>>		CMPUADR_SX;// comp. uncach. adr
	sc_signal<bool>				EQLUADR_SX;// equal uncach. adr

	sc_signal<sc_uint<32>>		MSK0ADR_SX;// uncach. masked adr

	sc_signal<bool>			 	UNCMISS_SX;// uncachable miss
	sc_signal<bool>				DIRMISS_SX;// directory  miss
	sc_signal<bool>				MISS_SX;// global     miss

	sc_signal<bool>				RESET_RX;// reset register
	sc_signal<sc_uint<2>>		RSTCNT_SX;// reset counter
	sc_signal<sc_uint<2>>		RSTCNT_RX;// reset counter
	sc_signal<sc_uint<3>>		RSTCRY_SX;// reset counter carry
	sc_signal<bool>				ENDRST_SX;// end of reset phase

	sc_signal<bool>				RSTPRS0_SX;// reset present bit 0
	sc_signal<bool>				RSTPRS1_SX;// reset present bit 1
	sc_signal<bool>				RSTPRS2_SX;// reset present bit 2
	sc_signal<bool>				RSTPRS3_SX;// reset present bit 3

	sc_signal<sc_uint<26>>		SNPBTAG_SX;// bloc tags for snoop
	sc_signal<sc_uint<26>>		SNPCMPT_SX;// comp tags for snoop
	sc_signal<sc_uint<2>>		SNPBLC_SX;// bloc #    for snoop
	sc_signal<bool>				SNPINVL_SX;// invalidate condition

	sc_signal<bool>				INVPRS0_SX;// invalid pres bit 0
	sc_signal<bool>				INVPRS1_SX;// invalid pres bit 1
	sc_signal<bool>				INVPRS2_SX;// invalid pres bit 2
	sc_signal<bool>				INVPRS3_SX;// invalid pres bit 3

	sc_signal<sc_uint<3>>		C_NXTS_SX;// next cache state
	sc_signal<sc_uint<3>>		C_STAT_RX;// cache state reg.

	sc_signal<sc_uint<32>>		UCACADR_RX;// uncach. data adr
	sc_signal<sc_uint<32>>		UCACDAT_RX;// uncach. data
	sc_signal<bool>				UCACVLD_RX;// uncach. data valid
	sc_signal<bool>				WRTUCAC_SX;// uncach. wrt enable

	sc_signal<sc_uint<32>>		RBLOCAD_RX;// read bloc adr
	sc_signal<sc_uint<32>>		REDBLC0_RX;// read bloc word 0
	sc_signal<sc_uint<32>>		REDBLC1_RX;// read bloc word 1
	sc_signal<sc_uint<32>>		REDBLC2_RX;// read bloc word 2
	sc_signal<sc_uint<32>>		REDBLC3_RX;// read bloc word 3

	sc_signal<bool>				WRTADR_SX;// read blc adr   wen
	sc_signal<bool>				WRTDATA_SX;// read blc dat   wen
	sc_signal<bool>				WRTDAT0_SX;// read blc dat 0 wen
	sc_signal<bool>				WRTDAT1_SX;// read blc dat 1 wen
	sc_signal<bool>				WRTDAT2_SX;// read blc dat 2 wen
	sc_signal<bool>				WRTDAT3_SX;// read blc dat 3 wen

	sc_signal<bool>				R_NXTS_SX;// rqst fifo nxt state
	sc_signal<bool>				R_STAT_RX;// rqst fifo state

	sc_signal<sc_biguint<72>>		RQSDIN_SX;// request fifo in
	sc_signal<bool>				RQSPSH_SX;// request fifo push
	sc_signal<bool>				RQSPOP_SX;// request fifo pull
	sc_signal<bool>				RQSEMTY_SX;// request fifo empty
	sc_signal<bool>				RQSFULL_SX;// request fifo full
	sc_signal<sc_biguint<72>>		RQSREG0_RX;// request fifo reg 0

	sc_signal<bool>				WRTRQS0_SX;// rqst reg 0 wen

	sc_signal<sc_uint<32>>		RQSADR_SX;// request adr
	sc_signal<sc_uint<32>>		RQSDAT_SX;// request adr
	sc_signal<bool>			RQSTYP_SX;// request adr
	sc_signal<sc_uint<2>>		RQSSIZ_SX;// request adr
	sc_signal<bool>			RQSMOD_SX;// request adr
	sc_signal<sc_uint<4>>		RQSACSC_SX;// request access #

	sc_signal<sc_uint<2>>		A_NXTS_SX;// answer fifo nxt st
	sc_signal<sc_uint<2>>		A_STAT_RX;// answer fifo state

	sc_signal<sc_biguint<65>>	ANSDIN_SX;// answer  fifo in
	sc_signal<bool>				ANSPSH_SX;// answer  fifo push
	sc_signal<bool>				ANSPOP_SX;// answer  fifo pull
	sc_signal<bool>				ANSEMTY_SX;// answer  fifo empty
	sc_signal<bool>				ANSFULL_SX;// answer  fifo full
	sc_signal<sc_biguint<65>>	ANSREG0_RX;// answer  fifo reg 0
	sc_signal<sc_biguint<65>>	ANSREG1_RX;// answer  fifo reg 1

	sc_signal<bool>				WRTANS0_SX;// answer reg 0 wen
	sc_signal<bool>				WRTANS1_SX;// answer reg 0 wen

	sc_signal<sc_uint<32>>		ANSDATA_SX;// answer  data
	sc_signal<sc_uint<32>>		ANSADR_SX;// answer  adr
	sc_signal<bool>				ANSFLAG_SX;// answer  flag
	sc_signal<bool>				SELANS0_SX;// select reg 0 input

	sc_signal<bool>				INITACS_SX;// init access cntr

	sc_signal<sc_uint<4>>		ACSCNT_RX;// access  countr
	sc_signal<sc_uint<4>>		ACSCNT_SX;// access  countr
	sc_signal<sc_uint<4>>		ACSCRY_SX;// access  countr cry
	sc_signal<bool>				MOREADR_SX;// more accesses to do

	sc_signal<sc_uint<32>>		ADRCNT_SX;// address countr
	sc_signal<sc_uint<32>>		ADRCRY_SX;// address countr cry

	sc_signal<sc_uint<32>>		SENDADR_RX;// address send on Pi

	sc_signal<sc_uint<3>>		P_NXTS_SX;// next pi bus state
	sc_signal<sc_uint<3>>		P_STAT_RX;// pi bus state reg.

	sc_signal<bool>				PILINEN_SX;// Pi line enable
	sc_signal<bool>				DEFLTEN_SX;// default Pi enable

	sc_signal<bool>				DATARDY_SX;// data ready on Pi

	sc_signal<bool>			WRITEOP_SX;
	sc_signal<bool>			WRTPEND_SX;
	sc_signal<bool>			REDPEND_SX;
	sc_signal<bool>			EFWRTOP_SX;
	sc_signal<bool>			WRTINVL_SX;
	sc_signal<bool>			ACSMOD_RX;
	sc_signal<bool>			ACSTYP_RX;
	sc_signal<sc_uint<32>>		ACSADR_RX;
	sc_signal<sc_uint<2>>		ACSSIZ_RX;
	sc_signal<sc_uint<4>>		OPCODE_SX;
	sc_signal<sc_uint<32>>		PI_ADR_RX;
	sc_signal<bool>			PI_READ_RX;
	sc_signal<bool>			DATREAD_SX;
	sc_signal<bool>			PIDATEN_SX;
	sc_signal<sc_uint<32>>		ACSDAT_RX;
	sc_signal<sc_uint<4>>		PIOPC_SX;

	sc_signal<bool>			WRIPRS0_SX;
	sc_signal<bool>			WRIPRS1_SX;
	sc_signal<bool>			WRIPRS2_SX;
	sc_signal<bool>			WRIPRS3_SX;

	sc_signal<bool>			SNIPRS0_SX;
	sc_signal<bool>			SNIPRS1_SX;
	sc_signal<bool>			SNIPRS2_SX;
	sc_signal<bool>			SNIPRS3_SX;

        sc_uint<32>                     u0_adr;
        sc_uint<32>                     u0_msk;

        sc_uint<3>                      c_redy;
        sc_uint<3>                      c_full;
        sc_uint<3>                      c_uwat;
        sc_uint<3>                      c_mwat;
        sc_uint<3>                      c_mwrt;
        sc_uint<3>                      c_rset;

        bool                            r_emty;
        bool                            r_full;

        bool                            r_red;
        bool                            r_wrt;

        sc_uint<2>                      r_wrd;
        sc_uint<2>                      r_hlf;
        sc_uint<2>                      r_byt;

        sc_uint<2>                      a_emty;
        sc_uint<2>                      a_half;
        sc_uint<2>                      a_full;

        bool                            a_sft;
        bool                            a_new;

        bool                            a_cnt;
        bool                            a_end;

        sc_uint<3>                      p_idle;
        sc_uint<3>                      p_rqst;
        sc_uint<3>                      p_fadr;
        sc_uint<3>                      p_nadr;
        sc_uint<3>                      p_ldat;
        sc_uint<3>                      p_watr;

        sc_uint<3>                      p_ack_rdy;
        sc_uint<3>                      p_ack_rdm;

        sc_uint<4>                      p_opc_nop;
        sc_uint<4>                      p_opc_wdu;

        bool                            p_red_red;
        bool                            p_red_wrt;

	SC_CTOR(d_cache)
	{
                c_redy=0x0;
                c_full=0x4;
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
		sensitive 
			<< CK;
		SC_METHOD(processADRTAG_SX);
		sensitive 
			<< D_A;
		SC_METHOD(processBLCNBR_SX);
		sensitive 
			<< D_A;
		SC_METHOD(processBLCOFS_SX);
		sensitive 
			<< D_A;
		SC_METHOD(processTAG_SX);
		sensitive 
			<< BLCNBR_SX 
			<< TAG0_RX 
			<< TAG1_RX 
			<< TAG2_RX 
			<< TAG3_RX;
		SC_METHOD(processPRES_SX);
		sensitive 
			<< BLCNBR_SX 
			<< PRES0_RX 
			<< PRES1_RX 
			<< PRES2_RX 
			<< PRES3_RX;
		SC_METHOD(processBLOC_SX);
		sensitive 
			<< BLCNBR_SX 
			<< BLOC0_RX 
			<< BLOC1_RX 
			<< BLOC2_RX 
			<< BLOC3_RX;
		SC_METHOD(processDATA_SX);
		sensitive 
			<< BLCOFS_SX 
			<< BLOC_SX;
		SC_METHOD(processCMPTAGS_SX);
		sensitive 
			<< ADRTAG_SX 
			<< TAG_SX;
		SC_METHOD(processEQLTAGS_SX);
		sensitive 
			<< CMPTAGS_SX;
		SC_METHOD(processDIRMISS_SX);
		sensitive 
			<< D_ATYPE 
			<< PRES_SX 
			<< EQLTAGS_SX 
			<< D_RQ;
		SC_METHOD(processCMPUADR_SX);
		sensitive 
			<< D_A 
			<< UCACADR_RX;
		SC_METHOD(processEQLUADR_SX);
		sensitive 
			<< CMPUADR_SX 
			<< UCACVLD_RX;
		SC_METHOD(processMSK0ADR_SX);
		sensitive 
			<< D_A;
		SC_METHOD(processUNCMISS_SX);
		sensitive 
			<< D_ATYPE 
			<< MSK0ADR_SX 
			<< EQLUADR_SX 
			<< D_RQ;
		SC_METHOD(processMISS_SX);
		sensitive 
			<< UNCMISS_SX 
			<< DIRMISS_SX;
		SC_METHOD(processREDPEND_SX);
		sensitive 
			<< MISS_SX 
			<< RQSFULL_SX;
		SC_METHOD(processWRITEOP_SX);
		sensitive 
			<< D_ATYPE 
			<< D_RQ;
		SC_METHOD(processWRTPEND_SX);
		sensitive 
			<< WRITEOP_SX 
			<< RQSFULL_SX;
		SC_METHOD(processEFWRTOP_SX);
		sensitive 
			<< WRITEOP_SX 
			<< RQSFULL_SX;
		SC_METHOD(processRSTCNT_SX);
		sensitive 
			<< RSTCNT_RX 
			<< RSTCRY_SX;
		SC_METHOD(processRSTCRY_SX);
		sensitive 
			<< RSTCNT_RX 
			<< RSTCRY_SX;
		SC_METHOD(processENDRST_SX);
		sensitive 
			<< RSTCRY_SX;
		SC_METHOD(processRSTPRS);
		sensitive 
			<< C_STAT_RX 
			<< RSTCNT_RX;
		SC_METHOD(processC_NXTS_SX);
		sensitive 
			<< RESET_RX 
			<< C_STAT_RX 
			<< ENDRST_SX 
			<< MISS_SX 
			<< WRTPEND_SX 
			<< RQSFULL_SX 
			<< ANSEMTY_SX
			
			<< WRTPEND_SX 
			<< REDPEND_SX 
			<< RQSFULL_SX 
			<< UNCMISS_SX 
			<< DIRMISS_SX 
			<< ANSFLAG_SX;
		SC_METHOD(processANSPOP_SX);
		sensitive 
			<< C_STAT_RX 
			<< ANSEMTY_SX;
		SC_METHOD(processWRTUCAC_SX);
		sensitive 
			<< C_STAT_RX 
			<< ANSEMTY_SX;
		SC_METHOD(processWRTADR_SX);
		sensitive 
			<< C_STAT_RX 
			<< ANSEMTY_SX 
			<< ANSFLAG_SX;
		SC_METHOD(processWRTDATA_SX);
		sensitive 
			<< C_STAT_RX 
			<< ANSEMTY_SX;
		SC_METHOD(processWRTDAT_SX);
		sensitive 
			<< ANSADR_SX 
			<< WRTDATA_SX;
		SC_METHOD(processWRTBLOC_SX);
		sensitive 
			<< C_STAT_RX;
		SC_METHOD(processWRTBNBR_SX);
		sensitive 
			<< RBLOCAD_RX;
		SC_METHOD(processWRTBTAG_SX);
		sensitive 
			<< RBLOCAD_RX;
		SC_METHOD(processBLOCDIN_SX);
		sensitive 
			<< REDBLC0_RX 
			<< REDBLC1_RX 
			<< REDBLC2_RX 
			<< REDBLC3_RX;
		SC_METHOD(processRQSPSH_SX);
		sensitive 
			<< C_STAT_RX 
			<< UNCMISS_SX 
			<< DIRMISS_SX 
			<< EFWRTOP_SX;
		SC_METHOD(processRQSDIN_SX);
		sensitive 
			<< D_A 
			<< D_IN 
			<< UNCMISS_SX 
			<< D_LOCK 
			<< DIRMISS_SX 
			<< D_ATYPE;
		SC_METHOD(processWRTINVL_SX);
		sensitive 
			<< C_STAT_RX 
			<< EFWRTOP_SX 
			<< EQLTAGS_SX;
		SC_METHOD(processWRIPRS_SX);
		sensitive 
			<< BLCNBR_SX 
			<< WRTINVL_SX;
		SC_METHOD(processR_NXTS_SX);
		sensitive 
			<< RESET_RX 
			<< R_STAT_RX 
			<< RQSPSH_SX 
			<< RQSPOP_SX;
		SC_METHOD(processRQSEMTY_SX);
		sensitive 
			<< R_STAT_RX;
		SC_METHOD(processRQSFULL_SX);
		sensitive 
			<< R_STAT_RX;
		SC_METHOD(processRQSADR_SX);
		sensitive 
			<< RQSREG0_RX;
		SC_METHOD(processWRTRQS0_SX);
		sensitive 
			<< R_STAT_RX 
			<< RQSPSH_SX;
		SC_METHOD(processA_NXTS_SX);
		sensitive 
			<< RESET_RX 
			<< A_STAT_RX 
			<< ANSPSH_SX 
			<< ANSPOP_SX;
		SC_METHOD(processANSEMTY_SX);
		sensitive 
			<< A_STAT_RX;
		SC_METHOD(processANSFULL_SX);
		sensitive 
			<< A_STAT_RX;
		SC_METHOD(processANSDATA_SX);
		sensitive 
			<< ANSREG0_RX;
		SC_METHOD(processWRTANS0_SX);
		sensitive 
			<< A_STAT_RX 
			<< ANSPSH_SX 
			<< ANSPOP_SX;
		SC_METHOD(processWRTANS1_SX);
		sensitive 
			<< A_STAT_RX 
			<< ANSPSH_SX;
		SC_METHOD(processSELANS0_SX);
		sensitive 
			<< A_STAT_RX 
			<< ANSPOP_SX;
		SC_METHOD(processP_NXTS_SX);
		sensitive 
			<< RESET_RX 
			<< P_STAT_RX 
			<< RQSEMTY_SX 
			<< DATARDY_SX 
			<< PI_GNT 
			<< MOREADR_SX 
			<< ACSMOD_RX;
		SC_METHOD(processDEFLTEN_SX);
		sensitive 
			<< PI_GNT;
		SC_METHOD(processPILINEN_SX);
		sensitive 
			<< P_STAT_RX 
			<< DEFLTEN_SX;
		SC_METHOD(processPIDATEN_SX);
		sensitive 
			<< P_STAT_RX 
			<< ACSTYP_RX;
		SC_METHOD(processOPCODE_SX);
		sensitive 
			<< ACSADR_RX 
			<< ACSSIZ_RX;
		SC_METHOD(processPIOPC_SX);
		sensitive 
			<< P_STAT_RX 
			<< ANSFULL_SX 
			<< ACSTYP_RX 
			<< OPCODE_SX;
		SC_METHOD(processDATREAD_SX);
		sensitive 
			<< PI_READ_RX;
		SC_METHOD(processANSPSH_SX);
		sensitive 
			<< P_STAT_RX 
			<< DATREAD_SX;
		SC_METHOD(processANSDIN_SX);
		sensitive 
			<< PI_ADR_RX 
			<< PI_D 
			<< P_STAT_RX;
		SC_METHOD(processINITACS_SX);
		sensitive 
			<< P_STAT_RX 
			<< PI_GNT 
			<< RQSEMTY_SX 
			<< ACSMOD_RX 
			<< RQSEMTY_SX;
		SC_METHOD(processRQSPOP_SX);
		sensitive 
			<< INITACS_SX;
		SC_METHOD(processADRCNT_SX);
		sensitive 
			<< ACSADR_RX 
			<< ADRCRY_SX;
		SC_METHOD(processADRCRY_SX);
		sensitive 
			<< ACSADR_RX 
			<< DATARDY_SX 
			<< ANSFULL_SX;
		SC_METHOD(processACSCNT_SX);
		sensitive 
			<< P_STAT_RX 
			<< ACSADR_RX 
			<< DATARDY_SX 
			<< ANSFULL_SX;

		SC_METHOD(processMOREADR_SX);
		sensitive 
			<< ACSCNT_RX;
		SC_METHOD(processDATARDY_SX);
		sensitive 
			<< PI_ACK;
		SC_METHOD(processSNPBLC_SX);
		sensitive 
			<< PI_A;
		SC_METHOD(processSNPBTAG_SX);
		sensitive 
			<< SNPBLC_SX 
			<< TAG0_RX 
			<< TAG1_RX 
			<< TAG2_RX 
			<< TAG3_RX;
		SC_METHOD(processSNPCMPT_SX);
		sensitive 
			<< PI_A 
			<< SNPBTAG_SX;
		SC_METHOD(processSNPINVL_SX);
		sensitive 
			<< SNPCMPT_SX 
			<< PI_READ;
		SC_METHOD(processINVPRS_SX);
		sensitive 
			<< SNPBLC_SX 
			<< SNPINVL_SX;
		SC_METHOD(processD_FRZ);
		sensitive 
			<< C_STAT_RX 
			<< MISS_SX 
			<< WRTPEND_SX;
		SC_METHOD(processD_OUT);
		sensitive 
			<< UCACVLD_RX 
			<< UCACDAT_RX 
			<< DATA_SX;
		SC_METHOD(processPI_DATA_LINES);
		sensitive 
			<< PIDATEN_SX 
			<< CK_SX 
			<< ACSDAT_RX;
		SC_METHOD(processPI_IREQ);
		sensitive 
			<< P_STAT_RX;
		SC_METHOD(processPI_LINES);
		sensitive 
			<< ACSADR_RX 
			<< PILINEN_SX 
			<< CK_SX 
			<< DEFLTEN_SX 
			<< ACSTYP_RX 
			<< P_STAT_RX 
			<< MOREADR_SX 
			<< ACSMOD_RX
			
			<< ANSFULL_SX 
			<< PIOPC_SX;
		SC_METHOD(processCLOCK);
		sensitive_pos 
			<< CK_SX;
		SC_METHOD(processD_BERR_N);
	}

	// ### ------------------------------------------------------ ###
	// #   internal clock						#
	// ### ------------------------------------------------------ ###

	void processCK_SX()
	{
		CK_SX.write(CK.read());
	}

	// ### ------------------------------------------------------ ###
	// #   extract from the instruction address:			#
	// #     - the tag						#
	// #     - the bloc number					#
	// #     - the offset inside the bloc				#
	// ### ------------------------------------------------------ ###

	void processADRTAG_SX()
	{
		sc_uint<32> d_a=D_A.read();

		ADRTAG_SX.write(d_a.range(31,6));
	}

	void processBLCNBR_SX()
	{
		sc_uint<32> d_a=D_A.read();

		BLCNBR_SX.write(d_a.range(5,4));
	}

	void processBLCOFS_SX()
	{
		sc_uint<32> d_a=D_A.read();

		BLCOFS_SX.write(d_a.range(3,2));
	}

	// ### ------------------------------------------------------ ###
	// #   read the tag and the present bit from the directory	#
	// ### ------------------------------------------------------ ###

	void processTAG_SX()
	{
		if (BLCNBR_SX.read()==0x0)
			TAG_SX.write(TAG0_RX.read());
		else if (BLCNBR_SX.read()==0x1)
			TAG_SX.write(TAG1_RX.read());
		else if (BLCNBR_SX.read()==0x2)
			TAG_SX.write(TAG2_RX.read());
		else if (BLCNBR_SX.read()==0x3)
			TAG_SX.write(TAG3_RX.read());
	}

	void processPRES_SX()
	{
		if (BLCNBR_SX.read()==0x0)
			PRES_SX.write(PRES0_RX.read());
		else if (BLCNBR_SX.read()==0x1)
			PRES_SX.write(PRES1_RX.read());
		else if (BLCNBR_SX.read()==0x2)
			PRES_SX.write(PRES2_RX.read());
		else if (BLCNBR_SX.read()==0x3)
			PRES_SX.write(PRES3_RX.read());
	}

	// ### ------------------------------------------------------ ###
	// #   read the data bloc					#
	// ### ------------------------------------------------------ ###

	void processBLOC_SX()
	{
		if (BLCNBR_SX.read()==0x0)
			BLOC_SX.write(BLOC0_RX.read());
		else if (BLCNBR_SX.read()==0x1)
			BLOC_SX.write(BLOC1_RX.read());
		else if (BLCNBR_SX.read()==0x2)
			BLOC_SX.write(BLOC2_RX.read());
		else if (BLCNBR_SX.read()==0x3)
			BLOC_SX.write(BLOC3_RX.read());
	}

	// ### ------------------------------------------------------ ###
	// #   extract the data from the bloc				#
	// ### ------------------------------------------------------ ###

	void processDATA_SX()
	{
		sc_biguint<128> bloc_sx=BLOC_SX.read();
		sc_uint<32> res_sx=0x0;

		// .range(x,y) on a sc_biguint<> does not work properly
		if (BLCOFS_SX.read()==0x0)
			for (int i=31;i>=0;i--)
				res_sx[i]=bloc_sx[i+96];
		else if (BLCOFS_SX.read()==0x1)
			for (int i=31;i>=0;i--)
				res_sx[i]=bloc_sx[i+64];
		else if (BLCOFS_SX.read()==0x2)
			for (int i=31;i>=0;i--)
				res_sx[i]=bloc_sx[i+32];
		else if (BLCOFS_SX.read()==0x3)
			for (int i=31;i>=0;i--)
				res_sx[i]=bloc_sx[i];
		DATA_SX.write(res_sx);
	}

	// ### ------------------------------------------------------ ###
	// #   compare the bloc's tag and the address's tag and		#
	// # determine if the the required bloc is present in the cache	#
	// ### ------------------------------------------------------ ###

	void processCMPTAGS_SX()
	{
		CMPTAGS_SX.write(ADRTAG_SX.read() ^ TAG_SX.read());
	}

	void processEQLTAGS_SX()
	{
		EQLTAGS_SX.write((CMPTAGS_SX.read()==0x0)?1:0);
	}

	// ### ------------------------------------------------------ ###
	// #   directory miss :						#
	// #								#
	// #   determine if the required bloc is present in the cache.	#
	// # A miss is generated if a read is attempted on a bloc non	#
	// # present in the cache.					#
	// ### ------------------------------------------------------ ###

	void processDIRMISS_SX()
	{
		DIRMISS_SX.write(((D_ATYPE.read()==m_read_w)&&((PRES_SX.read()==0)||(EQLTAGS_SX.read()==0)))?D_RQ.read():0);
	}

	// ### ------------------------------------------------------ ###
	// #   the required data may be an uncachable data recently	#
	// # fetched from the external memory. Compare the address with	#
	// # the address of the last uncachable word read from the	#
	// # external memory and check its validity.			#
	// ### ------------------------------------------------------ ###

	void processCMPUADR_SX()
	{
		CMPUADR_SX.write(D_A.read() ^ UCACADR_RX.read());
	}

	void processEQLUADR_SX()
	{
		EQLUADR_SX.write((CMPUADR_SX.read()==0x0)?UCACVLD_RX.read():0);
	}

	// ### ------------------------------------------------------ ###
	// #   uncachable data miss :					#
	// #								#
	// #   compare the address with the uncachable addresses. There	#
	// # is a miss if the address is an uncachable address and if	#
	// # the data is not the last uncachable data fetched from the	#
	// # memory							#
	// ### ------------------------------------------------------ ###

	void processMSK0ADR_SX()
	{
		MSK0ADR_SX.write(D_A.read() & u0_msk);
	}

	void processUNCMISS_SX()
	{
		UNCMISS_SX.write(((D_ATYPE.read()==m_read_w)&&(MSK0ADR_SX.read()==u0_adr)&&(EQLUADR_SX.read()==0))?D_RQ.read():0);
	}

	// ### ------------------------------------------------------ ###
	// #   global miss						#
	// ### ------------------------------------------------------ ###

	void processMISS_SX()
	{
		MISS_SX.write(UNCMISS_SX.read() | DIRMISS_SX.read());
	}

	// ### ------------------------------------------------------ ###
	// #   read operation pending :					#
	// #								#
	// #   on a miss on read, if the Request Fifo is full, the read	#
	// # request cannot be send to the bus interface controller and	#
	// # the read operation is suspended.				#
	// ### ------------------------------------------------------ ###

	void processREDPEND_SX()
	{
		REDPEND_SX.write(MISS_SX.read() & RQSFULL_SX.read());
	}

	// ### ------------------------------------------------------ ###
	// #   write operation pending :				#
	// #								#
	// #   on a write operation, if the Request Fifo is full, the	#
	// # write request cannot be send to the bus interface		#
	// # controller and the write operation is suspended.		#
	// ### ------------------------------------------------------ ###

	void processWRITEOP_SX()
	{
		if (D_ATYPE.read()==m_read_w)
			WRITEOP_SX.write(0);
		else
			WRITEOP_SX.write(D_RQ.read());
	}

	void processWRTPEND_SX()
	{
		WRTPEND_SX.write(WRITEOP_SX.read() & RQSFULL_SX.read());
	}

	void processEFWRTOP_SX()
	{
		EFWRTOP_SX.write(WRITEOP_SX.read() & !RQSFULL_SX.read());
	}

	// ### ------------------------------------------------------ ###
	// #   in case of reset, invalidate all the present bits in the	#
	// # directory. A counter is used to generate bloc numbers. One	#
	// # bit is reseted at each cycle. The cache stays in reset	#
	// # state until all the present bits are reseted.		#
	// ### ------------------------------------------------------ ###

	void processRSTCNT_SX()
	{
		sc_uint<2> rstcnt_sx=0x0;
		sc_uint<2> rstcnt_rx=RSTCNT_RX.read();
		sc_uint<2> rstcry_sx=RSTCRY_SX.read();

		rstcnt_sx.range(1,0)=rstcnt_rx.range(1,0) ^ rstcry_sx.range(1,0);
		RSTCNT_SX.write(rstcnt_sx);
	}

        void processRSTCRY_SX()
        {
                sc_uint<2> rstcnt_rx=RSTCNT_RX.read();
                sc_uint<3> rstcry_sx=RSTCRY_SX.read();

                rstcry_sx.range(2,1)=rstcnt_rx.range(1,0) & rstcry_sx.range(1,0);
                rstcry_sx[0]=1;
                RSTCRY_SX.write(rstcry_sx);
        }

	void processENDRST_SX()
	{
		sc_uint<3> rstcry_sx=RSTCRY_SX.read();

		ENDRST_SX.write(rstcry_sx[2]);
	}

	void processRSTPRS()
	{
		if ((C_STAT_RX.read() == c_rset) && (RSTCNT_RX.read()==0x0))
			RSTPRS0_SX.write(1);
		else
			RSTPRS0_SX.write(0);
		if ((C_STAT_RX.read() == c_rset) && (RSTCNT_RX.read()==0x1))
			RSTPRS1_SX.write(1);
		else
			RSTPRS1_SX.write(0);
		if ((C_STAT_RX.read() == c_rset) && (RSTCNT_RX.read()==0x2))
			RSTPRS2_SX.write(1);
		else
			RSTPRS2_SX.write(0);
		if ((C_STAT_RX.read() == c_rset) && (RSTCNT_RX.read()==0x3))
			RSTPRS3_SX.write(1);
		else
			RSTPRS3_SX.write(0);
	}

	// ### ----------------------------------------------------// ###
	// #   the cache controller finite state machine includes 6	#
	// # states:							#
	// #								#
	// #     - Ready (c_redy): the cache is ready to answer to any	#
	// #         request from the processor. For a read operation,	#
	// #         if the required data is in the cache, it is send	#
	// #         back to the processor and the cache remains in	#
	// #         ready state. For a write operation, the Request	#
	// #         Fifo is checked and if a place is available the a	#
	// #         write request is send the cache remaining in the	#
	// #         Ready state					#
	// #     - Request Full (c_full): the cache goes to this state	#
	// #         whenever a request has to be send to the Pi-bus	#
	// #         controller and the Request Fifo is full. In such	#
	// #         a case the request cannot be pushed on the fifo	#
	// #         and the cache has to delay the operation.		#
	// #     - Wait Uncachable (c_uwat): the cache goes to this	#
	// #         state when an uncachable data has been requested	#
	// #         by the processor. The cache remains in this state	#
	// #         until the data is fetched from the external memory.#
	// #         Then, the data is transfered into a special	#
	// #         register and is made available for the processor.	#
	// #     - Wait on Directory Miss (c_mwat): the cache goes to	#
	// #         this state if a miss has been generated from the	#
	// #         directory. It remains in this state until a bloc	#
	// #         is fetched from the memory. Each time a data (a	#
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
	// #       present bits have been reseted.			#
	// #								#
	// #     - otherwise the cache is in Ready state (c_redy) and	#
	// #       answers to processor's requests. The cache can leave	#
	// #       the Ready state on a read operation, if the required	#
	// #       data is not found in the cache (directory miss or	#
	// #       uncachable miss). If the Request Fifo is full, the	#
	// #       cache controller goes to the Rquest Full (c_full)	#
	// #       state and waits that a place becomes available in	#
	// #       the fifo and returns back to the Ready state. If the	#
	// #       Request Fifo was not full, the cache goes either to	#
	// #       the Wait Uncachable or to the Wait on Directory	#
	// #       Miss state. In the same time, the request's context	#
	// #       is send to the Request Fifo. In the next cycle the	#
	// #       fifo's empty line goes to false sending a request to	#
	// #       the bus interface controller. The cache can also	#
	// #       leave the Ready state on a write operation if the	#
	// #       Request Fifo is full the write request cannot be	#
	// #       send to the bus interface controller.		#
	// #								#
	// #     - in Request Full state (c_full) the cache waits for	#
	// #       a place in the Request Fifo.				#
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
	// #       written as part of a cache bloc in a special		#
	// #       register. Once a complete bloc has been fetched from	#
	// #       the memory, (the last data has its End flag set in	#
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

	void processC_NXTS_SX()
	{
		if (RESET_RX.read()==1)
			C_NXTS_SX.write(c_rset);
		else if ((C_STAT_RX.read() == c_rset) && (ENDRST_SX.read()==0))
			C_NXTS_SX.write(c_rset);

		else if ((C_STAT_RX.read() == c_rset) && (ENDRST_SX.read()==1))
			C_NXTS_SX.write(c_redy);
		else if ((C_STAT_RX.read() == c_redy) && (MISS_SX.read()==0) && (WRTPEND_SX.read()==0))
			C_NXTS_SX.write(c_redy);
		else if ((C_STAT_RX.read() == c_full) && (RQSFULL_SX.read()==0))
			C_NXTS_SX.write(c_redy);
		else if ((C_STAT_RX.read() == c_uwat) && (ANSEMTY_SX.read()==0))
			C_NXTS_SX.write(c_redy);
		else if (C_STAT_RX.read() == c_mwrt)
			C_NXTS_SX.write(c_redy);

		else if ((C_STAT_RX.read() == c_redy) && (WRTPEND_SX.read()==1))
			C_NXTS_SX.write(c_full);
		else if ((C_STAT_RX.read() == c_redy) && (REDPEND_SX.read()==1))
			C_NXTS_SX.write(c_full);
		else if ((C_STAT_RX.read() == c_full) && (RQSFULL_SX.read()==1))
			C_NXTS_SX.write(c_full);

		else if ((C_STAT_RX.read() == c_redy) && (UNCMISS_SX.read()==1) && (RQSFULL_SX.read()==0))
			C_NXTS_SX.write(c_uwat);
		else if ((C_STAT_RX.read() == c_uwat) && (ANSEMTY_SX.read()==1))
			C_NXTS_SX.write(c_uwat);

		else if ((C_STAT_RX.read() == c_redy) && (UNCMISS_SX.read()==0) && (DIRMISS_SX.read()==1) && (RQSFULL_SX.read()==0))
			C_NXTS_SX.write(c_mwat);
		else if ((C_STAT_RX.read() == c_mwat) && (ANSEMTY_SX.read()==1))
			C_NXTS_SX.write(c_mwat);
		else if ((C_STAT_RX.read() == c_mwat) && (ANSEMTY_SX.read()==0) && (ANSFLAG_SX.read()==a_cnt))
			C_NXTS_SX.write(c_mwat);

		else if ((C_STAT_RX.read() == c_mwat) && (ANSEMTY_SX.read()==0) && (ANSFLAG_SX.read()==a_end))
			C_NXTS_SX.write(c_mwrt);

		else
			C_NXTS_SX.write(c_rset);
	}

	// ### ------------------------------------------------------ ###
	// #   extract a data from the Answer Fifo when waiting for an	#
	// # answer and the fifo is not empty				#
	// ### ------------------------------------------------------ ###

	void processANSPOP_SX()
	{
		if ((C_STAT_RX.read()==c_uwat) && (ANSEMTY_SX.read()==0))
			ANSPOP_SX.write(1);
		else if ((C_STAT_RX.read()==c_mwat) && (ANSEMTY_SX.read()==0))
			ANSPOP_SX.write(1);
		else
			ANSPOP_SX.write(0);
	}

	// ### ------------------------------------------------------ ###
	// #   when an uncachable data has been fetched, extract it	#
	// # from the Answer Fifo and write it in a special register.	#
	// ### ------------------------------------------------------ ###

	void processWRTUCAC_SX()
	{
		if ((C_STAT_RX.read()==c_uwat) && (ANSEMTY_SX.read()==0))
			WRTUCAC_SX.write(1);
		else
			WRTUCAC_SX.write(0);
	}

	// ### ------------------------------------------------------ ###
	// #   when waiting for a missing bloc, each time a word has	#
	// # been read, extract it from the Answer Fifo and write it	#
	// # into a special register					#
	// ### ------------------------------------------------------ ###

	void processWRTADR_SX()
	{
		if ((C_STAT_RX.read()==c_mwat) && (ANSEMTY_SX.read()==0) && (ANSFLAG_SX.read()==a_end))
			WRTADR_SX.write(1);
		else
			WRTADR_SX.write(0);
	}

	void processWRTDATA_SX()
	{
		if ((C_STAT_RX.read()==c_mwat) && (ANSEMTY_SX.read()==0))
			WRTDATA_SX.write(1);
		else
			WRTDATA_SX.write(0);
	}

	void processWRTDAT_SX()
	{
		sc_uint<32> ansadr_sx=ANSADR_SX.read();

		if (ansadr_sx.range(3,2)==0x0)
			WRTDAT0_SX.write(WRTDATA_SX.read());
		else
			WRTDAT0_SX.write(0);
		if (ansadr_sx.range(3,2)==0x1)
			WRTDAT1_SX.write(WRTDATA_SX.read());
		else
			WRTDAT1_SX.write(0);
		if (ansadr_sx.range(3,2)==0x2)
			WRTDAT2_SX.write(WRTDATA_SX.read());
		else
			WRTDAT2_SX.write(0);
		if (ansadr_sx.range(3,2)==0x3)
			WRTDAT3_SX.write(WRTDATA_SX.read());
		else
			WRTDAT3_SX.write(0);
	}

	// ### ------------------------------------------------------ ###
	// #   when a complete bloc has been fetched, transfer it from	#
	// # a special register into the cache				#
	// ### ------------------------------------------------------ ###

	void processWRTBLOC_SX()
	{
		if (C_STAT_RX.read()==c_mwrt)
			WRTBLOC_SX.write(1);
		else
			WRTBLOC_SX.write(0);
	}

	void processWRTBNBR_SX()
	{
		sc_uint<32> rblocad_rx=RBLOCAD_RX.read();

		WRTBNBR_SX.write(rblocad_rx.range(5,4));
	}

	void processWRTBTAG_SX()
	{
		sc_uint<32> rblocad_rx=RBLOCAD_RX.read();

		WRTBTAG_SX.write(rblocad_rx.range(31,6));
	}

	// ### ------------------------------------------------------ ###
	// #   define the value to be saved into the cache bloc		#
	// ### ------------------------------------------------------ ###

	void processBLOCDIN_SX()
	{
		sc_biguint<128> blocdin_sx=0x0;
		sc_uint<32> redblc0_rx=REDBLC0_RX.read();
		sc_uint<32> redblc1_rx=REDBLC1_RX.read();
		sc_uint<32> redblc2_rx=REDBLC2_RX.read();
		sc_uint<32> redblc3_rx=REDBLC3_RX.read();

		// .range(x,y) on a sc_biguint<> does not work properly
		for (int i=31;i>=0;i--)
			blocdin_sx[i+96]=redblc0_rx[i];
		for (int i=31;i>=0;i--)
			blocdin_sx[i+64]=redblc1_rx[i];
		for (int i=31;i>=0;i--)
			blocdin_sx[i+32]=redblc2_rx[i];
		for (int i=31;i>=0;i--)
			blocdin_sx[i]=redblc3_rx[i];
		BLOCDIN_SX.write(blocdin_sx);
	}


	// ### ------------------------------------------------------ ###
	// #   push the context of a request on the Request Fifo:	#
	// #								#
	// #   in case of a miss on a read operation (uncachable or	#
	// # directory miss) send a request. The context contains the	#
	// # address of the missing data, the type of the access (read	#
	// # or write - here read), the type of the data (word, half-	#
	// # word or byte - here word), the value of the lock flag	#
	// # (locked or unlocked access - swap instruction) and the	#
	// # number of requierd data (minus 1).				#
	// #								#
	// #   in case of a write operation, send a request if there is	#
	// # a place available on the Request Fifo. In such a case, the	#
	// # context contains the address of the data, the data itself,	#
	// # the type of the access (write), the type of the data	#
	// # (word, half_word or byte) and the number of data transfer	#
	// # (here always 1).						#
	// ### ------------------------------------------------------ ###

	void processRQSPSH_SX()
	{
		if (C_STAT_RX.read()==c_redy)
			RQSPSH_SX.write(UNCMISS_SX.read() | DIRMISS_SX.read() | EFWRTOP_SX.read());
		else
			RQSPSH_SX.write(0);
	}

	void processRQSDIN_SX()
	{
		sc_biguint<72> rqsdin_sx=0x0;
		sc_uint<32> d_a=D_A.read();
		sc_uint<32> d_in=D_IN.read();

		if (UNCMISS_SX.read()==1)
		{
			for (int i=31;i>=0;i--)
				rqsdin_sx[i+40]=d_a[i];
			for (int i=31;i>=0;i--)
				rqsdin_sx[i+8]=d_in[i];
			rqsdin_sx[7]=r_red;
			rqsdin_sx[6]=0; // bits 6 and 5 define a r_wrd=00 operation
			rqsdin_sx[5]=0;
			rqsdin_sx[4]=D_LOCK.read();
			rqsdin_sx[3]=0;
			rqsdin_sx[2]=0;
			rqsdin_sx[1]=0;
			rqsdin_sx[0]=0;
		}
		else if (DIRMISS_SX.read()==1)
		{
			for (int i=31;i>=0;i--)
				rqsdin_sx[i+40]=d_a[i];
			for (int i=31;i>=0;i--)
				rqsdin_sx[i+8]=d_in[i];
			rqsdin_sx[7]=r_red;
			rqsdin_sx[6]=0; // bits 6 and 5 define a r_wrd=00 operation
			rqsdin_sx[5]=0;
			rqsdin_sx[4]=0;
			rqsdin_sx[3]=0;
			rqsdin_sx[2]=0;
			rqsdin_sx[1]=1;
			rqsdin_sx[0]=1;
		}
		else if (D_ATYPE.read()==m_writ_w)
		{
			for (int i=31;i>=0;i--)
				rqsdin_sx[i+40]=d_a[i];
			for (int i=31;i>=0;i--)
				rqsdin_sx[i+8]=d_in[i];
			rqsdin_sx[7]=r_wrt;
			rqsdin_sx[6]=0; // bits 6 and 5 define a r_wrd=00 operation
			rqsdin_sx[5]=0;
			rqsdin_sx[4]=0;
			rqsdin_sx[3]=0;
			rqsdin_sx[2]=0;
			rqsdin_sx[1]=0;
			rqsdin_sx[0]=0;
		}
		else if (D_ATYPE.read()==m_writ_w)
		{
			for (int i=31;i>=0;i--)
				rqsdin_sx[i+40]=d_a[i];
			for (int i=31;i>=0;i--)
				rqsdin_sx[i+8]=d_in[i];
			rqsdin_sx[7]=r_wrt;
			rqsdin_sx[6]=0; // bits 6 and 5 define a r_wrd=00 operation
			rqsdin_sx[5]=0;
			rqsdin_sx[4]=0;
			rqsdin_sx[3]=0;
			rqsdin_sx[2]=0;
			rqsdin_sx[1]=0;
			rqsdin_sx[0]=0;
		}
		else if (D_ATYPE.read()==m_writ_h)
		{
			for (int i=31;i>=0;i--)
				rqsdin_sx[i+40]=d_a[i];
			for (int i=31;i>=0;i--)
				rqsdin_sx[i+8]=d_in[i];
			rqsdin_sx[7]=r_wrt;
			rqsdin_sx[6]=0; // bits 6 and 5 define a r_hlf=01 operation
			rqsdin_sx[5]=1;
			rqsdin_sx[4]=0;
			rqsdin_sx[3]=0;
			rqsdin_sx[2]=0;
			rqsdin_sx[1]=0;
			rqsdin_sx[0]=0;
		}
		else
		{
			for (int i=31;i>=0;i--)
				rqsdin_sx[i+40]=d_a[i];
			for (int i=31;i>=0;i--)
				rqsdin_sx[i+8]=d_in[i];
			rqsdin_sx[7]=r_wrt;
			rqsdin_sx[6]=1; // bits 6 and 5 define a r_byt=11 operation
			rqsdin_sx[5]=1;
			rqsdin_sx[4]=0;
			rqsdin_sx[3]=0;
			rqsdin_sx[2]=0;
			rqsdin_sx[1]=0;
			rqsdin_sx[0]=0;
		}
		RQSDIN_SX.write(rqsdin_sx);
	}

	// ### ------------------------------------------------------ ###
	// #   bloc invalidation on write operation :			#
	// #								#
	// #   when a write request is send to the bus interface	#
	// # controller, check if the written bloc is present in the	#
	// # cache. In such a case, the present bit of the bloc is	#
	// # reseted to invalidate the bloc.				#
	// ### ------------------------------------------------------ ###

	void processWRTINVL_SX()
	{
		if (C_STAT_RX.read()==c_redy)
		{
			WRTINVL_SX.write(EFWRTOP_SX.read() & EQLTAGS_SX.read());
		}
		else
			WRTINVL_SX.write(0);
	}

	void processWRIPRS_SX()
	{
		if (BLCNBR_SX.read()==0x0)
			WRIPRS0_SX.write(WRTINVL_SX.read());
		else
			WRIPRS0_SX.write(0);

		if (BLCNBR_SX.read()==0x1)
			WRIPRS1_SX.write(WRTINVL_SX.read());
		else
			WRIPRS1_SX.write(0);

		if (BLCNBR_SX.read()==0x2)
			WRIPRS2_SX.write(WRTINVL_SX.read());
		else
			WRIPRS2_SX.write(0);

		if (BLCNBR_SX.read()==0x3)
			WRIPRS3_SX.write(WRTINVL_SX.read());
		else
			WRIPRS3_SX.write(0);
	}

	// ### ------------------------------------------------------ ###
	// #   the Request Fifo has only one place and its controller	#
	// # has two states: Empty (r_emty) and Full (r_full).		#
	// ### ------------------------------------------------------ ###

	void processR_NXTS_SX()
	{
		if (RESET_RX.read()==1)
			R_NXTS_SX.write(r_emty);
		else if ((R_STAT_RX.read() == r_emty) && (RQSPSH_SX.read()==0))
			R_NXTS_SX.write(r_emty);

		else if ((R_STAT_RX.read() == r_emty) && (RQSPSH_SX.read()==1))
			R_NXTS_SX.write(r_full);
		else if ((R_STAT_RX.read() == r_full) && (RQSPOP_SX.read()==0))
			R_NXTS_SX.write(r_full);

		else
			R_NXTS_SX.write(r_emty);
	}

	// ### ------------------------------------------------------ ###
	// #   define the Request Fifo's flag				#
	// ### ------------------------------------------------------ ###

	void processRQSEMTY_SX()
	{
		if (R_STAT_RX.read() == r_emty)
			RQSEMTY_SX.write(1);
		else
			RQSEMTY_SX.write(0);
	}

	void processRQSFULL_SX()
	{
		if (R_STAT_RX.read() == r_full)
			RQSFULL_SX.write(1);
		else
			RQSFULL_SX.write(0);
	}

    // ### ------------------------------------------------------ ###
	// #   define the Request Fifo's output				#
	// ### ------------------------------------------------------ ###

	void processRQSADR_SX()
	{
		sc_biguint<72> rqsreg0_rx=RQSREG0_RX.read();
		sc_uint<32> rqsadr_sx;
		sc_uint<32> rqsdat_sx;
		sc_uint<2> rqssiz_sx;
		sc_uint<4> rqsacsc_sx;

		for (int i=31;i>=0;i--)
			rqsadr_sx[i]=rqsreg0_rx[i+40];
		RQSADR_SX.write(rqsadr_sx);
		for (int i=31;i>=0;i--)
			rqsdat_sx[i]=rqsreg0_rx[i+8];
		RQSDAT_SX.write(rqsdat_sx);
		RQSTYP_SX.write(rqsreg0_rx[7]);
		for (int i=1;i>=0;i--)
			rqssiz_sx[i]=rqsreg0_rx[i+5];
		RQSSIZ_SX.write(rqssiz_sx);
		RQSMOD_SX.write(rqsreg0_rx[4]);
		for (int i=3;i>=0;i--)
			rqsacsc_sx[i]=rqsreg0_rx[i];
		RQSACSC_SX.write(rqsacsc_sx);
	}


	// ### ------------------------------------------------------ ###
	// #   define the Request Fifo's write condition		#
	// ### ------------------------------------------------------ ###

	void processWRTRQS0_SX()
	{
		if ((R_STAT_RX.read()==r_emty) && (RQSPSH_SX.read()==1))
			WRTRQS0_SX.write(1);
		else
			WRTRQS0_SX.write(0);
	}

	// ### ------------------------------------------------------ ###
	// #   the Answer Fifo has two places and its controller has	#
	// # 3 states: Empty (a_emty), Half-full (a_half) and Full	#
	// # (a_full).							#
	// ### ------------------------------------------------------ ###

	void processA_NXTS_SX()
	{
		if (RESET_RX.read()==1)
			A_NXTS_SX.write(a_emty);
		else if ((A_STAT_RX.read() == a_emty) && (ANSPSH_SX.read()==0))
			A_NXTS_SX.write(a_emty);
		else if ((A_STAT_RX.read() == a_half) && (ANSPSH_SX.read()==1) && (ANSPOP_SX.read()==1))
			A_NXTS_SX.write(a_half);

		else if ((A_STAT_RX.read() == a_emty) && (ANSPSH_SX.read()==1))
			A_NXTS_SX.write(a_half);
		else if ((A_STAT_RX.read() == a_half) && (ANSPSH_SX.read()==0) && (ANSPOP_SX.read()==0))
			A_NXTS_SX.write(a_half);
		else if ((A_STAT_RX.read() == a_half) && (ANSPSH_SX.read()==1) && (ANSPOP_SX.read()==1))
			A_NXTS_SX.write(a_half);
		else if ((A_STAT_RX.read() == a_full) && (ANSPOP_SX.read()==1))
			A_NXTS_SX.write(a_half);

		else if ((A_STAT_RX.read() == a_half) && (ANSPSH_SX.read()==1) && (ANSPOP_SX.read()==0))
			A_NXTS_SX.write(a_full);
		else if ((A_STAT_RX.read() == a_full) && (ANSPOP_SX.read()==0))
			A_NXTS_SX.write(a_full);

		else
			A_NXTS_SX.write(a_emty);
	}

	// ### ------------------------------------------------------ ###
	// #   define the Answer Fifo's flag				#
	// ### ------------------------------------------------------ ###

	void processANSEMTY_SX()
	{
		if (A_STAT_RX.read() == a_emty)
			ANSEMTY_SX.write(1);
		else
			ANSEMTY_SX.write(0);
	}

	void processANSFULL_SX()
	{
		if (A_STAT_RX.read() == a_full)
			ANSFULL_SX.write(1);
		else
			ANSFULL_SX.write(0);
	}
	// ### ------------------------------------------------------ ###
	// #   define the Answer Fifo's output				#
	// ### ------------------------------------------------------ ###


	void processANSDATA_SX()
	{
		sc_biguint<65> ansreg0_rx=ANSREG0_RX.read();
		sc_uint<32> ansdata_sx;
		sc_uint<32> ansadr_sx;

		for (int i=31;i>=0;i--)
			ansdata_sx[i]=ansreg0_rx[i+33];
		ANSDATA_SX.write(ansdata_sx);
		for (int i=31;i>=0;i--)
			ansadr_sx[i]=ansreg0_rx[i+1];
		ANSADR_SX.write(ansadr_sx);
		ANSFLAG_SX.write(ansreg0_rx[0]);
	}


	// ### ------------------------------------------------------ ###
	// #   define the Answer Fifo's write condition			#
	// ### ------------------------------------------------------ ###

	void processWRTANS0_SX()
	{
		if ((A_STAT_RX.read() == a_emty) && (ANSPSH_SX.read()==1))
			WRTANS0_SX.write(1);
		else if ((A_STAT_RX.read() == a_half) && (ANSPOP_SX.read()==1))
			WRTANS0_SX.write(1);
		else if ((A_STAT_RX.read() == a_full) && (ANSPOP_SX.read()==1))
			WRTANS0_SX.write(1);
		else
			WRTANS0_SX.write(0);
	}

	void processWRTANS1_SX()
	{
		if ((A_STAT_RX.read() == a_half) && (ANSPSH_SX.read()==1))
			WRTANS1_SX.write(1);
		else
			WRTANS1_SX.write(0);
	}

	void processSELANS0_SX()
	{
		if ((A_STAT_RX.read() == a_full) && (ANSPOP_SX.read()==1))
			SELANS0_SX.write(a_sft);
		else
			SELANS0_SX.write(a_new);
	}

	// ### ------------------------------------------------------ ###
	// #   the Pi-bus interface state machine includes 6 states:	#
	// #								#
	// #     - Idle (p_idle): the master is idle. No transaction is	#
	// #         on going.						#
	// #     - Request (p_rqst): A request is send on the Pi-bus	#
	// #     - First Address (p_fadr): a first address is send	#
	// #     - Next  Addresses (p_nadr): when multiple accesses	#
	// #         have to be performed, for all addresses except the	#
	// #         first, the Pi-bus controller remains in this state	#
	// #     - Last Data (p_ldat): make the last data transfer on	#
	// #         the bus						#
	// #     - Wait Request (p_watr): the bus is granted for the	#
	// #         cache which has to perform a locked access. It is	#
	// #         waiting for the next request			#
	// ### ------------------------------------------------------ ###

	// ### ------------------------------------------------------ ###
	// #   Pi-bus interface state machine - next state:		#
	// #								#
	// #     - in case of reset go to the Idle (p_idle) state.	#
	// #       Remain in the Idle state until a request is received	#
	// #       from the Request Fifo (Request Fifo not empty).	#
	// #								#
	// #     - when a request has been received go to the Request	#
	// #       (p_rqst) state if the default master is not granted.	#
	// #       A request for access to the bus is posted for the	#
	// #       Pi-bus. The controller remains in the Request state	#
	// #       until the bus is granted for the cache then, goes to	#
	// #       the First Address (p_fadr) state.			#
	// #								#
	// #     - when a cache request is received, if the default	#
	// #       master is granted, go immediatly to First Address	#
	// #       (p_fadr) state. Also, when after a request the bus	#
	// #       is granted, the next state is First Address. The	#
	// #       controller remains only one cycle in this state. The	#
	// #       first address is sent on the bus. Then, it goes	#
	// #       either to the Next Addresses (p_nadr) state if other	#
	// #       accesses are to be done or to the Last Data (p_ldat)	#
	// #       if only one data is to be transfered.		#
	// #								#
	// #     - the controller stays in Next Addresses (p_nadr)	#
	// #       state until the last address has been sent. Then, it	#
	// #       goes to the Last Data (p_ldat) state.		#
	// #								#
	// #     - in the Last Data (p_ldat) state, the last data is	#
	// #       transfered on the bus and the cache stops driving	#
	// #       the Pi-bus lines. In the next cycle the controller	#
	// #       returns to the Idle state.				#
	// #								#
	// #     - when a locked access has been initiated, after the	#
	// #       access has benn acheived, the controller has to	#
	// #       perform the next access without releasing the bus.	#
	// #       In this situation, if the next request has not been	#
	// #       received from the cache controller, the bus		#
	// #       controller goes to the Wait Request (p_watr) state.	#
	// ### ------------------------------------------------------ ###

	void processP_NXTS_SX()
	{
		if (RESET_RX.read()==1)
			P_NXTS_SX.write(p_idle);
		else if ((P_STAT_RX.read() == p_idle) && (RQSEMTY_SX.read()==1))
			P_NXTS_SX.write(p_idle);
		else if ((P_STAT_RX.read() == p_ldat) && (DATARDY_SX.read()==1))
			P_NXTS_SX.write(p_idle);

		else if ((P_STAT_RX.read() == p_idle) && (RQSEMTY_SX.read()==0) && (PI_GNT.read()==0))
			P_NXTS_SX.write(p_rqst);
		else if ((P_STAT_RX.read() == p_rqst) && (PI_GNT.read()==0))
			P_NXTS_SX.write(p_rqst);

		else if ((P_STAT_RX.read() == p_idle) && (RQSEMTY_SX.read()==0) && (PI_GNT.read()==1))
			P_NXTS_SX.write(p_fadr);
		else if ((P_STAT_RX.read() == p_rqst) && (PI_GNT.read()==1))
			P_NXTS_SX.write(p_fadr);

		else if ((P_STAT_RX.read() == p_fadr) && (MOREADR_SX.read()==1))
			P_NXTS_SX.write(p_nadr);
		else if ((P_STAT_RX.read() == p_fadr) && (ACSMOD_RX.read()==1) && (RQSEMTY_SX.read()==0))
			P_NXTS_SX.write(p_nadr);
		else if ((P_STAT_RX.read() == p_nadr) && (MOREADR_SX.read()==1))
			P_NXTS_SX.write(p_nadr);
		else if ((P_STAT_RX.read() == p_nadr) && (ACSMOD_RX.read()==1) && (RQSEMTY_SX.read()==0))
			P_NXTS_SX.write(p_nadr);

		else if ((P_STAT_RX.read() == p_fadr) && (MOREADR_SX.read()==0) && (ACSMOD_RX.read()==0))
			P_NXTS_SX.write(p_ldat);
		else if ((P_STAT_RX.read() == p_nadr) && (MOREADR_SX.read()==0) && (ACSMOD_RX.read()==0))
			P_NXTS_SX.write(p_ldat);
		else if ((P_STAT_RX.read() == p_ldat) && (DATARDY_SX.read()==0))
			P_NXTS_SX.write(p_ldat);

		else if ((P_STAT_RX.read() == p_fadr) && (MOREADR_SX.read()==0) && (ACSMOD_RX.read()==1) && (RQSEMTY_SX.read()==1))
			P_NXTS_SX.write(p_watr);
		else if ((P_STAT_RX.read() == p_nadr) && (MOREADR_SX.read()==0) && (ACSMOD_RX.read()==1) && (RQSEMTY_SX.read()==1))
			P_NXTS_SX.write(p_watr);
		else if ((P_STAT_RX.read() == p_watr) && (RQSEMTY_SX.read()==1))
			P_NXTS_SX.write(p_watr);

		else
			P_NXTS_SX.write(p_idle);
	}

	// ### ------------------------------------------------------ ###
	// #   Pi-bus lines' output enable :				#
	// #     enable output lines when the bus has been granted or	#
	// #     if the cache is the default master			#
	// ### ------------------------------------------------------ ###

	void processDEFLTEN_SX()
	{
		DEFLTEN_SX.write(PI_GNT.read());
	}

	void processPILINEN_SX()
	{
		if (DEFLTEN_SX.read() == 1)
			PILINEN_SX.write(1);
		else if ((P_STAT_RX.read() == p_fadr) || (P_STAT_RX.read() == p_nadr) || (P_STAT_RX.read() == p_watr))
			PILINEN_SX.write(1);
		else
			PILINEN_SX.write(0);
	}

	// ### ------------------------------------------------------ ###
	// #   Pi-bus data lines' output enable				#
	// ### ------------------------------------------------------ ###


	void processPIDATEN_SX()
	{
		if (((P_STAT_RX.read() == p_nadr) || (P_STAT_RX.read() == p_ldat)) && (ACSTYP_RX.read() == r_wrt))
			PIDATEN_SX.write(1);
		else
			PIDATEN_SX.write(0);
	}

	// ### ------------------------------------------------------ ###
	// #   Pi-bus operation code					#
	// ### ------------------------------------------------------ ###

	void processOPCODE_SX()
	{
		sc_uint<2> acsadr_rx=ACSADR_RX.read();

		if (ACSSIZ_RX.read() == r_wrd)
			OPCODE_SX.write(p_opc_wdu);
		else if ((ACSSIZ_RX.read() == r_hlf) && (acsadr_rx[1]==0))
			OPCODE_SX.write(p_opc_hw0);
		else if ((ACSSIZ_RX.read() == r_hlf) && (acsadr_rx[1]==1))
			OPCODE_SX.write(p_opc_hw1);
		else if ((ACSSIZ_RX.read() == r_byt) && (acsadr_rx.range(1,0)==0x0))
			OPCODE_SX.write(p_opc_by0);
		else if ((ACSSIZ_RX.read() == r_byt) && (acsadr_rx.range(1,0)==0x1))
			OPCODE_SX.write(p_opc_by1);
		else if ((ACSSIZ_RX.read() == r_byt) && (acsadr_rx.range(1,0)==0x2))
			OPCODE_SX.write(p_opc_by2);
		else if ((ACSSIZ_RX.read() == r_byt) && (acsadr_rx.range(1,0)==0x3))
			OPCODE_SX.write(p_opc_by3);
		else
			OPCODE_SX.write(p_opc_nop);
	}

	void processPIOPC_SX()
	{
		if (P_STAT_RX.read() == p_watr)
			PIOPC_SX.write(p_opc_nop);
		else if ((ANSFULL_SX.read()==1) && (ACSTYP_RX.read()==r_red))
			PIOPC_SX.write(p_opc_nop);
		else
			PIOPC_SX.write(OPCODE_SX.read());
	}

	// ### ------------------------------------------------------ ###
	// #   when a data has been read on the bus, push it into the	#
	// # Answer Fifo.						#
	// #   The value pushed into the Answer Fifo contains : the	#
	// # data, the address of the read data and a flag signaling	#
	// # that the read data is the least one.			#
	// ### ------------------------------------------------------ ###

	void processDATREAD_SX()
	{
		if (PI_READ_RX.read()==p_red_red)
			DATREAD_SX.write(DATARDY_SX.read());
		else
			DATREAD_SX.write(0);
	}

	void processANSPSH_SX()
	{
		if ((P_STAT_RX.read() == p_nadr) || (DATREAD_SX.read() == 1))
			ANSPSH_SX.write(1);
		else if ((P_STAT_RX.read() == p_ldat) || (DATREAD_SX.read() == 1))
			ANSPSH_SX.write(1);
		else if ((P_STAT_RX.read() == p_watr) || (DATREAD_SX.read() == 1))
			ANSPSH_SX.write(1);
		else
			ANSPSH_SX.write(0);
	}

	void processANSDIN_SX()
	{
		sc_biguint<65> ansdin_sx=0x0;
		sc_uint<32> pi_adr_rx=PI_ADR_RX.read();
		sc_uint<32> pi_d=PI_D.read();

		if (P_STAT_RX.read() == p_nadr)
			ansdin_sx[0]=a_cnt;
		else
			ansdin_sx[0]=a_end;

		for (int i=31;i>=0;i--)
			ansdin_sx[i+1]=pi_adr_rx[i];
		for (int i=31;i>=0;i--)
			ansdin_sx[i+33]=pi_d[i];
		ANSDIN_SX.write(ansdin_sx);
	}

	// ### ------------------------------------------------------ ###
	// #   initialize the address and the access counters when a	#
	// # request has been accepted. Also, initialize the access	#
	// # type (read or write), the size (word, half word or byte)	#
	// # and the access mode (locked or unlocked). Extract the	#
	// # request from the Request Fifo.				#
	// ### ------------------------------------------------------ ###

	void processINITACS_SX()
	{
		if ((P_STAT_RX.read() == p_rqst) && (PI_GNT.read() == 1))
			INITACS_SX.write(1);
		else if ((P_STAT_RX.read() == p_idle) & (PI_GNT.read() == 1))
			INITACS_SX.write(1);
		else if ((P_STAT_RX.read() == p_watr) && (RQSEMTY_SX.read() == 0))
			INITACS_SX.write(1);
		else if ((P_STAT_RX.read() == p_fadr) && (ACSMOD_RX.read() == 1) || (RQSEMTY_SX.read() == 0))
			INITACS_SX.write(1);
		else if ((P_STAT_RX.read() == p_nadr) && (ACSMOD_RX.read() == 1) || (RQSEMTY_SX.read() == 0))
			INITACS_SX.write(1);
		else
			INITACS_SX.write(0);
	}

	void processRQSPOP_SX()
	{
		RQSPOP_SX.write(INITACS_SX.read());
	}

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

	void processADRCNT_SX()
	{
		sc_uint<32> acsadr_rx=ACSADR_RX.read();
		sc_uint<32> adrcry_sx=ADRCRY_SX.read();

		sc_uint<32> adrcnt_sx=0x0;

		for (int i=31;i>0;i--)
			adrcnt_sx[i]=acsadr_rx[i] ^ adrcry_sx[i];
		ADRCNT_SX.write(adrcnt_sx);
	}

	void processADRCRY_SX()
	{
		sc_uint<32> adrcry_sx=0;
		sc_uint<32> acsadr_rx=ACSADR_RX.read();

		adrcry_sx[2]=((P_STAT_RX.read()==p_nadr) && ((DATARDY_SX.read()==0) || (ANSFULL_SX.read()==1)))?0:1;
		adrcry_sx[3]=acsadr_rx[2] & adrcry_sx[2];
		//adrcry_sx.range(31,4)=0x0;
		//adrcry_sx.range(1,0)=0x0;
		ADRCRY_SX.write(adrcry_sx);
	}

	// ### ------------------------------------------------------ ###
	// #   access counter :						#
	// #     - do not decrement the remaining access counter if the	#
	// #       previous access has not been finished (data not	#
	// #       ready) or if the present access cannot be acheived	#
	// #       (Answer Fifo is full)				#
	// ### ------------------------------------------------------ ###

	void processACSCNT_SX()
	{
		sc_uint<4> acscnt_rx=ACSCNT_RX.read();

		if (!((P_STAT_RX.read()==p_nadr) && ((DATARDY_SX.read()==0) || (ANSFULL_SX.read()==1))))
			ACSCNT_SX.write(acscnt_rx-1);
	}

	void processMOREADR_SX()
	{
			if (ACSCNT_RX.read()==0x0)
				MOREADR_SX.write(0);
			else
				MOREADR_SX.write(1);
	}

	// ### ------------------------------------------------------ ###
	// #   data transfered						#
	// ### ------------------------------------------------------ ###

	void processDATARDY_SX()
	{
			if ((PI_ACK.read()==p_ack_rdy) || (PI_ACK.read()==p_ack_rdm))
				DATARDY_SX.write(1);
			else
				DATARDY_SX.write(0);
	}

	// ### ------------------------------------------------------ ###
	// #   extract the bloc number from the address transmitted on	#
	// # the Pi_bus for the snoop-invalidate function		#
	// ### ------------------------------------------------------ ###

	void processSNPBLC_SX()
	{
		sc_uint<32> pi_a=PI_A.read();

		SNPBLC_SX.write(pi_a.range(5,4));
	}

	// ### ------------------------------------------------------ ###
	// #   read the tag of blocs present in the cache for the snoop	#
	// # invalidate function.					#
	// ### ------------------------------------------------------ ###

	void processSNPBTAG_SX()
	{
		if (SNPBLC_SX.read()==0x0)
			SNPBTAG_SX.write(TAG0_RX.read());
		else if (SNPBLC_SX.read()==0x1)
			SNPBTAG_SX.write(TAG1_RX.read());
		else if (SNPBLC_SX.read()==0x2)
			SNPBTAG_SX.write(TAG2_RX.read());
		else if (SNPBLC_SX.read()==0x3)
			SNPBTAG_SX.write(TAG3_RX.read());
	}

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

	void processSNPCMPT_SX()
	{
		sc_uint<32> pi_a=PI_A.read();

		SNPCMPT_SX.write(SNPBTAG_SX.read() ^ pi_a.range(31,6));
	}

	void processSNPINVL_SX()
	{
			if ((SNPCMPT_SX.read()==0x0) && (PI_READ.read()==p_red_wrt))
				SNPINVL_SX.write(1);
			else
				SNPINVL_SX.write(0);
	}

	void processINVPRS_SX()
	{
		if (SNPBLC_SX.read()==0x0)
			INVPRS0_SX.write(SNPINVL_SX.read());
		else
			INVPRS0_SX.write(0);
		if (SNPBLC_SX.read()==0x1)
			INVPRS1_SX.write(SNPINVL_SX.read());
		else
			INVPRS1_SX.write(0);
		if (SNPBLC_SX.read()==0x2)
			INVPRS2_SX.write(SNPINVL_SX.read());
		else
			INVPRS2_SX.write(0);
		if (SNPBLC_SX.read()==0x3)
			INVPRS3_SX.write(SNPINVL_SX.read());
		else
			INVPRS3_SX.write(0);
	}


	// ### ------------------------------------------------------ ###
	// #   assign output:						#
	// #     - processor's interface				#
	// ### ------------------------------------------------------ ###

	void processD_FRZ()
	{
		if ((C_STAT_RX.read()==c_redy) && (MISS_SX.read()==0) && (WRTPEND_SX.read()==0))
			D_FRZ.write(0);
		else
			D_FRZ.write(1);
	}

	void processD_OUT()
	{
     	if (UCACVLD_RX.read()==1)
			D_OUT.write(UCACDAT_RX.read());
		else
			D_OUT.write(DATA_SX.read());
	}

	void processPI_DATA_LINES()
	{
		if ((PIDATEN_SX.read()==1)&&(CK_SX.read()==0))
			PI_D.write(ACSDAT_RX.read()); // FPX hack
	}

	// ### ------------------------------------------------------ ###
	// #   assign output:						#
	// #     - Pi-bus's lines					#
	// ### ------------------------------------------------------ ###

	void processPI_IREQ()
	{
		if ((P_STAT_RX.read()==p_rqst) || (P_STAT_RX.read()==p_fadr))
			PI_IREQ.write(1);
		else
			PI_IREQ.write(0);
	}

	void processPI_LINES()
	{
		sc_uint<32> acsadr_rx=ACSADR_RX.read();

		if ((PILINEN_SX.read()==1)&&(CK_SX.read()==0))
		{
			PI_A.write(acsadr_rx.range(31,2));

			if (DEFLTEN_SX.read()==1)
				PI_READ.write(p_red_red);
			else if (ACSTYP_RX.read()==r_red)
				PI_READ.write(p_red_red);
			else
				PI_READ.write(p_red_wrt);

			if (DEFLTEN_SX.read()==1)
				PI_LOCK.write(0);
			else if ((P_STAT_RX.read()==p_fadr)&&(MOREADR_SX.read()==0))
				PI_LOCK.write(ACSMOD_RX.read());
			else if ((P_STAT_RX.read()==p_nadr)&&(MOREADR_SX.read()==0))
				PI_LOCK.write(ACSMOD_RX.read());
			else
				PI_LOCK.write(1);

			if (DEFLTEN_SX.read()==1)
				PI_OPC.write(p_opc_nop);
			else if (P_STAT_RX.read()==p_fadr)
				PI_OPC.write(PIOPC_SX.read());
			else if (P_STAT_RX.read()==p_nadr)
				PI_OPC.write(PIOPC_SX.read());
			else
				PI_OPC.write(p_opc_nop);
		}
	}


	void processCLOCK()
	{
	// ### ------------------------------------------------------ ###
	// #   request's parameters:					#
	// #     - access type, mode and size				#
	// ### ------------------------------------------------------ ###

  		ACSTYP_RX.write((INITACS_SX.read()==1)?RQSTYP_SX.read():ACSTYP_RX.read());
  		ACSMOD_RX.write((INITACS_SX.read()==1)?RQSMOD_SX.read():ACSMOD_RX.read());
  		ACSSIZ_RX.write((INITACS_SX.read()==1)?RQSSIZ_SX.read():ACSSIZ_RX.read());
  		ACSDAT_RX.write((INITACS_SX.read()==1)?RQSDAT_SX.read():ACSDAT_RX.read());

	// ### ------------------------------------------------------ ###
	// #   address and access counters				#
	// ### ------------------------------------------------------ ###

  		ACSADR_RX.write((INITACS_SX.read()==1)?RQSADR_SX.read():ADRCNT_SX.read());
  		ACSCNT_RX.write((INITACS_SX.read()==1)?RQSACSC_SX.read():ACSCNT_SX.read());

	// ### ------------------------------------------------------ ###
	// #   save the address and the opcode sent on the Pi-bus	#
	// ### ------------------------------------------------------ ###

  		PI_ADR_RX.write(ACSADR_RX.read());
  		PI_READ_RX.write((ACSTYP_RX.read()==r_red)?p_red_red:p_red_wrt);

	// ### ------------------------------------------------------ ###
	// #   request fifo						#
	// ### ------------------------------------------------------ ###

  		RQSREG0_RX.write((WRTRQS0_SX.read()==1)?RQSDIN_SX.read():RQSREG0_RX.read());

	// ### ------------------------------------------------------ ###
	// #   answer fifo						#
	// ### ------------------------------------------------------ ###

		if ((WRTANS0_SX.read()==1) && (SELANS0_SX.read()==a_new))
			ANSREG0_RX.write(ANSDIN_SX.read());
		else if ((WRTANS0_SX.read()==1) && (SELANS0_SX.read()==a_sft))
			ANSREG0_RX.write(ANSREG1_RX.read());

		if (WRTANS1_SX.read()==1)
			ANSREG1_RX.write(ANSDIN_SX.read());

	// ### ------------------------------------------------------ ###
	// #   state machines' state register				#
	// ### ------------------------------------------------------ ###

	  	C_STAT_RX.write(C_NXTS_SX);
	  	R_STAT_RX.write(R_NXTS_SX);
	  	P_STAT_RX.write(P_NXTS_SX);
	  	A_STAT_RX.write(A_NXTS_SX);

	// ### ------------------------------------------------------ ###
	// #   uncachable data						#
	// ### ------------------------------------------------------ ###

		if (WRTUCAC_SX.read()==1)
			UCACDAT_RX.write(ANSDATA_SX.read());

		if (WRTUCAC_SX.read()==1)
			UCACADR_RX.write(ANSADR_SX.read());

		if (RESET_RX.read()==1)
			UCACVLD_RX.write(0);
		else if (WRTUCAC_SX.read()==1)
			UCACVLD_RX.write(1);
		else if ((D_ACK.read()==1)&&(D_RQ.read()==1))
			UCACVLD_RX.write(0);

	// ### ------------------------------------------------------ ###
	// #   fetched bloc						#
	// ### ------------------------------------------------------ ###

		if (WRTADR_SX.read()==1)
			RBLOCAD_RX.write(ANSADR_SX.read());

		if (WRTDAT0_SX.read()==1)
			REDBLC0_RX.write(ANSDATA_SX.read());

		if (WRTDAT1_SX.read()==1)
			REDBLC1_RX.write(ANSDATA_SX.read());

		if (WRTDAT2_SX.read()==1)
			REDBLC2_RX.write(ANSDATA_SX.read());

		if (WRTDAT3_SX.read()==1)
			REDBLC3_RX.write(ANSDATA_SX.read());

	// ### ------------------------------------------------------ ###
	// #   update the cache						#
	// ### ------------------------------------------------------ ###

		if ((WRTBLOC_SX.read()==1)&&(WRTBNBR_SX.read()==0x0))
			BLOC0_RX.write(BLOCDIN_SX.read());

		if ((WRTBLOC_SX.read()==1)&&(WRTBNBR_SX.read()==0x1))
			BLOC1_RX.write(BLOCDIN_SX.read());

		if ((WRTBLOC_SX.read()==1)&&(WRTBNBR_SX.read()==0x2))
			BLOC2_RX.write(BLOCDIN_SX.read());

		if ((WRTBLOC_SX.read()==1)&&(WRTBNBR_SX.read()==0x3))
			BLOC3_RX.write(BLOCDIN_SX.read());

		if ((WRTBLOC_SX.read()==1)&&(WRTBNBR_SX.read()==0x0))
			TAG0_RX.write(WRTBTAG_SX.read());

		if ((WRTBLOC_SX.read()==1)&&(WRTBNBR_SX.read()==0x1))
			TAG1_RX.write(WRTBTAG_SX.read());

		if ((WRTBLOC_SX.read()==1)&&(WRTBNBR_SX.read()==0x2))
			TAG2_RX.write(WRTBTAG_SX.read());

		if ((WRTBLOC_SX.read()==1)&&(WRTBNBR_SX.read()==0x3))
			TAG3_RX.write(WRTBTAG_SX.read());

		if ((RSTPRS0_SX.read()==1) || (INVPRS0_SX.read()==1))
			PRES0_RX.write(0);
		else if ((SNIPRS0_SX.read()==1) && (WRIPRS0_SX.read()==1))
			PRES0_RX.write(0);
		else if ((WRTBLOC_SX.read()==1) && (WRTBNBR_SX.read()==0x0))
			PRES0_RX.write(1);

		if ((RSTPRS1_SX.read()==1) || (INVPRS1_SX.read()==1))
			PRES1_RX.write(0);
		else if ((SNIPRS1_SX.read()==1) && (WRIPRS1_SX.read()==1))
			PRES1_RX.write(0);
		else if ((WRTBLOC_SX.read()==1) && (WRTBNBR_SX.read()==0x1))
			PRES1_RX.write(1);

		if ((RSTPRS2_SX.read()==1) || (INVPRS2_SX.read()==1))
			PRES2_RX.write(0);
		else if ((SNIPRS2_SX.read()==1) && (WRIPRS2_SX.read()==1))
			PRES2_RX.write(0);
		else if ((WRTBLOC_SX.read()==1) && (WRTBNBR_SX.read()==0x2))
				PRES2_RX.write(1);

		if ((RSTPRS3_SX.read()==1) || (INVPRS3_SX.read()==1))
			PRES3_RX.write(0);
		else if ((SNIPRS3_SX.read()==1) && (WRIPRS3_SX.read()==1))
			PRES3_RX.write(0);
		else if ((WRTBLOC_SX.read()==1) && (WRTBNBR_SX.read()==0x3))
			PRES3_RX.write(1);

	// ### ------------------------------------------------------ ###
	// #   miscellaneous:						#
	// #     - reset register					#
	// #     - reset counter					#
	// ### ------------------------------------------------------ ###

		if (RESET_RX.read()==1)
			RSTCNT_RX.write(0x0);
		else
			RSTCNT_RX.write(RSTCNT_SX.read());

		RESET_RX.write(!RESET_N.read());
	}

	void processD_BERR_N()
	{
		D_BERR_N.write(1);
	}

};
#endif
