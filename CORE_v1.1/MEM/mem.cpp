#include "mem.h"

void mem::mem2wbk_concat() {
    sc_uint<42> ff_din;
    ff_din.range(31, 0) = data_sm.read();
    ff_din.range(36, 32) = EXE_DEST_SM.read();
    ff_din.range(38, 37) = EXE_MEM_SIZE_SM.read();
    ff_din[39] = wb_sm.read();
    ff_din[40] = SIGN_EXTEND_SM.read();
    ff_din[41] = MCACHE_LOAD_SM.read();
    mem2wbk_din_sm.write(ff_din);
}
void mem::mem2wbk_unconcat() {
    sc_uint<42> ff_dout = mem2wbk_dout_sm.read();
    WBK_DATA_SM.write(ff_dout.range(31, 0));
    WBK_DEST_SM.write(ff_dout.range(36, 32));
    WBK_MEM_SIZE_SM.write(ff_dout.range(38, 37));
    WBK_WB_SM.write(ff_dout[39]);
    WBK_SIGN_EXTEND_SM.write(ff_dout[40]);
    WBK_LOAD_SM.write(ff_dout[41]);
}

void mem::fifo_gestion() {
    bool stall = MCACHE_STALL_SM.read() || mem2wbk_full_sm.read() || EXE2MEM_EMPTY_SM.read();
    mem2wbk_push_sm.write(!stall && wb_sm.read());
    EXE2MEM_POP_SM.write(!stall);
}
void mem::mem_preprocess() {

    //Whether the register bank shoudl actually write the data
    wb_sm.write(EXE_WB_SM.read() || LOAD_SM.read());

    //Choose which data should be written to the register
    if (LOAD_SM.read()) {
        data_sm.write(MCACHE_RESULT_SM.read());
    }
    else {
        data_sm.write(EXE_RES_SM.read());
    }
    //The data sent to the actual memory
    MCACHE_DATA_SM.write(MEM_DATA_SM.read());
    MCACHE_ADR_SM.write(EXE_RES_SM);
    MCACHE_LOAD_SM.write(LOAD_SM.read());
    MCACHE_STORE_SM.write(STORE_SM.read());
    MCACHE_ADR_VALID_SM.write(!EXE2MEM_EMPTY_SM.read());
}

void mem::trace(sc_trace_file* tf) {
        sc_trace(tf, MCACHE_ADR_SM, GET_NAME(MCACHE_ADR_SM)); // adress in memory
        sc_trace(tf, MCACHE_DATA_SM, GET_NAME(MCACHE_DATA_SM));
        sc_trace(tf, MCACHE_ADR_VALID_SM, GET_NAME(MCACHE_ADR_VALID_SM));
        sc_trace(tf, MCACHE_STORE_SM, GET_NAME(MCACHE_STORE_SM));
        sc_trace(tf, MCACHE_LOAD_SM, GET_NAME(MCACHE_LOAD_SM));
        sc_trace(tf, MCACHE_RESULT_SM, GET_NAME(MCACHE_RESULT_SM));
        sc_trace(tf, MCACHE_STALL_SM, GET_NAME(MCACHE_STALL_SM));
        sc_trace(tf, EXE_RES_SM, GET_NAME(EXE_RES_SM));
        sc_trace(tf, MEM_DATA_SM, GET_NAME(MEM_DATA_SM));
        sc_trace(tf, EXE_DEST_SM, GET_NAME(EXE_DEST_SM));
        sc_trace(tf, EXE_MEM_SIZE_SM, GET_NAME(EXE_MEM_SIZE_SM));
        sc_trace(tf, EXE_WB_SM, GET_NAME(EXE_WB_SM));
        sc_trace(tf, SIGN_EXTEND_SM, GET_NAME(SIGN_EXTEND_SM));
        sc_trace(tf, LOAD_SM, GET_NAME(LOAD_SM));
        sc_trace(tf, STORE_SM, GET_NAME(STORE_SM));
        sc_trace(tf, EXE2MEM_EMPTY_SM, GET_NAME(EXE2MEM_EMPTY_SM)); 
        sc_trace(tf, EXE2MEM_POP_SM, GET_NAME(EXE2MEM_POP_SM));
        sc_trace(tf, MEM2WBK_POP_SM, GET_NAME(MEM2WBK_POP_SM));
        sc_trace(tf, mem2wbk_push_sm, GET_NAME(mem2wbk_push_sm));
        sc_trace(tf, mem2wbk_full_sm, GET_NAME(mem2wbk_full_sm));
        sc_trace(tf, MEM2WBK_EMPTY_SM, GET_NAME(MEM2WBK_EMPTY_SM)); 
        sc_trace(tf, WBK_DATA_SM, GET_NAME(WBK_DATA_SM));
        sc_trace(tf, WBK_DEST_SM, GET_NAME(WBK_DEST_SM));
        sc_trace(tf, WBK_MEM_SIZE_SM, GET_NAME(WBK_MEM_SIZE_SM));
        sc_trace(tf, WBK_WB_SM, GET_NAME(WBK_WB_SM));
        sc_trace(tf, WBK_SIGN_EXTEND_SM, GET_NAME(WBK_SIGN_EXTEND_SM));
        sc_trace(tf, WBK_LOAD_SM, GET_NAME(WBK_LOAD_SM));
        sc_trace(tf, mem2wbk_din_sm, GET_NAME(mem2wbk_din_sm));
        sc_trace(tf, mem2wbk_dout_sm, GET_NAME(mem2wbk_dout_sm));
        sc_trace(tf, data_sm, GET_NAME(data_sm));
        sc_trace(tf, wb_sm, GET_NAME(wb_sm));
        sc_trace(tf, CLK, GET_NAME(CLK));
        sc_trace(tf, RESET, GET_NAME(RESET));
        fifo_inst.trace(tf);
    }