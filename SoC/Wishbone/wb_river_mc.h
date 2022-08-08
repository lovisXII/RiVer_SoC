#pragma once
#include <systemc.h>
#include "../UTIL/debug_util.h"

typedef enum // MAE STATES
{
    R_IDLE = 0,
    R_REQ = 1,
    R_DC_READ = 2,
    R_DC_END_BURST = 3,
    R_DC_WRITE = 4,
    R_IC = 5,
    R_IC_END_BURST = 6
} river_states_fsm;

SC_MODULE(wb_river_mc)
{
    sc_in_clk   CLK;
    sc_in<bool> RESET_N;

    //interface with BUS
    sc_in<sc_uint<32>> DAT_I;
    sc_in<bool>        ACK_I;  // when asserted indicates the normal termination of a bus cycle

    sc_out<sc_uint<32>> DAT_O;
    sc_out<sc_uint<32>> ADR_O;
    sc_out<sc_uint<2>>  SEL_O;  // select which words on DAT_O are valid
    sc_out<bool>        WE_O;

    sc_out<bool>        STB_O;  // Master is talking on the bus
    
    //interface with ICACHE
    sc_in<sc_uint<32>> A_IC;
    sc_in<bool>        DTA_VALID_IC;

    sc_out<sc_uint<32>> DT_IC;
    sc_out<bool>        ACK_IC;

    //interface with DCACHE
    sc_in<bool>         DTA_VALID_DC;
    sc_in<bool>         READ_DC;
    sc_in<bool>         WRITE_DC;
    sc_in<sc_uint<2>>   SIZE_SEL_DC;
    sc_in<sc_uint<32>>  DT_DC;
    sc_in<sc_uint<32>>  A_DC;

    sc_out<sc_uint<32>> DT_RM;
    sc_out<sc_uint<32>> A_RM;
    sc_out<bool>        ACK_DC;
    
    //interface with BCU
    sc_in<bool>        GRANT_I;
    sc_out<bool>        CYC_O;

    //signals
    sc_signal<sc_uint<3>> current_state;
    sc_signal<sc_uint<3>> future_state;

    void new_state();
    void state_transition();
    void mae_output();

    void trace(sc_trace_file*);

    SC_CTOR(wb_river_mc)
    {
        SC_METHOD(new_state);
        sensitive << CLK.pos() << RESET_N;

        SC_METHOD(state_transition);
        sensitive << CLK.pos() << DTA_VALID_DC << DTA_VALID_IC 
                  << GRANT_I << WRITE_DC << READ_DC << ACK_I;

        SC_METHOD(mae_output);
        sensitive << current_state << DAT_I;
    }
};