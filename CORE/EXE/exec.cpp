#include "exec.h"
#include "alu.h"
#include "shifter.h"

void exec::preprocess_op() {
    sc_uint<32> op1 = op1_se.read();
    sc_uint<32> op2 = op2_se.read();
    if (NEG_OP2_RD.read()) {
        alu_in_op2_se.write(~op2);
    } else {
        alu_in_op2_se.write(op2);
    }
    shift_val_se.write(op2.range(4, 0));
}

void exec::select_exec_res() {
    sc_uint<32> alu_out     = alu_out_se.read();
    sc_uint<32> shifter_out = shifter_out_se.read();
    if (SELECT_SHIFT_RD.read()) {
        exe_res_se.write(shifter_out_se);
    } else if (SLT_RD.read()) {
        if (op1_se.read()[31] == 1 && op2_se.read()[31] == 0) {
            exe_res_se.write(0);
        } else if (op1_se.read()[31] == 0 && op2_se.read()[31] == 1) {
            exe_res_se.write(1);
        } else {
            exe_res_se.write(!(bool)alu_out_se.read()[31]);
        }
    } else if (SLTU_RD.read()) {
        if (op1_se.read()[31] == 1 && op2_se.read()[31] == 0) {
            exe_res_se.write(1);
        } else if (op1_se.read()[31] == 0 && op2_se.read()[31] == 1) {
            exe_res_se.write(0);
        } else {
            exe_res_se.write(!(bool)alu_out_se.read()[31]);
        }
    } else {
        exe_res_se.write(alu_out_se);
    }
}
void exec::fifo_concat() {
    sc_bv<153> ff_din;

    ff_din.range(31, 0)    = exe_res_se.read();
    ff_din.range(63, 32)   = bp_mem_data_sd.read();
    ff_din.range(69, 64)   = DEST_RD.read();
    ff_din.range(71, 70)   = MEM_SIZE_RD.read();
    ff_din[72]             = WB_RD.read();
    ff_din[73]             = MEM_LOAD_RD.read();
    ff_din[74]             = MEM_STORE_RD.read();
    ff_din[75]             = MEM_SIGN_EXTEND_RD.read();
    ff_din.range(107, 76)  = PC_DEC2EXE_RD.read();
    ff_din[108]            = CSR_type_operation_RD.read();
    ff_din.range(120, 109) = ADR_CSR_SD.read();
    ff_din.range(152, 121) = OP1_RD.read();  // sending OP1, can contains CSR

    exe2mem_din_se.write(ff_din);
}
void exec::fifo_unconcat() {
    sc_bv<153> ff_dout = exe2mem_dout_se.read();
    EXE_RES_RE.write((sc_bv_base)ff_dout.range(31, 0));
    MEM_DATA_RE.write((sc_bv_base)ff_dout.range(63, 32));
    DEST_RE.write((sc_bv_base)ff_dout.range(69, 64));
    MEM_SIZE_RE.write((sc_bv_base)ff_dout.range(71, 70));
    WB_RE.write((bool)ff_dout[72]);
    MEM_LOAD_RE.write((bool)ff_dout[73]);
    MEM_STORE_RE.write((bool)ff_dout[74]);
    MEM_SIGN_EXTEND_RE.write((bool)ff_dout[75]);
    PC_EXE2MEM_RE.write((sc_bv_base)ff_dout.range(107, 76));
    CSR_type_operation_RE.write((bool)ff_dout[108]);
    ADR_CSR_SE.write((sc_bv_base)ff_dout.range(120, 109));
    OP1_CSR_RE.write((sc_bv_base)ff_dout.range(152, 121));
}

void exec::manage_fifo() {
    bool stall = exe2mem_full_se.read() || DEC2EXE_EMPTY_SD.read() || blocked.read();
    if (stall) {
        exe2mem_push_se.write(false);
        DEC2EXE_POP_SE.write(false);
    } else {
        exe2mem_push_se.write(true);
        DEC2EXE_POP_SE.write(true);
    }
}

void exec::bypasses() {
    bool        blocked_var     = false;
    sc_uint<32> bp_mem_data_var = MEM_DATA_RD.read();

    if (RADR1_RD.read() == 0) {
        op1_se.write(OP1_RD.read());
    } else if (DEST_RE.read() == RADR1_RD.read() && !MEM_LOAD_RE) {
        op1_se.write(EXE_RES_RE.read());
    } else if (MEM_DEST_RM.read() == RADR1_RD.read()) {
        op1_se.write(MEM_RES_RM.read());
    } else if (DEST_RE.read() == RADR1_RD.read() && MEM_LOAD_RE && !EXE2MEM_EMPTY_SE) {
        blocked_var = true;
    } else {
        op1_se.write(OP1_RD.read());
    }

    if (RADR2_RD.read() == 0 || MEM_LOAD_RD.read()) {
        op2_se.write(OP2_RD.read());
    } else if (DEST_RE.read() == RADR2_RD.read() && !MEM_LOAD_RE) {
        if (MEM_STORE_RD.read()) {  // on stores we need to bypass to the data not adr
            bp_mem_data_var = EXE_RES_RE.read();
            op2_se.write(OP2_RD.read());
        } else {
            op2_se.write(EXE_RES_RE.read());
        }
    } else if (MEM_DEST_RM.read() == RADR2_RD.read()) {
        if (MEM_STORE_RD.read()) {
            bp_mem_data_var = MEM_RES_RM.read();
            op2_se.write(OP2_RD.read());
        } else {
            op2_se.write(MEM_RES_RM.read());
        }
    } else if (DEST_RE.read() == RADR2_RD.read() && MEM_LOAD_RE && !EXE2MEM_EMPTY_SE) {
        blocked_var = true;
    } else {
        op2_se.write(OP2_RD.read());
    }
    bp_mem_data_sd.write(bp_mem_data_var);
    blocked.write(blocked_var);
}

