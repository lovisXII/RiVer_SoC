#include "dec.h"

// ---------------------------------------------METHODS FOR DEC2IF GESTION
// :---------------------------------------------

void decod::dec2if_gestion() {
    dec2if_push_sd.write(!dec2if_full_sd);
    DEC2IF_EMPTY_SD.write(dec2if_empty_sd.read());
}

// ---------------------------------------------METHODS FOR IF2DEC GESTION
// :---------------------------------------------

void decod::if2dec_pop_method() {
    if (add_offset_to_pc_sd.read()) {
        IF2DEC_POP_SD.write(1);
        IF2DEC_FLUSH_SD.write(1);
    } else if (!stall && !IF2DEC_EMPTY_SI.read() && !dec2exe_full_sd.read()) {
        IF2DEC_POP_SD.write(1);
        IF2DEC_FLUSH_SD.write(0);
    } else {
        IF2DEC_POP_SD.write(0);
        IF2DEC_FLUSH_SD.write(0);
    }
}

// ---------------------------------------------METHODS FOR DEC2EXE GESTION
// :---------------------------------------------

void decod::dec2exe_push_method() {
    if (stall || dec2exe_full_sd.read() || IF2DEC_EMPTY_SI.read()) {
        dec2exe_push_sd.write(0);
    } else {
        dec2exe_push_sd.write(1);
    }
}

void decod::concat_dec2exe() {
    sc_bv<173> dec2exe_in_var;

    dec2exe_in_var[172]            = csr_type_operation_rd.read();
    dec2exe_in_var.range(171, 160) = adr_csr_sd.read();
    dec2exe_in_var.range(159, 128) = PC_IF2DEC_RI.read();
    dec2exe_in_var[127]            = r1_valid_sd.read();
    dec2exe_in_var[126]            = r2_valid_sd.read();
    dec2exe_in_var.range(125, 120) = RADR1_SD.read();
    dec2exe_in_var.range(119, 114) = RADR2_SD.read();
    dec2exe_in_var.range(113, 112) = exe_cmd_sd.read();
    dec2exe_in_var.range(111, 80)  = exe_op1_sd.read();
    dec2exe_in_var.range(79, 48)   = exe_op2_sd.read();
    dec2exe_in_var[47]             = exe_neg_op2_sd.read();
    dec2exe_in_var[46]             = exe_wb_sd.read();

    dec2exe_in_var.range(45, 14) = mem_data_sd.read();

    dec2exe_in_var[13] = mem_load_sd.read();
    dec2exe_in_var[12] = mem_store_sd.read();

    dec2exe_in_var[11]          = mem_sign_extend_sd.read();
    dec2exe_in_var.range(10, 9) = mem_size_sd.read();
    dec2exe_in_var[8]           = select_shift_sd.read();
    dec2exe_in_var.range(7, 2)  = adr_dest_sd.read();
    dec2exe_in_var[1]           = slt_i_sd.read() | slti_i_sd.read();
    dec2exe_in_var[0]           = sltu_i_sd.read() | sltiu_i_sd.read();

    dec2exe_in_sd.write(dec2exe_in_var);
}

void decod::unconcat_dec2exe() {
    sc_bv<172> dec2exe_out_var = dec2exe_out_sd.read();

    CSR_type_operation_RD.write((bool)dec2exe_out_var[172]);
    ADR_CSR_SD.write((sc_bv_base)dec2exe_out_var.range(171, 160));
    PC_DEC2EXE_RD.write((sc_bv_base)dec2exe_out_var.range(159, 128));
    BP_R1_VALID_RD.write((bool)dec2exe_out_var[127]);
    BP_R2_VALID_RD.write((bool)dec2exe_out_var[126]);

    BP_RADR1_RD.write((sc_bv_base)dec2exe_out_var.range(125, 120));
    BP_RADR2_RD.write((sc_bv_base)dec2exe_out_var.range(119, 114));

    EXE_CMD_RD.write((sc_bv_base)dec2exe_out_var.range(113, 112));
    OP1_RD.write((sc_bv_base)dec2exe_out_var.range(111, 80));
    OP2_RD.write((sc_bv_base)dec2exe_out_var.range(79, 48));
    NEG_OP2_RD.write((bool)dec2exe_out_var[47]);
    WB_RD.write((bool)dec2exe_out_var[46]);

    MEM_DATA_RD.write((sc_bv_base)dec2exe_out_var.range(45, 14));

    MEM_LOAD_RD.write((bool)dec2exe_out_var[13]);
    MEM_STORE_RD.write((bool)dec2exe_out_var[12]);

    MEM_SIGN_EXTEND_RD.write((bool)dec2exe_out_var[11]);
    MEM_SIZE_RD.write((sc_bv_base)dec2exe_out_var.range(10, 9));
    SELECT_SHIFT_RD.write((bool)dec2exe_out_var[8]);
    EXE_DEST_SD.write((sc_bv_base)dec2exe_out_var.range(7, 2));
    SLT_RD.write((bool)dec2exe_out_var[1]);
    SLTU_RD.write((bool)dec2exe_out_var[0]);
}

//---------------------------------------------INSTRUCTION TYPE DETECTION
//:---------------------------------------------

