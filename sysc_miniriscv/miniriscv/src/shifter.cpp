#include "shifter.h"

void shifter::shifter_sll() {
    sc_uint<32> din       = DIN_SE.read();
    sc_uint<5>  shift_val = SHIFT_VAL_SE.read();
    sll_dout_se           = din << shift_val;
}

void shifter::shifter_sr() {
    sc_uint<32> din       = DIN_SE.read();
    sc_uint<5>  shift_val = SHIFT_VAL_SE.read();
    sc_uint<32> shiftin;
    // compute the sign extension for arithmetic shift
    if (CMD_SE.read() == 2) {
        shiftin = 0xFFFFFFFF;
    } else {
        shiftin = 0x0;
    }
    /*
    We split the output in two part :
    |input| >> shift_val
    is transformed into
    |sign extension||shifted input|
    <------------->  <------------->
       shift_val      32 - shift_val


    sr_dout >> shift_val        produces |00000000000000||shifted input|
    shiftin << (32 - shift_val) produces |sign extension||0000000000000|

    So we simply have to apply an "or" operation to these two.
    */
    sr_dout_se = (din >> shift_val) | (shiftin << (32 - shift_val));
}
/*
choose the right result depending on the command
*/
void shifter::shifter_agregate() {
    if (CMD_SE.read() == 0) {
        DOUT_SE.write(sll_dout_se.read());
    } else {
        DOUT_SE.write(sr_dout_se.read());
    }
}

void shifter::trace(sc_trace_file* tf) {
    sc_trace(tf, DIN_SE, GET_NAME(DIN_SE));
    sc_trace(tf, SHIFT_VAL_SE, GET_NAME(SHIFT_VAL_SE));
    sc_trace(tf, CMD_SE, GET_NAME(CMD_SE));
    sc_trace(tf, DOUT_SE, GET_NAME(DOUT_SE));
    sc_trace(tf, sll_dout_se, GET_NAME(sll_dout_se));
    sc_trace(tf, sr_dout_se, GET_NAME(sr_dout_se));
}