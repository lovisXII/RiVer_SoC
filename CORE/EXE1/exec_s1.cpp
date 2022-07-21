#include "exec_s1.h"

void alu::operation() {
    sc_uint<32> res_interne;
    if (CMD_SE.read() == 0) {
        res_interne = OP1_SE_S1.read() + OP2_SE_S1.read() + CIN_SE;
    } else if (CMD_SE.read() == 1) {
        res_interne = OP1_SE_S1.read() & OP2_SE_S1.read();
    } else if (CMD_SE.read() == 2) {
        res_interne = OP1_SE_S1.read() | OP2_SE_S1.read();
    } else if (CMD_SE.read() == 3) {
        res_interne = OP1_SE_S1.read() ^ OP2_SE_S1.read();
    }
    RES_SE.write(res_interne);
}

void alu::trace(sc_trace_file* tf) {
    sc_trace(tf, OP1_SE_S1, GET_NAME(OP1_SE_S1));
    sc_trace(tf, OP2_SE_S1, GET_NAME(OP2_SE_S1));
    sc_trace(tf, CIN_SE, GET_NAME(CIN_SE));
    sc_trace(tf, CMD_SE, GET_NAME(CMD_SE));
    sc_trace(tf, RES_SE, GET_NAME(RES_SE));
}


void shifter::decode_cmd() {
    switch (CMD_SE.read()) {
        case 0:
            sll_se.write(1);
            srl_se.write(0);
            sra_se.write(0);
            break;
        case 1:
            sll_se.write(0);
            srl_se.write(1);
            sra_se.write(0);
            break;
        default:
            sll_se.write(0);
            srl_se.write(0);
            sra_se.write(1);
            break;
    }
}

void shifter::shifter_sll() {
    sc_uint<32> din       = DIN_SE.read();
    sc_uint<5>  shift_val = SHIFT_VAL_SE.read();
    sc_uint<32> sll_dout;
    switch (shift_val) {
        // from the mips code.
        case 0: sll_dout.range(31, 0) = din.range(31, 0); break;
        case 1:
            sll_dout.range(31, 1) = din.range(30, 0);
            sll_dout.range(0, 0)  = 0;
            break;
        case 2:
            sll_dout.range(31, 2) = din.range(29, 0);
            sll_dout.range(1, 0)  = 0;
            break;
        case 3:
            sll_dout.range(31, 3) = din.range(28, 0);
            sll_dout.range(2, 0)  = 0;
            break;
        case 4:
            sll_dout.range(31, 4) = din.range(27, 0);
            sll_dout.range(3, 0)  = 0;
            break;
        case 5:
            sll_dout.range(31, 5) = din.range(26, 0);
            sll_dout.range(4, 0)  = 0;
            break;
        case 6:
            sll_dout.range(31, 6) = din.range(25, 0);
            sll_dout.range(5, 0)  = 0;
            break;
        case 7:
            sll_dout.range(31, 7) = din.range(24, 0);
            sll_dout.range(6, 0)  = 0;
            break;
        case 8:
            sll_dout.range(31, 8) = din.range(23, 0);
            sll_dout.range(7, 0)  = 0;
            break;
        case 9:
            sll_dout.range(31, 9) = din.range(22, 0);
            sll_dout.range(8, 0)  = 0;
            break;
        case 10:
            sll_dout.range(31, 10) = din.range(21, 0);
            sll_dout.range(9, 0)   = 0;
            break;
        case 11:
            sll_dout.range(31, 11) = din.range(20, 0);
            sll_dout.range(10, 0)  = 0;
            break;
        case 12:
            sll_dout.range(31, 12) = din.range(19, 0);
            sll_dout.range(11, 0)  = 0;
            break;
        case 13:
            sll_dout.range(31, 13) = din.range(18, 0);
            sll_dout.range(12, 0)  = 0;
            break;
        case 14:
            sll_dout.range(31, 14) = din.range(17, 0);
            sll_dout.range(13, 0)  = 0;
            break;
        case 15:
            sll_dout.range(31, 15) = din.range(16, 0);
            sll_dout.range(14, 0)  = 0;
            break;
        case 16:
            sll_dout.range(31, 16) = din.range(15, 0);
            sll_dout.range(15, 0)  = 0;
            break;
        case 17:
            sll_dout.range(31, 17) = din.range(14, 0);
            sll_dout.range(16, 0)  = 0;
            break;
        case 18:
            sll_dout.range(31, 18) = din.range(13, 0);
            sll_dout.range(17, 0)  = 0;
            break;
        case 19:
            sll_dout.range(31, 19) = din.range(12, 0);
            sll_dout.range(18, 0)  = 0;
            break;
        case 20:
            sll_dout.range(31, 20) = din.range(11, 0);
            sll_dout.range(19, 0)  = 0;
            break;
        case 21:
            sll_dout.range(31, 21) = din.range(10, 0);
            sll_dout.range(20, 0)  = 0;
            break;
        case 22:
            sll_dout.range(31, 22) = din.range(9, 0);
            sll_dout.range(21, 0)  = 0;
            break;
        case 23:
            sll_dout.range(31, 23) = din.range(8, 0);
            sll_dout.range(22, 0)  = 0;
            break;
        case 24:
            sll_dout.range(31, 24) = din.range(7, 0);
            sll_dout.range(23, 0)  = 0;
            break;
        case 25:
            sll_dout.range(31, 25) = din.range(6, 0);
            sll_dout.range(24, 0)  = 0;
            break;
        case 26:
            sll_dout.range(31, 26) = din.range(5, 0);
            sll_dout.range(25, 0)  = 0;
            break;
        case 27:
            sll_dout.range(31, 27) = din.range(4, 0);
            sll_dout.range(26, 0)  = 0;
            break;
        case 28:
            sll_dout.range(31, 28) = din.range(3, 0);
            sll_dout.range(27, 0)  = 0;
            break;
        case 29:
            sll_dout.range(31, 29) = din.range(2, 0);
            sll_dout.range(28, 0)  = 0;
            break;
        case 30:
            sll_dout.range(31, 30) = din.range(1, 0);
            sll_dout.range(29, 0)  = 0;
            break;
        case 31:
            sll_dout.range(31, 31) = din.range(0, 0);
            sll_dout.range(30, 0)  = 0;
            break;
    }
    sll_dout_se.write(sll_dout);
}

