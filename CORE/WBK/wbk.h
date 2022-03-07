#pragma once
#include <systemc.h>
#include <iostream>
#include "../UTIL/debug_util.h"
/*
REG_DATA : data at destination of REG, will be WBK in REG
MEM_DATA : data get from a load access, if no load it's just the result from exe_res


*/


SC_MODULE(wbk)
{
    // Reg Interface :

    sc_out<sc_uint<6> >         REG_DEST_SW ;
    sc_out<sc_uint<32> >        REG_DATA_SW ;
    sc_out<bool>                REG_WB_SW ;
    
    //Mem Interface :

    sc_in< sc_uint<32> >        MEM_DATA_SW;
    sc_in< sc_uint<6> >         MEM_DEST_SW;
    sc_in< sc_uint<2> >         MEM_SIZE_SW ;
    sc_in< bool >               MEM_WB_SW;
    sc_in< bool >               SIGN_EXTEND_SW;
    sc_in< bool >               LOAD_SW;

    //Mem2wbk interface

    sc_in<bool>                 MEM2WBK_EMPTY_SW;
    sc_out<bool>                MEM2WBK_POP_SW;

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
        sensitive << MEM_DATA_SW << MEM_DEST_SW << MEM_SIZE_SW << MEM_WB_SW << SIGN_EXTEND_SW << MEM2WBK_EMPTY_SW;
    }
};