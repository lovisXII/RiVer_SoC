#include "exec.h"
#include "alu.h"
#include "shifter.h"


void exec::preprocess_op() {
    sc_uint<32> op1 = OP1.read();
    sc_uint<32> op2 = OP2.read() ;
    if (NEG_OP1.read()) {
        ALU_IN_OP1.write(~op1);
    }
    else {
        ALU_IN_OP1.write(op1);
    }
    SHIFT_VAL.write(op2.range(4, 0));
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
    sc_bv<78> ff_din;
    ff_din.range(31, 0) = FFIN_EXE_RES.read();
    ff_din.range(63, 32) = MEM_DATA.read();
    ff_din.range(67, 64) = DEST.read();
    ff_din.range(69, 68) = MEM_SIZE.read();
    ff_din[70] = WB.read();
    ff_din.range(71,73) = MEM_LOAD.read();
    ff_din.range(74,76) = MEM_STORE.read();
    ff_din[77] = MEM_SIGN_EXTEND.read();
    FF_DIN.write(ff_din);
    
}
void exec::fifo_unconcat() {
    sc_bv<78> ff_dout = FF_DOUT.read();
    FFOUT_EXE_RES.write((sc_bv_base) ff_dout.range(31, 0));
    FFOUT_MEM_DATA.write((sc_bv_base) ff_dout.range(63, 32));
    FFOUT_DEST.write((sc_bv_base) ff_dout.range(67, 64));
    FFOUT_MEM_SIZE.write((sc_bv_base) ff_dout.range(69, 68));
    FFOUT_WB.write((bool) ff_dout[70]);
    FFOUT_MEM_LOAD.write((sc_bv_base) ff_dout.range(71,73));
    FFOUT_MEM_STORE.write((sc_bv_base) ff_dout.range(74,76));
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

void exec::trace(sc_trace_file* tf) {
        sc_trace(tf, OP1, GET_NAME(OP1));
        sc_trace(tf, OP2, GET_NAME(OP2));
        sc_trace(tf, MEM_DATA, GET_NAME(MEM_DATA));
        sc_trace(tf, DEST, GET_NAME(DEST));
        sc_trace(tf, CMD, GET_NAME(CMD));
        sc_trace(tf, MEM_SIZE, GET_NAME(MEM_SIZE));
        sc_trace(tf, SELECT_SHIFT, GET_NAME(SELECT_SHIFT));
        sc_trace(tf, MEM_SIGN_EXTEND, GET_NAME(MEM_SIGN_EXTEND));
        sc_trace(tf, WB, GET_NAME(WB));
        sc_trace(tf, NEG_OP1, GET_NAME(NEG_OP1));
        sc_trace(tf, MEM_LOAD, GET_NAME(MEM_LOAD));
        sc_trace(tf, MEM_STORE, GET_NAME(MEM_STORE));
        sc_trace(tf, EXE2MEM_POP, GET_NAME(EXE2MEM_POP));
        sc_trace(tf, DEC2EXE_EMPTY, GET_NAME(DEC2EXE_EMPTY));
        sc_trace(tf, CLK, GET_NAME(CLK));
        sc_trace(tf, RESET, GET_NAME(RESET));
        sc_trace(tf, FFOUT_EXE_RES, GET_NAME(FFOUT_EXE_RES));
        sc_trace(tf, FFOUT_MEM_DATA, GET_NAME(FFOUT_MEM_DATA));
        sc_trace(tf, FFOUT_DEST, GET_NAME(FFOUT_DEST));
        sc_trace(tf, FFOUT_MEM_SIZE, GET_NAME(FFOUT_MEM_SIZE));
        sc_trace(tf, FFOUT_WB, GET_NAME(FFOUT_WB));
        sc_trace(tf, FFOUT_MEM_SIGN_EXTEND, GET_NAME(FFOUT_MEM_SIGN_EXTEND));
        sc_trace(tf, FFOUT_MEM_LOAD, GET_NAME(FFOUT_MEM_LOAD));
        sc_trace(tf, FFOUT_MEM_STORE, GET_NAME(FFOUT_MEM_STORE));
        sc_trace(tf, EXE2MEM_EMPTY, GET_NAME(EXE2MEM_EMPTY));
        sc_trace(tf, DEC2EXE_POP, GET_NAME(DEC2EXE_POP));
        sc_trace(tf, FFIN_EXE_RES, GET_NAME(FFIN_EXE_RES));
        sc_trace(tf, FF_DIN, GET_NAME(FF_DIN));
        sc_trace(tf, FF_DOUT, GET_NAME(FF_DOUT));
        sc_trace(tf, ALU_IN_OP1, GET_NAME(ALU_IN_OP1));
        sc_trace(tf, ALU_OUT, GET_NAME(ALU_OUT));
        sc_trace(tf, SHIFTER_OUT, GET_NAME(SHIFTER_OUT));
        sc_trace(tf, SHIFT_VAL, GET_NAME(SHIFT_VAL));
        sc_trace(tf, EXE2MEM_PUSH, GET_NAME(EXE2MEM_PUSH));
        sc_trace(tf, EXE2MEM_FULL, GET_NAME(EXE2MEM_FULL));
        alu_inst.trace(tf);
        shifter_inst.trace(tf);
        fifo_inst.trace(tf);
}

//0000010000101100010001100011011
//00111101000110110101100010111010