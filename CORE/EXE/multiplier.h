#pragma once
#include <systemc.h>
#include "../UTIL/debug_util.h"


//   OP1
// x OP2
//  ------

SC_MODULE(multiplier)
{
    sc_in< sc_uint<32> > OP1_SE, OP2_SE;
    sc_in< sc_uint<2> > CMD_SE;


    sc_out< sc_uint<32> > RES_SE;

    bool bit_adder(sc_uint<1>,sc_uint<1>,sc_uint<1>);
    bool carry_prod(sc_uint<1>,sc_uint<1>,sc_uint<1>);

    void operation();
    void trace(sc_trace_file* tf);

    SC_CTOR(multiplier)
    {
        SC_METHOD(operation);
        sensitive << OP1_SE << OP2_SE << CMD_SE;
    }
};