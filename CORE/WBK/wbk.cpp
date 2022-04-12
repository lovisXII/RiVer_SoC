#include "wbk.h"

void wbk::wbk_method() {
    WADR_SW.write(DEST_RM);
    WENABLE_SW.write(WB_RM && !MEM2WBK_EMPTY_SM);
    MEM2WBK_POP_SW.write(!MEM2WBK_EMPTY_SM);
    // if CSR operation we WBK the value of CSR in rd
    if (CSR_WENABLE_RM)
        WDATA_SW.write(CSR_RDATA_RM);
    else
        WDATA_SW.write(MEM_RES_RM);
}

void wbk::trace(sc_trace_file* tf) {
    sc_trace(tf, WADR_SW, GET_NAME(WADR_SW));
    sc_trace(tf, WDATA_SW, GET_NAME(WDATA_SW));
    sc_trace(tf, WENABLE_SW, GET_NAME(WENABLE_SW));
    sc_trace(tf, MEM_RES_RM, GET_NAME(MEM_RES_RM));
    sc_trace(tf, DEST_RM, GET_NAME(DEST_RM));
    sc_trace(tf, WB_RM, GET_NAME(WB_RM));
    sc_trace(tf, MEM2WBK_EMPTY_SM, GET_NAME(MEM2WBK_EMPTY_SM));
    sc_trace(tf, MEM2WBK_POP_SW, GET_NAME(MEM2WBK_POP_SW));
    sc_trace(tf, CSR_RDATA_RM, GET_NAME(CSR_RDATA_RM));
    sc_trace(tf, CSR_WENABLE_RM, GET_NAME(CSR_WENABLE_RM));
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET, GET_NAME(RESET));
}
