#ifndef _ROM
#define _ROM

#include <systemc.h>
#include "stdio.h"

#define ROM_READ_ADR        0
#define ROM_READ_VAL        1

SC_MODULE(rom)
{
  sc_in<sc_uint<10>> 		ADR;
  sc_out<sc_uint<32>> 		DAT;

  sc_uint<8> romContents[1024][4];

  SC_CTOR(rom)
  {
    SC_METHOD(memAccess);
    sensitive << ADR;
  }

  void memAccess();

  void initData(char *filename);
	
  void print();

};
#endif

