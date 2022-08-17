#pragma once

#include <systemc.h>
#include <iostream>
#include <string>
#include "debug_util.h"

SC_MODULE(shifter) {
    /*
    The Shifter is very simple, and only takes a few inputs, does an operation specified by CMD,
    and produces an output
    */
    sc_in<sc_uint<32>> DIN_SE;        // input
    sc_in<sc_uint<5>>  SHIFT_VAL_SE;  // shift value
    sc_in<sc_uint<2>>  CMD_SE;        // command

    sc_out<sc_uint<32>> DOUT_SE;  // output

    // Internal signals :
    // temporary output for each component
    sc_signal<sc_uint<32>> sll_dout_se;
    sc_signal<sc_uint<32>> sr_dout_se;

    void shifter_sll();       // component to shift left the input
    void shifter_sr();        // component to shift right the input, arithmetic or logic
    void shifter_agregate();  // component to choose which shift to use
    void trace(sc_trace_file * tf);

    SC_CTOR(shifter) {
        SC_METHOD(shifter_sll);
        sensitive << DIN_SE << SHIFT_VAL_SE;
        SC_METHOD(shifter_sr);
        sensitive << DIN_SE << SHIFT_VAL_SE << CMD_SE;
        SC_METHOD(shifter_agregate);
        sensitive << sll_dout_se << sr_dout_se << CMD_SE;
    }
};
