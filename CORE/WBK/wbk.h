#pragma once
#include <systemc.h>
#include <iostream>
#include "../UTIL/debug_util.h"
SC_MODULE(wbk)
{
    // Reg Interface :

    sc_out<sc_uint<6> >         REG_DEST ;
    sc_out<sc_uint<32> >        REG_DATA ;
    sc_out<bool>                REG_WB ;
    
    //Mem Interface :

    sc_in< sc_uint<32> >        MEM_DATA;
    sc_in< sc_uint<6> >         MEM_DEST;
    sc_in< sc_uint<2> >         MEM_SIZE ;
    sc_in< bool >               MEM_WB;
    sc_in< bool >               MEM_SIGN_EXTEND;

    //Mem2wbk interface

    sc_in<bool>                 MEM2WBK_EMPTY;
    sc_out<bool>                MEM2WBK_POP;

    //Internal signals

    //Global Interface :

    sc_in_clk                   CLK;
    sc_in_clk                   RESET;

    // FIFO

    void wbk_method();
    void trace(sc_trace_file* tf);
    SC_CTOR(wbk)
    {
        SC_METHOD(wbk_method);
        sensitive << MEM_DATA << MEM_DEST << MEM_SIZE << MEM_WB << MEM_SIGN_EXTEND << MEM2WBK_EMPTY;
    }
};