void shifter::shifter_sr() {
    sc_uint<32> din       = DIN_SE.read();
    sc_uint<5>  shift_val = SHIFT_VAL_SE.read();
    sc_uint<32> sr_dout;
    sc_uint<32> shiftin;
    // compute the sign extension for arithmetic shift
    if (sra_se.read() & din[31]) {
        shiftin = 0xFFFFFFFF;
    } else {
        shiftin = 0x0;
    }
    switch (shift_val) {
        case 0: sr_dout.range(31, 0) = din.range(31, 0); break;
        case 1:
            sr_dout.range(31, 31) = shiftin.range(31, 31);
            sr_dout.range(30, 0)  = din.range(31, 1);
            break;
        case 2:
            sr_dout.range(31, 30) = shiftin.range(31, 30);
            sr_dout.range(29, 0)  = din.range(31, 2);
            break;
        case 3:
            sr_dout.range(31, 29) = shiftin.range(31, 29);
            sr_dout.range(28, 0)  = din.range(31, 3);
            break;
        case 4:
            sr_dout.range(31, 28) = shiftin.range(31, 28);
            sr_dout.range(27, 0)  = din.range(31, 4);
            break;
        case 5:
            sr_dout.range(31, 27) = shiftin.range(31, 27);
            sr_dout.range(26, 0)  = din.range(31, 5);
            break;
        case 6:
            sr_dout.range(31, 26) = shiftin.range(31, 26);
            sr_dout.range(25, 0)  = din.range(31, 6);
            break;
        case 7:
            sr_dout.range(31, 25) = shiftin.range(31, 25);
            sr_dout.range(24, 0)  = din.range(31, 7);
            break;
        case 8:
            sr_dout.range(31, 24) = shiftin.range(31, 24);
            sr_dout.range(23, 0)  = din.range(31, 8);
            break;
        case 9:
            sr_dout.range(31, 23) = shiftin.range(31, 23);
            sr_dout.range(22, 0)  = din.range(31, 9);
            break;
        case 10:
            sr_dout.range(31, 22) = shiftin.range(31, 22);
            sr_dout.range(21, 0)  = din.range(31, 10);
            break;
        case 11:
            sr_dout.range(31, 21) = shiftin.range(31, 21);
            sr_dout.range(20, 0)  = din.range(31, 11);
            break;
        case 12:
            sr_dout.range(31, 20) = shiftin.range(31, 20);
            sr_dout.range(19, 0)  = din.range(31, 12);
            break;
        case 13:
            sr_dout.range(31, 19) = shiftin.range(31, 19);
            sr_dout.range(18, 0)  = din.range(31, 13);
            break;
        case 14:
            sr_dout.range(31, 18) = shiftin.range(31, 18);
            sr_dout.range(17, 0)  = din.range(31, 14);
            break;
        case 15:
            sr_dout.range(31, 17) = shiftin.range(31, 17);
            sr_dout.range(16, 0)  = din.range(31, 15);
            break;
        case 16:
            sr_dout.range(31, 16) = shiftin.range(31, 16);
            sr_dout.range(15, 0)  = din.range(31, 16);
            break;
        case 17:
            sr_dout.range(31, 15) = shiftin.range(31, 15);
            sr_dout.range(14, 0)  = din.range(31, 17);
            break;
        case 18:
            sr_dout.range(31, 14) = shiftin.range(31, 14);
            sr_dout.range(13, 0)  = din.range(31, 18);
            break;
        case 19:
            sr_dout.range(31, 13) = shiftin.range(31, 13);
            sr_dout.range(12, 0)  = din.range(31, 19);
            break;
        case 20:
            sr_dout.range(31, 12) = shiftin.range(31, 12);
            sr_dout.range(11, 0)  = din.range(31, 20);
            break;
        case 21:
            sr_dout.range(31, 11) = shiftin.range(31, 11);
            sr_dout.range(10, 0)  = din.range(31, 21);
            break;
        case 22:
            sr_dout.range(31, 10) = shiftin.range(31, 10);
            sr_dout.range(9, 0)   = din.range(31, 22);
            break;
        case 23:
            sr_dout.range(31, 9) = shiftin.range(31, 9);
            sr_dout.range(8, 0)  = din.range(31, 23);
            break;
        case 24:
            sr_dout.range(31, 8) = shiftin.range(31, 8);
            sr_dout.range(7, 0)  = din.range(31, 24);
            break;
        case 25:
            sr_dout.range(31, 7) = shiftin.range(31, 7);
            sr_dout.range(6, 0)  = din.range(31, 25);
            break;
        case 26:
            sr_dout.range(31, 6) = shiftin.range(31, 6);
            sr_dout.range(5, 0)  = din.range(31, 26);
            break;
        case 27:
            sr_dout.range(31, 5) = shiftin.range(31, 5);
            sr_dout.range(4, 0)  = din.range(31, 27);
            break;
        case 28:
            sr_dout.range(31, 4) = shiftin.range(31, 4);
            sr_dout.range(3, 0)  = din.range(31, 28);
            break;
        case 29:
            sr_dout.range(31, 3) = shiftin.range(31, 3);
            sr_dout.range(2, 0)  = din.range(31, 29);
            break;
        case 30:
            sr_dout.range(31, 2) = shiftin.range(31, 2);
            sr_dout.range(1, 0)  = din.range(31, 30);
            break;
        case 31:
            sr_dout.range(31, 1) = shiftin.range(31, 1);
            sr_dout.range(0, 0)  = din.range(31, 31);
            break;
    }
    sr_dout_se.write(sr_dout);
}

