#ifndef _PI_DEFAULT
#define _PI_DEFAULT
//#include "systemc.h"
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

	void processPI_GNT()
	{
		sc_uint<6> pi_gnt=0x0;

		pi_gnt[0]=PI_GNT0.read();
		pi_gnt[1]=PI_GNT1.read();
		pi_gnt[2]=PI_GNT2.read();
		pi_gnt[3]=PI_GNT3.read();
		pi_gnt[4]=PI_GNT4.read();
		pi_gnt[5]=PI_GNT5.read();
		PI_GNT.write(pi_gnt);
	}

        // ### ------------------------------------------------------ ###
        // #   internal clock                                           #
        // ### ------------------------------------------------------ ###

	void processCK_SX()
	{
		CK_SX.write(CK);
	}

        // ### ------------------------------------------------------ ###
        // #   bus request                                              #
        // ### ------------------------------------------------------ ###

	void processPI_IREQ()
	{
		PI_IREQ.write(0x0);
	}

        // ### ------------------------------------------------------ ###
        // #   Pi output lines' enable :                                #
        // #     enable output lines if the circuit is the default      #
        // #     master                                                 #
        // ### ------------------------------------------------------ ###

	void processDEFLTEN_SX()
	{
		DEFLTEN_SX.write((PI_GNT.read()==0x0)?0:1);
	}

        // ### ------------------------------------------------------ ###
        // #   assign output:                                           #
        // #     - Pi-bus's interface                                   #
        // ### ------------------------------------------------------ ###

	void processPI_LINES()
	{
		if (DEFLTEN_SX.read()==1)
		{
			PI_A.write(0x0);
			PI_READ.write(p_red_red);
			PI_LOCK.write(0);
			PI_OPC.write(p_opc_nop);
		}
	}

	void processPI_IREQS()
	{
		sc_uint<6> pi_ireq=PI_IREQ.read();
		PI_IREQ0.write(pi_ireq[0]);
		PI_IREQ1.write(pi_ireq[1]);
		PI_IREQ2.write(pi_ireq[2]);
		PI_IREQ3.write(pi_ireq[3]);
		PI_IREQ4.write(pi_ireq[4]);
		PI_IREQ5.write(pi_ireq[5]);
	}

};
#endif
