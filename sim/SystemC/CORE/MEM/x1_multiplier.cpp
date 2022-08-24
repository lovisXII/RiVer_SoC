#include "x1_multiplier.h"


void x1_multiplier::parse_data()
{
    sc_bv<320> in = IN_RX0.read();
    M[4].write((sc_bv_base)in.range(319,256));
    M[3].write((sc_bv_base)in.range(255,192));
    M[2].write((sc_bv_base)in.range(191,128));
    M[1].write((sc_bv_base)in.range(127,64));
    M[0].write((sc_bv_base)in.range(63,0));
}

// stage 6
void x1_multiplier::CSA1()
{
    sc_uint<64> xor_ = (sc_uint<64>)M[0] ^ (sc_uint<64>)M[1]; 
    product_s6[0] =  xor_ ^ (sc_uint<64>)M[2];

    sc_bv<64> shf = 0;
    shf.range(63, 1) = ((sc_uint<64>)M[0] & (sc_uint<64>)M[1]) | (xor_ & (sc_uint<64>)M[2]);
    product_s6[1] = shf;

}

/*void x1_multiplier::CSA2()
{
    sc_uint<64> xor_ = (sc_uint<64>)M[3] ^ (sc_uint<64>)M[4]; 
    product_s6[2] =  xor_ ^ (sc_uint<64>)M[5];

    sc_bv<64> shf = 0;
    shf.range(63, 1) = ((sc_uint<64>)M[3] & (sc_uint<64>)M[4]) | (xor_ & (sc_uint<64>)M[5]);  
    product_s6[3] = shf;
}*/

// stage 7
void x1_multiplier::CSA3()
{
    sc_uint<64> xor_ = (sc_uint<64>)product_s6[0] ^ (sc_uint<64>)product_s6[1]; 
    product_s7[0] =  xor_ ^ (sc_uint<64>)M[3];

    sc_bv<64> shf = 0;
    shf.range(63, 1) = ((sc_uint<64>)product_s6[0] & (sc_uint<64>)product_s6[1]) | (xor_ & (sc_uint<64>)M[3]);  
    product_s7[1] = shf;
}
// stage 8
void x1_multiplier::CSA4()
{
    sc_uint<64> xor_ = (sc_uint<64>)product_s7[0] ^ (sc_uint<64>)product_s7[1]; 
    product_s8[0] =  xor_ ^ (sc_uint<64>)M[4];

    sc_bv<64> shf = 0;
    shf.range(63, 1) = ((sc_uint<64>)product_s7[0] & (sc_uint<64>)product_s7[1]) | (xor_ & (sc_uint<64>)M[4]);  
    product_s8[1] = shf;
}
void x1_multiplier::fifo_concat() {
    sc_bv<x12x2_size> ff_din;
    ff_din[129]            = SIGNED_RES_RX0;   
    ff_din[128]            = SELECT_HIGHER_BITS_RX0;   
    ff_din.range(127, 64)  = (sc_bv_base)product_s8[1];
    ff_din.range(63, 0)    = (sc_bv_base)product_s8[0];

    x12x2_din_sx1.write(ff_din);
}
void x1_multiplier::fifo_unconcat()
{
    sc_bv<x12x2_size> ff_dout = x12x2_dout_sx1.read();
    SIGNED_RES_RX1.write((bool)ff_dout[129]);
    SELECT_HIGHER_BITS_RX1.write((bool)ff_dout[128]);
    RES_RX1.write(ff_dout.range(127, 0));
}
void x1_multiplier::manage_fifo() 
{
    bool stall = x12x2_full_sx1.read() || X02X1_EMPTY_SX0.read();
    x12x2_push_sx1.write(!stall);
    X02X1_POP_SX1.write(!stall);
}
void x1_multiplier::trace(sc_trace_file* tf)
{
    sc_trace(tf, x12x2_din_sx1, GET_NAME(x12x2_din_sx1));
    sc_trace(tf, x12x2_dout_sx1, GET_NAME(x12x2_dout_sx1));
    sc_trace(tf, x12x2_full_sx1, GET_NAME(x12x2_full_sx1));
    sc_trace(tf, X12X2_EMPTY_SX1, GET_NAME(X12X2_EMPTY_SX1));
    sc_trace(tf, IN_RX0, GET_NAME(IN_RX0));
    sc_trace(tf, RES_RX1, GET_NAME(RES_RX1));
    sc_trace(tf, X12X2_POP_SX2, GET_NAME(X12X2_POP_SX2));

    sc_trace(tf, SELECT_HIGHER_BITS_RX1, GET_NAME(SELECT_HIGHER_BITS_RX1));
    sc_trace(tf, SIGNED_RES_RX0, GET_NAME(SIGNED_RES_RX0));

    for(int i = 0; i < 5; i++)
    {
        std::string icname = "M_"+std::to_string(i);
        sc_trace(tf, M[i], signal_get_name(M[i].name(), icname.c_str()));
    }
    for(int i = 0; i < 2; i++)
    {
        std::string icname = "prod_s6_"+std::to_string(i);
        sc_trace(tf, product_s6[i], signal_get_name(product_s6[i].name(), icname.c_str()));
    }
    for(int i = 0; i < 2; i++)
    {
        std::string icname = "prod_s7_"+std::to_string(i);
        sc_trace(tf, product_s7[i], signal_get_name(product_s7[i].name(), icname.c_str()));
    }
    for(int i = 0; i < 2; i++)
    {
        std::string icname = "prod_s8_"+std::to_string(i);
        sc_trace(tf, product_s8[i], signal_get_name(product_s8[i].name(), icname.c_str()));
    }
    fifo_inst.trace(tf);
}