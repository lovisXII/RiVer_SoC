#include "x2_multiplier.h"




void x2_multiplier::pre_process()
{
    sc_bv<128> in = IN_SW.read();
    a = in.range(63, 0);
    b = in.range(127, 64);

    c[0] = false;

    //test
    sc_biguint<64> a_ = (sc_bv<64>)a;
    sc_biguint<64> b_ = (sc_bv<64>)b;
    sc_biguint<64> test = a_ + b_;
    for(int i = 0; i < 64; i++)
        S[i] = (bool)test[i];
}

void x2_multiplier::MFA_0()
{
    /*sc_bv<64> a_ = (sc_bv<64>)a;
    sc_bv<64> b_ = (sc_bv<64>)b;
    
    P[0] = (bool)a_[0] ^ (bool)b_[0];
    G[0] = (bool)a_[0] & (bool)b_[0];
    S[0] = ((bool)a_[0] ^ (bool)b_[0]) ^ c[0];*/
}
void x2_multiplier::C_gen_1()
{
    c[1] = G[0] + (P[0] & c[0]);
}

void x2_multiplier::RES()
{
    sc_bv<64> res;
    for(int i = 0; i < 64; i++)
        res[i] = S;

    if(RET_HIGH_SW)
        RES_SW.write((sc_bv_base)res.range(63, 32));
    else
        RES_SW.write((sc_bv_base)res.range(31, 0));
}