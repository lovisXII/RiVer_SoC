#pragma once
#include <systemc.h>
#include <iostream>
#include "../UTIL/fifo_generic/fifo_generic.h"
#include "../UTIL/debug_util.h"
SC_MODULE(ifetch)
{
    // Icache Interface :

    sc_out<sc_uint<32> > IF_ADR ; // @ which we search a data block from memory
    sc_out<bool> IF_ADR_VALID ; 

    sc_in<sc_bv<32> > IC_INST ;
    sc_in<bool> IC_STALL ;
    
    //Dec Interface :


    //dec2if interface :
    
    sc_in<bool> DEC2IF_EMPTY ; 
    sc_out<bool> DEC2IF_POP ;

    //if2dec interface
    
    sc_in<bool> IF2DEC_FLUSH ; // allow to flush if2dec in case of a branch
    sc_in<bool> IF2DEC_POP ;
    sc_signal<bool> IF2DEC_PUSH ;
    sc_signal<bool> IF2DEC_FULL ;
    sc_out<bool> IF2DEC_EMPTY ;
    
    sc_in<sc_bv<32>  > DEC_PC ; // PC coming to fetch an instruction
    sc_out<sc_bv<32> > IF_IR ; // instruction sent to if2dec 
    
    //Global Interface :

    sc_in_clk CLK;
    sc_in_clk RESET;

    // FIFO
    fifo_generic<32>    fifo_inst;

    void fetch_method();
    void trace(sc_trace_file* tf);
    SC_CTOR(ifetch) : 
    fifo_inst("if2dec")
    {
        fifo_inst.DIN(IC_INST);
        fifo_inst.DOUT(IF_IR);
        fifo_inst.EMPTY(IF2DEC_EMPTY);
        fifo_inst.FULL(IF2DEC_FULL);
        fifo_inst.PUSH(IF2DEC_PUSH);
        fifo_inst.POP(IF2DEC_POP);
        fifo_inst.CLK(CLK);
        fifo_inst.RESET_N(RESET);

        SC_METHOD(fetch_method);
        sensitive << DEC2IF_EMPTY << IF2DEC_FULL << DEC_PC << IF2DEC_FLUSH << IC_STALL << RESET; 
    }

};

