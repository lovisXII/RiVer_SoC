#pragma once

#include <systemc.h>
#include "RAM.h"
#include "wb_ram_sc.h"

SC_MODULE(IP_RAM)
{
    sc_in_clk   CLK;
    sc_in<bool> RESET_N;

    sc_in<sc_uint<32>>  DAT_I;
    sc_in<sc_uint<32>>  ADR_I;
    sc_in<bool>         CYC_I;  // when asserted indicates that a valid bus cycle in progress
    sc_in<sc_uint<2>>   SEL_I;  // select which words on DAT_O are valid
    sc_in<bool>         STB_I;
    sc_in<bool>         WE_I;

    sc_out<sc_uint<32>> DAT_O;
    sc_out<bool>        ACK_O;  // when asserted indicates the termination of a bus cycle
    
    sc_in<bool>        ARBITER_SEL_I;

    //signals
    sc_signal<sc_uint<32>> ADR;
    sc_signal<sc_uint<32>> RAM_I;
    sc_signal<sc_uint<32>> RAM_O;
    sc_signal<sc_uint<2>> MEM_SIZE;
    sc_signal<bool> WE;
    sc_signal<bool> RAM2WRAPPER_VALID;
    sc_signal<bool> WRAPPER2RAM_VALID;

    void init_mem(std::unordered_map<int,int>*);

    void trace(sc_trace_file*);

    RAM         ram_inst;
    wb_ram_sc   wrapper_inst;

    SC_CTOR(IP_RAM):
        ram_inst("RAM"),
        wrapper_inst("wb_ram_sc")
    {
        wrapper_inst.CLK(CLK);
        wrapper_inst.RESET_N(RESET_N);
        wrapper_inst.DAT_I(DAT_I);
        wrapper_inst.ADR_I(ADR_I);
        wrapper_inst.CYC_I(CYC_I);
        wrapper_inst.SEL_I(SEL_I);
        wrapper_inst.STB_I(STB_I);
        wrapper_inst.WE_I(WE_I);
        wrapper_inst.DAT_O(DAT_O);
        wrapper_inst.ACK_O(ACK_O);
        wrapper_inst.RAM_DTA_RDY_O(RAM2WRAPPER_VALID);
        wrapper_inst.RAM_DT_I(RAM_O);
        wrapper_inst.RAM_DT_O(RAM_I);
        wrapper_inst.RAM_ADR_O(ADR);
        wrapper_inst.RAM_WE_O(WE);
        wrapper_inst.RAM_SEL_O(MEM_SIZE);
        wrapper_inst.ARBITER_SEL_I(ARBITER_SEL_I);

        ram_inst.CLK(CLK);
        ram_inst.RESET_N(RESET_N);
        ram_inst.ADR_I(ADR);
        ram_inst.DAT_I(RAM_I);
        ram_inst.VALID_I(RAM2WRAPPER_VALID);
        ram_inst.WE_I(WE);
        ram_inst.MEM_SIZE(MEM_SIZE);
        ram_inst.DAT_O(RAM_O);
    }
};