void shifter::shifter_agregate() {
    if (sll_se.read()) {
        DOUT_SE.write(sll_dout_se.read());
    } else {
        DOUT_SE.write(sr_dout_se.read());
    }
}

void shifter::trace(sc_trace_file* tf) {
    sc_trace(tf, DIN_SE, GET_NAME(DIN_SE));
    sc_trace(tf, SHIFT_VAL_SE, GET_NAME(SHIFT_VAL_SE));
    sc_trace(tf, CMD_SE, GET_NAME(CMD_SE));
    sc_trace(tf, DOUT_SE, GET_NAME(DOUT_SE));
    sc_trace(tf, sra_se, GET_NAME(sra_se));
    sc_trace(tf, sll_se, GET_NAME(sll_se));
    sc_trace(tf, srl_se, GET_NAME(srl_se));
    sc_trace(tf, sll_dout_se, GET_NAME(sll_dout_se));
    sc_trace(tf, sr_dout_se, GET_NAME(sr_dout_se));
}

void exec_s1::preprocess_op() {
    sc_uint<32> op1 = op1_se_s1.read();
    sc_uint<32> op2 = op2_se.read();
    if (NEG_OP2_RD_S1.read()) {
        alu_in_op2_se.write(~op2);
    } else {
        alu_in_op2_se.write(op2);
    }
    shift_val_se.write(op2.range(4, 0));

    OP1_SE_S1.write(op1_se_s1);
    OP2_SE_S1.write(op2_se);
}

void exec_s1::select_exec_res() {
    sc_uint<32> alu_out     = alu_out_se.read();
    sc_uint<32> shifter_out = shifter_out_se.read();

    /*if (SELECT_TYPE_OPERATIONS_RD_S1.read() == 0b1000)
    {
        exe_res_se_s1.write(divider_out_se);
    }
    else */
    if (SELECT_TYPE_OPERATIONS_RD_S1.read() == 0b0010) {
        exe_res_se_s1.write(shifter_out_se);
    } else if (SELECT_TYPE_OPERATIONS_RD_S1.read() == 0b0001) {
        if (SLT_RD_S1.read()) {
            if (op1_se_s1.read()[31] == 1 && op2_se.read()[31] == 0) {
                exe_res_se_s1.write(1);
            } else if (op1_se_s1.read()[31] == 0 && op2_se.read()[31] == 1) {
                exe_res_se_s1.write(0);
            } else if (op1_se_s1.read() == op2_se.read()) {
                exe_res_se_s1.write(0);
            } else {
                exe_res_se_s1.write((bool)alu_out_se.read()[31]);
            }
        } else if (SLTU_RD_S1.read()) {
            if (op1_se_s1.read()[31] == 1 && op2_se.read()[31] == 0) {
                exe_res_se_s1.write(0);
            } else if (op1_se_s1.read()[31] == 0 && op2_se.read()[31] == 1) {
                exe_res_se_s1.write(1);
            } else if (op1_se_s1.read() == op2_se.read()) {
                exe_res_se_s1.write(0);
            } else {
                exe_res_se_s1.write((bool)alu_out_se.read()[31]);
            }
        } else {
            if (MEM_LOAD_RD_S1.read() || MEM_STORE_RD_S1.read()) {
                // MEM_SIZE = 0 -> Word
                // MEM_SIZE = 1 -> Half word
                // MEM_SIZE = 2 -> byte
                if (((alu_out_se.read() & 0b11) != 0 && MEM_SIZE_RD_S1.read() == 0) ||
                    ((alu_out_se.read() & 0b1) != 0 &&
                     MEM_SIZE_RD_S1.read() == 1)) {  // if adress isn't aligned it creates an exception
                                                     // loading bytes on byte-aligned adresses is legal
                    if (MEM_LOAD_RD_S1)
                        load_adress_missaligned_se = 1;
                    else
                        store_adress_missaligned_se = 1;
                } else {
                    load_adress_missaligned_se.write(0);
                    store_adress_missaligned_se.write(0);
                }
                if ((CURRENT_MODE_SM_S1.read() == 0))  // If in User Mode
                {
                    if (alu_out_se.read() > start_kernel_adress) {
                        if (MEM_LOAD_RD_S1)
                            load_access_fault_se = 1;
                        else
                            store_access_fault_se = 1;
                    } else {
                        load_access_fault_se  = 0;
                        store_access_fault_se = 0;
                    }
                } else {
                    load_access_fault_se  = 0;
                    store_access_fault_se = 0;
                }
            } else {
                load_adress_missaligned_se.write(0);
                load_access_fault_se.write(0);
                store_adress_missaligned_se = 0;
                store_access_fault_se       = 0;
            }
            exe_res_se_s1.write(alu_out_se);
        }
    }
}

