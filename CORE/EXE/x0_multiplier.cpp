#include "x0_multiplier.h"

void x0_multiplier::operation()
{
    // +-------------------------------------------------+
    // |               WALLACE MULTIPLIER                |
    // +-------------------------------------------------+
    // |                        a3    a2   a1   a0       | 
    // |                      x b3    b2   b1   b0       | 
    // |                       ------------------        |
    // | 0    0     0     0    b0a3  b0a2 b0a1 b0a0 = M0 | 
    // | 0    0     0    b1a3  b1a2  b1a1 b1a0  0   = M1 | 
    // | 0    0    b2a3  b2a2  b2a1  b2a0  0    0   = M2 | 
    // | 0   b3a3  b3a2  b3a1  b3a0   0    0    0   = M3 |
    // |    ----------------------------------------     |
    // |                                                 |
    // +-------------------------------------------------+

    sc_uint<32> op1 = OP1_SE;
    sc_uint<32> op2 = OP2_SE;

    for(int i = 0; i < 32; i++)
    {
        sc_uint<64> prod = 0;
        for(int j = i, t = 0; j < i+32 && t < 32; j++)
        {
            prod[j] = op1[i] & op2[t];
            t++;
        }
        //std::cout << "prod["<<i<<"]: "<<op1[i]<<" * "<<op2<<"    "<<prod<<std::endl;
        product[i] = prod;
    }
}
//stage 1 - M31  M30 CSA_29_27 CSA_26_24 CSA_23_21 CSA_20_18 CSA_17_15 
//        -          CSA_14_12 CSA_11_9  CSA_8_6   CSA_5_3   CSA_2_0
void x0_multiplier::CSA_1()
{
    sc_uint<64> xor_ = (sc_uint<64>)product[0] ^ (sc_uint<64>)product[1]; 
    product_s1[0] =  xor_ ^ (sc_uint<64>)product[2];
    product_s1[1] = ((sc_uint<64>)product[0] & (sc_uint<64>)product[1]) | (xor_ & (sc_uint<64>)product[2]);  
}
void x0_multiplier::CSA_2()
{
    sc_uint<64> xor_ = (sc_uint<64>)product[3] ^ (sc_uint<64>)product[4]; 
    product_s1[2] =  xor_ ^ (sc_uint<64>)product[5];
    product_s1[3] = ((sc_uint<64>)product[3] & (sc_uint<64>)product[4]) | (xor_ & (sc_uint<64>)product[5]);  
}
void x0_multiplier::CSA_3()
{
    sc_uint<64> xor_ = (sc_uint<64>)product[6] ^ (sc_uint<64>)product[7]; 
    product_s1[4] =  xor_ ^ (sc_uint<64>)product[8];
    product_s1[5] = ((sc_uint<64>)product[6] & (sc_uint<64>)product[7]) | (xor_ & (sc_uint<64>)product[8]);  
}
void x0_multiplier::CSA_4()
{
    sc_uint<64> xor_ = (sc_uint<64>)product[9] ^ (sc_uint<64>)product[10]; 
    product_s1[6] =  xor_ ^ (sc_uint<64>)product[11];
    product_s1[7] = ((sc_uint<64>)product[9] & (sc_uint<64>)product[10]) | (xor_ & (sc_uint<64>)product[11]);  
}
void x0_multiplier::CSA_5()
{
    sc_uint<64> xor_ = (sc_uint<64>)product[12] ^ (sc_uint<64>)product[13]; 
    product_s1[8] =  xor_ ^ (sc_uint<64>)product[14];
    product_s1[9] = ((sc_uint<64>)product[12] & (sc_uint<64>)product[13]) | (xor_ & (sc_uint<64>)product[14]);  
}
void x0_multiplier::CSA_6()
{
    sc_uint<64> xor_ = (sc_uint<64>)product[15] ^ (sc_uint<64>)product[16]; 
    product_s1[10] =  xor_ ^ (sc_uint<64>)product[17];
    product_s1[11] = ((sc_uint<64>)product[17] & (sc_uint<64>)product[16]) | (xor_ & (sc_uint<64>)product[17]);  
}
void x0_multiplier::CSA_7()
{
    sc_uint<64> xor_ = (sc_uint<64>)product[18] ^ (sc_uint<64>)product[19]; 
    product_s1[12] =  xor_ ^ (sc_uint<64>)product[20];
    product_s1[13] = ((sc_uint<64>)product[18] & (sc_uint<64>)product[19]) | (xor_ & (sc_uint<64>)product[20]);  
}
void x0_multiplier::CSA_8()
{
    sc_uint<64> xor_ = (sc_uint<64>)product[21] ^ (sc_uint<64>)product[22]; 
    product_s1[14] =  xor_ ^ (sc_uint<64>)product[23];
    product_s1[15] = ((sc_uint<64>)product[21] & (sc_uint<64>)product[22]) | (xor_ & (sc_uint<64>)product[23]);  
}
void x0_multiplier::CSA_9()
{
    sc_uint<64> xor_ = (sc_uint<64>)product[24] ^ (sc_uint<64>)product[25]; 
    product_s1[16] =  xor_ ^ (sc_uint<64>)product[26];
    product_s1[17] = ((sc_uint<64>)product[24] & (sc_uint<64>)product[25]) | (xor_ & (sc_uint<64>)product[26]);  
}
void x0_multiplier::CSA_10()
{
    sc_uint<64> xor_ = (sc_uint<64>)product[27] ^ (sc_uint<64>)product[28]; 
    product_s1[18] =  xor_ ^ (sc_uint<64>)product[29];
    product_s1[19] = ((sc_uint<64>)product[27] & (sc_uint<64>)product[28]) | (xor_ & (sc_uint<64>)product[29]);  
}


