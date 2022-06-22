#pragma once
#include <systemc.h>
#include <iostream>
#include "../UTIL/debug_util.h"

SC_MODULE(pic) {
    // Core interface

    sc_out<bool>        WRITE_MIP_SP;
    sc_out<sc_uint<32>> NEW_INTERRUPT_SP;
    sc_in<bool>         ACK_SC;

    // Timer interface
    sc_in<bool>  TIMER_INT_ST;
    sc_out<bool> ACK_SP;

    // Global Interface :

    sc_in_clk CLK;
    sc_in_clk RESET;

    // FIFO

    void trace(sc_trace_file * tf);
    SC_CTOR(pic) {}
};