void decod::decoding_instruction_type() {
    sc_uint<32> if_ir   = INSTR_RI.read();
    r_type_inst_sd      = if_ir.range(6, 0) == 0b0110011 ? 1 : 0;
    i_type_inst_sd      = (if_ir.range(6, 0) == 0b0010011 | if_ir.range(6, 0) == 0b0000011) ? 1 : 0;
    s_type_inst_sd      = if_ir.range(6, 0) == 0b0100011 ? 1 : 0;
    b_type_inst_sd      = if_ir.range(6, 0) == 0b1100011 ? 1 : 0;
    u_type_inst_sd      = if_ir.range(6, 0) == 0b0110111 ? 1 : 0;
    j_type_inst_sd      = if_ir.range(6, 0) == 0b1101111 ? 1 : 0;
    jalr_type_inst_sd   = if_ir.range(6, 0) == 0b1100111 ? 1 : 0;
    system_type_inst_sd = if_ir.range(6, 0) == 0b1110011 ? 1 : 0;
}

//---------------------------------------------INSTRUCTION DETECTION
//:---------------------------------------------

void decod::decoding_instruction() {
    sc_uint<32> if_ir = INSTR_RI.read();

    // R-type Instruction :

    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000000 && if_ir.range(14, 12) == 0b000)
        add_i_sd.write(1);
    else
        add_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000000 && if_ir.range(14, 12) == 0b010)
        slt_i_sd.write(1);
    else
        slt_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000000 && if_ir.range(14, 12) == 0b011)
        sltu_i_sd.write(1);
    else
        sltu_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000000 && if_ir.range(14, 12) == 0b111)
        and_i_sd.write(1);
    else
        and_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000000 && if_ir.range(14, 12) == 0b110)
        or_i_sd.write(1);
    else
        or_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000000 && if_ir.range(14, 12) == 0b100)
        xor_i_sd.write(1);
    else
        xor_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000000 && if_ir.range(14, 12) == 0b001)
        sll_i_sd.write(1);
    else
        sll_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000000 && if_ir.range(14, 12) == 0b101)
        srl_i_sd.write(1);
    else
        srl_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0100000 && if_ir.range(14, 12) == 0b000)
        sub_i_sd.write(1);
    else
        sub_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0100000 && if_ir.range(14, 12) == 0b101)
        sra_i_sd.write(1);
    else
        sra_i_sd.write(0);

    // I-type Instructions :

    if (if_ir.range(6, 0) == 0b0010011 && if_ir.range(14, 12) == 0b000)
        addi_i_sd.write(1);
    else
        addi_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0010011 && if_ir.range(14, 12) == 0b010)
        slti_i_sd.write(1);
    else
        slti_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0010011 && if_ir.range(14, 12) == 0b011)
        sltiu_i_sd.write(1);
    else
        sltiu_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0010011 && if_ir.range(14, 12) == 0b111)
        andi_i_sd.write(1);
    else
        andi_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0010011 && if_ir.range(14, 12) == 0b110)
        ori_i_sd.write(1);
    else
        ori_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0010011 && if_ir.range(14, 12) == 0b100)
        xori_i_sd.write(1);
    else
        xori_i_sd.write(0);

    // I-type shift instructions :

    if (if_ir.range(6, 0) == 0b0010011 && if_ir.range(31, 25) == 0b0000000 && if_ir.range(14, 12) == 0b001)
        slli_i_sd.write(1);
    else
        slli_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0010011 && if_ir.range(31, 25) == 0b0000000 && if_ir.range(14, 12) == 0b101)
        srli_i_sd.write(1);
    else
        srli_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0010011 && if_ir.range(31, 25) == 0b0100000 && if_ir.range(14, 12) == 0b101)
        srai_i_sd.write(1);
    else
        srai_i_sd.write(0);

    // I-type load instructions :

    if (if_ir.range(6, 0) == 0b0000011 && if_ir.range(14, 12) == 0b010)
        lw_i_sd.write(1);
    else
        lw_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0000011 && if_ir.range(14, 12) == 0b001)
        lh_i_sd.write(1);
    else
        lh_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0000011 && if_ir.range(14, 12) == 0b101)
        lhu_i_sd.write(1);
    else
        lhu_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0000011 && if_ir.range(14, 12) == 0b000)
        lb_i_sd.write(1);
    else
        lb_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0000011 && if_ir.range(14, 12) == 0b100)
        lbu_i_sd.write(1);
    else
        lbu_i_sd.write(0);

    // B-type Instructions :

    if (if_ir.range(6, 0) == 0b1100011 && if_ir.range(14, 12) == 0b000)
        beq_i_sd.write(1);
    else
        beq_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1100011 && if_ir.range(14, 12) == 0b001)
        bne_i_sd.write(1);
    else
        bne_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1100011 && if_ir.range(14, 12) == 0b100)
        blt_i_sd.write(1);
    else
        blt_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1100011 && if_ir.range(14, 12) == 0b101)
        bge_i_sd.write(1);
    else
        bge_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1100011 && if_ir.range(14, 12) == 0b110)
        bltu_i_sd.write(1);
    else
        bltu_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1100011 && if_ir.range(14, 12) == 0b111)
        bgeu_i_sd.write(1);
    else
        bgeu_i_sd.write(0);

    // U-type Instructions :

    if (if_ir.range(6, 0) == 0b0110111)
        lui_i_sd.write(1);
    else
        lui_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0010111)
        auipc_i_sd.write(1);
    else
        auipc_i_sd.write(0);

    // J-type Instructions :

    if (if_ir.range(6, 0) == 0b1101111)
        jal_i_sd.write(1);
    else
        jal_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1100111)
        jalr_i_sd.write(1);
    else
        jalr_i_sd.write(0);

    // S-type Instructions :

    if (if_ir.range(6, 0) == 0b0100011 && if_ir.range(14, 12) == 0b010)
        sw_i_sd.write(1);
    else
        sw_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0100011 && if_ir.range(14, 12) == 0b001)
        sh_i_sd.write(1);
    else
        sh_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0100011 && if_ir.range(14, 12) == 0b000)
        sb_i_sd.write(1);
    else
        sb_i_sd.write(0);

    // System-type Instructions :

    if (if_ir.range(6, 0) == 0b1110011 && if_ir.range(14, 12) == 0b000 && if_ir.range(31, 20) == 0b000000000000)
        ecall_i_sd.write(1);
    else
        ecall_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1110011 && if_ir.range(14, 12) == 0b000 && if_ir.range(31, 20) == 0b000000000001)
        ebreak_i_sd.write(1);
    else
        ebreak_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1110011 && if_ir.range(14, 12) == 0b001)
        csrrw_i_sd.write(1);
    else
        csrrw_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1110011 && if_ir.range(14, 12) == 0b001)
        csrrs_i_sd.write(1);
    else
        csrrs_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1110011 && if_ir.range(14, 12) == 0b011)
        csrrc_i_sd.write(1);
    else
        csrrc_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1110011 && if_ir.range(14, 12) == 0b101)
        csrrwi_i_sd.write(1);
    else
        csrrwi_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1110011 && if_ir.range(14, 12) == 0b110)
        csrrsi_i_sd.write(1);
    else
        csrrsi_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1110011 && if_ir.range(14, 12) == 0b111)
        csrrci_i_sd.write(1);
    else
        csrrci_i_sd.write(0);
}