//stage 2 - M31  CSA_20_18 CSA_17_15 
//        -      CSA_14_12 CSA_11_9  CSA_8_6   CSA_5_3   CSA_2_0

void x0_multiplier::CSA_11()
{
    sc_uint<64> xor_ = (sc_uint<64>)product_s1[0] ^ (sc_uint<64>)product_s1[1]; 
    product_s2[0] =  xor_ ^ (sc_uint<64>)product_s1[2];
    product_s2[1] = ((sc_uint<64>)product_s1[0] & (sc_uint<64>)product_s1[1]) | (xor_ & (sc_uint<64>)product_s1[2]);  
}
void x0_multiplier::CSA_12()
{
    sc_uint<64> xor_ = (sc_uint<64>)product_s1[3] ^ (sc_uint<64>)product_s1[4]; 
    product_s2[2] =  xor_ ^ (sc_uint<64>)product_s1[5];
    product_s2[3] = ((sc_uint<64>)product_s1[3] & (sc_uint<64>)product_s1[4]) | (xor_ & (sc_uint<64>)product_s1[5]);  
}
void x0_multiplier::CSA_13()
{
    sc_uint<64> xor_ = (sc_uint<64>)product_s1[6] ^ (sc_uint<64>)product_s1[7]; 
    product_s2[4] =  xor_ ^ (sc_uint<64>)product_s1[8];
    product_s2[5] = ((sc_uint<64>)product_s1[6] & (sc_uint<64>)product_s1[7]) | (xor_ & (sc_uint<64>)product_s1[8]);  
}
void x0_multiplier::CSA_14()
{
    sc_uint<64> xor_ = (sc_uint<64>)product_s1[9] ^ (sc_uint<64>)product_s1[10]; 
    product_s2[6] =  xor_ ^ (sc_uint<64>)product_s1[11];
    product_s2[7] = ((sc_uint<64>)product_s1[9] & (sc_uint<64>)product_s1[10]) | (xor_ & (sc_uint<64>)product_s1[11]);  
}
void x0_multiplier::CSA_15()
{
    sc_uint<64> xor_ = (sc_uint<64>)product_s1[12] ^ (sc_uint<64>)product_s1[13]; 
    product_s2[8] =  xor_ ^ (sc_uint<64>)product_s1[14];
    product_s2[9] = ((sc_uint<64>)product_s1[12] & (sc_uint<64>)product_s1[13]) | (xor_ & (sc_uint<64>)product_s1[14]);  
}
void x0_multiplier::CSA_16()
{
    sc_uint<64> xor_ = (sc_uint<64>)product_s1[15] ^ (sc_uint<64>)product_s1[16]; 
    product_s2[10] =  xor_ ^ (sc_uint<64>)product_s1[17];
    product_s2[11] = ((sc_uint<64>)product_s1[17] & (sc_uint<64>)product_s1[16]) | (xor_ & (sc_uint<64>)product_s1[17]);  
}
void x0_multiplier::CSA_17()
{
    sc_uint<64> xor_ = (sc_uint<64>)product_s1[18] ^ (sc_uint<64>)product_s1[19]; 
    product_s2[12] =  xor_ ^ (sc_uint<64>)product[30];
    product_s2[13] = ((sc_uint<64>)product_s1[18] & (sc_uint<64>)product_s1[19]) | (xor_ & (sc_uint<64>)product[30]);  
}

//stage 3 - CSA_14_12 CSA_11_9  CSA_8_6   CSA_5_3   CSA_2_0

