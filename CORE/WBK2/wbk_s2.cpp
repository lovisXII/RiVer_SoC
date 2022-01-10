#include "wbk_s2.h"

void wbk_s2::wbk_method() {
    WADR_SW_S2.write(DEST_RM_S2);
    WENABLE_SW_S2.write(WB_RM_S2 && !MEM2WBK_EMPTY_SM_S2);
    MEM2WBK_POP_SW_S2.write(!MEM2WBK_EMPTY_SM_S2);

    // if CSR operation we WBK the value of CSR in rd
    if (CSR_WENABLE_RM_S2)
        WDATA_SW_S2.write(CSR_RDATA_RM_S2);
    else if (MULT_INST_RM_S2)
        WDATA_SW_S2.write(X2_RES_RX2);
    else
        WDATA_SW_S2.write(MEM_RES_RM_S2);
}

void wbk_s2::trace(sc_trace_file* tf) {
    sc_trace(tf, WADR_SW_S2, GET_NAME(WADR_SW_S2));
    sc_trace(tf, WDATA_SW_S2, GET_NAME(WDATA_SW_S2));
    sc_trace(tf, WENABLE_SW_S2, GET_NAME(WENABLE_SW_S2));
    sc_trace(tf, MEM_RES_RM_S2, GET_NAME(MEM_RES_RM_S2));
    sc_trace(tf, DEST_RM_S2, GET_NAME(DEST_RM_S2));
    sc_trace(tf, WB_RM_S2, GET_NAME(WB_RM_S2));
    sc_trace(tf, MEM2WBK_EMPTY_SM_S2, GET_NAME(MEM2WBK_EMPTY_SM_S2));
    sc_trace(tf, MEM2WBK_POP_SW_S2, GET_NAME(MEM2WBK_POP_SW_S2));
    sc_trace(tf, CSR_RDATA_RM_S2, GET_NAME(CSR_RDATA_RM_S2));
    sc_trace(tf, CSR_WENABLE_RM_S2, GET_NAME(CSR_WENABLE_RM_S2));
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET, GET_NAME(RESET));
    sc_trace(tf, CURRENT_MODE_SM_S2, GET_NAME(CURRENT_MODE_SM_S2));
    sc_trace(tf, MULT_INST_RM_S2, GET_NAME(MULT_INST_RM_S2));
    sc_trace(tf, X2_RES_RX2, GET_NAME(X2_RES_RX2));
}
