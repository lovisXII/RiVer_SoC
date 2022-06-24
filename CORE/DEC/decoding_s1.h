#include <systemc.h>
#include "dec.h"
//---------------------------------------------INSTRUCTION TYPE DETECTION
//:---------------------------------------------

void decod::decoding_instruction_type_s1() {
    sc_uint<32> if_ir_s1   = INSTR_RI_S1.read();
    r_type_inst_sd_s1      = (if_ir_s1.range(6, 0) == 0b0110011 && if_ir_s1.range(31, 25) != 0b0000001) ? 1 : 0;
    i_type_inst_sd_s1      = (if_ir_s1.range(6, 0) == 0b0010011 | if_ir_s1.range(6, 0) == 0b0000011) ? 1 : 0;
    s_type_inst_sd_s1      = if_ir_s1.range(6, 0) == 0b0100011 ? 1 : 0;
    b_type_inst_sd_s1      = if_ir_s1.range(6, 0) == 0b1100011 ? 1 : 0;
    u_type_inst_sd_s1      = (if_ir_s1.range(6, 0) == 0b0110111 || if_ir_s1.range(6, 0) == 0b0010111) ? 1 : 0;
    j_type_inst_sd_s1      = if_ir_s1.range(6, 0) == 0b1101111 ? 1 : 0;
    jalr_type_inst_sd_s1   = if_ir_s1.range(6, 0) == 0b1100111 ? 1 : 0;
    system_type_inst_sd_s1 = if_ir_s1.range(6, 0) == 0b1110011 ? 1 : 0;
    m_type_inst_sd_s1      = (if_ir_s1.range(6, 0) == 0b0110011 && if_ir_s1.range(31, 25) == 0b0000001) ? 1 : 0;
}

//---------------------------------------------INSTRUCTION DETECTION
//:---------------------------------------------

