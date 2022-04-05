#pragma once
#include <systemc.h>
#include <iostream>

SC_MODULE(kreg) {
    // Input :

    sc_in<sc_uint<12>> ADR_CSR_SM;
    sc_in<sc_uint<32>> KREG_DATA_WRITE_SM;

    // Output :

    sc_out<sc_uint<32>> DATA_READ_CSR_SK;

    // General Interface :

    sc_in_clk   CLK;
    sc_in<bool> RESET_N;

    // Internal signals :

    sc_signal<sc_uint<32>> CSR_adress_SK[12];

    void writing_csr();
    void reading_csr();
    SC_CTOR(kreg) {
        SC_CTHREAD(writing_csr, CLK.pos());
        sensitive 
			<< KREG_DATA_WRITE_SM 
			<< RESET_N;
        SC_METHOD(reading_csr);
        sensitive 
			<< ADR_CSR_SM;
    }
};