#pragma once
#include <systemc.h>
#include "debug_util.h"

SC_MODULE(alu) {
    sc_in<sc_uint<32>>  OP1_SE_S1, OP2_SE_S1;
    sc_in<bool>         CIN_SE;
    sc_in<sc_uint<2>>   CMD_SE;
    sc_out<sc_uint<32>> RES_SE;

    void operation();
    void trace(sc_trace_file * tf);

    SC_CTOR(alu) {
        SC_METHOD(operation);
        sensitive << OP1_SE_S1 << OP2_SE_S1 << CIN_SE << CMD_SE;
    }
};

void alu::operation() {
    sc_uint<32> res_interne;
    if (CMD_SE.read() == 0) {
        res_interne = OP1_SE_S1.read() + OP2_SE_S1.read() + CIN_SE;
    } else if (CMD_SE.read() == 1) {
        res_interne = OP1_SE_S1.read() & OP2_SE_S1.read();
    } else if (CMD_SE.read() == 2) {
        res_interne = OP1_SE_S1.read() | OP2_SE_S1.read();
    } else if (CMD_SE.read() == 3) {
        res_interne = OP1_SE_S1.read() ^ OP2_SE_S1.read();
    }
    RES_SE.write(res_interne);
}

void alu::trace(sc_trace_file* tf) {
    sc_trace(tf, OP1_SE_S1, GET_NAME(OP1_SE_S1));
    sc_trace(tf, OP2_SE_S1, GET_NAME(OP2_SE_S1));
    sc_trace(tf, CIN_SE, GET_NAME(CIN_SE));
    sc_trace(tf, CMD_SE, GET_NAME(CMD_SE));
    sc_trace(tf, RES_SE, GET_NAME(RES_SE));
}