#pragma once
#include <systemc.h>
#include <iostream>
#include "../../UTIL/debug_util.h"

#define N_CSR        15
#define adress_mtvec 0x80000000
#define main_adresse 0x10054

SC_MODULE(csr) {
    // Input :

    sc_in<sc_uint<12>> CSR_WADR_SM;
    sc_in<sc_uint<32>> CSR_WDATA_SM;
    sc_in<bool>        CSR_ENABLE_SM;

    sc_in<bool>        EXCEPTION_SM;
    sc_in<sc_uint<32>> MSTATUS_WDATA_RM;
    sc_in<sc_uint<32>> MIP_WDATA_RM;
    sc_in<sc_uint<32>> MEPC_WDATA_RM;
    sc_in<sc_uint<32>> MCAUSE_WDATA_SM;
    sc_in<sc_uint<32>> MTVAL_WDATA_SM;

    sc_out<sc_uint<32>> MEPC_SC;
    sc_out<sc_uint<32>> MSTATUS_RC;
    sc_out<sc_uint<32>> MTVEC_VALUE_RC;
    sc_out<sc_uint<32>> MIP_VALUE_RC;
    sc_out<sc_uint<32>> MIE_VALUE_RC;
    sc_out<sc_uint<32>> MCAUSE_SC;
    sc_out<sc_uint<32>> KERNEL_ADR_SC;

    // Output :

    sc_in<sc_uint<12>>  CSR_RADR_SD;
    sc_out<sc_uint<32>> CSR_RDATA_SC;

    // Timer interface
    sc_out<bool>       TIMER_CONFIG_WB_SC;
    sc_out<bool>       TIMER_DIVIDER_WB_SC;
    sc_in<sc_uint<64>> TIME_RT;
    sc_in<bool>        TIMER_INT_ST;
    sc_out<bool>       ACK_SP;

    // General Interface :

    sc_in_clk   CLK;
    sc_in<bool> RESET_N;
    sc_in<sc_uint<32>>  PROC_ID;

    // Internal signals :
    /*** Register Map ***
     * 0:  mvendorid 0xF11
     * 1:  marchid 0xF12
     * 2:  mimpid 0xF13
     * 3:  mhartid 0xF14
     * 4:  mstatus 0x300
     * 5:  misa 0x301
     * 6:  mie 0x304
     * 7:  mtvec 0x305
     * 8:  mstatush 0x310
     * 9:  mepc 0x341
     * 10: mcause 0x342
     * 11: mtval 0x343
     * 12: mip 0x344
     * 13: mscratch 0x340
     * 14: kernel_adr 0x800 (custom)
     ***/
    sc_signal<sc_uint<32>> csr_rc[N_CSR];
    sc_signal<sc_uint<32>> test_sc;
    void                   writing_csr();
    void                   reading_csr();
    void                   transmit_to_timer();
    void                   trace(sc_trace_file * tf);
    SC_CTOR(csr) {
        SC_CTHREAD(writing_csr, CLK.pos());
        SC_METHOD(transmit_to_timer);
        sensitive << CSR_ENABLE_SM << CSR_WADR_SM;
        SC_METHOD(reading_csr);
        sensitive << CSR_WADR_SM << CSR_RADR_SD << CSR_ENABLE_SM << EXCEPTION_SM << MSTATUS_WDATA_RM << MIP_WDATA_RM
                  << MEPC_WDATA_RM << MCAUSE_WDATA_SM << TIMER_INT_ST;
        for (int i = 0; i < N_CSR; i++)
            sensitive << csr_rc[i];
    }
};