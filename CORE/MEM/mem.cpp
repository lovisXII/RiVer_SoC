#include "mem.h"

void mem::mem2wbk_concat() {
    sc_uint<38> ff_din;
    ff_din.range(31, 0) = data_sm.read();
    ff_din.range(36, 32) = DEST_RE.read();
    ff_din[37] = wb_sm.read();
    mem2wbk_din_sm.write(ff_din);
}
void mem::mem2wbk_unconcat() {
    sc_uint<38> ff_dout = mem2wbk_dout_sm.read();
    MEM_RES_RM.write(ff_dout.range(31, 0));
    DEST_RM.write(ff_dout.range(36, 32));
    WB_RM.write(ff_dout[37]);
}

void mem::fifo_gestion() {
    bool stall = MCACHE_STALL_SM.read() || mem2wbk_full_sm.read() || EXE2MEM_EMPTY_SE.read();
    mem2wbk_push_sm.write(!stall && wb_sm.read());
    EXE2MEM_POP_SM.write(!stall);
}
void mem::mem_preprocess() {

    //Whether the register bank shoudl actually write the data
    wb_sm.write(WB_RE.read() || LOAD_RE.read());
    
    //The data sent to the actual memory
    MCACHE_DATA_SM.write(MEM_DATA_RE.read());
    MCACHE_ADR_SM.write(EXE_RES_RE);
    MCACHE_LOAD_SM.write(LOAD_RE.read());
    MCACHE_STORE_SM.write(STORE_RE.read());
    MCACHE_ADR_VALID_SM.write(!EXE2MEM_EMPTY_SE.read());

}

void mem::sign_extend() {
    // sign extend
    int size = MEM_SIZE_RE.read();
    bool sign_extend = SIGN_EXTEND_RE.read();
    sc_uint<32> din = MCACHE_RESULT_SM.read();
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

    //Choose which data should be written to the register
    if (LOAD_RE.read()) {
        data_sm.write(dout);
    }
    else {
        data_sm.write(EXE_RES_RE.read());
    }
}

void mem::trace(sc_trace_file* tf) {
        sc_trace(tf, MCACHE_ADR_SM, GET_NAME(MCACHE_ADR_SM)); // adress in memory
        sc_trace(tf, MCACHE_DATA_SM, GET_NAME(MCACHE_DATA_SM));
        sc_trace(tf, MCACHE_ADR_VALID_SM, GET_NAME(MCACHE_ADR_VALID_SM));
        sc_trace(tf, MCACHE_STORE_SM, GET_NAME(MCACHE_STORE_SM));
        sc_trace(tf, MCACHE_LOAD_SM, GET_NAME(MCACHE_LOAD_SM));
        sc_trace(tf, MCACHE_RESULT_SM, GET_NAME(MCACHE_RESULT_SM));
        sc_trace(tf, MCACHE_STALL_SM, GET_NAME(MCACHE_STALL_SM));
        sc_trace(tf, EXE_RES_RE, GET_NAME(EXE_RES_RE));
        sc_trace(tf, MEM_DATA_RE, GET_NAME(MEM_DATA_RE));
        sc_trace(tf, DEST_RE, GET_NAME(DEST_RE));
        sc_trace(tf, MEM_SIZE_RE, GET_NAME(MEM_SIZE_RE));
        sc_trace(tf, WB_RE, GET_NAME(WB_RE));
        sc_trace(tf, SIGN_EXTEND_RE, GET_NAME(SIGN_EXTEND_RE));
        sc_trace(tf, LOAD_RE, GET_NAME(LOAD_RE));
        sc_trace(tf, STORE_RE, GET_NAME(STORE_RE));
        sc_trace(tf, EXE2MEM_EMPTY_SE, GET_NAME(EXE2MEM_EMPTY_SE)); 
        sc_trace(tf, EXE2MEM_POP_SM, GET_NAME(EXE2MEM_POP_SM));
        sc_trace(tf, MEM2WBK_POP_SW, GET_NAME(MEM2WBK_POP_SW));
        sc_trace(tf, mem2wbk_push_sm, GET_NAME(mem2wbk_push_sm));
        sc_trace(tf, mem2wbk_full_sm, GET_NAME(mem2wbk_full_sm));
        sc_trace(tf, MEM2WBK_EMPTY_SM, GET_NAME(MEM2WBK_EMPTY_SM)); 
        sc_trace(tf, MEM_RES_RM, GET_NAME(MEM_RES_RM));
        sc_trace(tf, DEST_RM, GET_NAME(DEST_RM));
        sc_trace(tf, WB_RM, GET_NAME(WB_RM));
        sc_trace(tf, mem2wbk_din_sm, GET_NAME(mem2wbk_din_sm));
        sc_trace(tf, mem2wbk_dout_sm, GET_NAME(mem2wbk_dout_sm));
        sc_trace(tf, data_sm, GET_NAME(data_sm));
        sc_trace(tf, wb_sm, GET_NAME(wb_sm));
        sc_trace(tf, CLK, GET_NAME(CLK));
        sc_trace(tf, RESET, GET_NAME(RESET));
        fifo_inst.trace(tf);
    }