#pragma once
#include <systemc.h>
#include "../UTIL/debug_util.h"

SC_MODULE(x2_multiplier)
{
    // Input :
    sc_in<sc_bv<128>> IN_RX1;
    sc_in<bool> SELECT_HIGHER_BITS_RX1;
    sc_in<bool> SIGNED_RES_RX1;

    sc_in<bool> X12X2_EMPTY_SX1;

    // Output :
    sc_out<sc_uint<32>> RES_RX2;
    sc_out<bool>        X12X2_POP_SX2;

    // General interace : 
    sc_in_clk   CLK;
    sc_in<bool> RESET;

    sc_signal<sc_bv<64>> a;
    sc_signal<sc_bv<64>> b;

    sc_signal<sc_bv<64>> result;

    void process();

    void manage_fifo();
    void trace(sc_trace_file* tf);

    SC_CTOR(x2_multiplier)
    {
        SC_METHOD(process);
        sensitive << IN_RX1 << SIGNED_RES_RX1 << SELECT_HIGHER_BITS_RX1;

        SC_METHOD(manage_fifo);
        sensitive << X12X2_EMPTY_SX1;
  }
};