//---------------------------------------------REGISTRE & OPERAND DETECTION
//:---------------------------------------------
// this needs to be done in two steps :
void decod::pre_reg_read_decoding() {
    sc_uint<32> if_ir = INSTR_RI.read();
    sc_uint<6>  radr1_var;
    sc_uint<6>  radr2_var;
    sc_uint<6>  adr_dest_var;
    sc_uint<32> dec2exe_op1_var;
    sc_uint<32> dec2exe_op2_var;
    sc_uint<32> offset_branch_var;
    sc_uint<32> mem_data_var;
    bool        inc_pc_var;
    bool        invalid_instr = false;

    // R-type Instruction :

    if (r_type_inst_sd == 1) {
        radr1_var    = if_ir.range(19, 15);
        radr2_var    = if_ir.range(24, 20);
        adr_dest_var = if_ir.range(11, 7);

        dec2exe_op1_var = (rdata1_sd.read());
        dec2exe_op2_var = (rdata2_sd.read());

        offset_branch_var = 0;
        mem_data_var      = 0;
        inc_pc_var        = 1;
    }

    // I-type Instruction :

    else if (i_type_inst_sd == 1) {
        radr1_var    = if_ir.range(19, 15);
        radr2_var    = 0;
        adr_dest_var = if_ir.range(11, 7);

        dec2exe_op1_var = rdata1_sd.read();

        // OP2 is sign extended
        if (if_ir.range(31, 31) == 1) {
            dec2exe_op2_var.range(31, 12) = 0b11111111111111111111;
        } else {
            dec2exe_op2_var.range(31, 12) = 0b00000000000000000000;
        }
        dec2exe_op2_var.range(11, 0) = if_ir.range(31, 20);

        offset_branch_var = 0;
        mem_data_var      = 0;
        inc_pc_var        = 1;

    }

    // S-type Instruction :

    else if (s_type_inst_sd == 1) {
        // The adress is obtained by adding rs1 to the 12 bit immediat sign
        // extended
        // rs2 is copied to the memory

        radr1_var    = if_ir.range(19, 15);
        radr2_var    = if_ir.range(24, 20);
        adr_dest_var = 0;

        dec2exe_op1_var = rdata1_sd.read();

        if (if_ir.range(31, 31) == 1) {
            dec2exe_op2_var.range(31, 12) = 0b11111111111111111111;
        } else {
            dec2exe_op2_var.range(31, 12) = 0b00000000000000000000;
        }
        dec2exe_op2_var.range(11, 5) = if_ir.range(31, 25);
        dec2exe_op2_var.range(4, 0)  = if_ir.range(11, 7);

        offset_branch_var = 0;

        mem_data_var = rdata2_sd.read();
        inc_pc_var   = 1;

    }

    // Branch Instruction :

    else if (b_type_inst_sd == 1) {
        radr1_var = if_ir.range(19, 15);
        radr2_var = if_ir.range(24, 20);

        adr_dest_var = 0;

        dec2exe_op1_var = rdata1_sd.read();
        dec2exe_op2_var = rdata2_sd.read();

        // Offset must be *4, so he's shift by 2 on the left

        if (if_ir.range(31, 31) == 1) {
            offset_branch_var.range(31, 13) = 0b1111111111111111111;
        } else {
            offset_branch_var.range(31, 13) = 0b0000000000000000000;
        }
        offset_branch_var.range(12, 12) = if_ir.range(31, 31);
        offset_branch_var.range(11, 11) = if_ir.range(7, 7);
        offset_branch_var.range(10, 5)  = if_ir.range(30, 25);
        offset_branch_var.range(4, 1)   = if_ir.range(11, 8);
        offset_branch_var.range(0, 0)   = 0;
        mem_data_var                    = 0;

        /*BRANCH CONDITION GESTION : */

        sc_uint<32> res = dec2exe_op1_var ^ dec2exe_op2_var;
        sc_uint<33> res_comparaison;
        res_comparaison = dec2exe_op1_var - dec2exe_op2_var;

        if (bne_i_sd.read()) {
            inc_pc_var = ((res == 0x0 ? 1 : 0));
        } else if (beq_i_sd.read()) {
            inc_pc_var = ((res == 0x0 ? 0 : 1));
        } else if (blt_i_sd.read()) {
            inc_pc_var = ((res_comparaison.range(32, 32) == 1 | res_comparaison.range(31, 31) == 1) ? 0 : 1);  // if bit 31 == 1, it means rs1 < rs2
        } else if (bltu_i_sd.read()) {
            inc_pc_var = ((res_comparaison.range(32, 32) == 1 | res_comparaison.range(31, 31) == 1) ? 1 : 0);
        } else if (bge_i_sd.read()) {
            inc_pc_var = ((res_comparaison.range(32, 32) == 0 && res_comparaison.range(31, 31) == 0) ? 0 : 1);  // if bit 31 == 1, it means rs1 < rs2
        } else if (bgeu_i_sd.read()) {
            inc_pc_var = ((res_comparaison.range(32, 32) == 0 && res_comparaison.range(31, 31) == 0) ? 1 : 0);
        }

    }

    // U-type Instruction :

    else if (u_type_inst_sd == 1) {
        radr1_var    = 0;
        adr_dest_var = if_ir.range(11, 7);

        dec2exe_op1_var.range(31, 12) = if_ir.range(31, 12);
        dec2exe_op1_var.range(11, 0)  = 0;

        offset_branch_var = 0;

        if (auipc_i_sd == 1)  // on case of an auipc instruction we need to send PC+imm to rd
                              // so we need to get the value from r33
        {
            radr2_var       = 0x2F;
            dec2exe_op2_var = rdata2_sd.read();
        } else  // if we don't do an auipc instruction
        {
            radr2_var       = 0;
            dec2exe_op2_var = 0;
        }
        mem_data_var = 0;
        inc_pc_var   = 1;
    }

    // J-type Instruction :

    else if (j_type_inst_sd == 1) {
        radr1_var    = 0;
        radr2_var    = 0;
        adr_dest_var = if_ir.range(11, 7);

        dec2exe_op1_var = READ_PC_SR.read();
        dec2exe_op2_var = 0x0;  // on va envoyer l'adresse de retour

        if (if_ir.range(31, 31) == 1) {
            offset_branch_var.range(31, 21) = 0b11111111111;
        } else {
            offset_branch_var.range(31, 21) = 0b00000000000;
        }
        offset_branch_var.range(20, 20) = if_ir.range(31, 31);
        offset_branch_var.range(19, 12) = if_ir.range(19, 12);
        offset_branch_var.range(11, 11) = if_ir.range(20, 20);
        offset_branch_var.range(10, 1)  = if_ir.range(30, 21);
        offset_branch_var.range(0, 0)   = 0;
        mem_data_var                    = 0;
        inc_pc_var                      = 0;

    }

    // JALR-type Instruction :

    else if (jalr_type_inst_sd == 1) {
        radr1_var = if_ir.range(19, 15);
        radr2_var = 0;

        adr_dest_var = if_ir.range(11, 7);

        dec2exe_op1_var = READ_PC_SR.read();
        dec2exe_op2_var = 0x0;

        if (if_ir.range(31, 31) == 1) {
            offset_branch_var.range(31, 12) = 0b11111111111111111111;
        } else {
            offset_branch_var.range(31, 12) = 0b00000000000000000000;
        }
        offset_branch_var.range(11, 0) = if_ir.range(31, 20);
        offset_branch_var += rdata1_sd.read() - READ_PC_SR.read() + 4;
        offset_branch_var.range(0, 0) = 0;
        mem_data_var                  = 0;
        inc_pc_var                    = 0;

    } else if (system_type_inst_sd == 1) {
        // in CSR operation we always have :
        // rd = CSR
        // CSR = (rs1 | 0) operation CSR
        // So CSR must be wbk in rd
        if (csrrw_i_sd | csrrs_i_sd | csrrc_i_sd | csrrw_i_sd | csrrsi_i_sd | csrrci_i_sd) {
            sc_uint<32> rdata1_signal_sd = rdata1_sd;
            adr_csr_sd                   = if_ir.range(31, 20);
            radr1_var                    = (csrrw_i_sd | csrrs_i_sd | csrrc_i_sd) ? if_ir.range(19, 15) : 0;
            radr2_var                    = 0;

            adr_dest_var = if_ir.range(11, 7);

            dec2exe_op1_var = DATA_READ_CSR_SK.read();

            if (csrrwi_i_sd | csrrs_i_sd)
                dec2exe_op2_var = rdata1_signal_sd;
            else if (csrrc_i_sd)
                dec2exe_op2_var = ~rdata1_signal_sd;
            else if (csrrwi_i_sd | csrrsi_i_sd)
                dec2exe_op2_var = if_ir.range(19, 15);
            else if (csrrci_i_sd)
                dec2exe_op2_var = ~if_ir.range(19, 15);

            offset_branch_var = 0;
            mem_data_var      = 0;
            inc_pc_var        = 0;

            csr_type_operation_rd = 1;
            ADR_CSR_KREG_SD.write(adr_csr_sd);
        } else if (ecall_i_sd || ebreak_i_sd) {
        }
    }
    // Default case :
    else {
        radr1_var         = 0;
        radr2_var         = 0;
        adr_dest_var      = 0;
        dec2exe_op1_var   = 0;
        dec2exe_op2_var   = 0;
        offset_branch_var = 0;
        mem_data_var      = 0;
        inc_pc_var        = 0;
        invalid_instr     = false;
    }

    invalid_instr = invalid_instr || IF2DEC_EMPTY_SI.read();

    RADR1_SD.write(radr1_var);
    RADR2_SD.write(radr2_var);
    adr_dest_sd.write(adr_dest_var);
    offset_branch_sd.write(offset_branch_var);
    exe_op1_sd.write(dec2exe_op1_var);
    exe_op2_sd.write(dec2exe_op2_var);
    mem_data_sd.write(mem_data_var);
    inc_pc_sd.write((inc_pc_var && dec2if_push_sd.read()) || invalid_instr);
    add_offset_to_pc_sd.write(!stall && !inc_pc_var && dec2if_push_sd.read() && !invalid_instr);
}

