#include "mem.h"

void mem::mem2wbk_concat() {
    sc_uint<41> ff_din;
    ff_din.range(31, 0) = DATA.read();
    ff_din.range(36, 32) = EXE_DEST.read();
    ff_din.range(38, 37) = EXE_MEM_SIZE.read();
    ff_din[39] = WB.read();
    ff_din[40] = WBK_MEM_SIGN_EXTEND.read();
    MEM2WBK_FFIN.write(ff_din);
}
void mem::mem2wbk_unconcat() {
    sc_uint<41> ff_dout = MEM2WBK_FFOUT.read();
    WBK_DATA.write(ff_dout.range(31, 0));
    WBK_DEST.write(ff_dout.range(36, 32));
    WBK_MEM_SIZE.write(ff_dout.range(38, 37));
    WB.write(ff_dout[39]);
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