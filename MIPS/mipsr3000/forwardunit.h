// ### -------------------------------------------------------------- ###
// # files      : forwardunit.h, forwardunit.cpp                        #
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

#ifndef _FORWARDUNIT
#define _FORWARDUNIT

#include <systemc.h>

SC_MODULE(forwardunit)
{
	sc_in<sc_uint<5> > 		RS_SD;
	sc_in<sc_uint<5> > 		RS_RD;
	sc_in<sc_uint<5> > 		RT_SD;
	sc_in<sc_uint<5> > 		RT_RD;
	sc_in<sc_uint<5> > 		RD_RD;
	sc_in<sc_uint<5> > 		RD_RE;
	sc_in<sc_uint<5> > 		RD_RM;
	sc_in<bool> 			I_READS_SD;
	sc_in<bool> 			I_READS_SE;
	sc_in<bool> 			I_READT_SD;
	sc_in<bool> 			I_READT_SE;
	sc_in<bool> 			I_WRITE_SE;
	sc_in<bool> 			I_WRITE_SM;
	sc_in<bool> 			I_WRITE_SW;
	sc_in<bool> 			I_DUSE_SD;
	sc_in<bool> 			I_EPDC_SE;
	sc_in<bool> 			I_MPDC_SE;
	sc_in<bool> 			I_MPDC_SM;
	sc_in<bool> 			I_EUSE_SE;
	sc_in<sc_uint<32> > 	RES_RE;
	sc_in<sc_uint<32> > 	DATA_RM;
	sc_in<sc_uint<32> > 	S_SD;
	sc_in<sc_uint<32> > 	T_SD;
	sc_in<sc_uint<32> > 	SOPER_RD;
	sc_in<sc_uint<32> >	TOPER_RD;
	sc_out<bool> 			HAZARDS_SD;
	sc_out<bool> 			HAZARDS_SE;
	sc_out<sc_uint<32> >	SOPER_SD;
	sc_out<sc_uint<32> >	TOPER_SD;
	sc_out<sc_uint<32> >	X_SE;
	sc_out<sc_uint<32> >	Y_SE;

	sc_signal<sc_uint<5> > 	CMP_SDE_SD;
	sc_signal<sc_uint<5> > 	CMP_SDM_SD;
	sc_signal<sc_uint<5> > 	CMP_SDW_SD;
	sc_signal<sc_uint<5> > 	CMP_TDE_SD;
	sc_signal<sc_uint<5> > 	CMP_TDM_SD;
	sc_signal<sc_uint<5> > 	CMP_TDW_SD;
	sc_signal<bool> 		SREADR0_SD;
	sc_signal<bool> 		TREADR0_SD;
	sc_signal<bool> 		HZD_SDE_SD;
	sc_signal<bool> 		HZD_SDM_SD;
	sc_signal<bool> 		HZD_SDW_SD;
	sc_signal<bool> 		HZD_TDE_SD;
	sc_signal<bool> 		HZD_TDM_SD;
	sc_signal<bool> 		HZD_TDW_SD;
	sc_signal<sc_uint<5> > 	CMP_SDM_SE;
	sc_signal<sc_uint<5> > 	CMP_SDW_SE;
	sc_signal<sc_uint<5> > 	CMP_TDM_SE;
	sc_signal<sc_uint<5> > 	CMP_TDW_SE;
	sc_signal<bool> 		SREADR0_SE;
	sc_signal<bool> 		TREADR0_SE;
	sc_signal<bool> 		HZD_SDM_SE;
	sc_signal<bool> 		HZD_SDW_SE;
	sc_signal<bool> 		HZD_TDM_SE;
	sc_signal<bool> 		HZD_TDW_SE;

	SC_CTOR(forwardunit):
		RS_SD("RS_SD"),
		RS_RD("RS_RD"),
		RT_SD("RT_SD"),
		RT_RD("RT_RD"),
		RD_RD("RD_RD"),
		RD_RE("RD_RE"),
		RD_RM("RD_RM"),
		I_READS_SD("I_READS_SD"),
		I_READS_SE("I_READS_SE"),
		I_READT_SD("I_READT_SD"),
		I_READT_SE("I_READT_SE"),
		I_WRITE_SE("I_WRITE_SE"),
		I_WRITE_SM("I_WRITE_SM"),
		I_WRITE_SW("I_WRITE_SW"),
		I_DUSE_SD("I_DUSE_SD"),
		I_EPDC_SE("I_EPDC_SE"),
		I_MPDC_SE("I_MPDC_SE"),
		I_MPDC_SM("I_MPDC_SM"),
		I_EUSE_SE("I_EUSE_SE"),
		RES_RE("RES_RE"),
		DATA_RM("DATA_RM"),
		S_SD("S_SD"),
		T_SD("T_SD"),
		SOPER_RD("SOPER_RD"),
		TOPER_RD("TOPER_RD"),
		HAZARDS_SD("HAZARDS_SD"),
		HAZARDS_SE("HAZARDS_SE"),
		X_SE("X_SE"),
		Y_SE("Y_SE"),

		CMP_SDE_SD("CMP_SDE_SD"),
		CMP_SDM_SD("CMP_SDM_SD"),
		CMP_SDW_SD("CMP_SDW_SD"),
		CMP_TDE_SD("CMP_TDE_SD"),
		CMP_TDM_SD("CMP_TDM_SD"),
		CMP_TDW_SD("CMP_TDW_SD"),
		SREADR0_SD("SREADR0_SD"),
		TREADR0_SD("TREADR0_SD"),
		HZD_SDE_SD("HZD_SDE_SD"),
		HZD_SDM_SD("HZD_SDM_SD"),
		HZD_SDW_SD("HZD_SDW_SD"),
		HZD_TDE_SD("HZD_TDE_SD"),
		HZD_TDM_SD("HZD_TDM_SD"),
		HZD_TDW_SD("HZD_TDW_SD"),
		CMP_SDM_SE("CMP_SDM_SE"),
		CMP_SDW_SE("CMP_SDW_SE"),
		CMP_TDM_SE("CMP_TDM_SE"),
		CMP_TDW_SE("CMP_TDW_SE"),
		SREADR0_SE("SREADR0_SE"),
		TREADR0_SE("TREADR0_SE"),
		HZD_SDM_SE("HZD_SDM_SE"),
		HZD_SDW_SE("HZD_SDW_SE"),
		HZD_TDM_SE("HZD_TDM_SE"),
		HZD_TDW_SE("HZD_TDW_SE")

	{
		SC_METHOD(processCMP_SDE_SD);
		sensitive << RS_SD << RD_RD;
		SC_METHOD(processCMP_SDM_SD);
		sensitive << RS_SD << RD_RE;
		SC_METHOD(processCMP_SDW_SD);
		sensitive << RS_SD << RD_RM;
		SC_METHOD(processCMP_TDE_SD);
		sensitive << RT_SD << RD_RD;
		SC_METHOD(processCMP_TDM_SD);
		sensitive << RT_SD << RD_RD;
		SC_METHOD(processCMP_TDW_SD);
		sensitive << RT_SD << RD_RD;
		SC_METHOD(processSREADR0_SD);
		sensitive << RS_SD;
		SC_METHOD(processTREADR0_SD);
		sensitive << RT_SD;
		SC_METHOD(processHZD_SDE_SD);
		sensitive << CMP_SDE_SD << SREADR0_SD << I_READS_SD << I_WRITE_SE;
		SC_METHOD(processHZD_SDM_SD);
		sensitive << CMP_SDM_SD << SREADR0_SD << I_READS_SD << I_WRITE_SM;
		SC_METHOD(processHZD_SDW_SD);
		sensitive << CMP_SDW_SD << SREADR0_SD << I_READS_SD << I_WRITE_SW;
		SC_METHOD(processHZD_TDE_SD);
		sensitive << CMP_TDE_SD << TREADR0_SD << I_READT_SD << I_WRITE_SE;
		SC_METHOD(processHZD_TDM_SD);
		sensitive << CMP_TDM_SD << TREADR0_SD << I_READT_SD << I_WRITE_SM;
		SC_METHOD(processHZD_TDW_SD);
		sensitive << CMP_TDW_SD << TREADR0_SD << I_READT_SD << I_WRITE_SW;
		SC_METHOD(processHAZARDS_SD);
		sensitive << HZD_SDE_SD << HZD_TDE_SD << I_DUSE_SD << I_EPDC_SE
		<< HZD_SDE_SD << HZD_TDE_SD << I_DUSE_SD << I_MPDC_SE
		<< HZD_SDM_SD << HZD_TDM_SD << I_DUSE_SD << I_MPDC_SM ;
		SC_METHOD(processSOPER_SD);
		sensitive << HZD_SDM_SD << RES_RE << HZD_SDW_SD << DATA_RM << S_SD;
		SC_METHOD(processTOPER_SD);
		sensitive << HZD_TDM_SD << RES_RE << HZD_TDW_SD << DATA_RM << T_SD;
		SC_METHOD(processCMP_SDM_SE);
		sensitive << RS_RD << RD_RE;
		SC_METHOD(processCMP_SDW_SE);
		sensitive << RS_RD << RD_RM;
		SC_METHOD(processCMP_TDM_SE);
		sensitive << RT_RD << RD_RE;
		SC_METHOD(processCMP_TDW_SE);
		sensitive << RT_RD << RD_RM;
		SC_METHOD(processSREADR0_SE);
		sensitive << RS_RD;
		SC_METHOD(processTREADR0_SE);
		sensitive << RT_RD;
		SC_METHOD(processHZD_SDM_SE);
		sensitive << CMP_SDM_SE << SREADR0_SE << I_READS_SE << I_WRITE_SM;
		SC_METHOD(processHZD_SDW_SE);
		sensitive << CMP_SDW_SE << SREADR0_SE << I_READS_SE << I_WRITE_SW;
		SC_METHOD(processHZD_TDM_SE);
		sensitive << CMP_TDM_SE << TREADR0_SE << I_READT_SE << I_WRITE_SM;
		SC_METHOD(processHZD_TDW_SE);
		sensitive << CMP_TDW_SE << TREADR0_SE << I_READT_SE << I_WRITE_SW;
		SC_METHOD(processHAZARDS_SE);
		sensitive << HZD_SDM_SE << HZD_TDM_SE << I_EUSE_SE << I_MPDC_SM;
		SC_METHOD(processX_SE);
		sensitive << HZD_SDM_SE << RES_RE << HZD_SDW_SE << DATA_RM << SOPER_RD;
		SC_METHOD(processY_SE);
		sensitive << HZD_TDM_SE << RES_RE << HZD_TDW_SE << DATA_RM << TOPER_RD;
	}

  // ### ------------------------------------------------------ ###
  // #   compare source registers of the current instruction with	#
  // # the destination register of previous instructions (data	#
  // # hazards in Instruction Decode cycle)			#
  // ### ------------------------------------------------------ ###


	void processCMP_SDE_SD();

	void processCMP_SDM_SD();

	void processCMP_SDW_SD();

	void processCMP_TDE_SD();

	void processCMP_TDM_SD();

	void processCMP_TDW_SD();

	void processSREADR0_SD();

	void processTREADR0_SD();

  // ### ------------------------------------------------------ ###
  // #   effective comparisons (data hazards in Instruction	#
  // # Decode cycle):						#
  // #     fields matches and are effectively used (comparisons	#
  // #     fail when r0 is used)					#
  // ### ------------------------------------------------------ ###

	void processHZD_SDE_SD();

	void processHZD_SDM_SD();

	void processHZD_SDW_SD();

	void processHZD_TDE_SD();

	void processHZD_TDM_SD();

	void processHZD_TDW_SD();

  // ### ------------------------------------------------------ ###
  // #   stall due to data hazards in Instruction Decode stage:	#
  // #								#
  // #      ADD  Ri, --, --          LW   Ri, --, --		#
  // #      BEQ  Ri, --              --   --, --, --		#
  // #                               BEQ  Ri, --			#
  // #      LW   Ri, --, --					#
  // #      BEQ  Ri, --						#
  // #								#
  // #   (use of s or t operand in Instruction Decode stage when	#
  // # the result is produced by the previous instruction in	#
  // # Execute or in Memory Access stage or when the result is	#
  // # produced by the second previous instruction in Memory	#
  // # Access stage).						#
  // ### ------------------------------------------------------ ###

	void processHAZARDS_SD();

  // ### ------------------------------------------------------ ###
  // #   compute effective operands (bypass)			#
  // ### ------------------------------------------------------ ###

	void processSOPER_SD();

	void processTOPER_SD();

  // ### ------------------------------------------------------ ###
  // #   compare source registers of the current instruction with	#
  // # the destination register of previous instructions		#
  // ### ------------------------------------------------------ ###

	void processCMP_SDM_SE();

	void processCMP_SDW_SE();

	void processCMP_TDM_SE();

	void processCMP_TDW_SE();

	void processSREADR0_SE();

	void processTREADR0_SE();

  // ### ------------------------------------------------------ ###
  // #   effective comparisons :					#
  // #     fields matches and are effectively used (comparisons	#
  // #     fail when r0 is used)					#
  // ### ------------------------------------------------------ ###

	void processHZD_SDM_SE();

	void processHZD_SDW_SE();

	void processHZD_TDM_SE();

	void processHZD_TDW_SE();

  // ### ------------------------------------------------------ ###
  // #   stall due to data hazards in Execute stage:		#
  // #								#
  // #      LW   Ri, --, --					#
  // #      ADD  --, Ri, --					#
  // #								#
  // #   (use of s or t operand in Execute stage when the result	#
  // #    is produced in Memory Access stage by the previous	#
  // #    instruction).						#
  // ### ------------------------------------------------------ ###

	void processHAZARDS_SE();

  // ### ------------------------------------------------------ ###
  // #   effective operands (bypasses)				#
  // ### ------------------------------------------------------ ###

	void processX_SE();

	void processY_SE();
};
#endif