//---------------------------------------------EXE & MEM SIGNAL DETECTION
//:---------------------------------------------

void decod::post_reg_read_decoding() {
    // We are going to setup commands sent to EXE here, so each if will be
    // execute for one type of command :

    // CMD : +
    int dec2exe_wb_var;
    if (add_i_sd || sub_i_sd || addi_i_sd || lw_i_sd || lh_i_sd | lhu_i_sd || lb_i_sd || lbu_i_sd || sw_i_sd || sh_i_sd || sb_i_sd || auipc_i_sd || lui_i_sd ||
        slti_i_sd || slt_i_sd || sltiu_i_sd || sltu_i_sd) {
        exe_cmd_sd.write(0);
        select_shift_sd.write(0);

        // NEG OP1 GESTION :

        if (sub_i_sd | slt_i_sd | slti_i_sd | sltu_i_sd | sltiu_i_sd) {
            exe_neg_op2_sd.write(1);
        } else {
            exe_neg_op2_sd.write(0);
        }

        // WBK GESTION :

        if (sw_i_sd | sh_i_sd | sb_i_sd)
            dec2exe_wb_var = 0;
        else
            dec2exe_wb_var = 1;

        // MEMORY GESTION :

        // LOAD :

        if (lw_i_sd | lh_i_sd | lhu_i_sd | lb_i_sd | lbu_i_sd) {
            mem_load_sd.write(1);
            if (lw_i_sd) {
                mem_size_sd.write(0);
                mem_sign_extend_sd.write(0);
            } else if (lh_i_sd | lhu_i_sd) {
                mem_size_sd.write(1);
                if (lhu_i_sd)
                    mem_sign_extend_sd.write(1);
                else
                    mem_sign_extend_sd.write(0);
            } else if (lb_i_sd | lbu_i_sd) {
                mem_size_sd.write(2);
                if (lbu_i_sd)
                    mem_sign_extend_sd.write(1);
                else
                    mem_sign_extend_sd.write(0);
            }
        } else {
            mem_load_sd.write(0);
            mem_sign_extend_sd.write(0);
        }

        // STORE :

        if (sw_i_sd | sh_i_sd | sb_i_sd) {
            mem_store_sd.write(1);
            if (sw_i_sd) {
                mem_size_sd.write(0);
            } else if (sh_i_sd) {
                mem_size_sd.write(1);
            } else if (sb_i_sd) {
                mem_size_sd.write(2);
            }
        } else {
            mem_store_sd.write(0);
        }

    }

    // CMD : &
    else if (and_i_sd || andi_i_sd || csrrc_i_sd || csrrc_i_sd) {
        exe_cmd_sd.write(1);
        exe_neg_op2_sd.write(0);
        dec2exe_wb_var = 1;
        mem_load_sd.write(0);
        mem_store_sd.write(0);
        mem_sign_extend_sd.write(0);
        mem_size_sd.write(0);

        select_shift_sd.write(0);

    }
    // CMD : |
    else if (or_i_sd || ori_i_sd || csrrs_i_sd || csrrsi_i_sd) {
        exe_cmd_sd.write(2);
        exe_neg_op2_sd.write(0);
        dec2exe_wb_var = 1;
        mem_load_sd.write(0);
        mem_store_sd.write(0);
        mem_sign_extend_sd.write(0);
        mem_size_sd.write(0);
        select_shift_sd.write(0);
    }
    // CMD : ^
    else if (xor_i_sd || xori_i_sd) {
        exe_cmd_sd.write(3);
        exe_neg_op2_sd.write(0);
        dec2exe_wb_var = 1;
        mem_load_sd.write(0);
        mem_store_sd.write(0);
        mem_sign_extend_sd.write(0);
        mem_size_sd.write(0);
        select_shift_sd.write(0);
    }
    // SHIFT : SLL
    else if (sll_i_sd || slli_i_sd) {
        exe_cmd_sd.write(0);
        exe_neg_op2_sd.write(0);
        dec2exe_wb_var = 1;
        mem_load_sd.write(0);
        mem_store_sd.write(0);
        mem_sign_extend_sd.write(0);
        mem_size_sd.write(0);
        select_shift_sd.write(1);

    }
    // SHIFT : SRL
    else if (srl_i_sd || srli_i_sd) {
        exe_cmd_sd.write(1);
        exe_neg_op2_sd.write(0);
        dec2exe_wb_var = 1;
        mem_load_sd.write(0);
        mem_store_sd.write(0);
        mem_sign_extend_sd.write(0);
        mem_size_sd.write(0);
        select_shift_sd.write(1);

    }
    // SHIFT SRA
    else if (sra_i_sd || srai_i_sd) {
        exe_cmd_sd.write(2);
        exe_neg_op2_sd.write(0);
        dec2exe_wb_var = 1;
        mem_load_sd.write(0);
        mem_store_sd.write(0);
        mem_sign_extend_sd.write(0);
        mem_size_sd.write(0);
        select_shift_sd.write(1);

    } else if (jalr_type_inst_sd.read() || j_type_inst_sd.read()) {
        exe_cmd_sd.write(0);
        exe_neg_op2_sd.write(0);
        dec2exe_wb_var = 1;
        mem_load_sd.write(0);
        mem_store_sd.write(0);
        mem_sign_extend_sd.write(0);
        mem_size_sd.write(0);
        select_shift_sd.write(0);
    } else {
        exe_cmd_sd.write(0);
        exe_neg_op2_sd.write(0);
        dec2exe_wb_var = 0;
        mem_load_sd.write(0);
        mem_store_sd.write(0);
        mem_sign_extend_sd.write(0);
        mem_size_sd.write(0);
        select_shift_sd.write(0);
    }
    exe_wb_sd.write(dec2exe_wb_var);
}