void decod::decoding_instruction_s1() {
    sc_uint<32> if_ir_s1 = INSTR_RI_S1.read();

    // R-type Instruction :

    if (if_ir_s1.range(6, 0) == 0b0110011 && if_ir_s1.range(31, 25) == 0b0000000 && if_ir_s1.range(14, 12) == 0b000)
        add_i_sd_s1.write(1);
    else
        add_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0110011 && if_ir_s1.range(31, 25) == 0b0000000 && if_ir_s1.range(14, 12) == 0b010)
        slt_i_sd_s1.write(1);
    else
        slt_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0110011 && if_ir_s1.range(31, 25) == 0b0000000 && if_ir_s1.range(14, 12) == 0b011)
        sltu_i_sd_s1.write(1);
    else
        sltu_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0110011 && if_ir_s1.range(31, 25) == 0b0000000 && if_ir_s1.range(14, 12) == 0b111)
        and_i_sd_s1.write(1);
    else
        and_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0110011 && if_ir_s1.range(31, 25) == 0b0000000 && if_ir_s1.range(14, 12) == 0b110)
        or_i_sd_s1.write(1);
    else
        or_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0110011 && if_ir_s1.range(31, 25) == 0b0000000 && if_ir_s1.range(14, 12) == 0b100)
        xor_i_sd_s1.write(1);
    else
        xor_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0110011 && if_ir_s1.range(31, 25) == 0b0000000 && if_ir_s1.range(14, 12) == 0b001)
        sll_i_sd_s1.write(1);
    else
        sll_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0110011 && if_ir_s1.range(31, 25) == 0b0000000 && if_ir_s1.range(14, 12) == 0b101)
        srl_i_sd_s1.write(1);
    else
        srl_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0110011 && if_ir_s1.range(31, 25) == 0b0100000 && if_ir_s1.range(14, 12) == 0b000)
        sub_i_sd_s1.write(1);
    else
        sub_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0110011 && if_ir_s1.range(31, 25) == 0b0100000 && if_ir_s1.range(14, 12) == 0b101)
        sra_i_sd_s1.write(1);
    else
        sra_i_sd_s1.write(0);

    // I-type Instructions :

    if (if_ir_s1.range(6, 0) == 0b0010011 && if_ir_s1.range(14, 12) == 0b000)
        addi_i_sd_s1.write(1);
    else
        addi_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0010011 && if_ir_s1.range(14, 12) == 0b010)
        slti_i_sd_s1.write(1);
    else
        slti_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0010011 && if_ir_s1.range(14, 12) == 0b011)
        sltiu_i_sd_s1.write(1);
    else
        sltiu_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0010011 && if_ir_s1.range(14, 12) == 0b111)
        andi_i_sd_s1.write(1);
    else
        andi_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0010011 && if_ir_s1.range(14, 12) == 0b110)
        ori_i_sd_s1.write(1);
    else
        ori_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0010011 && if_ir_s1.range(14, 12) == 0b100)
        xori_i_sd_s1.write(1);
    else
        xori_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0001111 && if_ir_s1.range(14, 12) == 0b000)
        fence_i_sd_s1.write(1);
    else
        fence_i_sd_s1.write(0);

    // I-type shift instructions :

    if (if_ir_s1.range(6, 0) == 0b0010011 && if_ir_s1.range(31, 25) == 0b0000000 && if_ir_s1.range(14, 12) == 0b001)
        slli_i_sd_s1.write(1);
    else
        slli_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0010011 && if_ir_s1.range(31, 25) == 0b0000000 && if_ir_s1.range(14, 12) == 0b101)
        srli_i_sd_s1.write(1);
    else
        srli_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0010011 && if_ir_s1.range(31, 25) == 0b0100000 && if_ir_s1.range(14, 12) == 0b101)
        srai_i_sd_s1.write(1);
    else
        srai_i_sd_s1.write(0);

    // I-type load instructions :

    if (if_ir_s1.range(6, 0) == 0b0000011 && if_ir_s1.range(14, 12) == 0b010)
        lw_i_sd_s1.write(1);
    else
        lw_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0000011 && if_ir_s1.range(14, 12) == 0b001)
        lh_i_sd_s1.write(1);
    else
        lh_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0000011 && if_ir_s1.range(14, 12) == 0b101)
        lhu_i_sd_s1.write(1);
    else
        lhu_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0000011 && if_ir_s1.range(14, 12) == 0b000)
        lb_i_sd_s1.write(1);
    else
        lb_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0000011 && if_ir_s1.range(14, 12) == 0b100)
        lbu_i_sd_s1.write(1);
    else
        lbu_i_sd_s1.write(0);

    // B-type Instructions :

    if (if_ir_s1.range(6, 0) == 0b1100011 && if_ir_s1.range(14, 12) == 0b000)
        beq_i_sd_s1.write(1);
    else
        beq_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b1100011 && if_ir_s1.range(14, 12) == 0b001)
        bne_i_sd_s1.write(1);
    else
        bne_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b1100011 && if_ir_s1.range(14, 12) == 0b100)
        blt_i_sd_s1.write(1);
    else
        blt_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b1100011 && if_ir_s1.range(14, 12) == 0b101)
        bge_i_sd_s1.write(1);
    else
        bge_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b1100011 && if_ir_s1.range(14, 12) == 0b110)
        bltu_i_sd_s1.write(1);
    else
        bltu_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b1100011 && if_ir_s1.range(14, 12) == 0b111)
        bgeu_i_sd_s1.write(1);
    else
        bgeu_i_sd_s1.write(0);

    // U-type Instructions :

    if (if_ir_s1.range(6, 0) == 0b0110111)
        lui_i_sd_s1.write(1);
    else
        lui_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0010111)
        auipc_i_sd_s1.write(1);
    else
        auipc_i_sd_s1.write(0);

    // J-type Instructions :

    if (if_ir_s1.range(6, 0) == 0b1101111)
        jal_i_sd_s1.write(1);
    else
        jal_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b1100111)
        jalr_i_sd_s1.write(1);
    else
        jalr_i_sd_s1.write(0);

    // S-type Instructions :

    if (if_ir_s1.range(6, 0) == 0b0100011 && if_ir_s1.range(14, 12) == 0b010)
        sw_i_sd_s1.write(1);
    else
        sw_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0100011 && if_ir_s1.range(14, 12) == 0b001)
        sh_i_sd_s1.write(1);
    else
        sh_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0100011 && if_ir_s1.range(14, 12) == 0b000)
        sb_i_sd_s1.write(1);
    else
        sb_i_sd_s1.write(0);

    // System-type Instructions :

    if (if_ir_s1.range(6, 0) == 0b1110011 && if_ir_s1.range(14, 12) == 0b000 &&
        if_ir_s1.range(31, 20) == 0b000000000000)
        ecall_i_sd_s1.write(1);
    else
        ecall_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b1110011 && if_ir_s1.range(14, 12) == 0b000 &&
        if_ir_s1.range(31, 20) == 0b000000000001)
        ebreak_i_sd_s1.write(1);
    else
        ebreak_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b1110011 && if_ir_s1.range(14, 12) == 0b001)
        csrrw_i_sd_s1.write(1);
    else
        csrrw_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b1110011 && if_ir_s1.range(14, 12) == 0b010)
        csrrs_i_sd_s1.write(1);
    else
        csrrs_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b1110011 && if_ir_s1.range(14, 12) == 0b011)
        csrrc_i_sd_s1.write(1);
    else
        csrrc_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b1110011 && if_ir_s1.range(14, 12) == 0b101)
        csrrwi_i_sd_s1.write(1);
    else
        csrrwi_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b1110011 && if_ir_s1.range(14, 12) == 0b110)
        csrrsi_i_sd_s1.write(1);
    else
        csrrsi_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b1110011 && if_ir_s1.range(14, 12) == 0b111)
        csrrci_i_sd_s1.write(1);
    else
        csrrci_i_sd_s1.write(0);
    if (if_ir_s1 == 0b00110000001000000000000001110011)
        mret_i_sd_s1.write(1);
    else
        mret_i_sd_s1.write(0);
    if (if_ir_s1 == 0b00010000001000000000000001110011)
        sret_i_sd_s1.write(1);
    else
        sret_i_sd_s1.write(0);

    // M-type Instructions :

    if (if_ir_s1.range(6, 0) == 0b0110011 && if_ir_s1.range(14, 12) == 0b000 && if_ir_s1.range(31, 25) == 0b0000001)
        mul_i_sd_s1.write(1);
    else
        mul_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0110011 && if_ir_s1.range(14, 12) == 0b001 && if_ir_s1.range(31, 25) == 0b0000001)
        mulh_i_sd_s1.write(1);
    else
        mulh_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0110011 && if_ir_s1.range(14, 12) == 0b010 && if_ir_s1.range(31, 25) == 0b0000001)
        mulhsu_i_sd_s1.write(1);
    else
        mulhsu_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0110011 && if_ir_s1.range(14, 12) == 0b011 && if_ir_s1.range(31, 25) == 0b0000001)
        mulhu_i_sd_s1.write(1);
    else
        mulhu_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0110011 && if_ir_s1.range(14, 12) == 0b100 && if_ir_s1.range(31, 25) == 0b0000001)
        div_i_sd_s1.write(1);
    else
        div_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0110011 && if_ir_s1.range(14, 12) == 0b101 && if_ir_s1.range(31, 25) == 0b0000001)
        divu_i_sd_s1.write(1);
    else
        divu_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0110011 && if_ir_s1.range(14, 12) == 0b110 && if_ir_s1.range(31, 25) == 0b0000001)
        rem_i_sd_s1.write(1);
    else
        rem_i_sd_s1.write(0);
    if (if_ir_s1.range(6, 0) == 0b0110011 && if_ir_s1.range(14, 12) == 0b111 && if_ir_s1.range(31, 25) == 0b0000001)
        remu_i_sd_s1.write(1);
    else
        remu_i_sd_s1.write(0);
}

