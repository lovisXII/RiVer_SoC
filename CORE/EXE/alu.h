#pragma once
#include <systemc.h>
#include "../UTIL/debug_util.h"



SC_MODULE(alu)
{
    sc_in< sc_uint<32> >  OP1,OP2 ;
    sc_in< bool > CIN ;
    sc_in< sc_uint<2> > CMD ;
    sc_out< sc_uint<32> > RES ;
        
    void operation() ;
    void trace(sc_trace_file* tf);

    SC_CTOR(alu)
    {
        SC_METHOD(operation);
        sensitive << OP1 << OP2 << CIN ;
    }
};

