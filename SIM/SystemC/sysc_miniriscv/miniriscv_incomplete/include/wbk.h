#pragma once
#include <systemc.h>
#include <iostream>
#include "debug_util.h"

SC_MODULE(wbk) {
    // Reg Interface :

    sc_out<sc_uint<6>>  WADR_SW;
    sc_out<sc_uint<32>> WDATA_SW;
    sc_out<bool>        WENABLE_SW;

    // Mem Interface :

    sc_in<sc_uint<32>> MEM_RES_RM;
    sc_in<sc_uint<6>>  DEST_RM;
    sc_in<bool>        WB_RM;

    // Mem2wbk interface

    sc_in<bool>  MEM2WBK_EMPTY_SM;
    sc_out<bool> MEM2WBK_POP_SW;

    // Global Interface :

    sc_in_clk CLK;
    sc_in_clk RESET;

    // FIFO

    void wbk_method();
    void trace(sc_trace_file * tf);
    SC_CTOR(wbk) {
        SC_METHOD(wbk_method);
        sensitive << MEM_RES_RM << DEST_RM << WB_RM << MEM2WBK_EMPTY_SM;
    }
};