//---------------------------------------------REGISTRE & OPERAND DETECTION
//:---------------------------------------------
// this needs to be done in two steps :
void decod::pre_reg_read_decoding_s1() {
    sc_uint<32> if_ir_s1 = INSTR_RI_S1.read();
    sc_uint<6>  radr1_var_s1;
    sc_uint<6>  radr2_var_s1;
    sc_uint<6>  adr_dest_var_s1;

    // R-type Instruction :
    if (r_type_inst_sd_s1 == 1) {
        radr1_var_s1    = if_ir_s1.range(19, 15);
        radr2_var_s1    = if_ir_s1.range(24, 20);
        adr_dest_var_s1 = if_ir_s1.range(11, 7);
        CSR_RADR_SD_S1.write(0);
    }
    // I-type Instruction :
    else if (i_type_inst_sd_s1 == 1) {
        radr1_var_s1    = if_ir_s1.range(19, 15);
        radr2_var_s1    = 0;
        adr_dest_var_s1 = if_ir_s1.range(11, 7);
        CSR_RADR_SD_S1.write(0);
    }
    // S-type Instruction :
    else if (s_type_inst_sd_s1 == 1) {
        radr1_var_s1    = if_ir_s1.range(19, 15);
        radr2_var_s1    = if_ir_s1.range(24, 20);
        adr_dest_var_s1 = 0;
        CSR_RADR_SD_S1.write(0);
    }
    // Branch Instruction :
    else if (b_type_inst_sd_s1 == 1) {
        radr1_var_s1    = if_ir_s1.range(19, 15);
        radr2_var_s1    = if_ir_s1.range(24, 20);
        adr_dest_var_s1 = 0;
        CSR_RADR_SD_S1.write(0);
    }
    // U-type Instruction :
    else if (u_type_inst_sd_s1 == 1) {
        radr1_var_s1    = 0;
        adr_dest_var_s1 = if_ir_s1.range(11, 7);
        radr2_var_s1    = 0;
        CSR_RADR_SD_S1.write(0);
    }
    // J-type Instruction :
    else if (j_type_inst_sd_s1 == 1) {
        radr1_var_s1    = 0;
        radr2_var_s1    = 0;
        adr_dest_var_s1 = if_ir_s1.range(11, 7);
        CSR_RADR_SD_S1.write(0);
    }
    // JALR-type Instruction :
    else if (jalr_type_inst_sd_s1 == 1) {
        radr1_var_s1    = if_ir_s1.range(19, 15);
        radr2_var_s1    = 0;
        adr_dest_var_s1 = if_ir_s1.range(11, 7);
        CSR_RADR_SD_S1.write(0);
    }
    // M-type Instruction :
    else if (m_type_inst_sd_s1 == 1) {
        radr1_var_s1    = if_ir_s1.range(19, 15);
        radr2_var_s1    = if_ir_s1.range(24, 20);
        adr_dest_var_s1 = if_ir_s1.range(11, 7);
    }
    // system-type Instruction
    else if (system_type_inst_sd_s1 == 1) {
        // in CSR operation we always have :
        // rd = CSR
        // CSR = (rs1 | 0) operation CSR
        // So CSR must be wbk in rd
        if (csrrw_i_sd_s1 || csrrs_i_sd_s1 || csrrc_i_sd_s1 || csrrwi_i_sd_s1 || csrrsi_i_sd_s1 || csrrci_i_sd_s1) {
            CSR_RADR_SD_S1.write(if_ir_s1.range(31, 20));
            csr_radr_sd_s1  = if_ir_s1.range(31, 20);
            radr1_var_s1    = (csrrw_i_sd_s1 | csrrs_i_sd_s1 | csrrc_i_sd_s1) ? if_ir_s1.range(19, 15) : 0;
            radr2_var_s1    = 0;
            adr_dest_var_s1 = if_ir_s1.range(11, 7);
        } else if (ecall_i_sd_s1 || ebreak_i_sd_s1) {
            radr1_var_s1    = 0;
            radr2_var_s1    = 0;
            adr_dest_var_s1 = 0;

            // Generate the exception corresponding to the right
            // call

            if (CURRENT_MODE_SM_S1.read() == 0 && ecall_i_sd_s1) {
                env_call_u_mode_sd_s1 = 1;
                env_call_s_mode_sd_s1 = 0;
                env_call_m_mode_sd_s1 = 0;
            } else if (ecall_i_sd_s1 && CURRENT_MODE_SM_S1.read() == 3) {
                env_call_u_mode_sd_s1 = 0;
                env_call_s_mode_sd_s1 = 0;
                env_call_m_mode_sd_s1 = 1;
            } else {
                cout << "other " << sc_time_stamp() << endl;
                env_call_u_mode_sd_s1 = 0;
                env_call_s_mode_sd_s1 = 0;
                env_call_m_mode_sd_s1 = 0;
            }

        } else if (sret_i_sd_s1 || mret_i_sd_s1) {
            // xRET instruction read the current value of MEPC register
            radr1_var_s1    = 0;
            radr2_var_s1    = 0;
            adr_dest_var_s1 = 0;

            // Exception will be generate in case of mret in wrong mode

            if (mret_i_sd_s1 && CURRENT_MODE_SM_S1.read() != 3) {
                env_call_wrong_mode_s1 = 1;
            } else if (sret_i_sd_s1 && CURRENT_MODE_SM_S1.read() != 2)
                env_call_s_mode_sd_s1 = 1;
            else {
                env_call_wrong_mode_s1 = 0;
            }
        }
    }
    // Default case :
    else {
        radr1_var_s1    = 0;
        radr2_var_s1    = 0;
        adr_dest_var_s1 = 0;
    }
    if (!ecall_i_sd_s1) {
        env_call_u_mode_sd_s1 = 0;
        env_call_s_mode_sd_s1 = 0;
        env_call_m_mode_sd_s1 = 0;
    }
    if (!mret_i_sd_s1 | !sret_i_sd_s1) { env_call_wrong_mode_s1 = 0; }
    RADR1_SD_S1.write(radr1_var_s1);
    RADR2_SD_S1.write(radr2_var_s1);
    adr_dest_sd_s1.write(adr_dest_var_s1);
}

