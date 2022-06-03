#pragma once
#include <systemc.h>
#include "../UTIL/debug_util.h"

SC_MODULE(x1_multiplier)
{
    sc_in<sc_bv<320>> IN_SM;

    sc_out<sc_bv<128>> RES_SM;
    //data from input
    sc_signal<sc_uint<64>> M0;
    sc_signal<sc_uint<64>> M1;
    sc_signal<sc_uint<64>> M2;
    sc_signal<sc_uint<64>> M3;
    sc_signal<sc_uint<64>> M4;

    //Res from stage 6
    sc_signal<sc_uint<64>> product_s6[2];

    //Res from stage 7
    sc_signal<sc_uint<64>> product_s7[2];

    //Res from stage 8
    sc_signal<sc_uint<64>> product_s8[2];

    void trace(sc_trace_file* tf);

    void parse_data();
    //stage 6 (1 CSA remind product from EXE M3 and M4)
    void CSA1();

    //stage 7 (1 CSA remind product from EXE M4)
    void CSA2();

    //stage 8 (1 CSA)
    void CSA3();

    // res => 64 bits => CPA(A + B)
    void RES();

    SC_CTOR(x1_multiplier)
    {
        SC_METHOD(parse_data);
        sensitive << IN_SM;
        SC_METHOD(CSA1);
        sensitive << M0 << M1 << M2;

        SC_METHOD(CSA2);
        sensitive << product_s6[0] << product_s6[1];

        SC_METHOD(CSA3);
        sensitive << product_s7[0] << product_s7[1];

        SC_METHOD(RES);
        sensitive << product_s8[0] << product_s8[1];
    }
};