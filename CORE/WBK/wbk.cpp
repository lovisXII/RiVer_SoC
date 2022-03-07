#include "wbk.h"

void wbk::wbk_method() {
    sc_uint<32> din = MEM_DATA_SW.read();
    int size = MEM_SIZE_SW.read();
    bool sign_extend = SIGN_EXTEND_SW.read();
    sc_uint<32> dout;
    int range_start; //The beginning of the range of din that should actually be written to the register
    switch (size) {
        case 2:
            range_start = 7;
            break;
        case 1:
            range_start = 15;
            break;
        case 0:
            range_start = 31;
            break;
        default:
            range_start = 31;
            break;
    }

    if (sign_extend && din(range_start,range_start)) {
        dout = 0xFFFFFFFF;
    }
    else {
        dout = 0;
    }
    dout.range(range_start, 0) = din.range(range_start, 0);
    if (LOAD_SW.read()) {
        REG_DATA_SW.write(dout);
    }
    else {
        REG_DATA_SW.write(din);
    }
    REG_DEST_SW.write(MEM_DEST_SW);
    if(MEM_WB_SW.read() && !MEM2WBK_EMPTY_SW.read()) REG_WB_SW.write(1) ;
    else REG_WB_SW.write(0) ;
    MEM2WBK_POP_SW.write(!MEM2WBK_EMPTY_SW.read());
}

void wbk::trace(sc_trace_file* tf) {
        sc_trace(tf, REG_DEST_SW, GET_NAME(REG_DEST_SW));
        sc_trace(tf, REG_DATA_SW, GET_NAME(REG_DATA_SW));
        sc_trace(tf, REG_WB_SW, GET_NAME(REG_WB_SW));
        sc_trace(tf, MEM_DATA_SW, GET_NAME(MEM_DATA_SW));
        sc_trace(tf, MEM_DEST_SW, GET_NAME(MEM_DEST_SW));
        sc_trace(tf, MEM_SIZE_SW, GET_NAME(MEM_SIZE_SW));
        sc_trace(tf, MEM_WB_SW, GET_NAME(MEM_WB_SW));
        sc_trace(tf, SIGN_EXTEND_SW, GET_NAME(SIGN_EXTEND_SW));
        sc_trace(tf, LOAD_SW, GET_NAME(LOAD_SW));
        sc_trace(tf, MEM2WBK_EMPTY_SW, GET_NAME(MEM2WBK_EMPTY_SW));
        sc_trace(tf, MEM2WBK_POP_SW, GET_NAME(MEM2WBK_POP_SW));
        sc_trace(tf, CLK, GET_NAME(CLK));
        sc_trace(tf, RESET, GET_NAME(RESET));
}