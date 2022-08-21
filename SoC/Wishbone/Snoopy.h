#pragma once

#include <systemc.h>

SC_MODULE(Snoopy)
{
    sc_in<sc_uint<32>> ADR_I;
    sc_in<sc_uint<32>> DAT_I;
    sc_in<sc_uint<2>> SEL_I;
    sc_in<bool> WE_I;

    sc_out<sc_uint<32>> ADR_O;
    sc_out<sc_uint<32>> DAT_O;
    sc_in<sc_uint<2>> SEL_O;
    sc_in<bool> WE_O;

    SC_CTOR(Snoopy)
    {

    }
};