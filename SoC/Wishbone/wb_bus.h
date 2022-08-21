#pragma once

#include "systemc.h"
#include "wb_arbiter.h"
#include "IP_RAM.h"
#include "IP_RIVER.h"
#include "Multiplexor.h"
#include "../UTIL/debug_util.h"
#include <unordered_map>

SC_MODULE(wb_bus)
{
    sc_in_clk   CLK;
    sc_in<bool> RESET_N;

    //bus signals
    sc_signal<bool>        ACK;
    sc_signal<sc_uint<32>> DAT_S2M;

    sc_signal<sc_uint<32>> ADR;
    sc_signal<sc_uint<32>> DAT;

    sc_signal<bool>        CYC;
    sc_signal<sc_uint<2>>  SEL;
    sc_signal<bool>        STB;
    sc_signal<bool>        WE;

    //signals from master 0
    sc_signal<sc_uint<32>> ADR_0;
    sc_signal<sc_uint<32>> DAT_M0;

    sc_signal<sc_uint<2>>  SEL_0;
    sc_signal<bool>        STB_0;
    sc_signal<bool>        WE_0;

    //signals from master 1
    sc_signal<sc_uint<32>> ADR_1;
    sc_signal<sc_uint<32>> DAT_M1;

    sc_signal<sc_uint<2>>  SEL_1;
    sc_signal<bool>        STB_1;
    sc_signal<bool>        WE_1;


    //arbiter-master0
    sc_signal<bool>        GRANT_0;
    sc_signal<bool>        CYC_REQ_0;

    //arbiter-master1
    sc_signal<bool>        GRANT_1;
    sc_signal<bool>        CYC_REQ_1;

    //arbiter-slave0
    sc_signal<bool>        SELECT_0;

    //debug
    sc_in<sc_uint<32>>     PC_RESET;
    sc_out<sc_uint<32>>    PC0_VALUE;
    sc_out<sc_uint<32>>    PC1_VALUE;

    //signals
    sc_signal<sc_uint<32>>  PROC0_ID;
    sc_signal<sc_uint<32>>  PROC1_ID;

    wb_arbiter     arbiter_inst;
    IP_RAM         ram_inst;
    IP_RIVER       river0_inst;
    IP_RIVER       river1_inst;
    Multiplexor     multiplexor_inst;

    void init_mem(std::unordered_map<int,int>*);

    void trace(sc_trace_file* tf);

    SC_CTOR(wb_bus) :
    arbiter_inst("wb_arbiter"),
    multiplexor_inst("Multiplexor"),
    ram_inst("IP_RAM"),
    river0_inst("IP_RIVER0"),
    river1_inst("IP_RIVER1")
    {
        PROC0_ID.write(0);
        PROC1_ID.write(1);

        arbiter_inst.CLK(CLK);
        arbiter_inst.RESET_N(RESET_N);
        arbiter_inst.ADR_I(ADR);
        arbiter_inst.CYC_0_I(CYC_REQ_0);
        arbiter_inst.GRANT_0_O(GRANT_0);
        arbiter_inst.CYC_1_I(CYC_REQ_1);
        arbiter_inst.GRANT_1_O(GRANT_1);
        arbiter_inst.CYC_O(CYC);
        arbiter_inst.SEL_0_O(SELECT_0);

        ram_inst.CLK(CLK);
        ram_inst.RESET_N(RESET_N);
        ram_inst.DAT_I(DAT);
        ram_inst.ADR_I(ADR);
        ram_inst.CYC_I(CYC);
        ram_inst.SEL_I(SEL);
        ram_inst.STB_I(STB);
        ram_inst.WE_I(WE);
        ram_inst.DAT_O(DAT_S2M);
        ram_inst.ACK_O(ACK);
        ram_inst.ARBITER_SEL_I(SELECT_0);

        multiplexor_inst.ADR_0(ADR_0);
        multiplexor_inst.DAT_0(DAT_M0);
        multiplexor_inst.SEL_0(SEL_0);
        multiplexor_inst.STB_0(STB_0);
        multiplexor_inst.WE_0(WE_0);
        multiplexor_inst.ADR_1(ADR_1);
        multiplexor_inst.DAT_1(DAT_M1);
        multiplexor_inst.SEL_1(SEL_1);
        multiplexor_inst.STB_1(STB_1);
        multiplexor_inst.WE_1(WE_1);
        multiplexor_inst.ADR(ADR);
        multiplexor_inst.DAT(DAT);
        multiplexor_inst.SEL(SEL);
        multiplexor_inst.STB(STB);
        multiplexor_inst.WE(WE);
        multiplexor_inst.GRANT_0(GRANT_0);
        multiplexor_inst.GRANT_1(GRANT_1);

        river0_inst.CLK(CLK);
        river0_inst.RESET_N(RESET_N);
        river0_inst.PROC_ID(PROC0_ID);
        river0_inst.ACK(ACK);
        river0_inst.DAT_I(DAT_S2M);
        river0_inst.GRANT(GRANT_0);
        river0_inst.ADR_I(ADR);
        river0_inst.ADR(ADR_0);
        river0_inst.DAT_O(DAT_M0);
        river0_inst.SEL(SEL_0);
        river0_inst.STB(STB_0);
        river0_inst.WE(WE_0);
        river0_inst.CYC(CYC_REQ_0);
        river0_inst.PC_RESET(PC_RESET);
        river0_inst.PC_VALUE(PC0_VALUE);

        river1_inst.CLK(CLK);
        river1_inst.RESET_N(RESET_N);
        river1_inst.PROC_ID(PROC1_ID);
        river1_inst.ACK(ACK);
        river1_inst.DAT_I(DAT_S2M);
        river1_inst.GRANT(GRANT_1);
        river1_inst.ADR_I(ADR);
        river1_inst.ADR(ADR_1);
        river1_inst.DAT_O(DAT_M1);
        river1_inst.SEL(SEL_1);
        river1_inst.STB(STB_1);
        river1_inst.WE(WE_1);
        river1_inst.CYC(CYC_REQ_1);
        river1_inst.PC_RESET(PC_RESET);
        river1_inst.PC_VALUE(PC1_VALUE);
    }
};