//---------------------------------------------EXE & MEM SIGNAL DETECTION
//:---------------------------------------------

void decod::post_reg_read_decoding_s1() {
    // We are going to setup commands sent to EXE here, so each if will be
    // execute for one type of command :

    // CMD : +
    bool        dec2exe_wb_var_s1;
    sc_uint<32> dec2exe_op1_var_s1;
    sc_uint<32> dec2exe_op2_var_s1;
    sc_uint<32> if_ir_s1 = INSTR_RI_S1.read();
    sc_uint<32> mem_data_var_s1;
    sc_uint<32> offset_branch_var_s1;
    bool        not_jump_var_s1;
    bool        illegal_inst_s1 = false;

    // R-type Instruction :

    if (r_type_inst_sd_s1 || i_type_inst_sd_s1 || u_type_inst_sd_s1 || m_type_inst_sd_s1) {
        csr_wenable_sd_s1.write(0);
        if (i_type_inst_sd_s1) {
            dec2exe_op1_var_s1 = (rdata1_sd_s1.read());
            if (if_ir_s1.range(31, 31) == 1) {
                dec2exe_op2_var_s1.range(31, 12) = 0b11111111111111111111;
            } else {
                dec2exe_op2_var_s1.range(31, 12) = 0b00000000000000000000;
            }
            dec2exe_op2_var_s1.range(11, 0) = if_ir_s1.range(31, 20);
        } else if (u_type_inst_sd_s1) {
            dec2exe_op1_var_s1.range(31, 12) = if_ir_s1.range(31, 12);
            dec2exe_op1_var_s1.range(11, 0)  = 0;
            if (auipc_i_sd_s1)
                dec2exe_op2_var_s1 = PC_IF2DEC_RI_S1.read();
            else
                dec2exe_op2_var_s1 = rdata2_sd_s1.read();
        } else {
            dec2exe_op1_var_s1 = (rdata1_sd_s1.read());
            dec2exe_op2_var_s1 = (rdata2_sd_s1.read());
        }

        if (lw_i_sd_s1 | lh_i_sd_s1 | lhu_i_sd_s1 | lb_i_sd_s1 | lbu_i_sd_s1) {
            mem_load_sd_s1.write(1);
            if (lw_i_sd_s1) {
                mem_size_sd_s1.write(0);
                mem_sign_extend_sd_s1.write(0);
            } else if (lh_i_sd_s1 | lhu_i_sd_s1) {
                mem_size_sd_s1.write(1);
                if (lhu_i_sd_s1)
                    mem_sign_extend_sd_s1.write(0);
                else
                    mem_sign_extend_sd_s1.write(1);
            } else if (lb_i_sd_s1 | lbu_i_sd_s1) {
                mem_size_sd_s1.write(2);
                if (lbu_i_sd_s1)
                    mem_sign_extend_sd_s1.write(0);
                else
                    mem_sign_extend_sd_s1.write(1);
            }
        } else {
            mem_load_sd_s1.write(0);
            mem_size_sd_s1.write(0);
            mem_sign_extend_sd_s1.write(0);
        }
        mem_store_sd_s1.write(0);
        dec2exe_wb_var_s1    = 1;
        mem_data_var_s1      = 0;
        offset_branch_var_s1 = 0;
        not_jump_var_s1      = 1;

        // sub
        exe_neg_op2_sd_s1.write(sub_i_sd_s1 | slt_i_sd_s1 | slti_i_sd_s1 | sltu_i_sd_s1 | sltiu_i_sd_s1);

        // Command for exe
        if (and_i_sd_s1 || andi_i_sd_s1 || srl_i_sd_s1 || srli_i_sd_s1 || mul_i_sd_s1 || div_i_sd_s1)
            exe_cmd_sd_s1.write(1);
        else if (or_i_sd_s1 || ori_i_sd_s1 || sra_i_sd_s1 || srai_i_sd_s1 || mulh_i_sd_s1 || mulhsu_i_sd_s1 ||
                 mulhu_i_sd_s1 || divu_i_sd_s1)
            exe_cmd_sd_s1.write(2);
        else if (xor_i_sd_s1 || xori_i_sd_s1 || rem_i_sd_s1)
            exe_cmd_sd_s1.write(3);
        else
            exe_cmd_sd_s1.write(0);

        if (div_i_sd_s1 || divu_i_sd_s1 || rem_i_sd_s1 || remu_i_sd_s1)
            select_type_operations_sd_s1.write(0b1000);
        else if (mul_i_sd_s1 || mulh_i_sd_s1 || mulhsu_i_sd_s1 || mulhu_i_sd_s1)
            select_type_operations_sd_s1.write(0b0100);
        else if (sll_i_sd_s1 || slli_i_sd_s1 || srl_i_sd_s1 || srli_i_sd_s1 || sra_i_sd_s1 || srai_i_sd_s1)
            select_type_operations_sd_s1.write(0b0010);
        else
            select_type_operations_sd_s1.write(0b0001);
    } else if (s_type_inst_sd_s1) {
        csr_wenable_sd_s1.write(0);
        exe_cmd_sd_s1.write(0);
        select_type_operations_sd_s1.write(0b0001);
        exe_neg_op2_sd_s1.write(0);
        mem_load_sd_s1.write(0);
        offset_branch_var_s1 = 0;
        not_jump_var_s1      = 1;
        dec2exe_op1_var_s1   = rdata1_sd_s1.read();

        // The adress is obtained by adding rs1 to the 12 bit immediat sign
        // extended
        // rs2 is copied to the memory
        if (if_ir_s1.range(31, 31) == 1) {
            dec2exe_op2_var_s1.range(31, 12) = 0b11111111111111111111;
        } else {
            dec2exe_op2_var_s1.range(31, 12) = 0b00000000000000000000;
        }
        dec2exe_op2_var_s1.range(11, 5) = if_ir_s1.range(31, 25);
        dec2exe_op2_var_s1.range(4, 0)  = if_ir_s1.range(11, 7);

        mem_data_var_s1 = rdata2_sd_s1.read();

        // STORE :

        if (sw_i_sd_s1 | sh_i_sd_s1 | sb_i_sd_s1) {
            mem_store_sd_s1.write(1);
            if (sw_i_sd_s1) {
                mem_size_sd_s1.write(0);
            } else if (sh_i_sd_s1) {
                mem_size_sd_s1.write(1);
            } else if (sb_i_sd_s1) {
                mem_size_sd_s1.write(2);
            }
        } else {
            mem_size_sd_s1.write(0);
            mem_store_sd_s1.write(0);
        }
    } else if (b_type_inst_sd_s1 == 1) {
        csr_wenable_sd_s1.write(0);
        mem_load_sd_s1.write(0);
        mem_store_sd_s1.write(0);
        mem_sign_extend_sd_s1.write(0);
        mem_size_sd_s1.write(0);
        dec2exe_wb_var_s1 = 1;
        mem_data_var_s1   = 0;
        exe_cmd_sd_s1.write(0);
        select_type_operations_sd_s1.write(0b0001);
        exe_neg_op2_sd_s1.write(0);
        // Offset must be *4, so he's shift by 2 on the left

        if (if_ir_s1.range(31, 31) == 1) {
            offset_branch_var_s1.range(31, 13) = 0b1111111111111111111;
        } else {
            offset_branch_var_s1.range(31, 13) = 0b0000000000000000000;
        }
        offset_branch_var_s1.range(12, 12) = if_ir_s1.range(31, 31);
        offset_branch_var_s1.range(11, 11) = if_ir_s1.range(7, 7);
        offset_branch_var_s1.range(10, 5)  = if_ir_s1.range(30, 25);
        offset_branch_var_s1.range(4, 1)   = if_ir_s1.range(11, 8);
        offset_branch_var_s1.range(0, 0)   = 0;

        /*BRANCH CONDITION GESTION : */

        dec2exe_op1_var_s1 = rdata1_sd_s1.read();
        dec2exe_op2_var_s1 = rdata2_sd_s1.read();
        sc_uint<33> res_comparaison;
        res_comparaison     = dec2exe_op1_var_s1 - dec2exe_op2_var_s1;
        bool different_sign = dec2exe_op1_var_s1[31] ^ dec2exe_op2_var_s1[31];
        bool equal          = res_comparaison == 0;
        bool branch;
        if (bne_i_sd_s1.read()) {
            branch = !equal;
        } else if (beq_i_sd_s1.read()) {
            branch = equal;
        } else if (blt_i_sd_s1.read()) {
            if (different_sign)
                branch = dec2exe_op1_var_s1[31];  // branch if the first number is negative
            else
                branch = res_comparaison[31];  // branch if the result is negative
        } else if (bltu_i_sd_s1.read()) {
            if (different_sign)
                branch = dec2exe_op2_var_s1[31];  // branch if the second number is bigger
            else
                branch = res_comparaison[31];  // branch if the result is negative
        } else if (bge_i_sd_s1.read()) {
            if (different_sign)
                branch = dec2exe_op2_var_s1[31];  // branch if the second number is negative
            else
                branch = !res_comparaison[31];  // branch if the result is positive
        } else if (bgeu_i_sd_s1.read()) {
            if (different_sign)
                branch = dec2exe_op1_var_s1[31];  // branch if the second number is negative
            else
                branch = !res_comparaison[31];  // branch if the result is positive
        }
        not_jump_var_s1 = !branch;
    }

    else if (jalr_type_inst_sd_s1.read() || j_type_inst_sd_s1.read()) {
        csr_wenable_sd_s1.write(0);
        exe_cmd_sd_s1.write(0);
        exe_neg_op2_sd_s1.write(0);
        dec2exe_wb_var_s1 = 1;
        mem_load_sd_s1.write(0);
        mem_store_sd_s1.write(0);
        mem_sign_extend_sd_s1.write(0);
        mem_size_sd_s1.write(0);
        select_type_operations_sd_s1.write(1);
        mem_data_var_s1 = 0;
        if (jalr_type_inst_sd_s1) {
            dec2exe_op1_var_s1 = READ_PC_SR.read();
            dec2exe_op2_var_s1 = 0x0;

            if (if_ir_s1.range(31, 31) == 1) {
                offset_branch_var_s1.range(31, 12) = 0b11111111111111111111;
            } else {
                offset_branch_var_s1.range(31, 12) = 0b00000000000000000000;
            }
            offset_branch_var_s1.range(11, 0) = if_ir_s1.range(31, 20);
            offset_branch_var_s1 += rdata1_sd_s1.read() - READ_PC_SR.read() + 4;
            offset_branch_var_s1.range(0, 0) = 0;
            mem_data_var_s1                  = 0;
            not_jump_var_s1                  = 0;
        } else {
            dec2exe_op1_var_s1 = READ_PC_SR.read();
            dec2exe_op2_var_s1 = 0x0;  // on va envoyer l'adresse de retour

            if (if_ir_s1.range(31, 31) == 1) {
                offset_branch_var_s1.range(31, 21) = 0b11111111111;
            } else {
                offset_branch_var_s1.range(31, 21) = 0b00000000000;
            }
            offset_branch_var_s1.range(20, 20) = if_ir_s1.range(31, 31);
            offset_branch_var_s1.range(19, 12) = if_ir_s1.range(19, 12);
            offset_branch_var_s1.range(11, 11) = if_ir_s1.range(20, 20);
            offset_branch_var_s1.range(10, 1)  = if_ir_s1.range(30, 21);
            offset_branch_var_s1.range(0, 0)   = 0;
            mem_data_var_s1                    = 0;
            not_jump_var_s1                    = 0;
        }
    } else if (system_type_inst_sd_s1 == 1) {
        mem_load_sd_s1.write(0);
        mem_store_sd_s1.write(0);
        mem_sign_extend_sd_s1.write(0);
        mem_size_sd_s1.write(0);
        select_type_operations_sd_s1.write(1);
        // in CSR operation we always have :
        // rd = CSR
        // CSR = (rs1 | 0) operation CSR
        // So CSR must be wbk in rd
        if (csrrw_i_sd_s1 || csrrs_i_sd_s1 || csrrc_i_sd_s1 || csrrwi_i_sd_s1 || csrrsi_i_sd_s1 || csrrci_i_sd_s1) {
            csr_wenable_sd_s1.write(1);
            sc_uint<32> rdata1_signal_sd = rdata1_sd_s1;  // loading value of rs1
            if (csrrc_i_sd_s1 || csrrci_i_sd_s1) {
                exe_neg_op2_sd_s1.write(0);
                exe_cmd_sd_s1.write(1);
            } else if (csrrw_i_sd_s1 || csrrwi_i_sd_s1) {
                exe_neg_op2_sd_s1.write(0);
                exe_cmd_sd_s1.write(0);
            } else {
                exe_neg_op2_sd_s1.write(0);
                exe_cmd_sd_s1.write(2);
            }

            // We put rdata1 or immediat value in rs1

            if (csrrw_i_sd_s1 | csrrc_i_sd_s1 | csrrs_i_sd_s1) {
                if (!csrrc_i_sd_s1)
                    dec2exe_op1_var_s1 = rdata1_sd_s1;
                else
                    dec2exe_op1_var_s1 = ~rdata1_sd_s1.read();
            } else {
                if (!csrrci_i_sd_s1)
                    dec2exe_op1_var_s1 = if_ir_s1.range(19, 15);
                else
                    dec2exe_op1_var_s1 = ~if_ir_s1.range(19, 15);
            }

            // We put CSR value or 0 inside rs2
            // If instruction are write one, the value of rs1 just
            // erase what's inside the csr
            if (csrrs_i_sd_s1 || csrrc_i_sd_s1 || csrrsi_i_sd_s1 || csrrci_i_sd_s1) {
                dec2exe_op2_var_s1 = CSR_RDATA_SC_S1;
            } else {
                dec2exe_op2_var_s1 = 0;
            }

            dec2exe_wb_var_s1    = 1;
            offset_branch_var_s1 = 0;
            mem_data_var_s1      = 0;
            not_jump_var_s1      = 1;

        } else if (ecall_i_sd_s1 || ebreak_i_sd_s1) {
            csr_wenable_sd_s1.write(0);
            exe_cmd_sd_s1.write(0);
            dec2exe_op1_var_s1 = 0;
            dec2exe_op2_var_s1 = 0;
            exe_neg_op2_sd_s1.write(0);
            dec2exe_wb_var_s1 = 0;
            mem_load_sd_s1.write(0);
            mem_store_sd_s1.write(0);
            mem_sign_extend_sd_s1.write(0);
            mem_size_sd_s1.write(0);
            offset_branch_var_s1 = 0;
            mem_data_var_s1      = 0;
            not_jump_var_s1      = 1;
        } else if (sret_i_sd_s1 || mret_i_sd_s1) {
            csr_wenable_sd_s1.write(0);
            exe_cmd_sd_s1.write(0);
            dec2exe_op1_var_s1 = 0;
            dec2exe_op2_var_s1 = 0;
            exe_neg_op2_sd_s1.write(0);
            dec2exe_wb_var_s1 = 0;
            mem_load_sd_s1.write(0);
            mem_store_sd_s1.write(0);
            mem_sign_extend_sd_s1.write(0);
            mem_size_sd_s1.write(0);
            select_type_operations_sd_s1.write(1);
            offset_branch_var_s1 = 0;
            mem_data_var_s1      = 0;
            not_jump_var_s1      = 1;
        }
    } else if (fence_i_sd_s1) {
        csr_wenable_sd_s1.write(0);
        exe_cmd_sd_s1.write(0);
        dec2exe_op1_var_s1 = 0;
        dec2exe_op2_var_s1 = 0;
        exe_neg_op2_sd_s1.write(0);
        dec2exe_wb_var_s1 = 0;
        mem_load_sd_s1.write(0);
        mem_store_sd_s1.write(0);
        mem_sign_extend_sd_s1.write(0);
        mem_size_sd_s1.write(0);
        select_type_operations_sd_s1.write(0b0001);
        offset_branch_var_s1 = 0;
        mem_data_var_s1      = 0;
        not_jump_var_s1      = 1;
    } else {
        csr_wenable_sd_s1.write(0);
        exe_cmd_sd_s1.write(0);
        dec2exe_op1_var_s1 = 0;
        dec2exe_op2_var_s1 = 0;
        exe_neg_op2_sd_s1.write(0);
        dec2exe_wb_var_s1 = 0;
        mem_load_sd_s1.write(0);
        mem_store_sd_s1.write(0);
        mem_sign_extend_sd_s1.write(0);
        mem_size_sd_s1.write(0);
        select_type_operations_sd_s1.write(0b0001);
        illegal_inst_s1 = true;
    }

    // Illegal instruction Gestion :

    if (r_type_inst_sd_s1) {
        if (if_ir_s1.range(31, 25) != 0 && if_ir_s1.range(31, 25) != 0b0100000) { illegal_inst_s1 = true; }
    } else if (b_type_inst_sd_s1) {
        if (if_ir_s1.range(14, 12) == 2 || if_ir_s1.range(14, 12) == 3) illegal_inst_s1 = true;
    }

    else if (s_type_inst_sd_s1) {
        if (if_ir_s1.range(14, 12) > 2) illegal_inst_s1 = true;
    } else if (system_type_inst_sd_s1)
        if (if_ir_s1.range(14, 12) == 4) illegal_inst_s1 = true;

    illegal_inst_s1 = illegal_inst_s1 && !IF2DEC_EMPTY_SI_S1.read();

    illegal_instruction_sd_s1.write(illegal_inst_s1);
    block_bp_sd_s1.write(jalr_type_inst_sd_s1);
    exe_wb_sd_s1.write(dec2exe_wb_var_s1);
    offset_branch_sd_s1.write(offset_branch_var_s1);
    exe_op1_sd_s1.write(dec2exe_op1_var_s1);
    exe_op2_sd_s1.write(dec2exe_op2_var_s1);
    mem_data_sd_s1.write(mem_data_var_s1);
    // inc_pc_sd_s1.write(((inc_pc_var || IF2DEC_EMPTY_SI_S1) && dec2if_push_sd.read()) && !EXCEPTION_SM);
    // add_offset_to_pc_sd.write((!stall_sd_s1 && !inc_pc_var && (b_type_inst_sd_s1 || j_type_inst_sd_s1
    //                             || jalr_type_inst_sd_s1) &&
    //                            dec2if_push_sd.read() && !illegal_inst_s1 && !IF2DEC_EMPTY_SI_S1) &&
    //                           !EXCEPTION_SM);
    jump_sd_s1 = !not_jump_var_s1;
}
