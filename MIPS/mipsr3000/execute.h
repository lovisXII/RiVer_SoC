// ### -------------------------------------------------------------- ###
// # file       : execute.h                                             #
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

#ifndef _EXECUTE
#define _EXECUTE

#include <systemc.h>
#include "constants.h"

SC_MODULE(execute)
{
	sc_in<sc_uint<32> > 	SR_RI;
	sc_in<sc_uint<25> > 	I_TYPE_RD;
	sc_in<sc_uint<8> > 	OPCOD_RD;
	sc_in<sc_uint<32> > 	IOPER_RD;

	sc_in<sc_uint<32> > 	NEXTSR_RX;
	sc_in<sc_uint<32> > 	NEXTPC_RD;
	sc_in<bool> 		SWAP_RD;
	sc_in<bool> 		I_BERR_N;
	
	sc_out<bool> 		I_IFMT_SE;
	sc_out<bool> 		I_EUSE_SE;

	sc_out<bool> 		I_READS_SE;
	sc_out<bool> 		I_READT_SE;

	sc_out<sc_uint<3> >	I_OPER_SE;
	sc_out<sc_uint<2> >	I_LOGIC_SE;
	sc_out<bool>		I_SUB_SE;
	sc_out<bool>		I_RIGHT_SE;
	sc_out<bool>		I_RSGND_SE;
	sc_out<bool>		I_OVRF_SE;
	sc_out<bool>		I_MPDC_SE;
	sc_out<bool>		I_EPDC_SE;
	sc_out<bool>		I_WRITE_SE;

	sc_signal<bool>		I_BRNCH_SE;

	sc_in<sc_uint<32> >	X_SE;
	sc_in<sc_uint<32> >	Y_SE;
	sc_in<bool>		OVERFLW_SE;

	sc_out<sc_uint<32> >	XOPER_SE;
	sc_out<sc_uint<32> >	YOPER_SE;

	sc_out<bool>		WREDOPC_SE;
	sc_out<bool>		IABUSER_XE;
	sc_out<bool>		BREAK_XE;
	sc_out<bool>		SYSCALL_XE;
	sc_out<bool>		OVF_XE;
	sc_out<bool>		IAMALGN_XE;
	sc_out<bool>		IASVIOL_XE;
	sc_out<bool>		COPYCAP_SE;
	
	SC_CTOR(execute):
	SR_RI("SR_RI"),
	I_TYPE_RD("I_TYPE_RD"),
	OPCOD_RD("OPCOD_RD"),
	IOPER_RD("IOPER_RD"),
	
	NEXTSR_RX("NEXTSR_RX"),
	NEXTPC_RD("NEXTPC_RD"),
	SWAP_RD("SWAP_RD"),
	I_BERR_N("I_BERR_N"),

	I_IFMT_SE("I_IFMT_SE"),
	I_EUSE_SE("I_EUSE_SE"),

	I_READS_SE("I_READS_SE"),
	I_READT_SE("I_READT_SE"),

	I_OPER_SE("I_OPER_SE"),
	I_LOGIC_SE("I_LOGIC_SE"),
	I_SUB_SE("I_SUB_SE"),
	I_RIGHT_SE("I_RIGHT_SE"),
	I_RSGND_SE("I_RSGND_SE"),
	I_OVRF_SE("I_OVRF_SE"),
	I_MPDC_SE("I_MPDC_SE"),
	I_EPDC_SE("I_EPDC_SE"),
	I_WRITE_SE("I_WRITE_SE"),

	I_BRNCH_SE("I_BRNCH_SE"),

	X_SE("X_SE"),
	Y_SE("Y_SE"),
	OVERFLW_SE("OVERFLW_SE"),
	XOPER_SE("XOPER_SE"),
	YOPER_SE("YOPER_SE"),

	WREDOPC_SE("WREDOPC_SE"),
	IABUSER_XE("IABUSER_XE"),
	BREAK_XE("BREAK_XE"),
	SYSCALL_XE("SYSCALL_XE"),
	OVF_XE("OVF_XE"),
	IAMALGN_XE("IAMALGN_XE"),
	IASVIOL_XE("IASVIOL_XE"),
	COPYCAP_SE("COPYCAP_SE")
	{
		SC_METHOD(processI_EUSE_SE);
		sensitive << I_TYPE_RD;
		SC_METHOD(processI_IFMT_SE);
		sensitive << I_TYPE_RD;
		SC_METHOD(processI_READS_SE);
		sensitive << I_TYPE_RD;
		SC_METHOD(processI_READT_SE);
		sensitive << I_TYPE_RD;
		SC_METHOD(processI_OPER_SE);
		sensitive << I_TYPE_RD;
		SC_METHOD(processI_LOGIC_SE);
		sensitive << I_TYPE_RD;
		SC_METHOD(processI_SUB_SE);
		sensitive << I_TYPE_RD;
		SC_METHOD(processI_RIGHT_SE);
		sensitive << I_TYPE_RD;
		SC_METHOD(processI_RSGND_SE);
		sensitive << I_TYPE_RD;
		SC_METHOD(processI_OVRF_SE);
		sensitive << I_TYPE_RD;
		SC_METHOD(processI_MPDC_SE);
		sensitive << I_TYPE_RD;
		SC_METHOD(processI_EPDC_SE);
		sensitive << I_TYPE_RD;
		SC_METHOD(processI_WRITE_SE);
		sensitive << I_TYPE_RD;
		SC_METHOD(processI_BRNCH_SE);
		sensitive << I_TYPE_RD;
		SC_METHOD(processXOPER_SE);
		sensitive << SR_RI << OPCOD_RD << IOPER_RD << X_SE;
		SC_METHOD(processYOPER_SE);
		sensitive << I_IFMT_SE << IOPER_RD << Y_SE;
		SC_METHOD(processWREDOPC_SE);
		sensitive << I_BRNCH_SE;
		SC_METHOD(processIABUSER_XE);
		sensitive << I_BERR_N;
		SC_METHOD(processBREAK_XE);
		sensitive << OPCOD_RD;
		SC_METHOD(processSYSCALL_XE);
		sensitive << OPCOD_RD;
		SC_METHOD(processOVF_XE);
		sensitive << OVERFLW_SE << I_OVRF_SE;
		SC_METHOD(processIAMALGN_XE);
		sensitive << NEXTPC_RD;
		SC_METHOD(processIASVIOL_XE);
		sensitive << NEXTPC_RD << NEXTSR_RX << OPCOD_RD;
		SC_METHOD(processCOPYCAP_SE);
		sensitive << SWAP_RD;
	}

	void processI_EUSE_SE();

	void processI_IFMT_SE();

	void processI_READS_SE();

	void processI_READT_SE();

	void processI_OPER_SE();

	void processI_LOGIC_SE();

	void processI_SUB_SE();

	void processI_RIGHT_SE();

	void processI_RSGND_SE();

	void processI_OVRF_SE();

	void processI_MPDC_SE();

	void processI_EPDC_SE();

	void processI_WRITE_SE();

	void processI_BRNCH_SE();

	void processXOPER_SE();

	void processYOPER_SE();


  // ### ------------------------------------------------------ ###
  // #   save the address of the branch instruction		#
  // ### ------------------------------------------------------ ###

	void processWREDOPC_SE();

  // ### ------------------------------------------------------ ###
  // #  exceptions detected during the Execute cycle:		#
  // #								#
  // #     - instruction address bus error			#
  // #     - executing a break or a sysscall instruction		#
  // #     - detecting an overflow				#
  // #     - instruction address miss aligned (this exception is	#
  // #       reported in the Execute cycle since it is relative	#
  // #       to the instruction that has computed the address)	#
  // #     - instruction address violating system space (this	#
  // #       exception is reported in the Execute cycle since it	#
  // #       is relative to the instruction that has computed the	#
  // #       address). The current mode is extracted from the	#
  // #       Status register. In the case of a Restore From	#
  // #       Exception instruction modifying the Status register,	#
  // #       the old mode s taken into account (anticipating the	#
  // #       execution of the rfe).				#
  // ### ------------------------------------------------------ ###

	void processIABUSER_XE();

	void processBREAK_XE();

	void processSYSCALL_XE();

	void processOVF_XE();

	void processIAMALGN_XE();

	void processIASVIOL_XE();

  // ### ------------------------------------------------------ ###
  // #   define the copying capability of the instruction for the	#
  // # next stage (Memory Access):				#
  // #     - the swap instruction can make a copy of itself	#
  // #       during its first memory access			#
  // ### ------------------------------------------------------ ###

	void processCOPYCAP_SE();

};
#endif