//---------------------------------------------PC GESTION
//:---------------------------------------------

void decod::pc_inc() {
    sc_uint<32> pc                = READ_PC_SR.read();
    sc_uint<32> pc_out            = pc;
    sc_uint<32> offset_branch_var = offset_branch_sd.read();

    if (inc_pc_sd) {
        pc_out = pc + 4;
        WRITE_PC_SD.write(pc_out);
        WRITE_PC_ENABLE_SD.write(1);
    } else if (!inc_pc_sd && add_offset_to_pc_sd.read()) {
        pc_out = pc + offset_branch_var - 4;
        WRITE_PC_SD.write(pc_out);
        WRITE_PC_ENABLE_SD.write(1);
    } else {
        WRITE_PC_ENABLE_SD.write(0);
    }
    dec2if_in_sd.write(pc_out);
}

void decod::bypasses() {
    if (RADR1_SD.read() == 0) {  // ignore r0
        rdata1_sd.write(RDATA1_SR.read());
        r1_valid_sd.write(true);
    } else if (RADR1_SD.read() == EXE_DEST_SD.read() && !DEC2EXE_EMPTY_SD.read()) {  // dont bypass if instr is currently in exe
        r1_valid_sd.write(false);
    } else if (RADR1_SD.read() == BP_DEST_RE.read() && BP_MEM_LOAD_RE.read() && !BP_EXE2MEM_EMPTY_SE) {  // dont bypass if load instr is
                                                                                                         // currently in mem
        r1_valid_sd.write(false);
    } else if (RADR1_SD.read() == BP_DEST_RE.read() && !BP_EXE2MEM_EMPTY_SE) {  // bypass E->D
        r1_valid_sd.write(true);
        rdata1_sd.write(BP_EXE_RES_RE.read());
    } else if (RADR1_SD.read() == BP_DEST_RM.read()) {  // bypass M->D
        r1_valid_sd.write(true);
        rdata1_sd.write(BP_MEM_RES_RM.read());
    } else {  // no bypass
        r1_valid_sd.write(true);
        rdata1_sd.write(RDATA1_SR.read());
    }

    if (RADR2_SD.read() == 0) {  // ignore r0
        rdata2_sd.write(RDATA2_SR.read());
        r2_valid_sd.write(true);
    } else if (RADR2_SD.read() == EXE_DEST_SD.read() && !DEC2EXE_EMPTY_SD.read()) {  // dont bypass if instr is currently in exe
        r2_valid_sd.write(false);
    } else if (RADR2_SD.read() == BP_DEST_RE.read() && BP_MEM_LOAD_RE.read() && !BP_EXE2MEM_EMPTY_SE) {  // dont bypass if load instr is
                                                                                                         // currently in mem
        r2_valid_sd.write(false);
    } else if (RADR2_SD.read() == BP_DEST_RE.read() && !BP_EXE2MEM_EMPTY_SE) {  // bypass E->D
        r2_valid_sd.write(true);
        rdata2_sd.write(BP_EXE_RES_RE.read());
    } else if (RADR2_SD.read() == BP_DEST_RM.read()) {  // bypass M->D
        r2_valid_sd.write(true);
        rdata2_sd.write(BP_MEM_RES_RM.read());
    } else {  // no bypass
        r2_valid_sd.write(true);
        rdata2_sd.write(RDATA2_SR.read());
    }
    // When a load is in exe, we can block the pipeline now
    // Avoid an issue with load - load - add sequence
    block_in_dec.write((RADR1_SD.read() == EXE_DEST_SD.read() && MEM_LOAD_RD && !DEC2EXE_EMPTY_SD.read()) ||
                       (RADR2_SD.read() == EXE_DEST_SD.read() && MEM_LOAD_RD && !DEC2EXE_EMPTY_SD.read()));
}

