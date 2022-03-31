#pragma once
#include <systemc.h>
#include <iostream>
#include "../UTIL/fifo.h"
#include "../UTIL/debug_util.h"
SC_MODULE(ifetch)
{
    // Icache Interface :

    sc_out<sc_uint<32>> ADR_SI ; // @ which we search a data block from memory
    sc_out<bool> ADR_VALID_SI ; 

    sc_in<sc_bv<32> > IC_INST_SI ;
    sc_in<bool> IC_STALL_SI ;
    
    //Dec Interface :


    //dec2if interface :
    
    sc_in<bool> DEC2IF_EMPTY_SI ; 
    sc_out<bool> DEC2IF_POP_SI ;

    //if2dec interface
    
    sc_in<bool> IF2DEC_FLUSH_SD ; // allow to flush if2dec in case of a branch
    sc_in<bool> IF2DEC_POP_SD ;
    sc_signal<bool> IF2DEC_PUSH_SI ;
    sc_signal<bool> IF2DEC_FULL_SI ;
    sc_out<bool> IF2DEC_EMPTY_SI ;
    
    sc_in<sc_bv<32>  > PC_RD ; // PC coming to fetch an instruction
    sc_out<sc_bv<32>> INSTR_RI ; // instruction sent to if2dec 
    sc_out<sc_bv<32>> PC_OUT_RI ; // pc sent to if2decs
    //Global Interface :

    sc_in_clk CLK;
    sc_in_clk RESET;

    // FIFO
    fifo<64>    fifo_inst;

    // Internals signals :

    sc_signal<sc_bv<64>>  if2dec_in ; 
    sc_signal<sc_bv<64> > instr_ri ; // instruction sent to if2dec 

    void fetch_method();
    void trace(sc_trace_file* tf);
    SC_CTOR(ifetch) : 
    fifo_inst("if2dec")
    {
        fifo_inst.DIN_S(if2dec_in);
        fifo_inst.DOUT_R(instr_ri);
        fifo_inst.EMPTY_S(IF2DEC_EMPTY_SI);
        fifo_inst.FULL_S(IF2DEC_FULL_SI);
        fifo_inst.PUSH_S(IF2DEC_PUSH_SI);
        fifo_inst.POP_S(IF2DEC_POP_SD);
        fifo_inst.CLK(CLK);
        fifo_inst.RESET_N(RESET);

        SC_METHOD(fetch_method);
        sensitive   << IC_INST_SI
                    << DEC2IF_EMPTY_SI 
                    << IF2DEC_FULL_SI 
                    << PC_RD 
                    << IF2DEC_FLUSH_SD 
                    << IC_STALL_SI 
                    << RESET; 
    }

};