void x0_multiplier::CSA_18()
{
    sc_uint<64> xor_ = (sc_uint<64>)product_s2[0] ^ (sc_uint<64>)product_s2[1]; 
    product_s3[0] =  xor_ ^ (sc_uint<64>)product_s2[2];
    product_s3[1] = ((sc_uint<64>)product_s2[0] & (sc_uint<64>)product_s2[1]) | (xor_ & (sc_uint<64>)product_s2[2]);  
}
void x0_multiplier::CSA_19()
{
    sc_uint<64> xor_ = (sc_uint<64>)product_s2[3] ^ (sc_uint<64>)product_s2[4]; 
    product_s3[2] =  xor_ ^ (sc_uint<64>)product_s2[5];
    product_s3[3] = ((sc_uint<64>)product_s2[3] & (sc_uint<64>)product_s2[4]) | (xor_ & (sc_uint<64>)product_s2[5]);  
}
void x0_multiplier::CSA_20()
{
    sc_uint<64> xor_ = (sc_uint<64>)product_s2[6] ^ (sc_uint<64>)product_s2[7]; 
    product_s3[4] =  xor_ ^ (sc_uint<64>)product_s2[8];
    product_s3[5] = ((sc_uint<64>)product_s2[6] & (sc_uint<64>)product_s2[7]) | (xor_ & (sc_uint<64>)product_s2[8]);  
}
void x0_multiplier::CSA_21()
{
    sc_uint<64> xor_ = (sc_uint<64>)product_s2[9] ^ (sc_uint<64>)product_s2[10]; 
    product_s3[6] =  xor_ ^ (sc_uint<64>)product_s2[11];
    product_s3[7] = ((sc_uint<64>)product_s2[9] & (sc_uint<64>)product_s2[10]) | (xor_ & (sc_uint<64>)product_s2[11]);  
}
void x0_multiplier::CSA_22()
{
    sc_uint<64> xor_ = (sc_uint<64>)product_s2[12] ^ (sc_uint<64>)product_s2[13]; 
    product_s3[8] =  xor_ ^ (sc_uint<64>)product[31];
    product_s3[9] = ((sc_uint<64>)product_s2[12] & (sc_uint<64>)product_s2[13]) | (xor_ & (sc_uint<64>)product[31]);  
}

//stage 4 - product_s3(9) CSA_8_6   CSA_5_3   CSA_2_0
void x0_multiplier::CSA_23()
{
    sc_uint<64> xor_ = (sc_uint<64>)product_s3[0] ^ (sc_uint<64>)product_s3[1]; 
    product_s4[0] =  xor_ ^ (sc_uint<64>)product_s3[2];
    product_s4[1] = ((sc_uint<64>)product_s3[0] & (sc_uint<64>)product_s3[1]) | (xor_ & (sc_uint<64>)product_s3[2]);  
}
void x0_multiplier::CSA_24()
{
    sc_uint<64> xor_ = (sc_uint<64>)product_s3[3] ^ (sc_uint<64>)product_s3[4]; 
    product_s4[2] =  xor_ ^ (sc_uint<64>)product_s3[5];
    product_s4[3] = ((sc_uint<64>)product_s3[3] & (sc_uint<64>)product_s3[4]) | (xor_ & (sc_uint<64>)product_s3[5]);  
}
void x0_multiplier::CSA_25()
{
    sc_uint<64> xor_ = (sc_uint<64>)product_s3[6] ^ (sc_uint<64>)product_s3[7]; 
    product_s4[4] =  xor_ ^ (sc_uint<64>)product_s3[8];
    product_s4[5] = ((sc_uint<64>)product_s3[6] & (sc_uint<64>)product_s3[7]) | (xor_ & (sc_uint<64>)product_s3[8]);  
}

//stage 5 - product_s3(9) CSA_5_3   CSA_2_0
void x0_multiplier::CSA_26()
{
    sc_uint<64> xor_ = (sc_uint<64>)product_s4[0] ^ (sc_uint<64>)product_s4[1]; 
    product_s5[0] =  xor_ ^ (sc_uint<64>)product_s4[2];
    product_s5[1] = ((sc_uint<64>)product_s4[0] & (sc_uint<64>)product_s4[1]) | (xor_ & (sc_uint<64>)product_s4[2]);  
}
void x0_multiplier::CSA_27()
{
    sc_uint<64> xor_ = (sc_uint<64>)product_s4[3] ^ (sc_uint<64>)product_s4[4]; 
    product_s5[2] =  xor_ ^ (sc_uint<64>)product_s4[5];
    product_s5[3] = ((sc_uint<64>)product_s4[3] & (sc_uint<64>)product_s4[4]) | (xor_ & (sc_uint<64>)product_s4[5]);  
}

void x0_multiplier::RES()
{
    sc_bv<320> res;
    res.range(319, 256) = product_s3[9];
    res.range(255, 192) = product_s5[3];
    res.range(191, 128) = product_s5[2];
    res.range(127, 64) = product_s5[1];
    res.range(63, 0) = product_s5[0];

    RES_SE.write(res);
}

void x0_multiplier::trace(sc_trace_file* tf)
{
    sc_trace(tf, OP1_SE, GET_NAME(OP1_SE));
    sc_trace(tf, OP2_SE, GET_NAME(OP2_SE));
    sc_trace(tf, RES_SE, GET_NAME(RES_SE));
}