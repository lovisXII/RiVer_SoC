#ifndef _RAM_CONTROL
#define _RAM_CONTROL

#include <systemc.h>

#include "pi_constants.h"

SC_MODULE(ram_control)
{
  sc_in<bool> CK;
  sc_in<bool> RESET_N;

  sc_in<sc_uint<32>> 	PI_A;		// Pi Address
  sc_inout<sc_uint<32>> 	PI_D;		// Pi Data
  sc_in<sc_uint<4>> 	PI_OPC;		// Pi Opcode
  sc_in<bool> 		PI_READ;	// Pi r/w 
  sc_in<bool> 		PI_SEL;		// Pi slave sel 
  sc_inout<sc_uint<3>> 	PI_ACK;		// Pi ack 

  sc_out<sc_uint<4>>	RAM_E;
  sc_out<bool>		RAM_W_N;
  sc_out<sc_uint<10>>	RAM_ADR;
  sc_inout<sc_uint<32>>	RAM_D;

  sc_signal<bool>		RESET_RX;
  sc_signal<bool>		CK_SX;
  sc_signal<sc_uint<32>>	ADDRESS_RX;
  sc_signal<bool>		READ_RX;
  sc_signal<bool>		SELECT_RX;
  sc_signal<sc_uint<4>>	OPCODE_RX;
  sc_signal<sc_uint<2>>	MODE_SX;
  sc_signal<sc_uint<4>>	BYTE_SX;

  SC_CTOR(ram_control)
  {
    SC_METHOD(processCK_SX);
    sensitive 
			<< CK;
    SC_METHOD(processMODE_SX);
    sensitive 
			<< OPCODE_RX;
    SC_METHOD(processBYTE_SX);
    sensitive 
			<< OPCODE_RX;
    SC_METHOD(processRAM_ADR);
    sensitive 
			<< ADDRESS_RX;
    SC_METHOD(processRAM_W_N);
    sensitive 
			<< READ_RX;
    SC_METHOD(processRAM_E);
    sensitive 
			<< BYTE_SX 
			<< SELECT_RX 
			<< CK.pos();
    SC_METHOD(processRAM_D);
    sensitive 
			<< READ_RX 
			<< CK_SX 
			<< MODE_SX 
			<< PI_D;
    SC_METHOD(processPI_ACK);
    sensitive 
			<< SELECT_RX;
    SC_METHOD(processPI_D);
    sensitive 
			<< SELECT_RX 
			<< READ_RX 
			<< RAM_D;
    
    SC_METHOD(processTRANSFER);
    sensitive 
			<< CK.pos();
    SC_METHOD(processRESET);
    sensitive 
			<< CK.pos();
  }

// ### ------------------------------------------------------ ###
// #   internal clock                                           #
// ### ------------------------------------------------------ ###

  void processCK_SX();

// ### ------------------------------------------------------ ###
// #   access mode                                              #
// ### ------------------------------------------------------ ###

  void processMODE_SX();

// ### ------------------------------------------------------ ###
// #   define which bytes of the memory are to be written       #
// ### ------------------------------------------------------ ###

  void processBYTE_SX();

// ### ------------------------------------------------------ ###
// #   assign outputs                                           #
// #     - RAM interface                                        #
// ### ------------------------------------------------------ ###

  void processRAM_ADR();

  void processRAM_W_N();

  void processRAM_E();

  void processRAM_D();

// ### ------------------------------------------------------ ###
// #   assign outputs                                           #
// #     - Pi-bus interface                                     #
// ### ------------------------------------------------------ ###

  void processPI_ACK();

  void processPI_D();

// ### ------------------------------------------------------ ###
// #   assign registers :                                       #
// #     - address        register                              #
// #     - data           register                              #
// #     - operation code register                              #
// #     - select         register                              #
// #     - access mode    register                              #
// ### ------------------------------------------------------ ###

  void processTRANSFER();

// ### ------------------------------------------------------ ###
// #   reset                                                    #
// ### ------------------------------------------------------ ###

  void processRESET();
};
#endif
