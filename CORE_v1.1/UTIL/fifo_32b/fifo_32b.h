#pragma once
#include <systemc.h>
#include "../debug_util.h"
SC_MODULE(fifo_32b)
{
    sc_in< sc_uint<32> > DIN ;
    sc_in_clk CLK ;
    sc_in<bool> PUSH, POP ;
    sc_in<bool> RESET_N ;
    
    sc_out<bool> FULL, EMPTY ;
    sc_out< sc_uint<32> > DOUT ;

    sc_signal<bool> fifo_v ;
    sc_signal< sc_uint<32> > data_inside ;
    
    void function() ;
    void flags_update() ;
    void trace(sc_trace_file* tf);
    SC_CTOR(fifo_32b)
    {
        SC_CTHREAD(function,fifo_32b::CLK.pos()) ;
        SC_METHOD(flags_update);
        sensitive << PUSH << POP << fifo_v << data_inside;

        reset_signal_is(RESET_N, false) ;
    }
};