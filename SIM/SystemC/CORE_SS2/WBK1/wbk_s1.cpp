#include "wbk_s1.h"

void wbk_s1::wbk_method() {
    WADR_SW_S1.write(DEST_RM_S1);
    WENABLE_SW_S1.write(WB_RM_S1 && !MEM2WBK_EMPTY_SM_S1);
    MEM2WBK_POP_SW_S1.write(!MEM2WBK_EMPTY_SM_S1);

    // if CSR operation we WBK the value of CSR in rd
    if (CSR_WENABLE_RM_S1)
        WDATA_SW_S1.write(CSR_RDATA_RM_S1);
    else if (MULT_INST_RM_S1)
        WDATA_SW_S1.write(X2_RES_RX2);
    else
        WDATA_SW_S1.write(MEM_RES_RM_S1);
}

void wbk_s1::trace(sc_trace_file* tf) {
    sc_trace(tf, WADR_SW_S1, GET_NAME(WADR_SW_S1));
    sc_trace(tf, WDATA_SW_S1, GET_NAME(WDATA_SW_S1));
    sc_trace(tf, WENABLE_SW_S1, GET_NAME(WENABLE_SW_S1));
    sc_trace(tf, MEM_RES_RM_S1, GET_NAME(MEM_RES_RM_S1));
    sc_trace(tf, DEST_RM_S1, GET_NAME(DEST_RM_S1));
    sc_trace(tf, WB_RM_S1, GET_NAME(WB_RM_S1));
    sc_trace(tf, MEM2WBK_EMPTY_SM_S1, GET_NAME(MEM2WBK_EMPTY_SM_S1));
    sc_trace(tf, MEM2WBK_POP_SW_S1, GET_NAME(MEM2WBK_POP_SW_S1));
    sc_trace(tf, CSR_RDATA_RM_S1, GET_NAME(CSR_RDATA_RM_S1));
    sc_trace(tf, CSR_WENABLE_RM_S1, GET_NAME(CSR_WENABLE_RM_S1));
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET, GET_NAME(RESET));
    sc_trace(tf, CURRENT_MODE_SM, GET_NAME(CURRENT_MODE_SM));
    sc_trace(tf, MULT_INST_RM_S1, GET_NAME(MULT_INST_RM_S1));
    sc_trace(tf, X2_RES_RX2, GET_NAME(X2_RES_RX2));
}
