#include "exec.h"
#include "alu.h"
#include "shifter.h"


void exec::preprocess_op() {
    sc_uint<32> op1 = OP1.read();
    if (NEG_OP1.read()) {
        ALU_IN_OP1.write(~op1);
    }
    else {
        ALU_IN_OP1.write(op1);
    }
    SHIFT_VAL.write(op1.range(4, 0));
}

void exec::select_exec_res() {
    sc_uint<32> alu_out = ALU_OUT.read();
    sc_uint<32> shifter_out = SHIFTER_OUT.read();
    if (SELECT_SHIFT.read()) {
        FFIN_EXE_RES.write(SHIFTER_OUT);
    }
    else {
        FFIN_EXE_RES.write(ALU_OUT);
    }

}
void exec::fifo_concat() {
    sc_bv<74> ff_din;
    ff_din.range(31, 0) = FFIN_EXE_RES.read();
    ff_din.range(63, 32) = MEM_DATA.read();
    ff_din.range(67, 64) = DEST.read();
    ff_din.range(69, 68) = MEM_SIZE.read();
    ff_din[70] = WB.read();
    ff_din[71] = MEM_LOAD.read();
    ff_din[72] = MEM_STORE.read();
    ff_din[73] = MEM_SIGN_EXTEND.read();
    FF_DIN.write(ff_din);
}
void exec::fifo_unconcat() {
    sc_bv<74> ff_dout = FF_DOUT.read();
    FFOUT_EXE_RES.write((sc_bv_base) ff_dout.range(31, 0));
    FFOUT_MEM_DATA.write((sc_bv_base) ff_dout.range(63, 32));
    FFOUT_DEST.write((sc_bv_base) ff_dout.range(67, 64));
    FFOUT_MEM_SIZE.write((sc_bv_base) ff_dout.range(69, 68));
    FFOUT_WB.write((bool) ff_dout[70]);
    FFOUT_MEM_LOAD.write((bool) ff_dout[71]);
    FFOUT_MEM_STORE.write((bool) ff_dout[72]);
    FFOUT_MEM_SIGN_EXTEND.write((bool) ff_dout[73]);
}

void exec::manage_fifo() {
    bool blocked = EXE2MEM_FULL.read() | DEC2EXE_EMPTY.read();
    if (blocked) {
        EXE2MEM_PUSH.write(false);
        DEC2EXE_POP.write(false);
    }
    else {
        EXE2MEM_PUSH.write(true);
        DEC2EXE_POP.write(true);
    }
}