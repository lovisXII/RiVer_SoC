#pragma once
#include <systemc.h>
#include <iostream>
#include "constants.h"
#include "debug_util.h"
#include "fifo.h"

SC_MODULE(ifetch) {
    /*****************************************************
        Interface with Instruction Cache, or RAM
    ******************************************************/

    // the instruction read from memory
    sc_in<sc_bv<32>> IC_INST_SI;
    // a stall signal, in case of a cache miss, or memory delay
    sc_in<bool> IC_STALL_SI;
    // The adress of the instruction to fetch
    sc_out<sc_uint<32>> ADR_SI;
    // Whether the adress is valid (no need to do a memory access if it is not)
    sc_out<bool> ADR_VALID_SI;

    /*****************************************************
                    Interface with Dec2if
    ******************************************************/

    sc_in<bool>  DEC2IF_EMPTY_SD;
    sc_out<bool> DEC2IF_POP_SI;

    /*****************************************************
                    Interface with If2Dec
    ******************************************************/

    sc_in<bool>  IF2DEC_FLUSH_SD;  // allow to flush if2dec in case of a branch
    sc_in<bool>  IF2DEC_POP_SD;
    sc_out<bool> IF2DEC_EMPTY_SI;

    /*****************************************************
                    Interface with Decod
    ******************************************************/
    sc_out<sc_uint<32>> PC_RI;     // pc sent to if2dec
    sc_out<sc_bv<32>>   INSTR_RI;  // instruction sent to if2dec
    sc_in<sc_bv<32>>    PC_RD;     // PC coming to fetch an instruction

    // Global Interface :
    sc_in_clk   CLK;
    sc_in<bool> RESET;

    // Component instanciation
    fifo<IF2DEC_SIZE> fifo_inst;

    // Internals signals :

    sc_signal<bool>               IF2DEC_PUSH_SI;
    sc_signal<bool>               IF2DEC_FULL_SI;
    sc_signal<sc_bv<IF2DEC_SIZE>> if2dec_in_si;
    sc_signal<sc_bv<IF2DEC_SIZE>> if2dec_out_si;  // instruction sent to if2dec

    void fetch_method();
    void trace(sc_trace_file * tf);

    SC_CTOR(ifetch) : fifo_inst("if2dec") {
        fifo_inst.DATAIN_S(if2dec_in_si);
        fifo_inst.DATAOUT_R(if2dec_out_si);
        fifo_inst.EMPTY_S(IF2DEC_EMPTY_SI);
        fifo_inst.FULL_S(IF2DEC_FULL_SI);
        fifo_inst.PUSH_S(IF2DEC_PUSH_SI);
        fifo_inst.POP_S(IF2DEC_POP_SD);
        fifo_inst.CLK(CLK);
        fifo_inst.RESET(RESET);

        SC_METHOD(fetch_method);
        sensitive << IC_INST_SI << DEC2IF_EMPTY_SD << IF2DEC_FULL_SI << PC_RD << IF2DEC_FLUSH_SD
                  << IC_STALL_SI << RESET;
    }
};