void exec_s1::fifo_concat() {
    sc_bv<exe2mem_size> ff_din;
    if (EXCEPTION_SM_S1.read() == 0) {
        ff_din.range(31, 0)    = exe_res_se_s1.read();
        ff_din.range(63, 32)   = bp_mem_data_sd.read();
        ff_din.range(69, 64)   = DEST_RD_S1.read();
        ff_din.range(71, 70)   = MEM_SIZE_RD_S1.read();
        ff_din[72]             = wb_re.read();
        ff_din[73]             = mem_load_re.read();
        ff_din[74]             = mem_store_re.read();
        ff_din[75]             = MEM_SIGN_EXTEND_RD_S1.read();
        ff_din.range(107, 76)  = PC_DEC2EXE_RD_S1.read();
        ff_din[108]            = CSR_WENABLE_RD_S1.read();
        ff_din.range(120, 109) = CSR_WADR_RD_S1.read();
        ff_din.range(152, 121) = CSR_RDATA_RD_S1.read();  // sending Csr read to wb to register
        ff_din[153]            = ENV_CALL_S_MODE_RD_S1.read();
        ff_din[154]            = ENV_CALL_WRONG_MODE_RD_S1.read();
        ff_din[155]            = ILLEGAL_INSTRUCTION_RD_S1.read();
        ff_din[156]            = ADRESS_MISSALIGNED_RD_S1.read();
        ff_din[157]            = ENV_CALL_U_MODE_RD_S1.read();
        ff_din[158]            = ENV_CALL_M_MODE_RD_S1.read();
        ff_din[159]            = exception_se.read();
        ff_din[160]            = load_adress_missaligned_se.read();
        ff_din[161]            = load_access_fault_se.read();
        ff_din[162]            = MRET_RD_S1.read();
        ff_din[163]            = store_adress_missaligned_se;
        ff_din[164]            = store_access_fault_se;
        ff_din[165]            = INSTRUCTION_ACCESS_FAULT_RD_S1;
        ff_din[166]            = EBREAK_RD_S1;
        ff_din[167]            = MULT_INST_RD_S1.read();
        ff_din.range(168, 199) = PC_BRANCH_VALUE_RD_S1.read();

    } else {
        ff_din.range(31, 0)    = 0;
        ff_din.range(63, 32)   = 0;
        ff_din.range(69, 64)   = 0;
        ff_din.range(71, 70)   = 0;
        ff_din[72]             = 1;
        ff_din[73]             = 0;
        ff_din[74]             = 0;
        ff_din[75]             = 0;
        ff_din.range(107, 76)  = PC_DEC2EXE_RD_S1.read();
        ff_din[108]            = 0;
        ff_din.range(120, 109) = 0;
        ff_din.range(152, 121) = 0;  // sending Csr read to wb to register
        ff_din[153]            = 0;
        ff_din[154]            = 0;
        ff_din[155]            = 0;
        ff_din[156]            = 0;
        ff_din[157]            = 0;
        ff_din[158]            = 0;
        ff_din[159]            = 0;
        ff_din[160]            = 0;
        ff_din[161]            = 0;
        ff_din[162]            = 0;
        ff_din[163]            = 0;
        ff_din[164]            = 0;
        ff_din[165]            = 0;
        ff_din[166]            = 0;
        ff_din[167]            = 0;
        ff_din.range(168, 199) = 0;
    }

    exe2mem_din_se_s1.write(ff_din);
}
void exec_s1::fifo_unconcat() {
    sc_bv<exe2mem_size> ff_dout = exe2mem_dout_se_s1.read();
    EXE_RES_RE_S1.write((sc_bv_base)ff_dout.range(31, 0));
    MEM_DATA_RE_S1.write((sc_bv_base)ff_dout.range(63, 32));
    DEST_RE_S1.write((sc_bv_base)ff_dout.range(69, 64));
    MEM_SIZE_RE_S1.write((sc_bv_base)ff_dout.range(71, 70));
    WB_RE_S1.write((bool)ff_dout[72]);
    MEM_LOAD_RE_S1.write((bool)ff_dout[73]);
    MEM_STORE_RE_S1.write((bool)ff_dout[74]);
    MEM_SIGN_EXTEND_RE_S1.write((bool)ff_dout[75]);
    PC_EXE2MEM_RE_S1.write((sc_bv_base)ff_dout.range(107, 76));
    CSR_WENABLE_RE_S1.write((bool)ff_dout[108]);
    CSR_WADR_RE_S1.write((sc_bv_base)ff_dout.range(120, 109));
    CSR_RDATA_RE_S1.write((sc_bv_base)ff_dout.range(152, 121));
    ENV_CALL_S_MODE_RE_S1.write((bool)ff_dout[153]);
    ENV_CALL_WRONG_MODE_RE_S1.write((bool)ff_dout[154]);
    ILLEGAL_INSTRUCTION_RE_S1.write((bool)ff_dout[155]);
    INSTRUCTION_ADRESS_MISSALIGNED_RE_S1.write((bool)ff_dout[156]);
    ENV_CALL_U_MODE_RE_S1.write((bool)ff_dout[157]);
    ENV_CALL_M_MODE_RE_S1.write((bool)ff_dout[158]);
    EXCEPTION_RE_S1.write((bool)ff_dout[159]);
    LOAD_ADRESS_MISSALIGNED_RE_S1.write((bool)ff_dout[160]);
    LOAD_ACCESS_FAULT_RE_S1.write((bool)ff_dout[161]);
    MRET_RE_S1.write((bool)ff_dout[162]);
    STORE_ADRESS_MISSALIGNED_RE_S1.write((bool)ff_dout[163]);
    STORE_ACCESS_FAULT_RE_S1.write((bool)ff_dout[164]);
    INSTRUCTION_ACCESS_FAULT_RE_S1.write((bool)ff_dout[165]);
    EBREAK_RE_S1.write((bool)ff_dout[166]);
    MULT_INST_RE_S1.write((bool)ff_dout[167]);
    PC_BRANCH_VALUE_RE_S1.write((sc_bv_base)ff_dout.range(168, 199));
}

void exec_s1::manage_fifo() {
    bool stall = exe2mem_full_se.read() || DEC2EXE_EMPTY_SD_S1.read() || blocked.read() || !r1_valid_se || !r2_valid_se;
    if (stall) {
        exe2mem_push_se.write(false);
        DEC2EXE_POP_SE_S1.write(false);
    } else {
        exe2mem_push_se.write(true);
        DEC2EXE_POP_SE_S1.write(true);
    }
    stall_se = stall;
}

