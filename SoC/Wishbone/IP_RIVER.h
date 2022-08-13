#pragma once

#include <systemc.h>
#include "../CORE/core.h"
#include "../CORE/CACHES/icache.h"
#include "../CORE/CACHES/dcache.h"
#include "wb_river_mc.h"

SC_MODULE(IP_RIVER)
{
    sc_in_clk           CLK;
    sc_in<bool>         RESET_N;

    sc_in<bool>        ACK;
    sc_in<sc_uint<32>> DAT_I;
    sc_in<bool>        GRANT;

    sc_out<sc_uint<32>> ADR;
    sc_out<sc_uint<32>> DAT_O;
    sc_out<sc_uint<2>>  SEL;
    sc_out<bool>        STB;
    sc_out<bool>        WE;
    sc_out<bool>        CYC;

    // DCache-Core
    sc_signal<sc_uint<32>> MEM_ADR;
    sc_signal<sc_uint<32>> MEM_DATA;
    sc_signal<bool>        MEM_ADR_VALID, MEM_STORE, MEM_LOAD;
    sc_signal<sc_uint<2>>  MEM_SIZE_SM;

    sc_signal<sc_uint<32>> MEM_RESULT;
    sc_signal<bool>        MEM_STALL;

    // ICache-Core
    sc_signal<sc_uint<32>> IF_ADR;
    sc_signal<bool>        IF_ADR_VALID;

    sc_signal<sc_bv<32>> IC_INST;
    sc_signal<bool>      IC_STALL;

    //DCache-Wrapper 
    sc_signal<bool> ICACHE_READ, ICACHE_WRITE;
    sc_signal<bool> DCACHE_DTA_VALID;
    sc_signal<sc_uint<32>> DCACHE_DT;
    sc_signal<sc_uint<32>> DCACHE_A;
    sc_signal<sc_uint<2>>  SIZE_SC;

    sc_signal<sc_uint<32>> WRAPPER_DT;
    sc_signal<bool>        DCACHE_ACK;

    sc_signal<bool>        STALL_O;

    //ICache-Wrapper
    sc_signal<bool>                 ICACHE_ACK;
    sc_signal<sc_uint<32>>          ICACHE_DT;
    sc_signal<sc_uint<32>>          ICACHE_A;
    sc_signal<bool>                 ICACHE_DTA_VALID;


    //Debug CORE
    sc_in<sc_uint<32>>  PC_RESET;
    sc_out<sc_uint<32>> PC_VALUE;

    void trace(sc_trace_file*);

    core        core_inst;
    icache      icache_inst;
    dcache      dcache_inst;
    wb_river_mc wrapper_inst;

    SC_CTOR(IP_RIVER):
        core_inst("core"),
        icache_inst("icache"),
        dcache_inst("dcache"),
        wrapper_inst("wb_river_mc")
    {
        //CORE map
        core_inst.CLK(CLK);
        core_inst.RESET(RESET_N);
        core_inst.DEBUG_PC_READ(PC_VALUE);
        core_inst.PC_INIT(PC_RESET);

        core_inst.MCACHE_ADR_SM(MEM_ADR);
        core_inst.MCACHE_DATA_SM(MEM_DATA);
        core_inst.MCACHE_ADR_VALID_SM(MEM_ADR_VALID);
        core_inst.MCACHE_STORE_SM(MEM_STORE);
        core_inst.MCACHE_LOAD_SM(MEM_LOAD);
        core_inst.MCACHE_RESULT_SM(MEM_RESULT);
        core_inst.MCACHE_STALL_SM(MEM_STALL);
        core_inst.MEM_SIZE_SM(MEM_SIZE_SM);
        core_inst.ADR_SI(IF_ADR);
        core_inst.ADR_VALID_SI(IF_ADR_VALID);
        core_inst.IC_INST_SI(IC_INST);
        core_inst.IC_STALL_SI(IC_STALL);

        //DCache map
        dcache_inst.CLK(CLK);
        dcache_inst.RESET_N(RESET_N);

        dcache_inst.DATA_ADR_SM(MEM_ADR);
        dcache_inst.DATA_SM(MEM_DATA);
        dcache_inst.LOAD_SM(MEM_LOAD);
        dcache_inst.STORE_SM(MEM_STORE);
        dcache_inst.MEM_SIZE_SM(MEM_SIZE_SM);
        dcache_inst.VALID_ADR_SM(MEM_ADR_VALID);
        dcache_inst.DATA_O(MEM_RESULT);
        dcache_inst.STALL_SC(MEM_STALL);
        dcache_inst.DTA_VALID_SC(DCACHE_DTA_VALID);
        dcache_inst.READ_SC(ICACHE_READ);
        dcache_inst.WRITE_SC(ICACHE_WRITE);
        dcache_inst.DT_O(DCACHE_DT);
        dcache_inst.A_O(DCACHE_A);
        dcache_inst.DT_I(WRAPPER_DT);
        dcache_inst.ACK(DCACHE_ACK);
        dcache_inst.SIZE_SC(SIZE_SC);
        dcache_inst.STALL_I(STALL_O);

        //ICache map
        icache_inst.CLK(CLK);
        icache_inst.RESET_N(RESET_N);
        
        icache_inst.ADR_SI(IF_ADR);
        icache_inst.ADR_VALID_SI(IF_ADR_VALID);
        icache_inst.IC_INST_SI(IC_INST);
        icache_inst.IC_STALL_SI(IC_STALL);
        icache_inst.DT(ICACHE_DT);
        icache_inst.A(ICACHE_A);
        icache_inst.DTA_VALID(ICACHE_DTA_VALID);
        icache_inst.ACK(ICACHE_ACK);
        icache_inst.STALL_I(STALL_O);

        //Wrapper map
        wrapper_inst.CLK(CLK);
        wrapper_inst.RESET_N(RESET_N);

        wrapper_inst.DAT_I(DAT_I);
        wrapper_inst.ACK_I(ACK);
        wrapper_inst.DAT_O(DAT_O);
        wrapper_inst.ADR_O(ADR);
        wrapper_inst.SEL_O(SEL);
        wrapper_inst.WE_O(WE);
        wrapper_inst.STB_O(STB);
        wrapper_inst.A_IC(ICACHE_A);
        wrapper_inst.DTA_VALID_IC(ICACHE_DTA_VALID);
        wrapper_inst.DT_IC(ICACHE_DT);
        wrapper_inst.ACK_IC(ICACHE_ACK);
        wrapper_inst.DTA_VALID_DC(DCACHE_DTA_VALID);
        wrapper_inst.READ_DC(ICACHE_READ);
        wrapper_inst.WRITE_DC(ICACHE_WRITE);
        wrapper_inst.SIZE_SEL_DC(SIZE_SC);
        wrapper_inst.DT_DC(DCACHE_DT);
        wrapper_inst.A_DC(DCACHE_A);
        wrapper_inst.DT_RM(WRAPPER_DT);
        wrapper_inst.ACK_DC(DCACHE_ACK);
        wrapper_inst.GRANT_I(GRANT);
        wrapper_inst.CYC_O(CYC);
        wrapper_inst.STALL_O(STALL_O);
    }
};