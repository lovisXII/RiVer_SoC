#ifndef _PI_DEFAULT
#define _PI_DEFAULT

#include <systemc.h>

#include "pi_constants.h"

SC_MODULE(pi_default)
{
	sc_in<bool> CK;

	sc_inout<sc_uint<32> > 	PI_A;		// Pi Address
	sc_inout<sc_uint<4> > 	PI_OPC;		// Pi Opcode
	sc_inout<bool> 		PI_READ;	// Pi r/w 
	sc_inout<bool> 		PI_LOCK;	// Pi lock 

	sc_out<bool> 		PI_IREQ0;	// Pi requests 
	sc_out<bool> 		PI_IREQ1;	// Pi requests 
	sc_out<bool> 		PI_IREQ2;	// Pi requests 
	sc_out<bool> 		PI_IREQ3;	// Pi requests 
	sc_out<bool> 		PI_IREQ4;	// Pi requests 
	sc_out<bool> 		PI_IREQ5;	// Pi requests 

	sc_in<bool> 		PI_GNT0;		// Pi grant 
	sc_in<bool> 		PI_GNT1;		// Pi grant 
	sc_in<bool> 		PI_GNT2;		// Pi grant 
	sc_in<bool> 		PI_GNT3;		// Pi grant 
	sc_in<bool> 		PI_GNT4;		// Pi grant 
	sc_in<bool> 		PI_GNT5;		// Pi grant 

	sc_signal<sc_uint<6> > 	PI_IREQ;	// Pi requests 
	sc_signal<sc_uint<6> > 	PI_GNT;		// Pi grant 
	sc_signal<bool>		CK_SX;
	sc_signal<bool>		DEFLTEN_SX;

	SC_CTOR(pi_default)
	{
		SC_METHOD(processCK_SX);
		sensitive << CK;
		SC_METHOD(processPI_IREQ);
		SC_METHOD(processDEFLTEN_SX);
		sensitive << PI_GNT;
		SC_METHOD(processPI_LINES);
		sensitive << DEFLTEN_SX;
		SC_METHOD(processPI_GNT);
		sensitive << PI_GNT0 << PI_GNT1 << PI_GNT2 << PI_GNT3 << PI_GNT4 << PI_GNT5;
		SC_METHOD(processPI_IREQS);
		sensitive << PI_IREQ;
	}

	void processPI_GNT();

  // ### ------------------------------------------------------ ###
  // #   internal clock                                           #
  // ### ------------------------------------------------------ ###

	void processCK_SX();

  // ### ------------------------------------------------------ ###
  // #   bus request                                              #
  // ### ------------------------------------------------------ ###

	void processPI_IREQ();

  // ### ------------------------------------------------------ ###
  // #   Pi output lines' enable :                                #
  // #     enable output lines if the circuit is the default      #
  // #     master                                                 #
  // ### ------------------------------------------------------ ###

	void processDEFLTEN_SX();

  // ### ------------------------------------------------------ ###
  // #   assign output:                                           #
  // #     - Pi-bus's interface                                   #
  // ### ------------------------------------------------------ ###

	void processPI_LINES();

	void processPI_IREQS();

};
#endif