void exec_s1::bypasses() {
    bool        blocked_var     = false;
    sc_uint<32> bp_mem_data_var = MEM_DATA_RD_S1.read();
    
    bool bp_s2_E2_is_taken =
    (DEST_RE_S1.read() == DEST_RE_S2.read()) && (DEST_RE_S1.read() != 0);
    
    bool bp_s2_M2_is_taken =
    (MEM_DEST_RM_S1.read() == MEM_DEST_RM_S2.read()) && (MEM_DEST_RM_S2.read() != 0);

    // ###############################
    // BYPASS on rs1_s1 :
    // ###############################

    if (RADR1_RD_S1.read() == 0 || BLOCK_BP_RD_S1.read()) 
    {
        op1_se_s1.write(OP1_RD_S1.read());
        r1_valid_se = true;
    } 
    else if (DEST_RE_S1.read() == RADR1_RD_S1.read() && CSR_WENABLE_RE_S1 && !bp_s2_E2_is_taken) 
    // E1->E1 and csr
    {
        op1_se_s1.write(CSR_RDATA_RE_S1.read());
        r1_valid_se = true;
    }
    else if (DEST_RE_S1.read() == RADR1_RD_S1.read() && !MEM_LOAD_RE_S1 && !bp_s2_E2_is_taken) 
    // E1->E1 and no load (normal bypass)
    {
        op1_se_s1.write(EXE_RES_RE_S1.read());
        r1_valid_se = !MULT_INST_RE_S1 || EXE2MEM_EMPTY_SE_S1;
    }
    else if (MEM_DEST_RM_S1.read() == RADR1_RD_S1.read() && CSR_WENABLE_RM_S1 && !bp_s2_M2_is_taken) 
    // M1->E1 and csr
    {
        op1_se_s1.write(CSR_RDATA_RM_S1.read());
        r1_valid_se = true;
    }
    else if (MEM_DEST_RM_S1.read() == RADR1_RD_S1.read() && !bp_s2_M2_is_taken) 
    // M1->E1 normal
    {
        op1_se_s1.write(MEM_RES_RM_S1.read());
        r1_valid_se = !MULT_INST_RM_S1 || MEM2WBK_EMPTY_SM_S1;
    }
    else if(DEST_RE_S2.read() == RADR1_RD_S1.read() && CSR_WENABLE_RE_S2)
    // E2->E1 and csr
    {
        op1_se_s1.write(CSR_RDATA_RE_S2.read());
        r1_valid_se = true;
    }
    else if(DEST_RE_S2.read() == RADR1_RD_S1.read() && !MEM_LOAD_RE_S2) 
    // E2->E1 and no load (normal bypass)
    {
        op1_se_s1.write(EXE_RES_RE_S2.read());
        r1_valid_se = true;
    }
    else if (MEM_DEST_RM_S2.read() == RADR1_RD_S1.read() && CSR_WENABLE_RM_S2) 
    // M2->E1 and csr
    {
        op1_se_s1.write(CSR_RDATA_RM_S2.read());
        r1_valid_se = true;
    }
    else if (MEM_DEST_RM_S2.read() == RADR1_RD_S1.read()) 
    // M2->E1 normal
    {
        op1_se_s1 = MEM_RES_RM_S2.read();
        r1_valid_se = true;
    }
    else if (DEST_RE_S1.read() == RADR1_RD_S1.read() && MEM_LOAD_RE_S1 && !EXE2MEM_EMPTY_SE_S1) 
    //stall in case of load in M1 and data dependencies in exe
    // ex :
    // lw r3,0(4)
    //  add r2,r3,r0
    // stall cause of r3
    {
        blocked_var = true;
        r1_valid_se = true;
    }
    else if (DEST_RE_S2.read() == RADR1_RD_S1.read() && MEM_LOAD_RE_S2 && !EXE2MEM_EMPTY_SE_S2) 
    // stall in case of load in M2 and data dependencies in exe
    {
        blocked_var = true;
        r1_valid_se = true;
    }
    else if(op1_is_saved_re){
        r1_valid_se = true ;
        op1_se_s1 = op1_bp_re;
    }
    else 
    {
        op1_se_s1.write(OP1_RD_S1.read());
        r1_valid_se = true;
    }

    // ###############################
    // BYPASS on rs2_s1 :
    // ###############################


    /*
    For rs2, there is no bypass in case of a load :
    lw rd, imm(rs1)

    But in case of a store :
    
    sw rs2,0(rs1)

    We must bypass the value 
    */
    if (RADR2_RD_S1.read() == 0 || MEM_LOAD_RD_S1.read() || BLOCK_BP_RD_S1.read()) 
    {
        op2_se.write(OP2_RD_S1.read());
        r2_valid_se = true;
    } 
    else if (DEST_RE_S1.read() == RADR2_RD_S1.read() && !MEM_LOAD_RE_S1 && !bp_s2_E2_is_taken) 
    // E1->E1 bypass
    {
        sc_uint<32> bp_value;
        if (CSR_WENABLE_RE_S1) // case with csr
            bp_value = CSR_RDATA_RE_S1;
        else
            bp_value = EXE_RES_RE_S1;

        if (MEM_STORE_RD_S1.read()) //case of a store 
        {  // on stores we need to bypass to the data not adr
            bp_mem_data_var = bp_value;
            op2_se.write(OP2_RD_S1.read());
            r2_valid_se = true;
        }
        else 
        {
            op2_se.write(bp_value);
            r2_valid_se = !MULT_INST_RE_S1 || EXE2MEM_EMPTY_SE_S1;
        }
    } 
    else if (MEM_DEST_RM_S1.read() == RADR2_RD_S1.read() && !bp_s2_M2_is_taken) 
    // M1->E1
    {
        sc_uint<32> bp_value;
        if (CSR_WENABLE_RM_S1)
            bp_value = CSR_RDATA_RM_S1;
        else
            bp_value = MEM_RES_RM_S1;
        
        if (MEM_STORE_RD_S1.read()) 
        {
            bp_mem_data_var = MEM_RES_RM_S1.read();
            op2_se.write(OP2_RD_S1.read());
            r2_valid_se = true;
        }
        else 
        {
            op2_se.write(MEM_RES_RM_S1.read());
            r2_valid_se = !MULT_INST_RM_S1 || MEM2WBK_EMPTY_SM_S1;
        }
    }
    else if (DEST_RE_S1.read() == RADR2_RD_S1.read() && MEM_LOAD_RE_S1 && !EXE2MEM_EMPTY_SE_S1 && !bp_s2_M2_is_taken) 
    //M1->E1 with load so need to stall
    {
        blocked_var = true;
        r2_valid_se = true;
    }
    else if(DEST_RE_S2.read() == RADR2_RD_S1.read() && !MEM_LOAD_RE_S2) 
    // E2->E1 bypass
    {
        sc_uint<32> bp_value;
        if (CSR_WENABLE_RE_S1) // case with csr
            bp_value = CSR_RDATA_RE_S2;
        else
            bp_value = EXE_RES_RE_S2;

        if (MEM_STORE_RD_S1.read()) //case of a store 
        {  // on stores we need to bypass to the data not adr
            bp_mem_data_var = bp_value;
            op2_se.write(OP2_RD_S1.read());
            r2_valid_se = true;
        }
        else 
        {
            op2_se.write(bp_value);
            r2_valid_se = true;
        }
    } 
    else if(MEM_DEST_RM_S2.read() == RADR2_RD_S1.read()) 
    // M2->E1
    {
        sc_uint<32> bp_value;
        if (CSR_WENABLE_RM_S2)
            bp_value = CSR_RDATA_RM_S2;
        else
            bp_value = MEM_RES_RM_S2;

        if (MEM_STORE_RD_S1.read()) {
            bp_mem_data_var = MEM_RES_RM_S2.read();
            op2_se.write(OP2_RD_S1.read());
            r2_valid_se = true;
        } else {
            op2_se.write(MEM_RES_RM_S2.read());
            r2_valid_se = true;
        }
    }
    else if (DEST_RE_S2.read() == RADR2_RD_S1.read() && MEM_LOAD_RE_S2 && !EXE2MEM_EMPTY_SE_S2) 
    {
        blocked_var = true;
        r2_valid_se = true;
    }
    else if(op2_is_saved_re){
        r2_valid_se = true ;
        op2_se = op2_bp_re;
    }
    else 
    {
        op2_se.write(OP2_RD_S1.read());
        r2_valid_se = true;
    }
    bp_mem_data_sd.write(bp_mem_data_var);
    blocked.write(blocked_var);
}

