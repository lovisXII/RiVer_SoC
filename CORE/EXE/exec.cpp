#include "exec.h"
#include "alu.h"
#include "shifter.h"


void exec::preprocess_op() {
    sc_uint<32> op1 = OP1_SE.read();
    sc_uint<32> op2 = OP2_SE.read() ;
    if (NEG_OP1_SE.read()) {
        alu_in_op1_se.write(~op1);
    }
    else {
        alu_in_op1_se.write(op1);
    }
    shift_val_se.write(op2.range(4, 0));
}

void exec::select_exec_res() {
    sc_uint<32> alu_out = alu_out_se.read();
    sc_uint<32> shifter_out = shifter_out_se.read();
    if (SELECT_SHIFT_SE.read()) {
        exe_res_se.write(shifter_out_se);
    }
    else if (SLT_SE.read()) {
        if (OP1_SE.read()[31] == 1 && OP2_SE.read()[31] == 0) {
            exe_res_se.write(1);
        }
        else if (OP1_SE.read()[31] == 0 && OP2_SE.read()[31] == 1) {
            exe_res_se.write(0);
        }
        else {
            exe_res_se.write(!(bool) alu_out_se.read()[31]);
        }
    }
    else if (SLTU_SE.read()) {
        if (OP1_SE.read()[31] == 1 && OP2_SE.read()[31] == 0) {
            exe_res_se.write(0);
        }
        else if (OP1_SE.read()[31] == 0 && OP2_SE.read()[31] == 1) {
            exe_res_se.write(1);
        }
        else {
            exe_res_se.write(!(bool) alu_out_se.read()[31]);
        }
    }
    else {
        exe_res_se.write(alu_out_se);
    }
    
}
void exec::fifo_concat() {
    sc_bv<76> ff_din;
    ff_din.range(31, 0) = exe_res_se.read();
    ff_din.range(63, 32) = IN_MEM_DATA_SE.read();
    ff_din.range(69, 64) = IN_DEST_SE.read();
    ff_din.range(71, 70) = IN_MEM_SIZE_SE.read();
    ff_din[72] = IN_WB_SE.read();
    ff_din[73] = IN_MEM_LOAD_SE.read();
    ff_din[74] = IN_MEM_STORE_SE.read();
    ff_din[75] = IN_MEM_SIGN_EXTEND_SE.read();
    exe2mem_din_se.write(ff_din);
    
}
void exec::fifo_unconcat() {
    sc_bv<76> ff_dout = exe2mem_dout_se.read();
    EXE_RES_SE.write((sc_bv_base) ff_dout.range(31, 0));
    OUT_MEM_DATA_SE.write((sc_bv_base) ff_dout.range(63, 32));
    OUT_DEST_SE.write((sc_bv_base) ff_dout.range(69, 64));
    OUT_MEM_SIZE_SE.write((sc_bv_base) ff_dout.range(71, 70));
    OUT_WB_SE.write((bool) ff_dout[72]);
    OUT_MEM_LOAD_SE.write((bool) ff_dout[73]);
    OUT_MEM_STORE_SE.write((bool) ff_dout[74]);
    OUT_MEM_SIGN_EXTEND_SE.write((bool) ff_dout[75]);
}

void exec::manage_fifo() {
    bool blocked = exe2mem_full_se.read() | DEC2EXE_EMPTY_SE.read();
    if (blocked) {
        exe2mem_push_se.write(false);
        DEC2EXE_POP_SE.write(false);
    }
    else {
        exe2mem_push_se.write(true);
        DEC2EXE_POP_SE.write(true);
    }
}

void exec::trace(sc_trace_file* tf) {
        sc_trace(tf, OP1_SE, GET_NAME(OP1_SE));
        sc_trace(tf, OP2_SE, GET_NAME(OP2_SE));
        sc_trace(tf, IN_MEM_DATA_SE, GET_NAME(IN_MEM_DATA_SE));
        sc_trace(tf, IN_DEST_SE, GET_NAME(IN_DEST_SE));
        sc_trace(tf, CMD_SE, GET_NAME(CMD_SE));
        sc_trace(tf, IN_MEM_SIZE_SE, GET_NAME(IN_MEM_SIZE_SE));
        sc_trace(tf, SELECT_SHIFT_SE, GET_NAME(SELECT_SHIFT_SE));
        sc_trace(tf, IN_MEM_SIGN_EXTEND_SE, GET_NAME(IN_MEM_SIGN_EXTEND_SE));
        sc_trace(tf, IN_WB_SE, GET_NAME(IN_WB_SE));
        sc_trace(tf, NEG_OP1_SE, GET_NAME(NEG_OP1_SE));
        sc_trace(tf, IN_MEM_LOAD_SE, GET_NAME(IN_MEM_LOAD_SE));
        sc_trace(tf, IN_MEM_STORE_SE, GET_NAME(IN_MEM_STORE_SE));
        sc_trace(tf, EXE2MEM_POP_SE, GET_NAME(EXE2MEM_POP_SE));
        sc_trace(tf, DEC2EXE_EMPTY_SE, GET_NAME(DEC2EXE_EMPTY_SE));
        sc_trace(tf, CLK, GET_NAME(CLK));
        sc_trace(tf, RESET, GET_NAME(RESET));
        sc_trace(tf, EXE_RES_SE, GET_NAME(EXE_RES_SE));
        sc_trace(tf, OUT_MEM_DATA_SE, GET_NAME(OUT_MEM_DATA_SE));
        sc_trace(tf, OUT_DEST_SE, GET_NAME(OUT_DEST_SE));
        sc_trace(tf, OUT_MEM_SIZE_SE, GET_NAME(OUT_MEM_SIZE_SE));
        sc_trace(tf, OUT_WB_SE, GET_NAME(OUT_WB_SE));
        sc_trace(tf, OUT_MEM_SIGN_EXTEND_SE, GET_NAME(OUT_MEM_SIGN_EXTEND_SE));
        sc_trace(tf, OUT_MEM_LOAD_SE, GET_NAME(OUT_MEM_LOAD_SE));
        sc_trace(tf, OUT_MEM_STORE_SE, GET_NAME(OUT_MEM_STORE_SE));
        sc_trace(tf, EXE2MEM_EMPTY_SE, GET_NAME(EXE2MEM_EMPTY_SE));
        sc_trace(tf, DEC2EXE_POP_SE, GET_NAME(DEC2EXE_POP_SE));
        sc_trace(tf, exe_res_se, GET_NAME(exe_res_se));
        sc_trace(tf, exe2mem_din_se, GET_NAME(exe2mem_din_se));
        sc_trace(tf, exe2mem_dout_se, GET_NAME(exe2mem_dout_se));
        sc_trace(tf, alu_in_op1_se, GET_NAME(alu_in_op1_se));
        sc_trace(tf, alu_out_se, GET_NAME(alu_out_se));
        sc_trace(tf, shifter_out_se, GET_NAME(shifter_out_se));
        sc_trace(tf, shift_val_se, GET_NAME(shift_val_se));
        sc_trace(tf, exe2mem_push_se, GET_NAME(exe2mem_push_se));
        sc_trace(tf, exe2mem_full_se, GET_NAME(exe2mem_full_se));
        alu_inst.trace(tf);
        shifter_inst.trace(tf);
        fifo_inst.trace(tf);
}

//0000010000101100010001100011011
//00111101000110110101100010111010