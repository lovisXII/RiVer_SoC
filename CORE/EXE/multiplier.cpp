#include "multiplier.h"

bool multiplier::bit_adder(sc_uint<1> op1, sc_uint<1> op2, sc_uint<1> cin)
{
    return (op1 ^ op2) ^ cin;
}
bool multiplier::carry_prod(sc_uint<1> op1, sc_uint<1> op2, sc_uint<1> cin)
{
    return (op1 & op2) | (cin & (op1 ^ op2));
}

void multiplier::operation()
{
    // +----------------------------------------------------------------------------------------------------------------+
    // |                                                 ARRAY MULTIPLIER                                               |
    // +------------------------------------------------+----------------+----------------------------------------------+
    // |                       a3    a2   a1   a0       | b0a0 = b0 & a0 | R0 = b0a0                                    |
    // |                     x b3    b2   b1   b0       | b0a1 = b0 & a1 | R1 = (b0a0 ^ b1a0) ^ 0b0                     |
    // |                      ------------------        | b0a2 = b0 & a2 | T0 = (b0a2 ^ b1a1) ^ cin(b0a0,b1a0,0b0)      |
    // |                      b0a3  b0a2 b0a1 b0a0      |   .......      | R2 = (T0   ^ b2a0) ^ 0b0                     |
    // |                b1a3  b1a2  b1a1 b1a0  -        | b3a2 = b3 & a2 |  ........                                    |
    // |          b2a3  b2a2  b2a1  b2a0  -             | b3a3 = b3 & a3 | Rj = (b0aj ^ b1aj-1) ^ cin(b0aj-1,b1aj-2,..) |
    // | +  b3a3  b3a2  b3a1  b3a0   -                  |                | for i from 2 to j-1 do                       |
    // |   ----------------------------------------     |                |   Rj = (Rj ^ biaj-1) ^ cin(bi-1aj-2,...)     |
    // |     R6    R5    R4    R3    R2   R1   R0       |                | Rj = (Rj ^ bja0) ^ 0b0                       |
    // +------------------------------------------------+----------------+----------------------------------------------+

    sc_uint<64> prod;

    sc_uint<32> op1 = OP1_SE.read();
    sc_uint<32> op2 = OP2_SE.read();

    sc_uint<32> cols[32];
    sc_uint<32> cin[31];

    //std::cout << "cols[j][i]"<<std::endl;
    //std::cout << "i -> "<<std::endl;
    for(int j = 0; j < 32; ++j)
    {
        //std::cout <<j<< " : ";
        for(int i = 0; i < 32; ++i)
        {
            cols[j].range(i,i) = op1.range(i,i) & op2.range(j,j) ? 1 : 0;
            //std::cout << cols[j].range(i,i);
        }
        //std::cout << std::endl;
    }

    // Prod 0
    prod.range(0,0) = cols[0].range(0,0);
   //std::cout <<"R0: "<<prod.range(0,0)<<std::endl;
    // Prod 1
    cin[0].range(0,0) = carry_prod(cols[0].range(1,1), cols[1].range(0,0), 0b0);
    prod.range(1,1) = bit_adder(cols[0].range(1,1), cols[1].range(0,0), 0b0);
   //std::cout <<"R1: "<<prod.range(1,1) << std::endl;
    // Prod i such that i > 1 and i < 31
    for(int j = 2; j < 32; ++j)
    {
        cin[0].range(j-1,j-1) = carry_prod(cols[0].range(j,j), cols[1].range(j-1,j-1), cin[0].range(j-2,j-2));
        prod.range(j,j) = bit_adder(cols[0].range(j,j), cols[1].range(j-1,j-1), cin[0].range(j-2,j-2));
        //std::cout << "cin["<<0<<"]["<<j-1<<"]: " <<cin[0].range(j-1,j-1)<< std::endl;             
        for(int i = 2; i < j; ++i)
        {
            cin[i-1].range(j-1,j-1) = carry_prod(prod.range(j,j), cols[i].range(j-i,j-i), cin[i-1].range(j-2,j-2));
            prod.range(j,j)       =  bit_adder(prod.range(j,j), cols[i].range(j-i,j-i), cin[i-1].range(j-2,j-2));
            //std::cout << "cin["<<i-1<<"]["<<j-1<<"]: " <<cin[i-1].range(j-1,j-1)<< std::endl;
        }
        cin[j-1].range(j-1,j-1) = carry_prod(prod.range(j,j), cols[j].range(0,0), 0b0);
        prod.range(j,j) = bit_adder(prod.range(j,j), cols[j].range(0,0), 0b0);
       //std::cout <<"R"<<j<<": "<<prod.range(j,j)<< std::endl;
    }

    // Prod 32
    cin[0].range(31,31) = carry_prod(cols[1].range(31,31), cin[0].range(30,30), 0b0);
    prod.range(32,32)   =  bit_adder(cols[1].range(31,31), cin[0].range(30,30), 0b0);
    //std::cout <<"   "<<prod.range(32,32)<< " =  cols[1][31]: "<<cols[1].range(31,31)
    //          <<"     cin[0][30]: "    << cin[0].range(30,30) << std::endl;
    for(int i = 1; i < 31; ++i)
    {
        cin[i].range(31-i,31-i) = carry_prod(prod.range(32,32), cols[i+1].range(31-i,31-i), cin[i].range(30,30));
        prod.range(32,32)       =  bit_adder(prod.range(32,32), cols[i+1].range(31-i,31-i), cin[i].range(30,30));
        //std::cout <<"   prod[32]: "<<prod.range(32,32)<<"   cols["<<i<<"][31]: "<< cols[i].range(31,31)
        //          << "      cin["<<i-1<<"][30]: "<<cin[i-1].range(30,30)<< std::endl;
    }
   //std::cout << "R32: " << prod.range(32,32)<< std::endl;
    // Prod i such that i > 32 and i < 63
    for(int j = 1; j < 31; ++j)
    {
        cin[j].range(31,31)   = carry_prod(cols[j+1].range(31,31), cin[j-1].range(31,31), cin[j].range(30,30));
        prod.range(j+32,j+32) =  bit_adder(cols[j+1].range(31,31), cin[j-1].range(31,31), cin[j].range(30,30));
       //std::cout <<"   cols["<<j+1<<"][31]: "<<cols[j+1].range(31,31)<< "   cin["<<j-1<<"][31]: " <<cin[j-1].range(31,31)
        //            <<"  cin["<<j<<"][30]: "<<cin[j].range(30,30) <<"  => "<<prod.range(j+32,j+32)<< std::endl;           
        for(int i = j+1, t = 30; i < 31; ++i, --t)
        {
        //    std::cout <<"   prod["<<j+32<<"]: "<<prod.range(j+32,j+32)<<"   cols["<<i+1<<"]["<<t<<"]: "<< cols[i+1].range(t,t)
        //          << "      cin["<<i<<"]["<<31-i<<"]: "<<cin[i].range(t-1,t-1);
            cin[i].range(t,t) = carry_prod(prod.range(j+32,j+32), cols[i+1].range(t,t), cin[i].range(t-1,t-1));
            prod.range(j+32,j+32)   =  bit_adder(prod.range(j+32,j+32), cols[i+1].range(t,t), cin[i].range(t-1,t-1));
          //  std::cout << "  =>  "<<prod.range(j+32,j+32)<<"     cin["<<i<<"]["<<t<<"]: "<<cin[i].range(t,t)<<std::endl;
        }
        //std::cout <<"R"<<j+32<<": "<<prod.range(j+32,j+32)<<std::endl;
    }
    //Prod 63
    prod.range(63,63) = cin[30].range(31,31);
    //std::cout <<"R"<<63<<": "<<prod.range(63,63)<<std::endl;

    if(CMD_SE.read() == 0b00)
        RES_SE.write(prod.range(31, 0));
    else 
        RES_SE.write(prod.range(63, 32));
}

void multiplier::trace(sc_trace_file* tf)
{
    sc_trace(tf, OP1_SE, GET_NAME(OP1_SE));
    sc_trace(tf, OP2_SE, GET_NAME(OP2_SE));
    sc_trace(tf, CMD_SE, GET_NAME(CMD_SE));
    sc_trace(tf, RES_SE, GET_NAME(RES_SE));
}