void exec_s1::save_op_bp_in_register(){
    if((RADR1_RD_S1 == MEM_DEST_RM_S1 || RADR1_RD_S1 == MEM_DEST_RM_S2) && blocked)
    {
        op1_is_saved_re = 1;
        op2_is_saved_re = 0;
        op1_bp_re = op1_se_s1;
    }
    else if((RADR2_RD_S1 == MEM_DEST_RM_S1 || RADR2_RD_S1 == MEM_DEST_RM_S2) && blocked){

        op1_is_saved_re = 0;
        op2_is_saved_re = 1;
        op1_bp_re = op2_se;
    } 
    else{

        op1_is_saved_re = 0;
        op2_is_saved_re = 0;
    }
}
void exec_s1::exception() {
    exception_se = EXCEPTION_RD_S1 | load_adress_missaligned_se | load_access_fault_se | store_access_fault_se |
                   store_adress_missaligned_se;

    if (MACHINE_SOFTWARE_INTERRUPT_SX | MACHINE_TIMER_INTERRUPT_SX | MACHINE_EXTERNAL_INTERRUPT_SX)
    // in case of interrupt or exception have to inform other stage
    {
        INTERRUPTION_SE_S1.write(1);
    } else {
        INTERRUPTION_SE_S1.write(0);
    }

    if (exception_se.read()) {
        wb_re.write(0);
        mem_load_re.write(0);
        mem_store_re.write(0);
    } else {
        wb_re.write(WB_RD_S1.read());
        mem_load_re.write(MEM_LOAD_RD_S1.read());
        mem_store_re.write(MEM_STORE_RD_S1.read());
    }
}

