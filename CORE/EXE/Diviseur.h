#pragma once

#include <systemc.h>
#include "../UTIL/debug_util.h"


SC_MODULE(Diviseur)
{
    // input : 
    sc_in<sc_uint<32>> OP1_SE;
    sc_in<sc_uint<32>> OP2_SE;

    sc_in<sc_uint<2>>  CMD_RD;

    sc_in<bool>        START_SE;

    // output :
    sc_out<sc_uint<32>> DIVIDER_RES_OUTPUT;

    sc_out<bool>        BUSY_SE;
    sc_out<bool>        DONE_SE;

    // General interface:
    sc_in_clk           CLK;

    // signals :
    sc_signal<sc_biguint<64>> divisor_re;
    sc_signal<sc_uint<32>>    quotient_re;
    sc_signal<sc_biguint<64>> reminder_re;

    sc_signal<sc_biguint<64>> divisor_se;
    sc_signal<sc_uint<32>>    quotient_se;
    sc_signal<sc_biguint<64>> reminder_se;

    sc_signal<sc_uint<3>> current_state;
    sc_signal<sc_uint<3>> next_state;

    sc_signal<sc_uint<6>> shift_cpt_se;
    sc_signal<sc_uint<6>> shift_cpt_re;

    sc_signal<bool> sign_reg_se;

    //mae
    void new_state();
    void state_transition();
    void mae_output();

    void RET();

    void trace(sc_trace_file * tf);

    SC_CTOR(Diviseur)
    {        
        SC_METHOD(new_state);
        sensitive << CLK.pos();

        SC_METHOD(state_transition);
        sensitive << current_state << START_SE << shift_cpt_re;

        SC_METHOD(mae_output);
        sensitive << current_state << START_SE << divisor_re
                  << quotient_re << reminder_re
                  << OP1_SE << OP2_SE << shift_cpt_re;

        SC_METHOD(RET);
        sensitive << reminder_re << quotient_re << CMD_RD;
    }
};