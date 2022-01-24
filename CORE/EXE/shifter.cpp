#include "shifter.h"


void shifter::shifter_sll() {
    sc_uint<32> din = DIN.read();
    sc_uint<5> shift_val = SHIFT_VAL.read();
    sc_uint<32> sll_dout;
    sll_dout = din << (shift_val - 1);
    SLL_DOUT.write(sll_dout);
}

void shifter::shifter_sr() {
    sc_uint<32> din = DIN.read();
    sc_uint<5> shift_val = SHIFT_VAL.read();
    sc_uint<32> sll_dout;
    sc_uint<32> shiftin;
    switch(shift_val)
    {
        case 0:  sll_dout.range(31,0) =din.range(31,0); break;
        case 1:  sll_dout.range(31,1) =din.range(30,0); sll_dout.range( 0,0)= 0; break;
        case 2:  sll_dout.range(31,2) =din.range(29,0); sll_dout.range( 1,0)= 0; break;
        case 3:  sll_dout.range(31,3) =din.range(28,0); sll_dout.range( 2,0)= 0; break;
        case 4:  sll_dout.range(31,4) =din.range(27,0); sll_dout.range( 3,0)= 0; break;
        case 5:  sll_dout.range(31,5) =din.range(26,0); sll_dout.range( 4,0)= 0; break;
        case 6:  sll_dout.range(31,6) =din.range(25,0); sll_dout.range( 5,0)= 0; break;
        case 7:  sll_dout.range(31,7) =din.range(24,0); sll_dout.range( 6,0)= 0; break;
        case 8:  sll_dout.range(31,8) =din.range(23,0); sll_dout.range( 7,0)= 0; break;
        case 9:  sll_dout.range(31,9) =din.range(22,0); sll_dout.range( 8,0)= 0; break;
        case 10: sll_dout.range(31,10)=din.range(21,0); sll_dout.range( 9,0)= 0; break;
        case 11: sll_dout.range(31,11)=din.range(20,0); sll_dout.range(10,0)= 0; break;
        case 12: sll_dout.range(31,12)=din.range(19,0); sll_dout.range(11,0)= 0; break;
        case 13: sll_dout.range(31,13)=din.range(18,0); sll_dout.range(12,0)= 0; break;
        case 14: sll_dout.range(31,14)=din.range(17,0); sll_dout.range(13,0)= 0; break;
        case 15: sll_dout.range(31,15)=din.range(16,0); sll_dout.range(14,0)= 0; break;
        case 16: sll_dout.range(31,16)=din.range(15,0); sll_dout.range(15,0)= 0; break;
        case 17: sll_dout.range(31,17)=din.range(14,0); sll_dout.range(16,0)= 0; break;
        case 18: sll_dout.range(31,18)=din.range(13,0); sll_dout.range(17,0)= 0; break;
        case 19: sll_dout.range(31,19)=din.range(12,0); sll_dout.range(18,0)= 0; break;
        case 20: sll_dout.range(31,20)=din.range(11,0); sll_dout.range(19,0)= 0; break;
        case 21: sll_dout.range(31,21)=din.range(10,0); sll_dout.range(20,0)= 0; break;
        case 22: sll_dout.range(31,22)=din.range( 9,0); sll_dout.range(21,0)= 0; break;
        case 23: sll_dout.range(31,23)=din.range( 8,0); sll_dout.range(22,0)= 0; break;
        case 24: sll_dout.range(31,24)=din.range( 7,0); sll_dout.range(23,0)= 0; break;
        case 25: sll_dout.range(31,25)=din.range( 6,0); sll_dout.range(24,0)= 0; break;
        case 26: sll_dout.range(31,26)=din.range( 5,0); sll_dout.range(25,0)= 0; break;
        case 27: sll_dout.range(31,27)=din.range( 4,0); sll_dout.range(26,0)= 0; break;
        case 28: sll_dout.range(31,28)=din.range( 3,0); sll_dout.range(27,0)= 0; break;
        case 29: sll_dout.range(31,29)=din.range( 2,0); sll_dout.range(28,0)= 0; break;
        case 30: sll_dout.range(31,30)=din.range( 1,0); sll_dout.range(29,0)= 0; break;
        case 31: sll_dout.range(31,31)=din.range( 0,0); sll_dout.range(30,0)= 0; break;
    }
}