#pragma once
#include <systemc.h>
#include <iostream>

SC_MODULE(kreg)
{
    sc_in<sc_bv<12>>       ADR_CSR_SD ;
    sc_out<sc_uint<32>>    DATA_CSR_SD ;
     
    sc_signal<sc_uint<32>> CSR_adress_SK[12] ;
    SC_CTOR(kreg)
    {

    }
};