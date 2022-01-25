#include <systemc.h>

SC_MODULE(fifo_32b)
{
    sc_in< sc_uint<32> > DIN ;
    sc_in_clk CLK ;
    sc_out< sc_uint<32> > DOUT ;
    sc_in<bool> PUSH, POP ;
    sc_in<bool> RESET_N ;
    
    void function() ;

    SC_CTOR(fifo_32b)
    {
        SC_CTHREAD(function,fifo_32b::CLK.pos()) ;
    }
};