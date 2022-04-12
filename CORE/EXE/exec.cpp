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
        if(MEM_LOAD_RD.read() || MEM_STORE_RD.read())
        {
            if(alu_out_se.read() && 0b11 != 0){ //if adress isn't aligned it creates an exception
                load_adress_missaligned_se.write(1) ;
            }
            else{
                load_adress_missaligned_se.write(0) ;
            }
            if(alu_out_se.read() > start_kernel_adress )
            {
                instruction_access_fault_se.write(1) ;
            }
            else{
                instruction_access_fault_se.write(0) ;
            }
        }
        exe_res_se.write(alu_out_se);
    }
}
void exec::fifo_concat() {
    sc_bv<exe2mem_size> ff_din;
    if(EXCEPTION_RM.read() == 0){
        ff_din.range(31, 0)    = exe_res_se.read();
        ff_din.range(63, 32)   = bp_mem_data_sd.read();
        ff_din.range(69, 64)   = DEST_RD.read();
        ff_din.range(71, 70)   = MEM_SIZE_RD.read();
        ff_din[72]             = wb_re.read();
        ff_din[73]             = mem_load_re.read();
        ff_din[74]             = mem_store_re.read();
        ff_din[75]             = MEM_SIGN_EXTEND_RD.read();
        ff_din.range(107, 76)  = PC_DEC2EXE_RD.read();
        ff_din[108]            = CSR_WENABLE_RD.read();
        ff_din.range(120, 109) = CSR_WADR_RD.read();
        ff_din.range(152, 121) = CSR_RDATA_RD.read();  // sending Csr read to wb to register
        ff_din[153]             = ECALL_I_RD.read();
        ff_din[154]             = EBREAK_I_RD.read();
        ff_din[155]             = ILLEGAL_INSTRUCTION_RD.read();
        ff_din[156]             = ADRESS_MISSALIGNED_RD.read();
        ff_din[157]             = SYSCALL_U_MODE_RD.read();
        ff_din[158]             = SYSCALL_M_MODE_RD.read();
        ff_din[159]             = exception_se.read();
        ff_din[160]             = load_adress_missaligned_se.read();
        ff_din[161]             = instruction_access_fault_se.read();
    }
    else{
        ff_din.range(31, 0)    = 0 ;
        ff_din.range(63, 32)   = 0 ;
        ff_din.range(69, 64)   = 0 ;
        ff_din.range(71, 70)   = 0 ;
        ff_din[72]             = 1 ;
        ff_din[73]             = 0 ;
        ff_din[74]             = 0 ;
        ff_din[75]             = 0 ;
        ff_din.range(107, 76)  = PC_DEC2EXE_RD.read();
        ff_din[108]            = 0 ;
        ff_din.range(120, 109) = 0 ;
        ff_din.range(152, 121) = 0 ;  // sending Csr read to wb to register
        ff_din[153]             = 0 ;
        ff_din[154]             = 0 ;
        ff_din[155]             = 0 ;
        ff_din[156]             = 0 ;
        ff_din[157]             = 0 ;
        ff_din[158]             = 0 ;
        ff_din[159]             = 0 ;
        ff_din[160]             = 0 ;
        ff_din[161]             = 0 ;
    }


    exe2mem_din_se.write(ff_din);
}
void exec::fifo_unconcat() {
    sc_bv<exe2mem_size> ff_dout = exe2mem_dout_se.read();
    EXE_RES_RE.write((sc_bv_base)ff_dout.range(31, 0));
    MEM_DATA_RE.write((sc_bv_base)ff_dout.range(63, 32));
    DEST_RE.write((sc_bv_base)ff_dout.range(69, 64));
    MEM_SIZE_RE.write((sc_bv_base)ff_dout.range(71, 70));
    WB_RE.write((bool)ff_dout[72]);
    MEM_LOAD_RE.write((bool)ff_dout[73]);
    MEM_STORE_RE.write((bool)ff_dout[74]);
    MEM_SIGN_EXTEND_RE.write((bool)ff_dout[75]);
    PC_EXE2MEM_RE.write((sc_bv_base)ff_dout.range(107, 76));
    CSR_WENABLE_RE.write((bool)ff_dout[108]);
    CSR_WADR_RE.write((sc_bv_base)ff_dout.range(120, 109));
    CSR_RDATA_RE.write((sc_bv_base)ff_dout.range(152, 121));
    ECALL_I_RE.write((bool)ff_dout[153]);
    EBREAK_I_RE.write((bool)ff_dout[154]);
    ILLEGAL_INSTRUCTION_RE.write((bool)ff_dout[155]);
    ADRESS_MISSALIGNED_RE.write((bool)ff_dout[156]);
    SYSCALL_U_MODE_RE.write((bool)ff_dout[157]);
    SYSCALL_M_MODE_RE.write((bool)ff_dout[158]);
    EXCEPTION_RE.write((bool)ff_dout[159]);
    LOAD_ADRESS_MISSALIGNED_RE.write((bool)ff_dout[160]);
    INSTRUCTION_ACCESS_FAULT_RE.write((bool)ff_dout[161]);
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
    } else if (DEST_RE.read() == RADR1_RD.read() && CSR_WENABLE_RE) {
        op1_se.write(CSR_RDATA_RE.read());
    } else if (DEST_RE.read() == RADR1_RD.read() && !MEM_LOAD_RE) {
        op1_se.write(EXE_RES_RE.read());
    } else if (MEM_DEST_RM.read() == RADR1_RD.read() && CSR_WENABLE_RM) {
        op1_se.write(CSR_RDATA_RM.read());
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
        sc_uint<32> bp_value;
        if (CSR_WENABLE_RE)
            bp_value = CSR_RDATA_RE;
        else
            bp_value = EXE_RES_RE;
        if (MEM_STORE_RD.read()) {  // on stores we need to bypass to the data not adr
            bp_mem_data_var = bp_value;
            op2_se.write(OP2_RD.read());
        } else {
            op2_se.write(bp_value);
        }
    } else if (MEM_DEST_RM.read() == RADR2_RD.read()) {
        sc_uint<32> bp_value;
        if (CSR_WENABLE_RM)
            bp_value = CSR_RDATA_RM;
        else
            bp_value = MEM_RES_RM;
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

void exec::exception() {

    exception_se = EXCEPTION_RD.read() || load_adress_missaligned_se || instruction_access_fault_se ;

    if (INTERRUPTION_SX.read() || EXCEPTION_RD.read())
    // in case of interrupt or exception have to inform other stage
    {
        INTERRUPTION_SE.write(1);
    }

    if (exception_se.read()) {
        wb_re.write(0);
        mem_load_re.write(0);
        mem_store_re.write(0);
    } else {
        wb_re.write(WB_RD.read());
        mem_load_re.write(MEM_LOAD_RD.read());
        mem_store_re.write(MEM_STORE_RD.read());
    }
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
    sc_trace(tf, CSR_WENABLE_RD, GET_NAME(CSR_WENABLE_RD));
    sc_trace(tf, CSR_WADR_RD, GET_NAME(CSR_WADR_RD));
    sc_trace(tf, CSR_RDATA_RD, GET_NAME(CSR_RDATA_RD));
    sc_trace(tf, ECALL_I_RD, GET_NAME(ECALL_I_RD));
    sc_trace(tf, EBREAK_I_RD, GET_NAME(EBREAK_I_RD));
    sc_trace(tf, ILLEGAL_INSTRUCTION_RD, GET_NAME(ILLEGAL_INSTRUCTION_RD));
    sc_trace(tf, ADRESS_MISSALIGNED_RD, GET_NAME(ADRESS_MISSALIGNED_RD));
    sc_trace(tf, SYSCALL_U_MODE_RD, GET_NAME(SYSCALL_U_MODE_RD));
    sc_trace(tf, SYSCALL_M_MODE_RD, GET_NAME(SYSCALL_M_MODE_RD));
    sc_trace(tf, EXCEPTION_RD, GET_NAME(EXCEPTION_RD));
    sc_trace(tf, INTERRUPTION_SE, GET_NAME(INTERRUPTION_SE));
    sc_trace(tf, INTERRUPTION_SX, GET_NAME(INTERRUPTION_SX));
    alu_inst.trace(tf);
    shifter_inst.trace(tf);
    fifo_inst.trace(tf);
}

// 0000010000101100010001100011011
// 00111101000110110101100010111010