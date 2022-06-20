#pragma once

#include <systemc.h>
#include "../UTIL/debug_util.h"


SC_MODULE(Diviseur)
{
    // input : 
    sc_in<sc_uint<32>> OP1_SE;
    sc_in<sc_uint<32>> OP2_SE;

    sc_in<sc_uint<4>> SELECT_TYPE_OPERATIONS_RD;
    sc_in<sc_uint<2>> EXE_CMD_RD;

    // output :
    sc_out<sc_uint<32>> DIV_RES_SE;

    sc_out<bool>        BUSY_SE;
    sc_out<bool>        DONE_SE;

    // General interface:
    sc_in_clk           CLK;

    // signals :
    sc_signal<sc_biguint<64>> divisor_re;
    sc_signal<sc_uint<32>>    quotient_re;
    sc_signal<sc_biguint<64>> remainder_re;

    sc_signal<sc_biguint<64>> divisor_se;
    sc_signal<sc_uint<32>>    quotient_se;
    sc_signal<sc_biguint<64>> remainder_se;

    sc_signal<bool> busy_se;
    sc_signal<bool> done_se;

    sc_signal<sc_uint<3>> current_state;
    sc_signal<sc_uint<3>> next_state;

    sc_signal<sc_uint<6>> shift_cpt_se;

    void preprocess();
    void control();

    SC_CTOR(Diviseur)
    {
        SC_METHOD(preprocess);
        sensitive << OP1_SE << OP2_SE << SELECT_TYPE_OPERATIONS_RD 
                  << quotient_se << remainder_se << divisor_se << busy_se;
        
        SC_METHOD(control);
        sensitive << CLK.pos();
    }
};