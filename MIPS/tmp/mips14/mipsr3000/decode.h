// ### -------------------------------------------------------------- ###
// # file       : decode.h                                              #
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

#ifndef _DECODE
#define _DECODE
//#include "systemc.h"
#include <systemc.h>

SC_MODULE(decode)
{
	sc_in<sc_uint<32>> 	IR_RI;
	sc_out<sc_uint<8>> 	OPCOD_SD;
	sc_in<sc_uint<25>> 	I_TYPE_SD;

	sc_signal<sc_uint<8>> 	COP0_SD;
	sc_signal<bool> 	I_ILLG_SD;
	sc_signal<bool> 	I_JFMT_SD;
	sc_signal<bool> 	I_IFMT_SD;
	sc_signal<bool> 	I_RFMT_SD;

	sc_out<bool> 		I_DUSE_SD;
	sc_out<bool> 		I_READS_SD;
	sc_out<bool> 		I_READT_SD;

	sc_signal<bool> 	I_OSGND_SD;
	sc_signal<bool> 	I_WRT31_SD;
	sc_signal<bool> 	I_WRITE_SD;

	sc_out<bool> 		I_BRNCH_SD;
	sc_out<bool> 		SWAP_SD;
	sc_out<sc_uint<5>> 	COP0D_SD;
	sc_out<sc_uint<5>> 	RS_SD;
	sc_out<sc_uint<5>> 	RT_SD;
	sc_out<sc_uint<5>> 	RD_SD;

	sc_signal<sc_uint<32>> COP0OP_SD;
	sc_signal<bool> 	IMDSGN_SD;
	sc_signal<sc_uint<16>>	IMDSEX_SD;
	sc_out<sc_uint<32>>	IOPER_SD;
	sc_signal<sc_uint<32>>	OFFSET_SD;
	sc_signal<sc_uint<5>>	S_CMP_T_SD;
	sc_signal<bool>		S_EQ_T_SD;
	sc_signal<bool>		S_LT_Z_SD;
	sc_signal<bool>		S_LE_Z_SD;

	sc_out<bool> 		SLEEP_SD;
	sc_out<sc_uint<32>> 	NEXTPC_SD;
	sc_out<bool> 		ILLGINS_XD;
	sc_out<bool> 		C0UNUSE_XD;

	sc_signal<sc_uint<32>> JMPADR_SD;
	sc_signal<sc_uint<32>> BRAADR_SD;
	sc_signal<sc_uint<32>> SEQADR_SD;

	sc_in<sc_uint<32>> 	BADVADR_RM;
	sc_in<sc_uint<32>> 	NEXTSR_RX;
	sc_in<sc_uint<32>> 	EPC_RX;
	sc_in<sc_uint<32>> 	CAUSE_RX;
	sc_in<sc_uint<32>> 	LO_RW;
	sc_in<sc_uint<32>> 	HI_RW;
	sc_in<sc_uint<32>> 	SOPER_SD;
	sc_in<sc_uint<32>> 	TOPER_SD;
	sc_in<sc_uint<32>> 	SR_RI;
	sc_in<sc_uint<32>> 	NEXTPC_RD;

	SC_CTOR(decode)/*:
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
	*/
	{
		SC_METHOD(processCOP0_SD);
		sensitive 
			<< IR_RI;
		SC_METHOD(processOPCOD_SD);
		sensitive 
			<< IR_RI 
			<< COP0_SD;
		SC_METHOD(processI_ILLG_SD);
		sensitive 
			<< I_TYPE_SD;
		SC_METHOD(processI_JFMT_SD);
		sensitive 
			<< I_TYPE_SD;
		SC_METHOD(processI_IFMT_SD);
		sensitive 
			<< I_TYPE_SD;
		SC_METHOD(processI_RFMT_SD);
		sensitive 
			<< I_TYPE_SD;
		SC_METHOD(processI_DUSE_SD);
		sensitive 
			<< I_TYPE_SD;
		SC_METHOD(processI_READS_SD);
		sensitive 
			<< I_TYPE_SD;
		SC_METHOD(processI_READT_SD);
		sensitive 
			<< I_TYPE_SD;
		SC_METHOD(processI_OSGND_SD);
		sensitive 
			<< I_TYPE_SD;
		SC_METHOD(processI_WRT31_SD);
		sensitive 
			<< I_TYPE_SD;
		SC_METHOD(processI_WRITE_SD);
		sensitive 
			<< I_TYPE_SD;
		SC_METHOD(processI_BRNCH_SD);
		sensitive 
			<< I_TYPE_SD;
		SC_METHOD(processSWAP_SD);
		sensitive 
			<< OPCOD_SD;
		SC_METHOD(processCOP0D_SD);
		sensitive 
			<< COP0D_SD;
		SC_METHOD(processRS_SD);
		sensitive 
			<< IR_RI;
		SC_METHOD(processRT_SD);
		sensitive 
			<< IR_RI;
		SC_METHOD(processRD_SD);
		sensitive 
			<< IR_RI 
			<< I_WRT31_SD 
			<< I_WRITE_SD 
			<< I_RFMT_SD 
			<< I_IFMT_SD;
		SC_METHOD(processCOP0OP_SD);
		sensitive 
			<< IR_RI 
			<< BADVADR_RM 
			<< NEXTSR_RX 
			<< EPC_RX 
			<< CAUSE_RX;
		SC_METHOD(processIMDSGN_SD);
		sensitive 
			<< IR_RI;
		SC_METHOD(processIMDSEX_SD);
		sensitive 
			<< IR_RI 
			<< IMDSGN_SD 
			<< I_OSGND_SD;
		SC_METHOD(processIOPER_SD);
		sensitive 
			<< IR_RI 
			<< OPCOD_SD 
			<< SEQADR_SD 
			<< LO_RW 
			<< HI_RW 
			<< IMDSEX_SD;
		SC_METHOD(processOFFSET_SD);
		sensitive 
			<< IR_RI 
			<< IMDSEX_SD;
		SC_METHOD(processS_CMP_T_SD);
		sensitive 
			<< SOPER_SD 
			<< TOPER_SD;
		SC_METHOD(processS_EQ_T_SD);
		sensitive 
			<< S_CMP_T_SD;
		SC_METHOD(processS_LT_Z_SD);
		sensitive 
			<< SOPER_SD;
		SC_METHOD(processS_LE_Z_SD);
		sensitive 
			<< SOPER_SD;
		SC_METHOD(processSLEEP_SD);
		sensitive 
			<< OPCOD_SD;
		SC_METHOD(processNEXTPC_SD);
		sensitive 
			<< SOPER_SD 
			<< JMPADR_SD 
			<< S_EQ_T_SD 
			<< S_LT_Z_SD 
			<< S_LE_Z_SD 
			<< BRAADR_SD 
			<< SEQADR_SD;
		SC_METHOD(processILLGINS_XD);
		sensitive 
			<< I_ILLG_SD;
		SC_METHOD(processC0UNUSE_XD);
		sensitive 
			<< SR_RI 
			<< OPCOD_SD;
		SC_METHOD(processJMPADR_SD);
		sensitive 
			<< NEXTPC_RD 
			<< IR_RI;
		SC_METHOD(processBRAADR_SD);
		sensitive 
			<< NEXTPC_RD 
			<< OFFSET_SD;
		SC_METHOD(processSEQADR_SD);
		sensitive 
			<< NEXTPC_RD;
	}

	// ### ------------------------------------------------------ ###
	// #   extract the opcode from instruction register		#
	// ### ------------------------------------------------------ ###

	void processCOP0_SD()
	{
		sc_uint<32> ir_ri = IR_RI.read();

		if (ir_ri[25]==0)
			COP0_SD.write((cop0_g 
			<< 6) | (ir_ri.range(22,21) 
			<< 3) | (ir_ri.range(24,23))) ;
		else
			COP0_SD.write((cop0_g 
			<< 6) | 0x20 | ir_ri.range(4,0));
	}

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

	void processOPCOD_SD()
	{
		sc_uint<32> ir_ri = IR_RI.read();

		if (ir_ri.range(31,26)==special_i)
			OPCOD_SD.write((special_g 
			<< 6) | ir_ri.range(5,0));
		else if (ir_ri.range(31,26)==bcond_i)
			OPCOD_SD.write((bcond_g 
			<< 5) | ir_ri.range(20,16));
		else if (ir_ri.range(31,26)==cop0_i)
			OPCOD_SD.write(COP0_SD.read());
		else
			OPCOD_SD.write((others_g 
			<< 6) | ir_ri.range(31,26));
	}

	void processI_ILLG_SD()
	{
		sc_uint<25> i_type_sd=I_TYPE_SD.read();

		I_ILLG_SD.write(i_type_sd[24]);
	}

	void processI_JFMT_SD()
	{
		sc_uint<25> i_type_sd=I_TYPE_SD.read();

		I_JFMT_SD.write(i_type_sd[23]);
	}

	void processI_IFMT_SD()
	{
		sc_uint<25> i_type_sd=I_TYPE_SD.read();

		I_IFMT_SD.write(i_type_sd[22]);
	}

	void processI_RFMT_SD()
	{
		sc_uint<25> i_type_sd=I_TYPE_SD.read();

		I_RFMT_SD.write(i_type_sd[21]);
	}

	void processI_DUSE_SD()
	{
		sc_uint<25> i_type_sd=I_TYPE_SD.read();

		I_DUSE_SD.write(i_type_sd[20]);
	}

	void processI_READS_SD()
	{
		sc_uint<25> i_type_sd=I_TYPE_SD.read();

		I_READS_SD.write(i_type_sd[18]);
	}

	void processI_READT_SD()
	{
		sc_uint<25> i_type_sd=I_TYPE_SD.read();

		I_READT_SD.write(i_type_sd[17]);
	}

	void processI_OSGND_SD()
	{
		sc_uint<25> i_type_sd=I_TYPE_SD.read();

		I_OSGND_SD.write(i_type_sd[16]);
	}

	void processI_WRT31_SD()
	{
		sc_uint<25> i_type_sd=I_TYPE_SD.read();

		I_WRT31_SD.write((i_type_sd[8] | i_type_sd[7]) & i_type_sd[6]);
	}

	void processI_WRITE_SD()
	{
		sc_uint<25> i_type_sd=I_TYPE_SD.read();

		I_WRITE_SD.write(i_type_sd[8] | i_type_sd[7]);
	}

	void processI_BRNCH_SD()
	{
		sc_uint<25> i_type_sd=I_TYPE_SD.read();

		I_BRNCH_SD.write(i_type_sd[5]);
	}

		// ### ------------------------------------------------------ ###
		// #   swap instruction						#
		// ### ------------------------------------------------------ ###


	void processSWAP_SD()
	{
		SWAP_SD.write(((int)OPCOD_SD.read() == swap_i) ? 1 : 0);
	}

		// ### ------------------------------------------------------ ###
		// #   extract special destination registers number from the	#
		// # instruction.						#
		// #								#
		// #   extract source and destination registers number from the	#
		// # instruction. Destination register number is set to r0 when	#
		// # not used.							#
		// ### ------------------------------------------------------ ###

	void processCOP0D_SD()
	{
		sc_uint<32> ir_ri=IR_RI.read();

		COP0D_SD.write(ir_ri.range(15,11));
	}

	void processRS_SD()
	{
		sc_uint<32> ir_ri=IR_RI.read();

		RS_SD.write(ir_ri.range(25,21));
	}

	void processRT_SD()
	{
		sc_uint<32> ir_ri=IR_RI.read();

		RT_SD.write(ir_ri.range(20,16));
	}

	void processRD_SD()
	{
		sc_uint<32> ir_ri=IR_RI.read();

		if (I_WRT31_SD.read() == 1)
			RD_SD.write(0x1F);
		else if ((I_WRITE_SD.read() & I_RFMT_SD.read()) == 1)
			RD_SD.write( ir_ri.range(15,11));
		else if ((I_WRITE_SD.read() & I_IFMT_SD.read()) == 1)
			RD_SD.write( ir_ri.range(20,16));
		else
			RD_SD.write( 0x0);
	}

		// ### ------------------------------------------------------ ###
		// #   read the special register to be transfered into an	#
		// # integer register (mfc0 instruction)			#
		// ### ------------------------------------------------------ ###

	void processCOP0OP_SD()
	{
		sc_uint<32> ir_ri=IR_RI.read();

		if ((int)ir_ri.range(15,11)==badvadr_s)
			COP0OP_SD.write(BADVADR_RM.read());
		else if ((int)ir_ri.range(15,11)==status_s)
			COP0OP_SD.write(NEXTSR_RX.read());
		else if ((int)ir_ri.range(15,11)==epc_s)
			COP0OP_SD.write(EPC_RX.read());
		else if ((int)ir_ri.range(15,11)==cause_s)
			COP0OP_SD.write(CAUSE_RX.read());
		else
			COP0OP_SD.write(0x0);
	}

		// ### ------------------------------------------------------ ###
		// #   compute immediate operand				#
		// ### ------------------------------------------------------ ###

	void processIMDSGN_SD()
	{
		sc_uint<32> ir_ri=IR_RI.read();

		IMDSGN_SD.write(ir_ri[15]);
	}

	void processIMDSEX_SD()
	{
		sc_uint<32> ir_ri=IR_RI.read();

		if ((IMDSGN_SD.read() == 1) && (I_OSGND_SD.read() == 1))
			IMDSEX_SD.write(0xFFFF);
		else
			IMDSEX_SD.write(0x0);
	}

	void processIOPER_SD()
	{
		sc_uint<32> ir_ri=IR_RI.read();

		if (((int)OPCOD_SD.read() == bltzal_i)||
			((int)OPCOD_SD.read() == jalr_i)||
			((int)OPCOD_SD.read() == jal_i)||
			((int)OPCOD_SD.read() == bgezal_i))
			IOPER_SD.write( SEQADR_SD.read());
		else if (((int)OPCOD_SD.read() == sra_i)||
				((int)OPCOD_SD.read() == srl_i)||
				((int)OPCOD_SD.read() == sll_i))
			IOPER_SD.write( ir_ri.range(13,6));
		else if ((int)OPCOD_SD.read() == lui_i)
			IOPER_SD.write( ir_ri.range(15,0)<<16);
		else if ((int)OPCOD_SD.read() == mflo_i)
			IOPER_SD.write(LO_RW.read());
		else if ((int)OPCOD_SD.read() == mfhi_i)
			IOPER_SD.write(HI_RW.read());
		else
			IOPER_SD.write(IMDSEX_SD.read() 
			<< 16 | ir_ri.range(15,0));
	}

		// ### ------------------------------------------------------ ###
		// #   compute the offset to be added to the current		#
		// # instruction address :					#
		// #   - short offset for conditional branches			#
		// ### ------------------------------------------------------ ###

	void processOFFSET_SD()
	{
		sc_uint<32> imdsex_sd=IMDSEX_SD.read();
		sc_uint<32> ir_ri=IR_RI.read();

		OFFSET_SD.write(imdsex_sd.range(13,0) 
			<< 18 | ir_ri.range(15,0) 
			<< 2);
	}

		// ### ------------------------------------------------------ ###
		// #   conditional branches' condition				#
		// ### ------------------------------------------------------ ###

	void processS_CMP_T_SD()
	{
		S_CMP_T_SD.write(SOPER_SD.read() ^ TOPER_SD.read());
	}

	void processS_EQ_T_SD()
	{
		S_EQ_T_SD.write((S_CMP_T_SD.read()==0x0)?1:0);
	}

	void processS_LT_Z_SD()
	{
		sc_uint<32> soper_sd=SOPER_SD.read();

		S_LT_Z_SD.write((soper_sd[31]==1)?1:0);
	}

	void processS_LE_Z_SD()
	{
		sc_uint<32> soper_sd=SOPER_SD.read();

		S_LE_Z_SD.write(((soper_sd[31]==1)||(soper_sd==0x0))?1:0);
	}

		// ### ------------------------------------------------------ ###
		// #   decoding the sleep instruction. This signal is used to	#
		// # stall the pipeline.					#
		// ### ------------------------------------------------------ ###


	void processSLEEP_SD()
	{
		SLEEP_SD.write(((int)OPCOD_SD.read() == sleep_i) ? 1 : 0);
	}

		// ### ------------------------------------------------------ ###
		// #   next instruction's address				#
		// ### ------------------------------------------------------ ###

	void processNEXTPC_SD()
	{
		if (((int)OPCOD_SD.read() == jr_i)||((int)OPCOD_SD.read() == jalr_i))
			NEXTPC_SD.write(SOPER_SD.read());
		else if (((int)OPCOD_SD.read() == j_i)||((int)OPCOD_SD.read() == jal_i))
			NEXTPC_SD.write(JMPADR_SD.read());
		else if (
					(((int)OPCOD_SD.read() == beq_i)&&(S_EQ_T_SD.read() == 1))||
					(((int)OPCOD_SD.read() == bne_i)&&(S_EQ_T_SD.read() == 0))||
					(((int)OPCOD_SD.read() == bltz_i)&&(S_LT_Z_SD.read() == 1))||
					(((int)OPCOD_SD.read() == bltzal_i)&&(S_LT_Z_SD.read() == 1))||
					(((int)OPCOD_SD.read() == blez_i)&&(S_LE_Z_SD.read() == 1))||
					(((int)OPCOD_SD.read() == bgtz_i)&&(S_LE_Z_SD.read() == 0))||
					(((int)OPCOD_SD.read() == bgez_i)&&(S_LT_Z_SD.read() == 0))||
					(((int)OPCOD_SD.read() == bgezal_i)&&(S_LT_Z_SD.read() == 0))
				)
			NEXTPC_SD.write(BRAADR_SD.read());
		else
			NEXTPC_SD.write(SEQADR_SD.read());
	}

		// ### ------------------------------------------------------ ###
		// #   exception detected during the Instruction Decode cycle:	#
		// #								#
		// #     - illegal instruction					#
		// #     - coprocessor zero unusable				#
		// ### ------------------------------------------------------ ###

	void processILLGINS_XD()
	{
		ILLGINS_XD.write(I_ILLG_SD.read());
	}

	void processC0UNUSE_XD()
	{
		sc_uint<32> sr_ri=SR_RI.read();

		if (((int)OPCOD_SD.read() == rfe_i)||
			((int)OPCOD_SD.read() == mfc0_i)||
			((int)OPCOD_SD.read() == mtc0_i))
			C0UNUSE_XD.write(sr_ri[1] & (!sr_ri[28]));
		else
			C0UNUSE_XD.write(0);
	}

	void processJMPADR_SD()
	{
		sc_uint<32> nextpc_rd=NEXTPC_RD.read();
		sc_uint<32> ir_ri = IR_RI.read();
		sc_uint<32> jres = 0x0;

		jres.range(31,28)=nextpc_rd.range(31,28);
		jres.range(27,2)=ir_ri.range(25,0);
		JMPADR_SD.write(jres);
	}

	void processBRAADR_SD()
	{
		BRAADR_SD.write(NEXTPC_RD.read() + OFFSET_SD.read());
	}

	void processSEQADR_SD()
	{
		SEQADR_SD.write(NEXTPC_RD.read() + 4);
	}

};
#endif

