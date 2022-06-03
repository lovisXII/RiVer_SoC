#pragma once
#include <systemc.h>
#include "../UTIL/debug_util.h"


SC_MODULE(x2_multiplier)
{
    sc_in<sc_bv<128>> IN_SW;
    sc_in<bool> RET_HIGH_SW;

    sc_out<sc_uint<32>> RES_SW;

    sc_signal<sc_bv<64>> a;
    sc_signal<sc_bv<64>> b;
    sc_signal<bool> S[64];
    sc_signal<bool> G[64];
    sc_signal<bool> P[64];

    sc_signal<bool> c[64];

    void pre_process();
    //modified full adder
    void MFA_0();
    void C_gen_1();

    void RES();

    SC_CTOR(x2_multiplier)
    {
        SC_METHOD(pre_process);
        sensitive << IN_SW;

        SC_METHOD(MFA_0);
        sensitive << a << b << c[0];
        SC_METHOD(C_gen_1);
        sensitive << G[0] << P[0] << c[0];

        SC_METHOD(RES);
        sensitive << S[63];
  }
};