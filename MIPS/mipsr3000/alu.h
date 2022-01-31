// ### -------------------------------------------------------------- ###
// # file       : alu.h                                                 #
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

#ifndef _ALU
#define _ALU

#include <systemc.h>
#include "constants.h"

SC_MODULE(alu)
{
//IN  
  sc_in<sc_uint<32>>  XOPER_SE;    //operand X
  sc_in<sc_uint<32>> 	YOPER_SE;    //operand Y

  sc_in<bool>		      I_SUB_SE;    //si 1 donc X - Y sinon X + Y
  sc_in<sc_uint<3>>	  I_OPER_SE;   //type d'instruction (ti)
  sc_in<bool>         I_RSGND_SE;  // ? 
  sc_in<bool>         I_RIGHT_SE;  // ?
  sc_in<sc_uint<2>>	  I_LOGIC_SE;  //command de ALU

  sc_in<sc_uint<32>>	IOPER_RD;    //result si ti est "nop"
  sc_in<sc_uint<32>>	Y_SE;        //result si ti est "toper"
  sc_in<sc_uint<32>>	X_SE;        //result si ti est "soper"
  sc_in<sc_uint<8>>	  OPCOD_RD;    //bit de test pour slt/i et sltu/i

//OUT
  sc_out<sc_uint<32> >	RES_SE;      //Resultat

//SIGNAL
  sc_signal<sc_uint<32> >	XARITH_SE; //xoper pour operation arithmetique
  sc_signal<sc_uint<32> >	YARITH_SE; //yoper pour operation arithmetique
  sc_signal<sc_uint<32> >	RARITH_SE; 
  sc_signal<sc_uint<33> >	CARITH_SE;
  sc_signal<bool>			OVERFLW_SE;
  sc_signal<bool>			S_LT_T_SE;
  sc_signal<bool>			S_LTU_T_SE;
  sc_signal<bool>			SETBIT_SE;
  sc_signal<sc_uint<32> >	RTEST_SE;
  sc_signal<sc_uint<32> >	RSHIFT_SE;
  sc_signal<sc_uint<32> >	SHIFTIN_SE;
    
  sc_signal<sc_uint<32> >	SHLEFT_SE;   //shift left
  sc_signal<sc_uint<32> >	SHRIGHT_SE;  //shift right
    
  sc_signal<sc_uint<32> >	RLOGIC_SE;

  SC_CTOR(alu):
    XOPER_SE("XOPER_SE"),
    YOPER_SE("YOPER_SE"),
    I_SUB_SE("I_SUB_SE"),
    I_OPER_SE("I_OPER_SE"),
    I_RSGND_SE("I_RSGND_SE"),
    I_RIGHT_SE("I_RIGHT_SE"),
    I_LOGIC_SE("I_LOGIC_SE"),
    IOPER_RD("IOPER_RD"),
    Y_SE("Y_SE"),
    X_SE("X_SE"),
    OPCOD_RD("OPCOD_RD"),
    RES_SE("RES_SE"),

    XARITH_SE("XARITH_SE"),
    YARITH_SE("YARITH_SE"),
    RARITH_SE("RARITH_SE"),
    CARITH_SE("CARITH_SE"),
    OVERFLW_SE("OVERFLW_SE"),
    S_LT_T_SE("S_LT_T_SE"),
    S_LTU_T_SE("S_LTU_T_SE"),
    SETBIT_SE("SETBIT_SE"),
    RTEST_SE("RTEST_SE"),
    RSHIFT_SE("RSHIFT_SE"),
    SHIFTIN_SE("SHIFTIN_SE"),
    SHLEFT_SE("SHLEFT_SE"),
    SHRIGHT_SE("SHRIGHT_SE"),
    RLOGIC_SE("RLOGIC_SE")
      {
		SC_METHOD(processXARITH_SE);
		sensitive << XOPER_SE;
		SC_METHOD(processYARITH_SE);
		sensitive << YOPER_SE << I_SUB_SE;
		SC_METHOD(processRARITH_SE);
		sensitive << CARITH_SE << XARITH_SE << YARITH_SE;
		SC_METHOD(processCARITH_SE);
		sensitive << CARITH_SE << XARITH_SE << YARITH_SE << I_SUB_SE;
		SC_METHOD(processOVERFLW_SE);
		sensitive << CARITH_SE;
		SC_METHOD(processS_LT_T_SE);
		sensitive << RARITH_SE << OVERFLW_SE;
		SC_METHOD(processS_LTU_T_SE);
		sensitive << CARITH_SE;
		SC_METHOD(processSETBIT_SE);
		sensitive << OPCOD_RD << S_LT_T_SE << S_LTU_T_SE;
		SC_METHOD(processRTEST_SE);
		sensitive << SETBIT_SE;
		SC_METHOD(processSHIFTIN_SE);
		sensitive << YOPER_SE << I_RSGND_SE << I_RIGHT_SE;
		SC_METHOD(processSHLEFT_SE);
		sensitive << XOPER_SE << SHIFTIN_SE << YOPER_SE;
		SC_METHOD(processSHRIGHT_SE);
		sensitive << XOPER_SE << SHIFTIN_SE << YOPER_SE;
		SC_METHOD(processRSHIFT_SE);
		sensitive << I_RIGHT_SE << SHRIGHT_SE << SHLEFT_SE;
		SC_METHOD(processRLOGIC_SE);
		sensitive << I_LOGIC_SE << XOPER_SE << YOPER_SE;
		SC_METHOD(processRES_SE);
		sensitive << I_OPER_SE << RARITH_SE << RLOGIC_SE << RSHIFT_SE << RTEST_SE << Y_SE << X_SE << IOPER_RD;
      }

// ### ------------------------------------------------------ ###
// #   operands for arithmetic operations			#
// ### ------------------------------------------------------ ###

  void processXARITH_SE();

  void processYARITH_SE();

// ### ------------------------------------------------------ ###
// #   arithmetic result					#
// ### ------------------------------------------------------ ###

  void processRARITH_SE();

  void processCARITH_SE();

// ### ------------------------------------------------------ ###
// #   arithmetic overflow					#
// ### ------------------------------------------------------ ###

  void processOVERFLW_SE();

// ### ------------------------------------------------------ ###
// #   test and set unit's result				#
// ### ------------------------------------------------------ ###

  void processS_LT_T_SE();

  void processS_LTU_T_SE();

  void processSETBIT_SE();

  void processRTEST_SE();

// ### ------------------------------------------------------ ###
// #   shifter's result						#
// ### ------------------------------------------------------ ###

  void processSHIFTIN_SE();

  void processSHLEFT_SE();

  void processSHRIGHT_SE();

  void processRSHIFT_SE();

// ### ------------------------------------------------------ ###
// #   logic unit's result					#
// ### ------------------------------------------------------ ###

  void processRLOGIC_SE();

// ### ------------------------------------------------------ ###
// #   result out of alu					#
// ### ------------------------------------------------------ ###

  void processRES_SE();
};
#endif

