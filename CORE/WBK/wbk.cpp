#include "wbk.h"

void wbk::wbk_method() {
    sc_uint<32> din = MEM_DATA.read();
    int size = MEM_SIZE.read();
    bool sign_extend = MEM_SIGN_EXTEND.read();
    sc_uint<32> dout;
    int range_start; //The beginning of the range of din that should actually be written to the register
    switch (size) {
        case 0:
            range_start = 7;
            break;
        case 1:
            range_start = 15;
            break;
        case 2:
            range_start = 31;
            break;
        default:
            range_start = 31;
            break;
    }

    if (sign_extend && din[range_start]) {
        dout = 0xFFFFFFFF;
    }
    else {
        dout = 0;
    }
    dout.range(range_start, 0) = din.range(range_start, 0);
    REG_DATA.write(dout);
    REG_DEST.write(MEM_DEST);
    REG_WB.write(MEM_WB && !MEM2WBK_EMPTY.read());
    MEM2WBK_POP.write(!MEM2WBK_EMPTY.read());
}

void wbk::trace(sc_trace_file* tf) {
        sc_trace(tf, REG_DEST, GET_NAME(REG_DEST));
        sc_trace(tf, REG_DATA, GET_NAME(REG_DATA));
        sc_trace(tf, REG_WB, GET_NAME(REG_WB));
        sc_trace(tf, MEM_DATA, GET_NAME(MEM_DATA));
        sc_trace(tf, MEM_DEST, GET_NAME(MEM_DEST));
        sc_trace(tf, MEM_SIZE, GET_NAME(MEM_SIZE));
        sc_trace(tf, MEM_WB, GET_NAME(MEM_WB));
        sc_trace(tf, MEM_SIGN_EXTEND, GET_NAME(MEM_SIGN_EXTEND));
        sc_trace(tf, MEM2WBK_EMPTY, GET_NAME(MEM2WBK_EMPTY));
        sc_trace(tf, MEM2WBK_POP, GET_NAME(MEM2WBK_POP));
        sc_trace(tf, CLK, GET_NAME(CLK));
        sc_trace(tf, RESET, GET_NAME(RESET));
}