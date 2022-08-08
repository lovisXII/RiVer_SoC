#pragma once
#include <systemc.h>
#include <iostream>
#include "../UTIL/debug_util.h"

#define N_CSR        13
#define adress_mtvec 0x80000000
#define main_adresse 0x10054

SC_MODULE(csr) {
    // Input from M1:

    sc_in<sc_uint<12>> CSR_WADR_SM_S1;
    sc_in<sc_uint<32>> CSR_WDATA_SM_S1;
    sc_in<bool>        CSR_ENABLE_SM_S1;

<<<<<<< HEAD
    // Input from M2
=======
    sc_in<sc_uint<32>> MSTATUS_WDATA_RM_S1;
    sc_in<sc_uint<32>> MIP_WDATA_RM_S1;
    sc_in<sc_uint<32>> MEPC_WDATA_RM_S1;
    sc_in<sc_uint<32>> MCAUSE_WDATA_SM_S1;
    sc_in<sc_uint<32>> MTVAL_WDATA_SM_S1;
>>>>>>> 0af37f80 (error with csr in S2, try to find the source of it. Rewritte of csr to allow mem2 to write it)

    // Input from M2

    sc_in<sc_uint<12>> CSR_WADR_SM_S2;
    sc_in<sc_uint<32>> CSR_WDATA_SM_S2;
    sc_in<bool>        CSR_ENABLE_SM_S2;
        
    sc_in<sc_uint<32>> MSTATUS_WDATA_RM_S2;
    sc_in<sc_uint<32>> MIP_WDATA_RM_S2;
    sc_in<sc_uint<32>> MEPC_WDATA_RM_S2;
    sc_in<sc_uint<32>> MCAUSE_WDATA_SM_S2;
    sc_in<sc_uint<32>> MTVAL_WDATA_SM_S2;

    // Reading csr :

    sc_in<bool>        EXCEPTION_SM_S1;
    sc_in<bool>        EXCEPTION_SM_S2;

    sc_out<sc_uint<32>> MEPC_SC;
    sc_out<sc_uint<32>> MSTATUS_RC;
    sc_out<sc_uint<32>> MTVEC_VALUE_RC;
    sc_out<sc_uint<32>> MIP_VALUE_RC;
    sc_out<sc_uint<32>> MCAUSE_SC;
    sc_out<sc_uint<32>> KERNEL_ADR_SC;

    // Output for S1:

    sc_in<sc_uint<12>>  CSR_RADR_SD_S1;
    sc_out<sc_uint<32>> CSR_RDATA_SC_S1;
    
    // Output for S2:

    sc_in<sc_uint<12>>  CSR_RADR_SD_S2;
    sc_out<sc_uint<32>> CSR_RDATA_SC_S2;

    // General Interface :

    sc_in_clk   CLK;
    sc_in<bool> RESET_N;

    // Internal signals :
    /*** Register Map ***
     * 0: mvendorid 0xF11
     * 1: marchid 0xF12
     * 2: mimpid 0xF13
     * 3: mstatus 0x300
     * 4: misa 0x301
     * 5: mie 0x304
     * 6: mtvec 0x305
     * 7: mstatush 0x310
     * 8: mepc 0x341
     * 9: mcause 0x342
     * 10: mtval 0x343
     * 11: mip 0x344
     * 12: mscratch 0x340
     * 13 : kernel_adr 0x800 (custom)
     ***/
    sc_signal<sc_uint<32>> csr_rc[N_CSR];
    sc_signal<sc_uint<32>> test_sc;
    void                   writing_csr();
    void                   reading_csr();
    void                   trace(sc_trace_file * tf);
    SC_CTOR(csr) {
        SC_CTHREAD(writing_csr, CLK.pos());
        SC_METHOD(reading_csr);
<<<<<<< HEAD
        sensitive << CSR_WADR_SM_S1 << CSR_RADR_SD_S1 << CSR_ENABLE_SM_S1
        << CSR_RADR_SD_S2 ;
=======
        sensitive << CSR_WADR_SM_S1 << CSR_RADR_SD_S1 << CSR_ENABLE_SM_S1 << EXCEPTION_SM_S1 << MSTATUS_WDATA_RM_S1
                  << MIP_WDATA_RM_S1 << MEPC_WDATA_RM_S1 << MCAUSE_WDATA_SM_S1;
>>>>>>> 0af37f80 (error with csr in S2, try to find the source of it. Rewritte of csr to allow mem2 to write it)
        for (int i = 0; i < N_CSR; i++)
            sensitive << csr_rc[i];
    }
};