void exec::interruption() {
    if (INTERRUPTION_SX.read()) { INTERRUPTION_SE.write(1); }
}

void exec::trace(sc_trace_file* tf) {
    sc_trace(tf, op1_se, GET_NAME(op1_se));
    sc_trace(tf, op2_se, GET_NAME(op2_se));
    sc_trace(tf, MEM_DATA_RD, GET_NAME(MEM_DATA_RD));
    sc_trace(tf, DEST_RD, GET_NAME(DEST_RD));
    sc_trace(tf, CMD_RD, GET_NAME(CMD_RD));
    sc_trace(tf, MEM_SIZE_RD, GET_NAME(MEM_SIZE_RD));
    sc_trace(tf, SELECT_SHIFT_RD, GET_NAME(SELECT_SHIFT_RD));
    sc_trace(tf, MEM_SIGN_EXTEND_RD, GET_NAME(MEM_SIGN_EXTEND_RD));
    sc_trace(tf, WB_RD, GET_NAME(WB_RD));
    sc_trace(tf, NEG_OP2_RD, GET_NAME(NEG_OP2_RD));
    sc_trace(tf, MEM_LOAD_RD, GET_NAME(MEM_LOAD_RD));
    sc_trace(tf, MEM_STORE_RD, GET_NAME(MEM_STORE_RD));
    sc_trace(tf, EXE2MEM_POP_SM, GET_NAME(EXE2MEM_POP_SM));
    sc_trace(tf, DEC2EXE_EMPTY_SD, GET_NAME(DEC2EXE_EMPTY_SD));
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET, GET_NAME(RESET));
    sc_trace(tf, EXE_RES_RE, GET_NAME(EXE_RES_RE));
    sc_trace(tf, MEM_DATA_RE, GET_NAME(MEM_DATA_RE));
    sc_trace(tf, DEST_RE, GET_NAME(DEST_RE));
    sc_trace(tf, MEM_SIZE_RE, GET_NAME(MEM_SIZE_RE));
    sc_trace(tf, WB_RE, GET_NAME(WB_RE));
    sc_trace(tf, MEM_SIGN_EXTEND_RE, GET_NAME(MEM_SIGN_EXTEND_RE));
    sc_trace(tf, MEM_LOAD_RE, GET_NAME(MEM_LOAD_RE));
    sc_trace(tf, MEM_STORE_RE, GET_NAME(MEM_STORE_RE));
    sc_trace(tf, EXE2MEM_EMPTY_SE, GET_NAME(EXE2MEM_EMPTY_SE));
    sc_trace(tf, DEC2EXE_POP_SE, GET_NAME(DEC2EXE_POP_SE));
    sc_trace(tf, exe_res_se, GET_NAME(exe_res_se));
    sc_trace(tf, exe2mem_din_se, GET_NAME(exe2mem_din_se));
    sc_trace(tf, exe2mem_dout_se, GET_NAME(exe2mem_dout_se));
    sc_trace(tf, alu_in_op2_se, GET_NAME(alu_in_op2_se));
    sc_trace(tf, alu_out_se, GET_NAME(alu_out_se));
    sc_trace(tf, shifter_out_se, GET_NAME(shifter_out_se));
    sc_trace(tf, shift_val_se, GET_NAME(shift_val_se));
    sc_trace(tf, exe2mem_push_se, GET_NAME(exe2mem_push_se));
    sc_trace(tf, RADR1_RD, GET_NAME(RADR1_RD));
    sc_trace(tf, RADR2_RD, GET_NAME(RADR2_RD));
    sc_trace(tf, MEM_DEST_RM, GET_NAME(MEM_DEST_RM));
    sc_trace(tf, MEM_RES_RM, GET_NAME(MEM_RES_RM));
    sc_trace(tf, blocked, GET_NAME(blocked));
    sc_trace(tf, OP1_RD, GET_NAME(OP1_RD));
    sc_trace(tf, OP2_RD, GET_NAME(OP2_RD));
    alu_inst.trace(tf);
    shifter_inst.trace(tf);
    fifo_inst.trace(tf);
}

// 0000010000101100010001100011011
// 00111101000110110101100010111010