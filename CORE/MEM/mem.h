#pragma once
#include <systemc.h>
#include <iostream>
#include "../UTIL/debug_util.h"
#include "../UTIL/fifo.h"

/*
Assuming the following stuff :

MEM_SIZE = 00 -> access in word
MEM_SIZE = 01 -> access in half word
MEM_SIZE = 10  -> acces in byte

*/
SC_MODULE(mem) {
    // Mcache Interface :

    sc_out<sc_uint<32>> MCACHE_ADR_SM;  // adress in memory
    sc_out<sc_uint<32>> MCACHE_DATA_SM;
    sc_out<bool>        MCACHE_ADR_VALID_SM, MCACHE_STORE_SM, MCACHE_LOAD_SM;

    sc_in<sc_uint<32>> MCACHE_RESULT_SM;
    sc_in<bool>        MCACHE_STALL_SM;

    // Exe Interface :

    sc_in<sc_uint<32>> EXE_RES_RE;
    sc_in<sc_uint<32>> MEM_DATA_RE;
    sc_in<sc_uint<6>>  DEST_RE;
    sc_in<sc_uint<2>>  MEM_SIZE_RE;
    sc_in<sc_uint<32>> PC_EXE2MEM_RE;
    sc_in<bool>        WB_RE,
        SIGN_EXTEND_RE;  // taille fifo entrée : 74
    sc_in<bool> LOAD_RE, STORE_RE;

    sc_in<bool>        CSR_WENABLE_RE;
    sc_in<sc_uint<12>> CSR_WADR_SE;
    // sc_in<sc_uint<32>> OP1_CSR_RE;

    // exe2mem interface :

    sc_in<bool>  EXE2MEM_EMPTY_SE;
    sc_out<bool> EXE2MEM_POP_SM;

    // mem2wbk interface

    sc_in<bool>         MEM2WBK_POP_SW;
    sc_signal<bool>     mem2wbk_push_sm;
    sc_signal<bool>     mem2wbk_full_sm;
    sc_out<bool>        MEM2WBK_EMPTY_SM;
    sc_out<sc_uint<32>> PC_MEM2WBK_RM;
    sc_out<bool>        CSR_WENABLE_RM;

    // WBK interface
    sc_out<sc_uint<32>> MEM_RES_RM;
    sc_out<sc_uint<6>>  DEST_RM;
    sc_out<bool>        WB_RM;

    // Internal signals
    sc_signal<sc_bv<107>> mem2wbk_din_sm;
    sc_signal<sc_bv<107>> mem2wbk_dout_sm;

    sc_signal<sc_uint<32>> data_sm;
    sc_signal<bool>        wb_sm;
    // Global Interface :

    sc_in_clk CLK;
    sc_in_clk RESET;

    // Interruption :

    sc_in<bool> INTERRUPTION_SE;

    // CSR Interface :

    sc_out<sc_uint<12>> CSR_WADR_SM;
    sc_out<sc_uint<32>> CSR_WDATA_SM;

    // FIFO
    fifo<107> fifo_inst;

    void mem2wbk_concat();
    void mem2wbk_unconcat();
    void fifo_gestion();
    void mem_preprocess();
    void sign_extend();
    void interruption();
    void csr();

    void trace(sc_trace_file * tf);

    SC_CTOR(mem) : fifo_inst("mem2wbk") {
        fifo_inst.DIN_S(mem2wbk_din_sm);
        fifo_inst.DOUT_R(mem2wbk_dout_sm);
        fifo_inst.EMPTY_S(MEM2WBK_EMPTY_SM);
        fifo_inst.FULL_S(mem2wbk_full_sm);
        fifo_inst.PUSH_S(mem2wbk_push_sm);
        fifo_inst.POP_S(MEM2WBK_POP_SW);
        fifo_inst.CLK(CLK);
        fifo_inst.RESET_N(RESET);

        SC_METHOD(mem2wbk_concat);
        sensitive << data_sm << DEST_RE << MEM_SIZE_RE << wb_sm << SIGN_EXTEND_RE;
        SC_METHOD(mem2wbk_unconcat);
        sensitive << mem2wbk_dout_sm;
        SC_METHOD(fifo_gestion);
        sensitive << MCACHE_STALL_SM << mem2wbk_full_sm << EXE2MEM_EMPTY_SE << wb_sm;
        SC_METHOD(mem_preprocess);
        sensitive << WB_RE << LOAD_RE << MCACHE_RESULT_SM << EXE_RES_RE << MEM_DATA_RE << STORE_RE << EXE2MEM_EMPTY_SE;
        SC_METHOD(sign_extend);
        sensitive << MEM_SIZE_RE << SIGN_EXTEND_RE << MCACHE_RESULT_SM << EXE_RES_RE << LOAD_RE;
    }
};