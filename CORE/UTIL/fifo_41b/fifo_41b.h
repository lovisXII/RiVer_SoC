#include <systemc.h>
#include "../debug_util.h"
SC_MODULE(fifo_41b)
{
    sc_in< sc_uint<41> > DIN ;
    sc_in_clk CLK ;
    sc_in<bool> PUSH, POP ;
    sc_in<bool> RESET_N ;
    
    sc_out<bool> FULL, EMPTY ;
    sc_out< sc_uint<41> > DOUT ;

    sc_signal<bool> fifo_v ;
    sc_signal< sc_uint<41> > data_inside ;
    
    void function() ;
    void trace(sc_trace_file* tf);
    SC_CTOR(fifo_41b)
    {
        SC_CTHREAD(function,fifo_41b::CLK.pos()) ;
        reset_signal_is(RESET_N, false) ;
    }
};