#pragma once
#include <systemc.h>
#include "../UTIL/debug_util.h"

typedef enum // MAE STATES
{
    SLAVE_IDLE = 0,
    SLAVE_BUS_REQUEST = 1,
    SLAVE_MODULE_WAIT_D0 = 2,
    SLAVE_MODULE_WAIT_D1 = 3,
    SLAVE_MODULE_WAIT_D2 = 4,
    SLAVE_MODULE_WAIT_D3 = 5
} ram_states_fsm;

SC_MODULE(wb_ram_sc)
{
    sc_in_clk   CLK;
    sc_in<bool> RESET_N;

    //interface with BUS

    sc_in<sc_uint<32>> DAT_I;

    sc_in<sc_uint<32>>  ADR_I;
    sc_in<bool>         CYC_I;  // when asserted indicates that a valid bus cycle in progress
 
    sc_in<sc_uint<2>>  SEL_I;  // select which words on DAT_O are valid
    sc_in<bool>        STB_I;

    sc_in<bool>        WE_I;


    sc_out<sc_uint<32>> DAT_O;
    
    sc_out<bool>        ACK_O;  // when asserted indicates the termination of a bus cycle
    
    //interface with module
    sc_in<sc_uint<32>>  RAM_DT_I;

    sc_out<sc_uint<32>> RAM_DT_O;
    sc_out<sc_uint<32>> RAM_ADR_O;
    sc_out<bool>        RAM_WE_O;
    sc_out<sc_uint<2>>  RAM_SEL_O;
    sc_out<bool>        RAM_DTA_RDY_O;

    //interface with BCU
    sc_in<bool>        ARBITER_SEL_I;

    //signals
    sc_signal<sc_uint<4>> current_state;
    sc_signal<sc_uint<4>> future_state;

    sc_signal<sc_uint<32>> ADR_REG;

    sc_signal<bool>    strobe;

    void strobe_signal();
    void new_state();
    void state_transition();
    void mae_output();

    void trace(sc_trace_file*);

    SC_CTOR(wb_ram_sc)
    {
        SC_METHOD(new_state);
        sensitive << CLK.pos() << RESET_N;
        SC_METHOD(state_transition);
        sensitive << current_state << STB_I << CYC_I << ARBITER_SEL_I << WE_I;
        SC_METHOD(mae_output);
        sensitive << current_state << WE_I;
    }
};