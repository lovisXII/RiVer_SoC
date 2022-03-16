#include "wbk.h"

void wbk::wbk_method() {
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

