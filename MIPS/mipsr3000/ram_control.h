#ifndef _RAM_CONTROL
#define _RAM_CONTROL
//#include "systemc.h"
#include <systemc.h>
#include "pi_constants.h"

SC_MODULE(ram_control)
{
	sc_in<bool> CK;
	sc_in<bool> RESET_N;

	sc_in<sc_uint<32> > 	PI_A;		// Pi Address
	sc_inout<sc_uint<32> > 	PI_D;		// Pi Data
	sc_in<sc_uint<4> > 	PI_OPC;		// Pi Opcode
	sc_in<bool> 		PI_READ;	// Pi r/w 
	sc_in<bool> 		PI_SEL;		// Pi slave sel 
	sc_inout<sc_uint<3> > 	PI_ACK;		// Pi ack 

	sc_out<sc_uint<4> >	RAM_E;
	sc_out<bool>		RAM_W_N;
	sc_out<sc_uint<10> >	RAM_ADR;
	sc_inout<sc_uint<32> >	RAM_D;

	sc_signal<bool>		RESET_RX;
	sc_signal<bool>		CK_SX;
	sc_signal<sc_uint<32> >	ADDRESS_RX;
	sc_signal<bool>		READ_RX;
	sc_signal<bool>		SELECT_RX;
	sc_signal<sc_uint<4> >	OPCODE_RX;
	sc_signal<sc_uint<2> >	MODE_SX;
	sc_signal<sc_uint<4> >	BYTE_SX;

	SC_CTOR(ram_control)
	{
		SC_METHOD(processCK_SX);
		sensitive << CK;
		SC_METHOD(processMODE_SX);
		sensitive << OPCODE_RX;
		SC_METHOD(processBYTE_SX);
		sensitive << OPCODE_RX;
		SC_METHOD(processRAM_ADR);
		sensitive << ADDRESS_RX;
		SC_METHOD(processRAM_W_N);
		sensitive << READ_RX;
		SC_METHOD(processRAM_E);
		sensitive << BYTE_SX << SELECT_RX << CK_SX;
		SC_METHOD(processRAM_D);
		sensitive << READ_RX << CK_SX << MODE_SX << PI_D;
		SC_METHOD(processPI_ACK);
		sensitive << SELECT_RX;
		SC_METHOD(processPI_D);
		sensitive << SELECT_RX << READ_RX << RAM_D;
		SC_METHOD(processTRANSFER);
		sensitive_pos << CK_SX;
		SC_METHOD(processRESET);
		sensitive_pos << CK_SX;
	}

        // ### ------------------------------------------------------ ###
        // #   internal clock                                           #
        // ### ------------------------------------------------------ ###

	void processCK_SX()
	{
		CK_SX.write(CK);
	}

        // ### ------------------------------------------------------ ###
        // #   access mode                                              #
        // ### ------------------------------------------------------ ###

	void processMODE_SX()
	{
		sc_uint<4> opcode_rx=OPCODE_RX.read();

		MODE_SX.write(opcode_rx.range(3,2));
	}

        // ### ------------------------------------------------------ ###
        // #   define which bytes of the memory are to be written       #
        // ### ------------------------------------------------------ ###

	void processBYTE_SX()
	{
		if (OPCODE_RX.read()==p_opc_wdu)
			BYTE_SX.write(0xF);
		else if (OPCODE_RX.read()==p_opc_hw0)
			BYTE_SX.write(0x3);
		else if (OPCODE_RX.read()==p_opc_hw1)
			BYTE_SX.write(0xC);
		else if (OPCODE_RX.read()==p_opc_by0)
			BYTE_SX.write(0x1);
		else if (OPCODE_RX.read()==p_opc_by1)
			BYTE_SX.write(0x2);
		else if (OPCODE_RX.read()==p_opc_by2)
			BYTE_SX.write(0x4);
		else if (OPCODE_RX.read()==p_opc_by3)
			BYTE_SX.write(0x8);
		else
			BYTE_SX.write(0x0);
	}

        // ### ------------------------------------------------------ ###
        // #   assign outputs                                           #
        // #     - RAM interface                                        #
        // ### ------------------------------------------------------ ###

	void processRAM_ADR()
	{
		sc_uint<32> address_rx=ADDRESS_RX.read();

		unsigned int t=(int)address_rx;
		printf ("RAM_ADR address_rx=%x\n",t);
		//RAM_ADR.write(address_rx.range(11,2));
		RAM_ADR.write(address_rx.range(9,0));
	}

	void processRAM_W_N()
	{
		RAM_W_N.write(READ_RX.read());
	}

	void processRAM_E()
	{
		if ((SELECT_RX.read()==1) && (CK_SX.read()==0))
			RAM_E.write(BYTE_SX.read());
		else
			RAM_E.write(0x0);
	}

	void processRAM_D()
	{
		sc_uint<32> pi_d=PI_D.read();
		sc_uint<32> res=0x0;

		if ((READ_RX.read()==0) && (CK_SX.read()==0))
		{
			if (MODE_SX.read()==m_word)
				RAM_D.write(pi_d);
			else if (MODE_SX.read()==m_half)
			{
			 	res.range(31,16)=pi_d.range(15,0);	
			 	res.range(15,0)=pi_d.range(15,0);	
				RAM_D.write(res);
			}
			else
			{
			 	res.range(31,24)=pi_d.range(7,0);	
			 	res.range(23,16)=pi_d.range(7,0);	
			 	res.range(15,8)=pi_d.range(7,0);	
			 	res.range(7,0)=pi_d.range(7,0);	
				RAM_D.write(res);
			}
		}
	}

        // ### ------------------------------------------------------ ###
        // #   assign outputs                                           #
        // #     - Pi-bus interface                                     #
        // ### ------------------------------------------------------ ###

	void processPI_ACK()
	{
		if (SELECT_RX.read()==1)
			PI_ACK.write(p_ack_rdy);
		else
			PI_ACK.write(7);
	}

	void processPI_D()
	{
		if ((SELECT_RX.read()==1) && (READ_RX.read()==1))
			PI_D.write(RAM_D.read());
	}

        // ### ------------------------------------------------------ ###
        // #   assign registers :                                       #
        // #     - address        register                              #
        // #     - data           register                              #
        // #     - operation code register                              #
        // #     - select         register                              #
        // #     - access mode    register                              #
        // ### ------------------------------------------------------ ###

	void processTRANSFER()
	{
		ADDRESS_RX.write(PI_A.read());
		READ_RX.write(PI_READ.read());
		SELECT_RX.write((RESET_RX.read()==1)?0:PI_SEL.read());
		OPCODE_RX.write((RESET_RX.read()==1)?p_opc_nop:PI_OPC.read());
	}

        // ### ------------------------------------------------------ ###
        // #   reset                                                    #
        // ### ------------------------------------------------------ ###

	void processRESET()
	{
		RESET_RX.write(!RESET_N.read());
	}
};
#endif
