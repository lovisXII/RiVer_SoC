#include "mem.h"

void mem::mem2wbk_concat() {
    sc_uint<41> ff_din;
    ff_din.range(31, 0) = DATA.read();
    ff_din.range(36, 32) = EXE_DEST.read();
    ff_din.range(38, 37) = EXE_MEM_SIZE.read();
    ff_din[39] = WB.read();
    ff_din[40] = EXE_MEM_SIGN_EXTEND.read();
    MEM2WBK_FFIN.write(ff_din);
}
void mem::mem2wbk_unconcat() {
    sc_uint<41> ff_dout = MEM2WBK_FFOUT.read();
    WBK_DATA.write(ff_dout.range(31, 0));
    WBK_DEST.write(ff_dout.range(36, 32));
    WBK_MEM_SIZE.write(ff_dout.range(38, 37));
    WBK_WB.write(ff_dout[39]);
    WBK_MEM_SIGN_EXTEND.write(ff_dout[40]);
}

void mem::fifo_gestion() {
    bool stall = MEM_STALL.read() || MEM2WBK_FULL.read() || EXE2MEM_EMPTY.read();
    MEM2WBK_PUSH.write(!stall && WB.read());
    EXE2MEM_POP.write(!stall);
}
void mem::mem_preprocess() {

    //Whether the register bank shoudl actually write the data
    WB.write(EXE_WB.read() || EXE_MEM_LOAD.read());

    //Choose which data should be written to the register
    if (EXE_MEM_LOAD.read()) {
        DATA.write(MEM_RESULT.read());
    }
    else {
        DATA.write(EXE_RES.read());
    }
    //The data sent to the actual memory
    MEM_DATA.write(EXE_MEM_DATA.read());
    MEM_ADR.write(EXE_RES);
    MEM_LOAD.write(EXE_MEM_LOAD.read());
    MEM_STORE.write(EXE_MEM_STORE.read());
    MEM_ADR_VALID.write(!EXE2MEM_EMPTY.read());
}

void mem::trace(sc_trace_file* tf) {
        sc_trace(tf, MEM_ADR, GET_NAME(MEM_ADR)); // adress in memory
        sc_trace(tf, MEM_DATA, GET_NAME(MEM_DATA));
        sc_trace(tf, MEM_ADR_VALID, GET_NAME(MEM_ADR_VALID));
        sc_trace(tf, MEM_STORE, GET_NAME(MEM_STORE));
        sc_trace(tf, MEM_LOAD, GET_NAME(MEM_LOAD));
        sc_trace(tf, MEM_RESULT, GET_NAME(MEM_RESULT));
        sc_trace(tf, MEM_STALL, GET_NAME(MEM_STALL));
        sc_trace(tf, EXE_RES, GET_NAME(EXE_RES));
        sc_trace(tf, EXE_MEM_DATA, GET_NAME(EXE_MEM_DATA));
        sc_trace(tf, EXE_DEST, GET_NAME(EXE_DEST));
        sc_trace(tf, EXE_MEM_SIZE, GET_NAME(EXE_MEM_SIZE));
        sc_trace(tf, EXE_WB, GET_NAME(EXE_WB));
        sc_trace(tf, EXE_MEM_SIGN_EXTEND, GET_NAME(EXE_MEM_SIGN_EXTEND));
        sc_trace(tf, EXE_MEM_LOAD, GET_NAME(EXE_MEM_LOAD));
        sc_trace(tf, EXE_MEM_STORE, GET_NAME(EXE_MEM_STORE));
        sc_trace(tf, EXE2MEM_EMPTY, GET_NAME(EXE2MEM_EMPTY)); 
        sc_trace(tf, EXE2MEM_POP, GET_NAME(EXE2MEM_POP));
        sc_trace(tf, MEM2WBK_POP, GET_NAME(MEM2WBK_POP));
        sc_trace(tf, MEM2WBK_PUSH, GET_NAME(MEM2WBK_PUSH));
        sc_trace(tf, MEM2WBK_FULL, GET_NAME(MEM2WBK_FULL));
        sc_trace(tf, MEM2WBK_EMPTY, GET_NAME(MEM2WBK_EMPTY)); 
        sc_trace(tf, WBK_DATA, GET_NAME(WBK_DATA));
        sc_trace(tf, WBK_DEST, GET_NAME(WBK_DEST));
        sc_trace(tf, WBK_MEM_SIZE, GET_NAME(WBK_MEM_SIZE));
        sc_trace(tf, WBK_WB, GET_NAME(WBK_WB));
        sc_trace(tf, WBK_MEM_SIGN_EXTEND, GET_NAME(WBK_MEM_SIGN_EXTEND));
        sc_trace(tf, MEM2WBK_FFIN, GET_NAME(MEM2WBK_FFIN));
        sc_trace(tf, MEM2WBK_FFOUT, GET_NAME(MEM2WBK_FFOUT));
        sc_trace(tf, DATA, GET_NAME(DATA));
        sc_trace(tf, WB, GET_NAME(WB));
        sc_trace(tf, CLK, GET_NAME(CLK));
        sc_trace(tf, RESET, GET_NAME(RESET));
        fifo_inst.trace(tf);
    }