void exec_s1::trace(sc_trace_file* tf) {
    sc_trace(tf, OP1_RD_S1, GET_NAME(OP1_RD_S1));  // can contains CSR if CSR_type_operation_RD == 1
    sc_trace(tf, OP2_RD_S1, GET_NAME(OP2_RD_S1));
    sc_trace(tf, OP1_VALID_RD_S1, GET_NAME(OP1_VALID_RD_S1));
    sc_trace(tf, OP2_VALID_RD_S1, GET_NAME(OP2_VALID_RD_S1));
    sc_trace(tf, RADR1_RD_S1, GET_NAME(RADR1_RD_S1));
    sc_trace(tf, RADR2_RD_S1, GET_NAME(RADR2_RD_S1));

    sc_trace(tf, PC_DEC2EXE_RD_S1, GET_NAME(PC_DEC2EXE_RD_S1));
    sc_trace(tf, MEM_DATA_RD_S1, GET_NAME(MEM_DATA_RD_S1));
    sc_trace(tf, DEST_RD_S1, GET_NAME(DEST_RD_S1));
    sc_trace(tf, CMD_RD_S1, GET_NAME(CMD_RD_S1));
    sc_trace(tf, MEM_SIZE_RD_S1, GET_NAME(MEM_SIZE_RD_S1));
    sc_trace(tf, NEG_OP2_RD_S1, GET_NAME(NEG_OP2_RD_S1));
    sc_trace(tf, WB_RD_S1, GET_NAME(WB_RD_S1));
    sc_trace(tf, MEM_SIGN_EXTEND_RD_S1, GET_NAME(MEM_SIGN_EXTEND_RD_S1));
    sc_trace(tf, SELECT_TYPE_OPERATIONS_RD_S1, GET_NAME(SELECT_TYPE_OPERATIONS_RD_S1));
    sc_trace(tf, MEM_LOAD_RD_S1, GET_NAME(MEM_LOAD_RD_S1));
    sc_trace(tf, MEM_STORE_RD_S1, GET_NAME(MEM_STORE_RD_S1));
    sc_trace(tf, EXE2MEM_POP_SM_S1, GET_NAME(EXE2MEM_POP_SM_S1));
    sc_trace(tf, DEC2EXE_EMPTY_SD_S1, GET_NAME(DEC2EXE_EMPTY_SD_S1));
    sc_trace(tf, SLT_RD_S1, GET_NAME(SLT_RD_S1));
    sc_trace(tf, SLTU_RD_S1, GET_NAME(SLTU_RD_S1));
    sc_trace(tf, CSR_WENABLE_RD_S1, GET_NAME(CSR_WENABLE_RD_S1));
    sc_trace(tf, CSR_WADR_RD_S1, GET_NAME(CSR_WADR_RD_S1));
    sc_trace(tf, CSR_RDATA_RD_S1, GET_NAME(CSR_RDATA_RD_S1));
    sc_trace(tf, EXCEPTION_RD_S1, GET_NAME(EXCEPTION_RD_S1));  // tells if an instruction have been made in DEC

    sc_trace(tf, ENV_CALL_S_MODE_RD_S1, GET_NAME(ENV_CALL_S_MODE_RD_S1));
    sc_trace(tf, ENV_CALL_WRONG_MODE_RD_S1, GET_NAME(ENV_CALL_WRONG_MODE_RD_S1));
    sc_trace(tf, ILLEGAL_INSTRUCTION_RD_S1, GET_NAME(ILLEGAL_INSTRUCTION_RD_S1));  // accessing stuff in wrong mode
    sc_trace(tf, ADRESS_MISSALIGNED_RD_S1, GET_NAME(ADRESS_MISSALIGNED_RD_S1));    // branch offset is misaligned
    sc_trace(tf, ENV_CALL_U_MODE_RD_S1, GET_NAME(ENV_CALL_U_MODE_RD_S1));
    sc_trace(tf, ENV_CALL_M_MODE_RD_S1, GET_NAME(ENV_CALL_M_MODE_RD_S1));

    sc_trace(tf, EXCEPTION_RE_S1, GET_NAME(EXCEPTION_RE_S1));
    sc_trace(
        tf, LOAD_ADRESS_MISSALIGNED_RE_S1, GET_NAME(LOAD_ADRESS_MISSALIGNED_RE_S1));  // adress from store/load isn't aligned
    sc_trace(tf, LOAD_ACCESS_FAULT_RE_S1,
             GET_NAME(LOAD_ACCESS_FAULT_RE_S1));  // trying to access memory in wrong mode
    sc_trace(tf, ENV_CALL_U_MODE_RE_S1, GET_NAME(ENV_CALL_U_MODE_RE_S1));
    sc_trace(tf, ENV_CALL_WRONG_MODE_RE_S1, GET_NAME(ENV_CALL_WRONG_MODE_RE_S1));
    sc_trace(tf, ILLEGAL_INSTRUCTION_RE_S1, GET_NAME(ILLEGAL_INSTRUCTION_RE_S1));  // accessing stuff in wrong mode
    sc_trace(tf,
             INSTRUCTION_ADRESS_MISSALIGNED_RE_S1,
             GET_NAME(INSTRUCTION_ADRESS_MISSALIGNED_RE_S1));  // branch offset is misaligned
    sc_trace(tf, ENV_CALL_S_MODE_RE_S1, GET_NAME(ENV_CALL_S_MODE_RE_S1));
    sc_trace(tf, ENV_CALL_M_MODE_RE_S1, GET_NAME(ENV_CALL_M_MODE_RE_S1));
    sc_trace(tf, PC_BRANCH_VALUE_RD_S1, GET_NAME(PC_BRANCH_VALUE_RD_S1));
    sc_trace(tf, PC_BRANCH_VALUE_RE_S1, GET_NAME(PC_BRANCH_VALUE_RE_S1));

    // Interruption :

    sc_trace(tf, INTERRUPTION_SE_S1, GET_NAME(INTERRUPTION_SE_S1));
    sc_trace(tf, MACHINE_SOFTWARE_INTERRUPT_SX, GET_NAME(MACHINE_SOFTWARE_INTERRUPT_SX));
    sc_trace(tf, MACHINE_TIMER_INTERRUPT_SX, GET_NAME(MACHINE_TIMER_INTERRUPT_SX));
    sc_trace(tf, MACHINE_EXTERNAL_INTERRUPT_SX, GET_NAME(MACHINE_EXTERNAL_INTERRUPT_SX));
    sc_trace(tf, MACHINE_SOFTWARE_INTERRUPT_SE_S1, GET_NAME(MACHINE_SOFTWARE_INTERRUPT_SE_S1));
    sc_trace(tf, MACHINE_TIMER_INTERRUPT_SE_S1, GET_NAME(MACHINE_TIMER_INTERRUPT_SE_S1));
    sc_trace(tf, MACHINE_EXTERNAL_INTERRUPT_SE_S1, GET_NAME(MACHINE_EXTERNAL_INTERRUPT_SE_S1));

    // bypasses

    sc_trace(tf, MEM_DEST_RM_S1, GET_NAME(MEM_DEST_RM_S1));
    sc_trace(tf, MEM_RES_RM_S1, GET_NAME(MEM_RES_RM_S1));
    sc_trace(tf, CSR_WENABLE_RM_S1, GET_NAME(CSR_WENABLE_RM_S1));
    sc_trace(tf, CSR_RDATA_RM_S1, GET_NAME(CSR_RDATA_RM_S1));

    // Genral Interface :

    sc_trace(tf, EXCEPTION_SM_S1, GET_NAME(EXCEPTION_SM_S1));
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET, GET_NAME(RESET));

    // Fifo exe2mem interface :

    sc_trace(tf, EXE_RES_RE_S1, GET_NAME(EXE_RES_RE_S1));
    sc_trace(tf, MEM_DATA_RE_S1, GET_NAME(MEM_DATA_RE_S1));
    sc_trace(tf, DEST_RE_S1, GET_NAME(DEST_RE_S1));
    sc_trace(tf, MEM_SIZE_RE_S1, GET_NAME(MEM_SIZE_RE_S1));
    sc_trace(tf, PC_EXE2MEM_RE_S1, GET_NAME(PC_EXE2MEM_RE_S1));

    sc_trace(tf, WB_RE_S1, GET_NAME(WB_RE_S1));
    sc_trace(tf, MEM_SIGN_EXTEND_RE_S1, GET_NAME(MEM_SIGN_EXTEND_RE_S1));
    sc_trace(tf, MEM_LOAD_RE_S1, GET_NAME(MEM_LOAD_RE_S1));
    sc_trace(tf, MEM_STORE_RE_S1, GET_NAME(MEM_STORE_RE_S1));
    sc_trace(tf, EXE2MEM_EMPTY_SE_S1, GET_NAME(EXE2MEM_EMPTY_SE_S1));
    sc_trace(tf, DEC2EXE_POP_SE_S1, GET_NAME(DEC2EXE_POP_SE_S1));

    sc_trace(tf, CSR_WENABLE_RE_S1, GET_NAME(CSR_WENABLE_RE_S1));
    sc_trace(tf, CSR_WADR_RE_S1, GET_NAME(CSR_WADR_RE_S1));
    sc_trace(tf, CSR_RDATA_RE_S1, GET_NAME(CSR_RDATA_RE_S1));

    // Internals signals :

    sc_trace(tf, exe_res_se_s1, GET_NAME(exe_res_se_s1));

    sc_trace(tf, exe2mem_din_se_s1, GET_NAME(exe2mem_din_se_s1));  // concatenation of exe_res, mem_data...etc
    sc_trace(tf, exe2mem_dout_se_s1, GET_NAME(exe2mem_dout_se_s1));

    sc_trace(tf, op1_se_s1, GET_NAME(op1_se_s1));
    sc_trace(tf, op2_se, GET_NAME(op2_se));
    sc_trace(tf, alu_in_op2_se, GET_NAME(alu_in_op2_se));
    sc_trace(tf, alu_out_se, GET_NAME(alu_out_se));
    sc_trace(tf, shifter_out_se, GET_NAME(shifter_out_se));
    sc_trace(tf, bp_mem_data_sd, GET_NAME(bp_mem_data_sd));
    sc_trace(tf, shift_val_se, GET_NAME(shift_val_se));
    sc_trace(tf, exe2mem_push_se, GET_NAME(exe2mem_push_se));
    sc_trace(tf, exe2mem_full_se, GET_NAME(exe2mem_full_se));
    sc_trace(tf, blocked, GET_NAME(blocked));

    sc_trace(tf, wb_re, GET_NAME(wb_re));
    sc_trace(tf, mem_load_re, GET_NAME(mem_load_re));
    sc_trace(tf, mem_store_re, GET_NAME(mem_store_re));
    sc_trace(tf, MULT_INST_RE_S1, GET_NAME(MULT_INST_RE_S1));
    // Exception :

    sc_trace(tf, exception_se, GET_NAME(exception_se));
    sc_trace(tf, MRET_RD_S1, GET_NAME(MRET_RD_S1));
    sc_trace(tf, MRET_RE_S1, GET_NAME(MRET_RE_S1));
    sc_trace(tf, store_access_fault_se, GET_NAME(store_access_fault_se));
    sc_trace(tf, store_adress_missaligned_se, GET_NAME(store_adress_missaligned_se));
    sc_trace(tf, INSTRUCTION_ACCESS_FAULT_RE_S1, GET_NAME(INSTRUCTION_ACCESS_FAULT_RE_S1));
    sc_trace(
        tf, load_adress_missaligned_se, GET_NAME(load_adress_missaligned_se));  // adress from store/load isn't aligned
    sc_trace(tf, load_access_fault_se, GET_NAME(load_access_fault_se));

    sc_trace(tf, stall_se, GET_NAME(stall_se));
    sc_trace(tf, r1_valid_se, GET_NAME(r1_valid_se));
    sc_trace(tf, r2_valid_se, GET_NAME(r2_valid_se));
    sc_trace(tf, MULT_INST_RM_S1, GET_NAME(MULT_INST_RM_S1));

    sc_trace(tf, OP1_SE_S1, GET_NAME(OP1_SE_S1));
    sc_trace(tf, OP2_SE_S1, GET_NAME(OP2_SE_S1));

    sc_trace(tf, BLOCK_BP_RD_S1, GET_NAME(BLOCK_BP_RD_S1));

    sc_trace(tf, MEM_DEST_RM_S2, GET_NAME(MEM_DEST_RM_S2));
    sc_trace(tf, MEM_RES_RM_S2, GET_NAME(MEM_RES_RM_S2));
    sc_trace(tf, CSR_WENABLE_RM_S2, GET_NAME(CSR_WENABLE_RM_S2));
    sc_trace(tf, CSR_RDATA_RM_S2, GET_NAME(CSR_RDATA_RM_S2));
    sc_trace(tf, EXE_RES_RE_S2, GET_NAME(EXE_RES_RE_S2));
    sc_trace(tf, DEST_RE_S2, GET_NAME(DEST_RE_S2));
    sc_trace(tf, CSR_WENABLE_RE_S2, GET_NAME(CSR_WENABLE_RE_S2));
    sc_trace(tf, MEM_LOAD_RE_S2, GET_NAME(MEM_LOAD_RE_S2));
    sc_trace(tf, CSR_RDATA_RE_S2, GET_NAME(CSR_RDATA_RE_S2));
    
    sc_trace(tf, op1_bp_re, GET_NAME(op1_bp_re));
    sc_trace(tf, op2_bp_re, GET_NAME(op2_bp_re));
    sc_trace(tf, op1_is_saved_re, GET_NAME(op1_is_saved_re));
    sc_trace(tf, op2_is_saved_re, GET_NAME(op2_is_saved_re));
    // sc_trace(tf, bp_s2_E2_is_taken, GET_NAME(bp_s2_E2_is_taken));
    alu_inst.trace(tf);
    shifter_inst.trace(tf);
    fifo_inst.trace(tf);
}

// 0000010000101100010001100011011
// 00111101000110110101100010111010
