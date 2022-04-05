#ifndef _RAM
#define _RAM

#include <systemc.h>
#include "stdio.h"

#define READ_ADR	0
#define READ_VAL	1

SC_MODULE(ram)
{
  sc_in<sc_uint<10>> 		ADR;
  sc_inout<sc_uint<32>> 		DAT;
  sc_in<sc_uint<4>> 		E;
  sc_in<bool> 			W_N;	// W_N=0 for a write, W_N=1 for a read

  sc_uint<8> ramContents[1024][4];

  SC_CTOR(ram)
  {
    SC_METHOD(memAccess);
    sensitive 
			<< ADR 
			<< DAT 
			<< E 
			<< W_N;
  }

  void memAccess();

  void initData(char *filename);

  void print();

};
#endif

