#include <systemc.h>

SC_MODULE(fifo_32b)
{
    sc_in< sc_uint<32> > din ;
    sc_in<bool> clk ;
    sc_out< sc_uint<32> > dout ;
    sc_in<bool> push, pop ;
    sc_in<bool> reset_n ;
    void function() ;

    SC_CTOR(fifo_32b)
    {
        SC_METHOD(function) ;
        sensitive << clk ;
    }
};