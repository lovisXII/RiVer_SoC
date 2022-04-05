#pragma once
#include <systemc.h>
#include <iostream>
#define N_CSR 12
SC_MODULE(csr) {
    // Input :

    sc_in<sc_uint<12>> WADR_CSR_SM;
    sc_in<sc_uint<32>> CSR_DATA_WRITE_SM;
    sc_in<bool>        CSR_WRITE_ENABLE_SM;

    // Output :

    sc_in<sc_uint<12>>  RADR_CSR_SD;
    sc_out<sc_uint<32>> DATA_READ_CSR_SC;

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
     ***/
    sc_signal<sc_uint<32>> csr_rc[N_CSR];

    void writing_csr();
    void reading_csr();
    SC_CTOR(csr) {
        SC_CTHREAD(writing_csr, CLK.pos());
        SC_METHOD(reading_csr);
        sensitive << RADR_CSR_SD;
    }
};