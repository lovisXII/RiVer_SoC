// ### -------------------------------------------------------------- ###
// # file       : pipeunit.h                                            #
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

#ifndef _PIPEUNIT
#define _PIPEUNIT

#include <systemc.h>

SC_MODULE(pipeunit)
{
	sc_out<bool> KILL_SI;
	sc_out<bool> STALL_SI;
	sc_out<bool> COPY_SI;
	sc_out<bool> EXEC_SI;
	sc_out<bool> BUBBLE_SI;
	sc_out<bool> HOLD_SI;
	sc_out<bool> SHIFT_SI;
	sc_out<bool> LOAD_SI;
	sc_out<bool> KEEP_SI;

	sc_out<bool> KILL_SD;
	sc_out<bool> STALL_SD;
	sc_out<bool> COPY_SD;
	sc_out<bool> EXEC_SD;
	sc_out<bool> BUBBLE_SD;
	sc_out<bool> HOLD_SD;
	sc_out<bool> SHIFT_SD;
	sc_out<bool> LOAD_SD;
	sc_out<bool> KEEP_SD;

	sc_out<bool> KILL_SE;
	sc_out<bool> STALL_SE;
	sc_out<bool> COPY_SE;
	sc_out<bool> EXEC_SE;
	sc_out<bool> BUBBLE_SE;
	sc_out<bool> HOLD_SE;
	sc_out<bool> SHIFT_SE;
	sc_out<bool> LOAD_SE;
	sc_out<bool> KEEP_SE;

	sc_out<bool> KILL_SM;
	sc_out<bool> STALL_SM;
	sc_out<bool> COPY_SM;
	sc_out<bool> EXEC_SM;
	sc_out<bool> BUBBLE_SM;
	sc_out<bool> HOLD_SM;
	sc_out<bool> SHIFT_SM;
	sc_out<bool> LOAD_SM;
	sc_out<bool> KEEP_SM;

	sc_out<bool> KILL_SW;
	sc_out<bool> STALL_SW;
	sc_out<bool> COPY_SW;
	sc_out<bool> EXEC_SW;
	sc_out<bool> BUBBLE_SW;
	sc_out<bool> HOLD_SW;
	sc_out<bool> SHIFT_SW;
	sc_out<bool> LOAD_SW;
	sc_out<bool> KEEP_SW;

	sc_in<bool> EXCRQ_XM;
	sc_in<bool> INTRQ_XX;
	sc_in<bool> RESET_RX;
	sc_in<bool> SLEEP_SD;
	sc_in<bool> I_FRZ;
	sc_in<bool> HAZARDS_SD;
	sc_in<bool> HAZARDS_SE;
	sc_in<bool> D_FRZ;
	sc_in<bool> COPYCAP_RE;

	SC_CTOR(pipeunit):
		KILL_SI("KILL_SI"),
		STALL_SI("STALL_SI"),
		COPY_SI("COPY_SI"),
		EXEC_SI("EXEC_SI"),
		BUBBLE_SI("BUBBLE_SI"),
		HOLD_SI("HOLD_SI"),
		SHIFT_SI("SHIFT_SI"),
		LOAD_SI("LOAD_SI"),
		KEEP_SI("KEEP_SI"),

		KILL_SD("KILL_SD"),
		STALL_SD("STALL_SD"),
		COPY_SD("COPY_SD"),
		EXEC_SD("EXEC_SD"),
		BUBBLE_SD("BUBBLE_SD"),
		HOLD_SD("HOLD_SD"),
		SHIFT_SD("SHIFT_SD"),
		LOAD_SD("LOAD_SD"),
		KEEP_SD("KEEP_SD"),

		KILL_SE("KILL_SE"),
		STALL_SE("STALL_SE"),
		COPY_SE("COPY_SE"),
		EXEC_SE("EXEC_SE"),
		BUBBLE_SE("BUBBLE_SE"),
		HOLD_SE("HOLD_SE"),
		SHIFT_SE("SHIFT_SE"),
		LOAD_SE("LOAD_SE"),
		KEEP_SE("KEEP_SE"),

		KILL_SM("KILL_SM"),
		STALL_SM("STALL_SM"),
		COPY_SM("COPY_SM"),
		EXEC_SM("EXEC_SM"),
		BUBBLE_SM("BUBBLE_SM"),
		HOLD_SM("HOLD_SM"),
		SHIFT_SM("SHIFT_SM"),
		LOAD_SM("LOAD_SM"),
		KEEP_SM("KEEP_SM"),

		KILL_SW("KILL_SW"),
		STALL_SW("STALL_SW"),
		COPY_SW("COPY_SW"),
		EXEC_SW("EXEC_SW"),
		BUBBLE_SW("BUBBLE_SW"),
		HOLD_SW("HOLD_SW"),
		SHIFT_SW("SHIFT_SW"),
		LOAD_SW("LOAD_SW"),
		KEEP_SW("KEEP_SW"),

		EXCRQ_XM("EXCRQ_XM"),
		INTRQ_XX("INTRQ_XX"),
		RESET_RX("RESET_RX"),
		SLEEP_SD("SLEEP_SD"),
		I_FRZ("I_FRZ"),
		HAZARDS_SD("HAZARDS_SD"),
		HAZARDS_SE("HAZARDS_SE"),
		D_FRZ("D_FRZ"),
		COPYCAP_RE("COPYCAP_RE")
	{
		SC_METHOD(processKILL_SI);
		sensitive << EXCRQ_XM << INTRQ_XX << RESET_RX << SLEEP_SD;
		SC_METHOD(processSTALL_SI);
		sensitive << KILL_SI << COPY_SD << STALL_SD << I_FRZ;
		SC_METHOD(processCOPY_SI);

		SC_METHOD(processEXEC_SI);
		sensitive << KILL_SI << STALL_SI << COPY_SI;

		SC_METHOD(processKILL_SD);
		sensitive << EXCRQ_XM << INTRQ_XX << RESET_RX;
		SC_METHOD(processSTALL_SD);
		sensitive << KILL_SD << COPY_SE << STALL_SE << HAZARDS_SD << I_FRZ;
		SC_METHOD(processCOPY_SD);
		sensitive << KILL_SD << STALL_SD << SLEEP_SD;
		SC_METHOD(processEXEC_SD);
		sensitive << KILL_SD << STALL_SD << COPY_SD;
		SC_METHOD(processKILL_SE);
		sensitive << EXCRQ_XM << INTRQ_XX << RESET_RX;
		SC_METHOD(processSTALL_SE);
		sensitive << KILL_SE << COPY_SM << STALL_SM << HAZARDS_SE;
		SC_METHOD(processCOPY_SE);

		SC_METHOD(processEXEC_SE);
		sensitive << KILL_SE << STALL_SE << COPY_SE;

		SC_METHOD(processKILL_SM);
		sensitive << EXCRQ_XM << RESET_RX;
		SC_METHOD(processSTALL_SM);
		sensitive << KILL_SM << D_FRZ;
		SC_METHOD(processCOPY_SM);
		sensitive << KILL_SM << STALL_SM << COPYCAP_RE;
		SC_METHOD(processEXEC_SM);
		sensitive << KILL_SM << STALL_SM << COPY_SM;

		SC_METHOD(processKILL_SW);

		SC_METHOD(processSTALL_SW);

		SC_METHOD(processCOPY_SW);

		SC_METHOD(processEXEC_SW);

		SC_METHOD(processBUBBLE_SI);
		sensitive << STALL_SI << EXEC_SD << KILL_SI << EXEC_SD << KILL_SD;
		SC_METHOD(processHOLD_SI);
		sensitive << STALL_SD << COPY_SD;
		SC_METHOD(processSHIFT_SI);
		sensitive << EXEC_SI << COPY_SI;

		SC_METHOD(processBUBBLE_SD);
		sensitive << STALL_SD << EXEC_SE << KILL_SD << EXEC_SE << KILL_SE;
		SC_METHOD(processHOLD_SD);
		sensitive << STALL_SE << COPY_SE;
		SC_METHOD(processSHIFT_SD);
		sensitive << EXEC_SD << COPY_SD;

		SC_METHOD(processBUBBLE_SE);
		sensitive << STALL_SE << EXEC_SM << KILL_SE << EXEC_SM << KILL_SM;
		SC_METHOD(processHOLD_SE);
		sensitive << STALL_SM << COPY_SM;
		SC_METHOD(processSHIFT_SE);
		sensitive << EXEC_SE << COPY_SE;

		SC_METHOD(processBUBBLE_SM);
		sensitive << STALL_SM << EXEC_SW << KILL_SM << EXEC_SW << KILL_SW;
		SC_METHOD(processHOLD_SM);
		sensitive << STALL_SW << COPY_SW;
		SC_METHOD(processSHIFT_SM);
		sensitive << EXEC_SM << COPY_SM;

		SC_METHOD(processLOAD_SI);
		sensitive << COPY_SI << EXEC_SI;
		SC_METHOD(processKEEP_SI);
		sensitive << KILL_SI << STALL_SI;
		SC_METHOD(processLOAD_SD);
		sensitive << COPY_SD << EXEC_SD;
		SC_METHOD(processKEEP_SD);
		sensitive << KILL_SD << STALL_SD;
		SC_METHOD(processLOAD_SE);
		sensitive << COPY_SE << EXEC_SE;
		SC_METHOD(processKEEP_SE);
		sensitive << KILL_SE << STALL_SE;
		SC_METHOD(processLOAD_SM);
		sensitive << COPY_SM << EXEC_SM;
		SC_METHOD(processKEEP_SM);
		sensitive << KILL_SM << STALL_SM;
	}

  // ### ---------------------------------------------------- ###
  // #   instruction flow control :				#
  // #								#
  // #   three cases can happen :					#
  // # (1) Kill : the instruction in the corresponding stage is	#
  // #            killed						#
  // # (2) Stall: the instruction is not allowed to pass to the	#
  // #            next pipe stage					#
  // # (3) Copy : the instruction is duplicated. A copy remains	#
  // #            in the current stage and the other goes down	#
  // #            the pipe					#
  // # (4) Exec : the instruction can be executed			#
  // #								#
  // #   Here follows a summary of different situations.		#
  // #								#
  // #                          | I | D | E | M | W |		#
  // #     ---------------------+---+---+---+---+---|		#
  // #     reset                | K | K | K | K | E |		#
  // #     exception            | K | K | K | K | E |		#
  // #     interrupt            | K | K | K | E | E |		#
  // #     I_FRZ                | S | S | E | E | E |		#
  // #     D_FRZ                | S | S | S | S | E |		#
  // #     hazard in DEC        | S | S | E | E | E |		#
  // #     hazard in EXE        | S | S | S | E | E |		#
  // #     SLEEP                | K | C | E | E | E |		#
  // #     SWAP - first access  | S | S | S | C | E |		#
  // #								#
  // # Note that if more than one situation occur in the same	#
  // # time Kill is prior than Stall which is prior than Exec	#
  // #								#
  // ### ---------------------------------------------------- ###

  // ### ---------------------------------------------------- ###
  // #   The instruction in Instruction Fetch is never copied.	#
  // #								#
  // #   It is stalled (the fetch must be retried) if:		#
  // #     - the next stage (Instruction Decode) is occupied	#
  // #     - the instruction memory is not able to answer the	#
  // #       instruction fetch request				#
  // #								#
  // #   It is killed if :					#
  // #     - the third previous instruction causes an exception	#
  // #     - a hardware or software interrupt occurs		#
  // #     - a hardware reset is detected				#
  // #     - the previous instruction is a sleep			#
  // ### ---------------------------------------------------- ###

	void processKILL_SI();

	void processSTALL_SI();

	void processCOPY_SI();

	void processEXEC_SI();

  // ### ---------------------------------------------------- ###
  // #   The instruction in Instruction Decode is copied if :	#
  // #     - the current instruction is a sleep			#
  // #								#
  // #   It is stalled if :					#
  // #     - the next stage (Execute) is occupied			#
  // #     - there is a data hazard that cannot be resolved by	#
  // #       bypasses						#
  // #     - the instruction memory cannot answer the instruction	#
  // #       fetch (the instruction cannot be executed because it	#
  // #       may change the instruction stream)			#
  // #								#
  // #   It is killed if :					#
  // #     - the second previous instruction causes an exception	#
  // #     - a hardware reset is detected				#
  // #     - a hardware or a software interrupt occurs		#
  // ### ---------------------------------------------------- ###

	void processKILL_SD();

	void processSTALL_SD();

	void processCOPY_SD();

	void processEXEC_SD();

  // ### ---------------------------------------------------- ###
  // #   The instruction in Execute is never copied.		#
  // #								#
  // #   It is stalled if :					#
  // #     - the next stage (Memory Access) is occupied		#
  // #     - there is a data hazard that cannot be resolved by	#
  // #       bypasses						#
  // #								#
  // #   It is killed if :					#
  // #     - the previous instruction causes an exception		#
  // #     - a hardware reset is detected				#
  // #     - a hardware or a software interrupt occurs		#
  // ### ---------------------------------------------------- ###

	void processKILL_SE();

	void processSTALL_SE();

	void processCOPY_SE();

	void processEXEC_SE();

  // ### ---------------------------------------------------- ###
  // #   The instruction in Memory Access is copied if:		#
  // #     - the current instruction has a copying capability	#
  // #       (that is, it is a swap instruction and is making	#
  // #       its first access).					#
  // #								#
  // #   It is stalled if :					#
  // #     - the data memory is not able to answer the request	#
  // #								#
  // #   It is killed if :					#
  // #     - it causes an exception				#
  // #     - a hardware reset is detected				#
  // ### ---------------------------------------------------- ###

	void processKILL_SM();

	void processSTALL_SM();

	void processCOPY_SM();

	void processEXEC_SM();

  // ### ---------------------------------------------------- ###
  // #   The instruction in Write Back is always executed		#
  // ### ---------------------------------------------------- ###

	void processKILL_SW();

	void processSTALL_SW();

	void processCOPY_SW();

	void processEXEC_SW();

  // ### ---------------------------------------------------- ###
  // #   actions on registers :					#
  // #								#
  // #   Three actions may be made on control registers:		#
  // #      (1) shift : shift a new instruction into the stage	#
  // #      (2) bubble: introduce a bubble (nop) into the pipe	#
  // #      (3) hold  : hold the instruction			#
  // #								#
  // #   In each stage the action can be defined by the following	#
  // # table (note that Write Back is always shifting):		#
  // #								#
  // #   stage   next stage   action in stage			#
  // #   ------+------------+----------------			#
  // #     K   |     K      |    bubble				#
  // #     K   |     S      |     hold				#
  // #     K   |     C      |     hold				#
  // #     K   |     E      |    bubble				#
  // #    -----+------------+----------------			#
  // #     S   |     S      |     hold				#
  // #     S   |     C      |     hold				#
  // #     S   |     E      |    bubble				#
  // #    -----+------------+----------------			#
  // #     C   |     E      |    shift				#
  // #    -----+------------+----------------			#
  // #     E   |     E      |    shift				#
  // ### ---------------------------------------------------- ###

	void processBUBBLE_SI();

	void processHOLD_SI();

	void processSHIFT_SI();

	void processBUBBLE_SD();

	void processHOLD_SD();

	void processSHIFT_SD();

	void processBUBBLE_SE();

	void processHOLD_SE();

	void processSHIFT_SE();

	void processBUBBLE_SM();

	void processHOLD_SM();

	void processSHIFT_SM();


		// ### ---------------------------------------------------- ###
		// #   actions on registers :					#
		// #								#
		// #   Two actions may be made on data registers (note that	#
		// # Write Back is always loading) :				#
		// #								#
		// #      (1) load : load a new data into  the reg. (C or E)	#
		// #      (2) keep : hold the same data in the reg. (K or S)	#
		// ### ---------------------------------------------------- ###

	void processLOAD_SI();

	void processKEEP_SI();

	void processLOAD_SD();

	void processKEEP_SD();

	void processLOAD_SE();

	void processKEEP_SE();

	void processLOAD_SM();

	void processKEEP_SM();

};
#endif

