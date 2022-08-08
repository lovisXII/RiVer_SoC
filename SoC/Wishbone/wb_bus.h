#pragma once

#include "systemc.h"
#include "wb_arbiter.h"
#include "IP_RAM.h"
#include "IP_RIVER.h"
#include "../UTIL/debug_util.h"
#include <unordered_map>

SC_MODULE(wb_bus)
{
    sc_in_clk   CLK;
    sc_in<bool> RESET_N;

    //bus signals
    sc_signal<sc_uint<32>> ADR;
    sc_signal<bool>        ACK;

    sc_signal<sc_uint<32>> DAT_M2S;
    sc_signal<sc_uint<32>> DAT_S2M;

    sc_signal<bool>        CYC;
    sc_signal<sc_uint<2>>  SEL;
    sc_signal<bool>        STB;
    sc_signal<bool>        WE;

    //arbiter-master0
    sc_signal<bool>        GRANT_0;
    sc_signal<bool>        CYC_0;

    //arbiter-slave0
    sc_signal<bool>        SEL_0;

    //debug
    sc_in<sc_uint<32>>     PC_RESET;
    sc_out<sc_uint<32>>    PC_VALUE;

    wb_arbiter     arbiter_inst;
    IP_RAM         ram_inst;
    IP_RIVER       river_inst;

    void init_mem(std::unordered_map<int,int>*);

    void trace(sc_trace_file* tf);

    SC_CTOR(wb_bus) :
    arbiter_inst("wb_arbiter"),
    ram_inst("IP_RAM"),
    river_inst("IP_RIVER")
    {
        arbiter_inst.CLK(CLK);
        arbiter_inst.RESET_N(RESET_N);
        arbiter_inst.ADR_I(ADR);
        arbiter_inst.CYC_0_I(CYC_0);
        arbiter_inst.CYC_O(CYC);
        arbiter_inst.GRANT_0_O(GRANT_0);
        arbiter_inst.SEL_0_O(SEL_0);

        ram_inst.CLK(CLK);
        ram_inst.RESET_N(RESET_N);
        ram_inst.DAT_I(DAT_M2S);
        ram_inst.ADR_I(ADR);
        ram_inst.CYC_I(CYC);
        ram_inst.SEL_I(SEL);
        ram_inst.STB_I(STB);
        ram_inst.WE_I(WE);
        ram_inst.DAT_O(DAT_S2M);
        ram_inst.ACK_O(ACK);
        ram_inst.ARBITER_SEL_I(SEL_0);

        river_inst.CLK(CLK);
        river_inst.RESET_N(RESET_N);
        river_inst.ACK(ACK);
        river_inst.DAT_I(DAT_S2M);
        river_inst.GRANT(GRANT_0);
        river_inst.ADR(ADR);
        river_inst.DAT_O(DAT_M2S);
        river_inst.SEL(SEL);
        river_inst.STB(STB);
        river_inst.WE(WE);
        river_inst.CYC(CYC_0);
        river_inst.PC_RESET(PC_RESET);
        river_inst.PC_VALUE(PC_VALUE);
    }
};