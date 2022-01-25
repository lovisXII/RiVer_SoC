#include <systemc.h>

SC_MODULE(fifo_32b)
{
    sc_in< sc_uint<32> > din ;
    sc_in<bool> clk ;
    sc_in<bool> push, pop ;
    sc_in<bool> reset_n ;
    
    sc_out< sc_uint<32> > dout ;
 
    void processClock() ;

    SC_CTOR(fifo_32b)
    {
        SC_METHOD(processClock) ;
        sensitive << clk ;
    }
};