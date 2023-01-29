#pragma once

#include <systemc.h>
//#include "../CORE/core.h"
#include "../../CORE/core.h"
#include "CACHES/icache.h"
#include "CACHES/dcache.h"
#include "wb_river_mc.h"

SC_MODULE(IP_RIVER)
{
    sc_in_clk           CLK;
    sc_in<bool>         RESET_N;

    //interface bus
    sc_in<bool>        ACK;
    sc_in<sc_uint<32>> DAT_I;
    sc_in<bool>        GRANT;
    sc_in<sc_uint<32>> ADR_I;

    sc_out<sc_uint<32>> ADR;
    sc_out<sc_uint<32>> DAT_O;
    sc_out<sc_uint<2>>  SEL;
    sc_out<bool>        STB;
    sc_out<bool>        WE;
    sc_out<bool>        CYC;

    // DCache-Core
    sc_signal<sc_uint<32>> DATA_ADR_SM;
    sc_signal<sc_uint<32>> DATA_SM;
    sc_signal<bool>        VALID_ADR_SM;
    sc_signal<bool>        STORE_SM;
    sc_signal<bool>        LOAD_SM;
    sc_signal<sc_uint<2>>  MEM_SIZE_SM;

    sc_signal<sc_uint<32>> DATA_SDC;
    sc_signal<bool>        STALL_SDC;

    // ICache-Core
    sc_signal<sc_uint<32>> ADR_SI;
    sc_signal<bool>        ADR_VALID_SI;

    sc_signal<sc_bv<32>> INST_SIC;
    sc_signal<bool>      STALL_SIC;

    //DCache-Wrapper 
    sc_signal<bool> READ_SDC;
    sc_signal<bool> WRITE_SDC;
    sc_signal<bool> DTA_VALID_SDC;
    sc_signal<sc_uint<32>> DCACHE_DT;
    sc_signal<sc_uint<32>> A_SDC;
    sc_signal<sc_uint<2>>  SIZE_SDC;

    sc_signal<sc_uint<32>> WRAPPER_DT;
    sc_signal<bool>        DCACHE_ACK;

    sc_signal<bool>        STALL_SW;

    //ICache-Wrapper
    sc_signal<bool>                 ICACHE_ACK;
    sc_signal<sc_uint<32>>          ICACHE_DT;
    sc_signal<sc_uint<32>>          A_SIC;
    sc_signal<bool>                 DTA_VALID_SIC;


    //Debug CORE
    sc_in<sc_uint<32>>  PC_RESET;
    sc_out<sc_uint<32>> PC_VALUE;
    sc_in<sc_uint<32>>  PROC_ID;

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
        core_inst.PROC_ID(PROC_ID);

        core_inst.MCACHE_ADR_SM(DATA_ADR_SM);
        core_inst.MCACHE_DATA_SM(DATA_SM);
        core_inst.MCACHE_ADR_VALID_SM(VALID_ADR_SM);
        core_inst.MCACHE_STORE_SM(STORE_SM);
        core_inst.MCACHE_LOAD_SM(LOAD_SM);
        core_inst.MCACHE_RESULT_SM(DATA_SDC);
        core_inst.MCACHE_STALL_SM(STALL_SDC);
        core_inst.MEM_SIZE_SM(MEM_SIZE_SM);
        core_inst.ADR_SI(ADR_SI);
        core_inst.ADR_VALID_SI(ADR_VALID_SI);
        core_inst.INST_SIC(INST_SIC);
        core_inst.STALL_SIC(STALL_SIC);

        //DCache map
        dcache_inst.CLK(CLK);
        dcache_inst.RESET_N(RESET_N);

        dcache_inst.DATA_ADR_SM(DATA_ADR_SM);
        dcache_inst.DATA_SM(DATA_SM);
        dcache_inst.LOAD_SM(LOAD_SM);
        dcache_inst.STORE_SM(STORE_SM);
        dcache_inst.VALID_ADR_SM(VALID_ADR_SM);
        dcache_inst.MEM_SIZE_SM(MEM_SIZE_SM);

        dcache_inst.DATA_SDC(DATA_SDC);
        dcache_inst.STALL_SDC(STALL_SDC);

        dcache_inst.DTA_VALID_SDC(DTA_VALID_SDC);
        dcache_inst.READ_SDC(READ_SDC);
        dcache_inst.WRITE_SDC(WRITE_SDC);
        dcache_inst.SIZE_SDC(SIZE_SDC);

        dcache_inst.DT_SDC(DCACHE_DT);
        dcache_inst.A_SDC(A_SDC);

        dcache_inst.DT_SW(WRAPPER_DT);
        dcache_inst.ACK_SW(DCACHE_ACK);
        
        dcache_inst.ADR_SW(ADR_I);
        dcache_inst.GRANT(GRANT);    // signal for snoopy control
        dcache_inst.STALL_SW(STALL_SW);

        //ICache map
        icache_inst.CLK(CLK);
        icache_inst.RESET_N(RESET_N);
        
        icache_inst.ADR_SI(ADR_SI);
        icache_inst.ADR_VALID_SI(ADR_VALID_SI);
        icache_inst.INST_SIC(INST_SIC);
        icache_inst.STALL_SIC(STALL_SIC);
        icache_inst.DT_SW(ICACHE_DT);
        icache_inst.A_SIC(A_SIC);
        icache_inst.DTA_VALID_SIC(DTA_VALID_SIC);
        icache_inst.ACK_SW(ICACHE_ACK);
        icache_inst.STALL_SW(STALL_SW);

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
        wrapper_inst.A_IC(A_SIC);
        wrapper_inst.DTA_VALID_IC(DTA_VALID_SIC);
        wrapper_inst.DT_IC(ICACHE_DT);
        wrapper_inst.ACK_IC(ICACHE_ACK);
        wrapper_inst.DTA_VALID_DC(DTA_VALID_SDC);
        wrapper_inst.READ_DC(READ_SDC);
        wrapper_inst.WRITE_DC(WRITE_SDC);
        wrapper_inst.SIZE_SEL_DC(SIZE_SDC);
        wrapper_inst.DT_DC(DCACHE_DT);
        wrapper_inst.A_DC(A_SDC);
        wrapper_inst.DT_RM(WRAPPER_DT);
        wrapper_inst.ACK_DC(DCACHE_ACK);
        wrapper_inst.GRANT_I(GRANT);
        wrapper_inst.CYC_O(CYC);
        wrapper_inst.STALL_SW(STALL_SW);
    }
};