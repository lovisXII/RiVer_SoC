#pragma once
#include <systemc.h>
#include "constants.h"
#include "debug_util.h"
SC_MODULE(reg) {
    // Reading Port :

    sc_in<sc_uint<6>> RADR1_SD;
    sc_in<sc_uint<6>> RADR2_SD;

    sc_out<sc_uint<32>> RDATA1_SR;
    sc_out<sc_uint<32>> RDATA2_SR;

    // Writing Port :

    sc_in<sc_uint<6>>  WADR_SW;
    sc_in<bool>        WENABLE_SW;
    sc_in<sc_uint<32>> WDATA_SW;

    sc_in<sc_uint<32>> WRITE_PC_SD;
    sc_in<bool>        WRITE_PC_ENABLE_SD;

    // PC Gestion :

    sc_out<sc_uint<32>> READ_PC_SR;

    // Global Interface :

    sc_in_clk   CLK;
    sc_in<bool> RESET;

    // Registres :
    sc_signal<sc_uint<32>> REG_RR[32];
    sc_signal<sc_uint<32>> PC_RR;

    void read();
    void write();
    void trace(sc_trace_file * tf);

    SC_CTOR(reg) {
        SC_METHOD(read);
        sensitive << RADR1_SD << RADR2_SD << RESET << PC_RR;
        for (int i = 0; i < 32; i++)
            sensitive << REG_RR[i];
        SC_CTHREAD(write, reg::CLK.pos());
        reset_signal_is(RESET, false);
    }
};