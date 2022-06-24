#include "x2_multiplier.h"

void x2_multiplier::process()
{
    sc_bv<128> in = IN_RX1.read();
    sc_biguint<64> a = (sc_bv<64>)in.range(63, 0);
    sc_biguint<64> b = (sc_bv<64>)in.range(127, 64);

    sc_bv<64> add = a + b;
    result = add;
    /*if(SIGNED_RES_RX1.read())
    {
        int i;
        for(i = 63; i >= 0; i--)
            if(add[i] == 1)
                break;
        for(; i < 64; i++)
            add[i] = 1;
    }*/

    if(SELECT_HIGHER_BITS_RX1)
        RES_RX2.write((sc_bv_base)add.range(63, 32));
    else
        RES_RX2.write((sc_bv_base)add.range(31, 0));
}
void x2_multiplier::manage_fifo() 
{
    X12X2_POP_SX2.write(!X12X2_EMPTY_SX1);
}
void x2_multiplier::trace(sc_trace_file* tf)
{
    sc_trace(tf, IN_RX1, GET_NAME(IN_RX1));
    sc_trace(tf, a, GET_NAME(a));
    sc_trace(tf, b, GET_NAME(b));
    sc_trace(tf, RES_RX2, GET_NAME(RES_RX2));
    sc_trace(tf, SELECT_HIGHER_BITS_RX1, GET_NAME(SELECT_HIGHER_BITS_RX1));
    sc_trace(tf, result, GET_NAME(result));
}