void decod::stall_method() {
    stall.write((!r1_valid_sd || !r2_valid_sd) && (b_type_inst_sd || jalr_type_inst_sd || j_type_inst_sd || block_in_dec));
}

//---------------------------------------------METHOD TO TRACE SIGNALS
//:---------------------------------------------

void decod::trace(sc_trace_file* tf) {
    dec2if.trace(tf);
    dec2exe.trace(tf);
    sc_trace(tf, rdata1_sd, GET_NAME(rdata1_sd));
    sc_trace(tf, rdata2_sd, GET_NAME(rdata2_sd));
    sc_trace(tf, r1_valid_sd, GET_NAME(r1_valid_sd));
    sc_trace(tf, r2_valid_sd, GET_NAME(r2_valid_sd));
    sc_trace(tf, RADR1_SD, GET_NAME(RADR1_SD));
    sc_trace(tf, RADR2_SD, GET_NAME(RADR2_SD));
    sc_trace(tf, EXE_DEST_SD, GET_NAME(EXE_DEST_SD));
    sc_trace(tf, READ_PC_SR, GET_NAME(READ_PC_SR));
    sc_trace(tf, OP1_RD, GET_NAME(OP1_RD));
    sc_trace(tf, OP2_RD, GET_NAME(OP2_RD));
    sc_trace(tf, EXE_CMD_RD, GET_NAME(EXE_CMD_RD));
    sc_trace(tf, NEG_OP2_RD, GET_NAME(NEG_OP2_RD));
    sc_trace(tf, WB_RD, GET_NAME(WB_RD));
    sc_trace(tf, MEM_DATA_RD, GET_NAME(MEM_DATA_RD));
    sc_trace(tf, MEM_LOAD_RD, GET_NAME(MEM_LOAD_RD));
    sc_trace(tf, MEM_STORE_RD, GET_NAME(MEM_STORE_RD));
    sc_trace(tf, MEM_SIGN_EXTEND_RD, GET_NAME(MEM_SIGN_EXTEND_RD));
    sc_trace(tf, MEM_SIZE_RD, GET_NAME(MEM_SIZE_RD));
    sc_trace(tf, SELECT_SHIFT_RD, GET_NAME(SELECT_SHIFT_RD));
    sc_trace(tf, DEC2IF_POP_SI, GET_NAME(DEC2IF_POP_SI));
    sc_trace(tf, DEC2IF_EMPTY_SD, GET_NAME(DEC2IF_EMPTY_SD));
    sc_trace(tf, PC_RD, GET_NAME(PC_RD));
    sc_trace(tf, INSTR_RI, GET_NAME(INSTR_RI));
    sc_trace(tf, IF2DEC_EMPTY_SI, GET_NAME(IF2DEC_EMPTY_SI));
    sc_trace(tf, IF2DEC_POP_SD, GET_NAME(IF2DEC_POP_SD));
    sc_trace(tf, DEC2EXE_POP_SE, GET_NAME(DEC2EXE_POP_SE));
    sc_trace(tf, DEC2EXE_EMPTY_SD, GET_NAME(DEC2EXE_EMPTY_SD));
    sc_trace(tf, dec2exe_out_sd, GET_NAME(dec2exe_out_sd));
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET_N, GET_NAME(RESET_N));
    sc_trace(tf, dec2if_in_sd, GET_NAME(dec2if_in_sd));
    sc_trace(tf, dec2if_push_sd, GET_NAME(dec2if_push_sd));
    sc_trace(tf, dec2if_empty_sd, GET_NAME(dec2if_empty_sd));
    sc_trace(tf, dec2if_full_sd, GET_NAME(dec2if_full_sd));
    sc_trace(tf, dec2if_out_sd, GET_NAME(dec2if_out_sd));
    sc_trace(tf, dec2exe_in_sd, GET_NAME(dec2exe_in_sd));
    sc_trace(tf, dec2exe_push_sd, GET_NAME(dec2exe_push_sd));
    sc_trace(tf, dec2exe_full_sd, GET_NAME(dec2exe_full_sd));
    sc_trace(tf, r_type_inst_sd, GET_NAME(r_type_inst_sd));
    sc_trace(tf, i_type_inst_sd, GET_NAME(i_type_inst_sd));
    sc_trace(tf, s_type_inst_sd, GET_NAME(s_type_inst_sd));
    sc_trace(tf, b_type_inst_sd, GET_NAME(b_type_inst_sd));
    sc_trace(tf, u_type_inst_sd, GET_NAME(u_type_inst_sd));
    sc_trace(tf, j_type_inst_sd, GET_NAME(j_type_inst_sd));
    sc_trace(tf, jalr_type_inst_sd, GET_NAME(jalr_type_inst_sd));
    sc_trace(tf, add_i_sd, GET_NAME(add_i_sd));
    sc_trace(tf, slt_i_sd, GET_NAME(slt_i_sd));
    sc_trace(tf, sltu_i_sd, GET_NAME(sltu_i_sd));
    sc_trace(tf, and_i_sd, GET_NAME(and_i_sd));
    sc_trace(tf, or_i_sd, GET_NAME(or_i_sd));
    sc_trace(tf, xor_i_sd, GET_NAME(xor_i_sd));
    sc_trace(tf, sll_i_sd, GET_NAME(sll_i_sd));
    sc_trace(tf, srl_i_sd, GET_NAME(srl_i_sd));
    sc_trace(tf, sub_i_sd, GET_NAME(sub_i_sd));
    sc_trace(tf, sra_i_sd, GET_NAME(sra_i_sd));
    sc_trace(tf, addi_i_sd, GET_NAME(addi_i_sd));
    sc_trace(tf, slti_i_sd, GET_NAME(slti_i_sd));
    sc_trace(tf, sltiu_i_sd, GET_NAME(sltiu_i_sd));
    sc_trace(tf, andi_i_sd, GET_NAME(andi_i_sd));
    sc_trace(tf, ori_i_sd, GET_NAME(ori_i_sd));
    sc_trace(tf, xori_i_sd, GET_NAME(xori_i_sd));
    sc_trace(tf, jalr_i_sd, GET_NAME(jalr_i_sd));
    sc_trace(tf, slli_i_sd, GET_NAME(slli_i_sd));
    sc_trace(tf, srli_i_sd, GET_NAME(srli_i_sd));
    sc_trace(tf, srai_i_sd, GET_NAME(srai_i_sd));
    sc_trace(tf, lw_i_sd, GET_NAME(lw_i_sd));
    sc_trace(tf, lh_i_sd, GET_NAME(lh_i_sd));
    sc_trace(tf, lhu_i_sd, GET_NAME(lhu_i_sd));
    sc_trace(tf, lb_i_sd, GET_NAME(lb_i_sd));
    sc_trace(tf, lbu_i_sd, GET_NAME(lbu_i_sd));
    sc_trace(tf, beq_i_sd, GET_NAME(beq_i_sd));
    sc_trace(tf, bne_i_sd, GET_NAME(bne_i_sd));
    sc_trace(tf, blt_i_sd, GET_NAME(blt_i_sd));
    sc_trace(tf, bge_i_sd, GET_NAME(bge_i_sd));
    sc_trace(tf, bltu_i_sd, GET_NAME(bltu_i_sd));
    sc_trace(tf, bgeu_i_sd, GET_NAME(bgeu_i_sd));
    sc_trace(tf, lui_i_sd, GET_NAME(lui_i_sd));
    sc_trace(tf, auipc_i_sd, GET_NAME(auipc_i_sd));
    sc_trace(tf, jal_i_sd, GET_NAME(jal_i_sd));
    sc_trace(tf, sw_i_sd, GET_NAME(sw_i_sd));
    sc_trace(tf, sh_i_sd, GET_NAME(sh_i_sd));
    sc_trace(tf, sb_i_sd, GET_NAME(sb_i_sd));
    sc_trace(tf, offset_branch_sd, GET_NAME(offset_branch_sd));
    sc_trace(tf, inc_pc_sd, GET_NAME(inc_pc_sd));
    sc_trace(tf, adr_dest_sd, GET_NAME(adr_dest_sd));
    sc_trace(tf, exe_op1_sd, GET_NAME(exe_op1_sd));
    sc_trace(tf, exe_op2_sd, GET_NAME(exe_op2_sd));
    sc_trace(tf, mem_size_sd, GET_NAME(mem_size_sd));
    sc_trace(tf, mem_load_sd, GET_NAME(mem_load_sd));
    sc_trace(tf, mem_store_sd, GET_NAME(mem_store_sd));
    sc_trace(tf, exe_cmd_sd, GET_NAME(exe_cmd_sd));
    sc_trace(tf, select_shift_sd, GET_NAME(select_shift_sd));
    sc_trace(tf, exe_neg_op2_sd, GET_NAME(exe_neg_op2_sd));
    sc_trace(tf, exe_wb_sd, GET_NAME(exe_wb_sd));
    sc_trace(tf, mem_sign_extend_sd, GET_NAME(mem_sign_extend_sd));
    sc_trace(tf, WRITE_PC_SD, GET_NAME(WRITE_PC_SD));
    sc_trace(tf, WRITE_PC_ENABLE_SD, GET_NAME(WRITE_PC_ENABLE_SD));
    sc_trace(tf, add_offset_to_pc_sd, GET_NAME(add_offset_to_pc_sd));
    sc_trace(tf, IF2DEC_FLUSH_SD, GET_NAME(IF2DEC_FLUSH_SD));
    sc_trace(tf, stall, GET_NAME(stall));
    sc_trace(tf, BP_DEST_RE, GET_NAME(BP_DEST_RE));
    sc_trace(tf, BP_EXE_RES_RE, GET_NAME(BP_EXE_RES_RE));
    sc_trace(tf, BP_DEST_RM, GET_NAME(BP_DEST_RM));
    sc_trace(tf, BP_MEM_RES_RM, GET_NAME(BP_MEM_RES_RM));
    sc_trace(tf, BP_MEM_LOAD_RE, GET_NAME(BP_MEM_LOAD_RE));
    sc_trace(tf, RDATA1_SR, GET_NAME(RDATA1_SR));
    sc_trace(tf, RDATA2_SR, GET_NAME(RDATA2_SR));
    sc_trace(tf, PC_DEC2EXE_RD, GET_NAME(PC_DEC2EXE_RD));
    sc_trace(tf, PC_IF2DEC_RI, GET_NAME(PC_IF2DEC_RI));
}