#pragma once

#include "systemc.h"
#include "../../UTIL/debug_util.h"


SC_MODULE(Multiplexor)
{
    //interface 0
    sc_in<sc_uint<32>> ADR_0;
    sc_in<sc_uint<32>> DAT_0;

    sc_in<sc_uint<2>>  SEL_0;
    sc_in<bool>        STB_0;
    sc_in<bool>        WE_0;

    //interface 1
    sc_in<sc_uint<32>> ADR_1;
    sc_in<sc_uint<32>> DAT_1;

    sc_in<sc_uint<2>>  SEL_1;
    sc_in<bool>        STB_1;
    sc_in<bool>        WE_1;

    //out 
    sc_out<sc_uint<32>> ADR;
    sc_out<sc_uint<32>> DAT;

    sc_out<sc_uint<2>>  SEL;
    sc_out<bool>        STB;
    sc_out<bool>        WE;

    //granteds
    sc_in<bool>        GRANT_0;
    sc_in<bool>        GRANT_1;

    void select();

    void trace(sc_trace_file*);

    SC_CTOR(Multiplexor)
    {
        SC_METHOD(select);
        sensitive << GRANT_0 << GRANT_1 << ADR_0 << DAT_0 << SEL_0
                  << STB_0 << WE_0 << ADR_1 << DAT_1 << SEL_1
                  << STB_1 << WE_1;
    }
};
