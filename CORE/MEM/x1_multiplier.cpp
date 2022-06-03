#include "x1_multiplier.h"


void x1_multiplier::parse_data()
{
    sc_bv<320> in = IN_SM.read();
    M4.write((sc_bv_base)in.range(319,256));
    M3.write((sc_bv_base)in.range(255,192));
    M2.write((sc_bv_base)in.range(191,128));
    M1.write((sc_bv_base)in.range(127,64));
    M0.write((sc_bv_base)in.range(63,0));
}

void x1_multiplier::CSA1()
{
    sc_uint<64> xor_ = (sc_uint<64>)M0 ^ (sc_uint<64>)M1; 
    product_s6[0] =  xor_ ^ (sc_uint<64>)M2;
    product_s6[1] = ((sc_uint<64>)M0 & (sc_uint<64>)M1) | (xor_ & (sc_uint<64>)M2);  
}

void x1_multiplier::CSA2()
{
    sc_uint<64> xor_ = (sc_uint<64>)product_s6[0] ^ (sc_uint<64>)product_s6[1]; 
    product_s7[0] =  xor_ ^ (sc_uint<64>)M3;
    product_s7[1] = ((sc_uint<64>)product_s6[0] & (sc_uint<64>)product_s6[1]) | (xor_ & (sc_uint<64>)M3);  
}

void x1_multiplier::CSA3()
{
    sc_uint<64> xor_ = (sc_uint<64>)product_s7[0] ^ (sc_uint<64>)product_s7[1]; 
    product_s8[0] =  xor_ ^ (sc_uint<64>)M4;
    product_s8[1] = ((sc_uint<64>)product_s7[0] & (sc_uint<64>)product_s7[1]) | (xor_ & (sc_uint<64>)M4);  
}

void x1_multiplier::RES()
{
    sc_bv<128> res;
    res.range(127, 64) = product_s8[1];
    res.range(63, 0) = product_s8[0];

    RES_SM.write(res);
}

void x1_multiplier::trace(sc_trace_file* tf)
{
    sc_trace(tf, IN_SM, GET_NAME(IN_SM));
    sc_trace(tf, RES_SM, GET_NAME(RES_SM));
}