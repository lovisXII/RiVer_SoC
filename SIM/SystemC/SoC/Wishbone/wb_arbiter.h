#pragma once
#include <systemc.h>
#include "../UTIL/debug_util.h"

typedef enum // MAE STATES
{
    IDLE = 0,
    COMM = 1,
} states_fsm;

#define nb_slaves  1
#define nb_masters  1

#define ram_adr_deb 0x00000000
#define ram_adr_end 0xFFFFFFFF

SC_MODULE(wb_arbiter)
{
    sc_in_clk   CLK;
    sc_in<bool> RESET_N;

    //interface bus
    sc_in<sc_uint<32>>  ADR_I;
    
    //interface master0
    sc_in<bool>     CYC_0_I;
    sc_out<bool>    GRANT_0_O;

    //interface master1
    sc_in<bool>     CYC_1_I;
    sc_out<bool>    GRANT_1_O;

    //interface slave0
    sc_out<bool>    CYC_O;
    sc_out<bool>    SEL_0_O;

    //signals
    sc_signal<sc_uint<1>> master_priority;

    // slave memory const registers

    sc_signal<sc_uint<32>> slave_deb_adr[nb_slaves];
    sc_signal<sc_uint<32>> slave_end_adr[nb_slaves];

    void master_selector();
    void slave_selector();

    void init_memory();

    void trace(sc_trace_file*);

    SC_CTOR(wb_arbiter)
    {
        SC_METHOD(master_selector);
        sensitive << RESET_N << CYC_0_I << CYC_1_I;

        SC_METHOD(slave_selector);
        sensitive << ADR_I;

        SC_METHOD(init_memory);
        sensitive << RESET_N;
    };
};