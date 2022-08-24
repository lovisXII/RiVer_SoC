#pragma once
#include <systemc.h>
#include "debug_util.h"

SC_MODULE(alu) {
    /*
    The ALU is very simple, and only takes a few inputs, does an operation specified by CMD,
    and produces an output
    */
    sc_in<sc_uint<32>> OP1_SE;
    sc_in<sc_uint<32>> OP2_SE;
    sc_in<bool>        CIN_SE;
    /*
    ALU command :
    - 0 : add
    - 1 : and
    - 2 : or
    - 3 : xor
    */
    sc_in<sc_uint<2>> CMD_SE;

    sc_out<sc_uint<32>> RES_SE;

    void operation();
    void trace(sc_trace_file * tf);

    SC_CTOR(alu) {
        SC_METHOD(operation);
        sensitive << OP1_SE << OP2_SE << CIN_SE << CMD_SE;
    }
};
