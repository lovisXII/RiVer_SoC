#include "dec.h"

/************************************************************
                        fifo wrappers
*************************************************************/
/*
fifo can only have one internal register, so we combine every output signals into
one, which will be the fifo input, and the fifo output is split in different signals.
*/

void decod::concat_dec2exe() {
    sc_bv<DEC2EXE_SIZE> dec2exe_in_var;
    dec2exe_in_var[129]            = block_bp_sd;
    dec2exe_in_var.range(128, 123) = RADR1_SD.read();
    dec2exe_in_var.range(122, 117) = RADR2_SD.read();
    dec2exe_in_var.range(116, 115) = exe_cmd_sd.read();
    dec2exe_in_var.range(114, 83)  = exe_op1_sd.read();
    dec2exe_in_var.range(82, 51)   = exe_op2_sd.read();
    dec2exe_in_var[50]             = exe_neg_op2_sd.read();
    dec2exe_in_var[49]             = exe_wb_sd.read();

    dec2exe_in_var.range(48, 17) = mem_data_sd.read();

    dec2exe_in_var[16] = mem_load_sd.read();
    dec2exe_in_var[15] = mem_store_sd.read();

    dec2exe_in_var[14]           = mem_sign_extend_sd.read();
    dec2exe_in_var.range(13, 12) = mem_size_sd.read();
    dec2exe_in_var.range(11, 8)  = optype_sd.read();
    dec2exe_in_var.range(7, 2)   = adr_dest_sd.read();
    dec2exe_in_var[1]            = slt_i_sd.read() | slti_i_sd.read();
    dec2exe_in_var[0]            = sltu_i_sd.read() | sltiu_i_sd.read();

    dec2exe_in_sd.write(dec2exe_in_var);
}
void decod::unconcat_dec2exe() {
    sc_bv<DEC2EXE_SIZE> dec2exe_out_var = dec2exe_out_sd.read();

    BLOCK_BP_RD.write((bool)dec2exe_out_var[129]);

    BP_RADR1_RD.write((sc_bv_base)dec2exe_out_var.range(128, 123));
    BP_RADR2_RD.write((sc_bv_base)dec2exe_out_var.range(122, 117));

    EXE_CMD_RD.write((sc_bv_base)dec2exe_out_var.range(116, 115));
    OP1_RD.write((sc_bv_base)dec2exe_out_var.range(114, 83));
    OP2_RD.write((sc_bv_base)dec2exe_out_var.range(82, 51));
    NEG_OP2_RD.write((bool)dec2exe_out_var[50]);
    WB_RD.write((bool)dec2exe_out_var[49]);

    MEM_DATA_RD.write((sc_bv_base)dec2exe_out_var.range(48, 17));

    MEM_LOAD_RD.write((bool)dec2exe_out_var[16]);
    MEM_STORE_RD.write((bool)dec2exe_out_var[15]);

    MEM_SIGN_EXTEND_RD.write((bool)dec2exe_out_var[14]);
    MEM_SIZE_RD.write((sc_bv_base)dec2exe_out_var.range(13, 12));
    OPTYPE_RD.write((sc_bv_base)dec2exe_out_var.range(11, 8));
    EXE_DEST_SD.write((sc_bv_base)dec2exe_out_var.range(7, 2));
}

/************************************************************
                    Instruction type
*************************************************************/
/*
The first decoding step is to determine which instruction format is used, as defined in
the specification
*/
void decod::decoding_instruction_type() {
    sc_uint<32> if_ir = INSTR_RI.read();
    r_type_inst_sd = (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) != 0b0000001) ? 1 : 0;
    i_type_inst_sd = (if_ir.range(6, 0) == 0b0010011 || if_ir.range(6, 0) == 0b0000011) ? 1 : 0;
    s_type_inst_sd = if_ir.range(6, 0) == 0b0100011 ? 1 : 0;
    b_type_inst_sd = if_ir.range(6, 0) == 0b1100011 ? 1 : 0;
    u_type_inst_sd = (if_ir.range(6, 0) == 0b0110111 || if_ir.range(6, 0) == 0b0010111) ? 1 : 0;
    j_type_inst_sd = if_ir.range(6, 0) == 0b1101111 ? 1 : 0;
    jalr_type_inst_sd = if_ir.range(6, 0) == 0b1100111 ? 1 : 0;
}

/************************************************************
                    Instruction decoding
*************************************************************/
/*
Then, we need to know which instruction is being executed, oce again as defined in
the specification.
*/
void decod::decoding_instruction() {
    sc_uint<32> if_ir = INSTR_RI.read();

    // R-type Instruction :

    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000000 &&
        if_ir.range(14, 12) == 0b000)
        add_i_sd.write(1);
    else
        add_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000000 &&
        if_ir.range(14, 12) == 0b010)
        slt_i_sd.write(1);
    else
        slt_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000000 &&
        if_ir.range(14, 12) == 0b011)
        sltu_i_sd.write(1);
    else
        sltu_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000000 &&
        if_ir.range(14, 12) == 0b111)
        and_i_sd.write(1);
    else
        and_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000000 &&
        if_ir.range(14, 12) == 0b110)
        or_i_sd.write(1);
    else
        or_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000000 &&
        if_ir.range(14, 12) == 0b100)
        xor_i_sd.write(1);
    else
        xor_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000000 &&
        if_ir.range(14, 12) == 0b001)
        sll_i_sd.write(1);
    else
        sll_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000000 &&
        if_ir.range(14, 12) == 0b101)
        srl_i_sd.write(1);
    else
        srl_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0100000 &&
        if_ir.range(14, 12) == 0b000)
        sub_i_sd.write(1);
    else
        sub_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0100000 &&
        if_ir.range(14, 12) == 0b101)
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
    if (if_ir.range(6, 0) == 0b0001111 && if_ir.range(14, 12) == 0b000)
        fence_i_sd.write(1);
    else
        fence_i_sd.write(0);

    // I-type shift instructions :

    if (if_ir.range(6, 0) == 0b0010011 && if_ir.range(31, 25) == 0b0000000 &&
        if_ir.range(14, 12) == 0b001)
        slli_i_sd.write(1);
    else
        slli_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0010011 && if_ir.range(31, 25) == 0b0000000 &&
        if_ir.range(14, 12) == 0b101)
        srli_i_sd.write(1);
    else
        srli_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0010011 && if_ir.range(31, 25) == 0b0100000 &&
        if_ir.range(14, 12) == 0b101)
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
}

/************************************************************
                    Register bank inputs
*************************************************************/
/*
The next step is to read the source registers. We also set the destination adress here.
*/
void decod::pre_reg_read_decoding() {
    sc_uint<32> if_ir = INSTR_RI.read();
    sc_uint<6>  radr1_var;
    sc_uint<6>  radr2_var;
    sc_uint<6>  adr_dest_var;

    // R-type Instruction :
    if (r_type_inst_sd == 1) {
        radr1_var    = if_ir.range(19, 15);
        radr2_var    = if_ir.range(24, 20);
        adr_dest_var = if_ir.range(11, 7);
    }
    // I-type Instruction :
    else if (i_type_inst_sd == 1) {
        radr1_var    = if_ir.range(19, 15);
        radr2_var    = 0;
        adr_dest_var = if_ir.range(11, 7);
    }
    // S-type Instruction :
    else if (s_type_inst_sd == 1) {
        radr1_var    = if_ir.range(19, 15);
        radr2_var    = if_ir.range(24, 20);
        adr_dest_var = 0;
    }
    // Branch Instruction :
    else if (b_type_inst_sd == 1) {
        radr1_var    = if_ir.range(19, 15);
        radr2_var    = if_ir.range(24, 20);
        adr_dest_var = 0;
    }
    // U-type Instruction :
    else if (u_type_inst_sd == 1) {
        radr1_var    = 0;
        adr_dest_var = if_ir.range(11, 7);
        radr2_var    = 0;
    }
    // J-type Instruction :
    else if (j_type_inst_sd == 1) {
        radr1_var    = 0;
        radr2_var    = 0;
        adr_dest_var = if_ir.range(11, 7);
    }
    // JALR-type Instruction :
    else if (jalr_type_inst_sd == 1) {
        radr1_var    = if_ir.range(19, 15);
        radr2_var    = 0;
        adr_dest_var = if_ir.range(11, 7);
    }
    // Default case :
    else {
        radr1_var    = 0;
        radr2_var    = 0;
        adr_dest_var = 0;
    }
    RADR1_SD.write(radr1_var);
    RADR2_SD.write(radr2_var);
    adr_dest_sd.write(adr_dest_var);
}

/************************************************************
                        Bypasses
*************************************************************/
/*
This process handles bypasses : it checks the data currently in
the pipeline, and if an instruction not yet finished will write
data to one of the source registers used in the current instruction,
it bypass the pipeline to read the data directly (if it can), or stall
the pipeline.

The following bypasses are implemented :
EXE->DEC
MEM->DEC
EXE->EXE
MEM->EXE

those 4 bypasses take the data from the end of the first stage and pipe it to the beginning of the
second. They all handle both source registers, making 8 bypasses.
*/

void decod::bypasses() {
    // **********Bypasse for source registers 1*********
    if (RADR1_SD.read() == 0) {
        // We ignore the regiter r0, which us always 0
        invalid_operand_sd = false;
        rdata1_sd          = RDATA1_SR;
    } else if (RADR1_SD == EXE_DEST_SD && !DEC2EXE_EMPTY_SD) {
        /*
         If instruction is currently in EXE, the result is not ready, and we mark it as such.
         We need to check DEC2EXE_EMPTY, because if that fifo is empty, it means there is no
         instruction in EXE, and the values sent by the bypass are garbage.
        */
        invalid_operand_sd = true;
    } else if (RADR1_SD == BP_DEST_RE && BP_MEM_LOAD_RE && !BP_EXE2MEM_EMPTY_SE) {
        /*
         exactly the same condition, but for MEM, and we check that it is a memory instruction
         to ensure the result is not ready
        */
        invalid_operand_sd = true;
    } else if (RADR1_SD == BP_DEST_RE && !BP_EXE2MEM_EMPTY_SE) {
        /*
         EXE -> DEC Bypass
         If the result if EXE will be written in the register we need, we can use the bypass
         to get the value directly, saving clock cycles.
         Once again, we need to check the FIFO is not empty to ensure the value is valid.
        */
        invalid_operand_sd = false;
        rdata1_sd          = BP_EXE_RES_RE;
    } else if (RADR1_SD == BP_DEST_RM && !BP_MEM2WBK_EMPTY_SM) {
        /*
         EXE -> DEC Bypass
         Same bypass, but in MEM. Bypass in EXE has priority over bypass in MEM, because the value
         is more recent.
        */
        invalid_operand_sd = false;
        rdata1_sd          = BP_MEM_RES_RM;
    } else {
        /*
         If none of these conditions is true, we just use the value from registers.
        */
        invalid_operand_sd = false;
        rdata1_sd          = RDATA1_SR;
    }

    // **********Bypasse for source registers 2 (it's the same thing)*********
    if (RADR2_SD.read() == 0) {
        // We ignore the regiter r0, which us always 0
        rdata2_sd = RDATA2_SR;
    } else if (RADR2_SD == EXE_DEST_SD && !DEC2EXE_EMPTY_SD) {
        /*
         If instruction is currently in EXE, the result is not ready, and we mark it as such.
         We need to check DEC2EXE_EMPTY, because if that fifo is empty, it means there is no
         instruction in EXE, and the values sent by the bypass are garbage.
        */
        invalid_operand_sd = true;
    } else if (RADR2_SD == BP_DEST_RE && BP_MEM_LOAD_RE && !BP_EXE2MEM_EMPTY_SE) {
        /*
         exactly the same condition, but for MEM, and we check that it is a memory instruction
         to ensure the result is not ready
        */
        invalid_operand_sd = true;
    } else if (RADR2_SD == BP_DEST_RE && !BP_EXE2MEM_EMPTY_SE) {
        /*
         EXE -> DEC Bypass
         If the result of EXE will be written in the register we need, we can use the bypass
         to get the value directly, saving clock cycles.
         Once again, we need to check the FIFO is not empty to ensure the value is valid.
        */
        rdata2_sd = BP_EXE_RES_RE;
    } else if (RADR2_SD == BP_DEST_RM && !BP_MEM2WBK_EMPTY_SM) {
        /*
         MEM -> DEC Bypass
         Same bypass, but in MEM. Bypass in EXE has priority over bypass in MEM, because the value
         is more recent.
        */
        rdata2_sd = BP_MEM_RES_RM;
    } else {
        /*
         If none of these conditions is true, we just use the value from registers.
        */
        rdata2_sd = RDATA2_SR;
    }

    // When a load is in exe, we can block the pipeline now
    // Avoid an issue with load - load - add sequence
    if ((RADR1_SD == EXE_DEST_SD || RADR2_SD == EXE_DEST_SD) && MEM_LOAD_RD && !DEC2EXE_EMPTY_SD)
        block_in_dec_sd = true;
    else
        block_in_dec_sd = false;
}

/************************************************************
                       Setting outputs
*************************************************************/
/*
This step sets all outputs of DEC for the next stages, depending on the instruction, and
of the data read in registers in the previous step.
*/

void decod::post_reg_read_decoding() {
    bool        dec2exe_wb_var;
    sc_uint<32> dec2exe_op1_var;
    sc_uint<32> dec2exe_op2_var;
    sc_uint<32> if_ir = INSTR_RI.read();
    sc_uint<32> mem_data_var;
    sc_uint<32> offset_branch_var;
    bool        jump_var     = false;
    bool        illegal_inst = false;

    // Data instructions : R, I and U-type
    if (r_type_inst_sd || i_type_inst_sd || u_type_inst_sd) {
        // Setting operands
        if (i_type_inst_sd) {
            dec2exe_op1_var = rdata1_sd;
            // Sign extend the immediate
            if (if_ir.range(31, 31) == 1) {
                dec2exe_op2_var.range(31, 12) = 0b11111111111111111111;
            } else {
                dec2exe_op2_var.range(31, 12) = 0b00000000000000000000;
            }
            dec2exe_op2_var.range(11, 0) = if_ir.range(31, 20);
        } else if (u_type_inst_sd) {
            dec2exe_op1_var.range(31, 12) = if_ir.range(31, 12);
            dec2exe_op1_var.range(11, 0)  = 0;
            // In case of AUIPC, the szcond operand is PC
            if (auipc_i_sd)
                dec2exe_op2_var = PC_RI;
            else
                dec2exe_op2_var = rdata2_sd;
        } else {
            dec2exe_op1_var = rdata1_sd;
            dec2exe_op2_var = rdata2_sd;
        }

        // memory loads processing
        if (lw_i_sd | lh_i_sd | lhu_i_sd | lb_i_sd | lbu_i_sd) {
            mem_load_sd = true;

            // Setting the size of load (word, half word or byte)
            if (lw_i_sd)
                mem_size_sd = 0;
            else if (lh_i_sd | lhu_i_sd)
                mem_size_sd = 1;
            else if (lb_i_sd | lbu_i_sd)
                mem_size_sd = 2;

            // Extend the sign if signed load of byte or half word
            if (lb_i_sd || lh_i_sd)
                mem_sign_extend_sd = true;
            else
                mem_sign_extend_sd = false;
        } else {
            mem_load_sd        = false;
            mem_size_sd        = 0;
            mem_sign_extend_sd = false;
        }
        mem_store_sd      = false;
        dec2exe_wb_var    = true;
        mem_data_var      = 0;
        offset_branch_var = 0;
        jump_var          = false;

        // Set the negation of OP2 in case a substraction is needed (for slt or sub)
        exe_neg_op2_sd.write(sub_i_sd | slt_i_sd | slti_i_sd | sltu_i_sd | sltiu_i_sd);

        // Command for exe
        if (and_i_sd || andi_i_sd || srl_i_sd || srli_i_sd || sltu_i_sd || sltiu_i_sd)
            exe_cmd_sd = 1;
        else if (or_i_sd || ori_i_sd || sra_i_sd || srai_i_sd)
            exe_cmd_sd = 2;
        else if (xor_i_sd || xori_i_sd)
            exe_cmd_sd = 3;
        else
            exe_cmd_sd = 0;

        // Operation type in exe
        if (sll_i_sd || slli_i_sd || srl_i_sd || srli_i_sd || sra_i_sd || srai_i_sd)
            optype_sd = 1;
        else if (slti_i_sd || slt_i_sd || sltu_i_sd || sltiu_i_sd)
            optype_sd = 2;
        else
            optype_sd = 0;

    } else if (s_type_inst_sd) {
        exe_cmd_sd        = 0;
        optype_sd         = 0;
        exe_neg_op2_sd    = false;
        mem_load_sd       = false;
        offset_branch_var = 0;
        jump_var          = false;
        dec2exe_op1_var   = rdata1_sd.read();
        mem_store_sd      = true;
        dec2exe_wb_var    = true;

        /*
        The adress is obtained by adding rs1 to the 12 bit immediate,
        */
        if (if_ir.range(31, 31) == 1) {
            dec2exe_op2_var.range(31, 12) = 0b11111111111111111111;
        } else {
            dec2exe_op2_var.range(31, 12) = 0b00000000000000000000;
        }
        dec2exe_op2_var.range(11, 5) = if_ir.range(31, 25);
        dec2exe_op2_var.range(4, 0)  = if_ir.range(11, 7);

        // rs2 is written to memory
        mem_data_var = rdata2_sd.read();

        // Whether we store a word, half-word or byte
        if (sw_i_sd) {
            mem_size_sd = 0;
        } else if (sh_i_sd) {
            mem_size_sd = 1;
        } else if (sb_i_sd) {
            mem_size_sd = 2;
        }
    } else if (b_type_inst_sd == 1) {
        mem_load_sd        = false;
        mem_store_sd       = false;
        mem_sign_extend_sd = false;
        mem_size_sd        = 0;
        dec2exe_wb_var     = true;
        mem_data_var       = 0;
        exe_cmd_sd         = 0;
        optype_sd          = 0;
        exe_neg_op2_sd     = 0;

        // Decoding the offset (see specification for details)
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

        /*
        Branch conditions :
        The evaluation of the branch condition is done directly in the decode stage.
        */

        dec2exe_op1_var = rdata1_sd.read();
        dec2exe_op2_var = rdata2_sd.read();
        sc_uint<33> res_comparaison;
        res_comparaison     = dec2exe_op1_var - dec2exe_op2_var;
        bool different_sign = dec2exe_op1_var[31] ^ dec2exe_op2_var[31];
        bool equal          = res_comparaison == 0;
        if (bne_i_sd.read()) {
            // bne : jump if not equal
            jump_var = !equal;
        } else if (beq_i_sd.read()) {
            // beq : jump if equal
            jump_var = equal;
        } else if (blt_i_sd.read()) {
            if (different_sign)
                // if the signs are different, branch if the first number is negative
                jump_var = dec2exe_op1_var[31];
            else
                // else branch if the substraction result is negative
                jump_var = res_comparaison[31];  //
        } else if (bltu_i_sd.read()) {
            if (different_sign)
                jump_var = dec2exe_op2_var[31];  // branch if the second number is negative
            else
                jump_var = res_comparaison[31];  // branch if the result is negative
        } else if (bge_i_sd.read()) {
            if (different_sign)
                jump_var = dec2exe_op2_var[31];  // branch if the second number is negative
            else
                jump_var = !res_comparaison[31];  // branch if the result is positive
        } else if (bgeu_i_sd.read()) {
            if (different_sign)
                jump_var = dec2exe_op1_var[31];  // branch if the first number is negative
            else
                jump_var = !res_comparaison[31];  // branch if the result is positive
        }
    } else if (jalr_type_inst_sd.read() || j_type_inst_sd.read()) {
        exe_cmd_sd         = 0;
        exe_neg_op2_sd     = false;
        dec2exe_wb_var     = true;
        mem_load_sd        = false;
        mem_store_sd       = false;
        mem_sign_extend_sd = false;
        mem_size_sd        = 0;
        optype_sd          = 0;
        mem_data_var       = 0;
        jump_var           = true;
        if (jalr_type_inst_sd) {
            // JALR jumps to Rs2 + offset - PC
            dec2exe_op1_var = READ_PC_SR.read();
            dec2exe_op2_var = 0;

            // sign extend the immediate
            if (if_ir.range(31, 31) == 1) {
                offset_branch_var.range(31, 12) = 0b11111111111111111111;
            } else {
                offset_branch_var.range(31, 12) = 0b00000000000000000000;
            }
            offset_branch_var.range(11, 0) = if_ir.range(31, 20);
            offset_branch_var += rdata1_sd.read() - PC_RI.read();
        } else {
            dec2exe_op1_var = READ_PC_SR.read();
            dec2exe_op2_var = 0;

            // get offset, see specification for details
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
        }
    } else if (fence_i_sd) {
        // Fence is equivalent to NOP in this architecture
        exe_cmd_sd         = 0;
        dec2exe_op1_var    = 0;
        dec2exe_op2_var    = 0;
        exe_neg_op2_sd     = false;
        dec2exe_wb_var     = false;
        mem_load_sd        = false;
        mem_store_sd       = false;
        mem_sign_extend_sd = false;
        mem_size_sd        = 0;
        optype_sd          = 0;
        offset_branch_var  = 0;
        mem_data_var       = 0;
        jump_var           = false;
    } else {
        exe_cmd_sd         = 0;
        dec2exe_op1_var    = 0;
        dec2exe_op2_var    = 0;
        exe_neg_op2_sd     = false;
        dec2exe_wb_var     = false;
        mem_load_sd        = false;
        mem_store_sd       = false;
        mem_sign_extend_sd = false;
        mem_size_sd        = 0;
        optype_sd          = 0;
        offset_branch_var  = 0;
        mem_data_var       = 0;
        jump_var           = false;
        illegal_inst       = true;
    }

    // Illegal instruction Gestion :

    if (r_type_inst_sd) {
        if (if_ir.range(31, 25) != 0 && if_ir.range(31, 25) != 0b0100000) { illegal_inst = true; }
    } else if (b_type_inst_sd) {
        if (if_ir.range(14, 12) == 2 || if_ir.range(14, 12) == 3) illegal_inst = true;
    } else if (s_type_inst_sd) {
        if (if_ir.range(14, 12) > 2) illegal_inst = true;
    }

    illegal_inst = illegal_inst && !IF2DEC_EMPTY_SI.read();

    block_bp_sd.write(jalr_type_inst_sd);
    exe_wb_sd.write(dec2exe_wb_var);
    offset_branch_sd.write(offset_branch_var);
    exe_op1_sd.write(dec2exe_op1_var);
    exe_op2_sd.write(dec2exe_op2_var);
    mem_data_sd.write(mem_data_var);
    jump_sd = jump_var;
}

/************************************************************
                    Stall Gestion
*************************************************************/
/*
This process decides to stall the CPU when necessary.
It is necessary in the following cases :
- data dependancy in DEC not satisfied (for the source register of a jump)
- Empty input FIFO
- full output FIFO
*/

void decod::stall_method() {
    stall_sd = (invalid_operand_sd && (b_type_inst_sd || jalr_type_inst_sd || j_type_inst_sd)) ||
               block_in_dec_sd || IF2DEC_EMPTY_SI || dec2exe_full_sd;
}

/************************************************************
                    PC & FIFO Gestion
*************************************************************/
/*
This process sets PC to the next correct value, with jumps, stalls,
or the regular PC + 4 increase.
*/
void decod::pc_inc() {
    /* PC Gestion
    - We POP when there is no stall, meaning when an instruction was executed.
    - We FLUSH the fifo, leaning we remove the data inside, when there is a jump.
      This prevent a "delayed slot" like in MIPS.
    */

    sc_uint<32> pc                = READ_PC_SR.read();
    sc_uint<32> pc_out            = pc;
    sc_uint<32> offset_branch_var = offset_branch_sd.read();

    if (dec2if_full_sd) {
        // if dec2if is full, a new value cannot be pushed to IFetch, no PC is not changed
        WRITE_PC_ENABLE_SD = 0;
        dec2if_push_sd     = 0;
    } else if (IF2DEC_EMPTY_SI || !jump_sd) {
        // If there is no jump, (or if the instruction is invalid), PC is just incremented by 4
        pc_out             = pc + 4;
        WRITE_PC_ENABLE_SD = 1;
        dec2if_push_sd     = 1;
    } else if (jump_sd && !IF2DEC_EMPTY_SI && !stall_sd) {
        // If there is a jump (and the instruction is valid) we jump
        // We need to wait for the end of stalls for the offset to be computed
        pc_out             = PC_RI.read() + offset_branch_sd.read();
        WRITE_PC_ENABLE_SD = 1;
        dec2if_push_sd     = 1;
    } else {
        // Aside from these cases, do nothing
        WRITE_PC_ENABLE_SD = 0;
        dec2if_push_sd     = 0;
    }

    /* IF2DEC Gestion
    - We POP when there is no stall, meaning when an instruction was executed.
    - We FLUSH the fifo, leaning we remove the data inside, when there is a jump.
      This prevent a "delayed slot" like in MIPS.
    */

    if (stall_sd)
        IF2DEC_POP_SD = false;
    else
        IF2DEC_POP_SD = true;

    if (jump_sd && !stall_sd)
        IF2DEC_FLUSH_SD = true;
    else
        IF2DEC_FLUSH_SD = false;

    // DEC2EXE Gestion

    if (stall_sd) {
        dec2exe_push_sd.write(0);
    } else {
        dec2exe_push_sd.write(1);
    }

    WRITE_PC_SD  = pc_out;
    dec2if_in_sd = pc_out;
}

/************************************************************
                        Tracing
*************************************************************/
/*
This method registers every signal for tracing
*/
void decod::trace(sc_trace_file* tf) {
    dec2if.trace(tf);
    dec2exe.trace(tf);
    sc_trace(tf, RDATA1_SR, GET_NAME(RDATA1_SR));
    sc_trace(tf, RDATA2_SR, GET_NAME(RDATA2_SR));
    sc_trace(tf, READ_PC_SR, GET_NAME(READ_PC_SR));
    sc_trace(tf, RADR1_SD, GET_NAME(RADR1_SD));
    sc_trace(tf, RADR2_SD, GET_NAME(RADR2_SD));
    sc_trace(tf, WRITE_PC_SD, GET_NAME(WRITE_PC_SD));
    sc_trace(tf, WRITE_PC_ENABLE_SD, GET_NAME(WRITE_PC_ENABLE_SD));
    sc_trace(tf, OP1_RD, GET_NAME(OP1_RD));
    sc_trace(tf, OP2_RD, GET_NAME(OP2_RD));
    sc_trace(tf, EXE_CMD_RD, GET_NAME(EXE_CMD_RD));
    sc_trace(tf, NEG_OP2_RD, GET_NAME(NEG_OP2_RD));
    sc_trace(tf, WB_RD, GET_NAME(WB_RD));
    sc_trace(tf, EXE_DEST_SD, GET_NAME(EXE_DEST_SD));
    sc_trace(tf, OPTYPE_RD, GET_NAME(OPTYPE_RD));
    sc_trace(tf, MEM_DATA_RD, GET_NAME(MEM_DATA_RD));
    sc_trace(tf, MEM_LOAD_RD, GET_NAME(MEM_LOAD_RD));
    sc_trace(tf, MEM_STORE_RD, GET_NAME(MEM_STORE_RD));
    sc_trace(tf, MEM_SIGN_EXTEND_RD, GET_NAME(MEM_SIGN_EXTEND_RD));
    sc_trace(tf, MEM_SIZE_RD, GET_NAME(MEM_SIZE_RD));
    sc_trace(tf, DEC2IF_POP_SI, GET_NAME(DEC2IF_POP_SI));
    sc_trace(tf, DEC2IF_EMPTY_SD, GET_NAME(DEC2IF_EMPTY_SD));
    sc_trace(tf, PC_RD, GET_NAME(PC_RD));
    sc_trace(tf, PC_RI, GET_NAME(PC_RI));
    sc_trace(tf, INSTR_RI, GET_NAME(INSTR_RI));
    sc_trace(tf, IF2DEC_EMPTY_SI, GET_NAME(IF2DEC_EMPTY_SI));
    sc_trace(tf, IF2DEC_POP_SD, GET_NAME(IF2DEC_POP_SD));
    sc_trace(tf, DEC2EXE_POP_SE, GET_NAME(DEC2EXE_POP_SE));
    sc_trace(tf, DEC2EXE_EMPTY_SD, GET_NAME(DEC2EXE_EMPTY_SD));
    sc_trace(tf, dec2exe_out_sd, GET_NAME(dec2exe_out_sd));
    sc_trace(tf, BP_DEST_RE, GET_NAME(BP_DEST_RE));
    sc_trace(tf, BP_EXE_RES_RE, GET_NAME(BP_EXE_RES_RE));
    sc_trace(tf, BP_MEM_LOAD_RE, GET_NAME(BP_MEM_LOAD_RE));
    sc_trace(tf, BP_EXE2MEM_EMPTY_SE, GET_NAME(BP_EXE2MEM_EMPTY_SE));
    sc_trace(tf, BP_MEM2WBK_EMPTY_SM, GET_NAME(BP_MEM2WBK_EMPTY_SM));
    sc_trace(tf, BP_DEST_RM, GET_NAME(BP_DEST_RM));
    sc_trace(tf, BP_MEM_RES_RM, GET_NAME(BP_MEM_RES_RM));
    sc_trace(tf, BP_RADR1_RD, GET_NAME(BP_RADR1_RD));
    sc_trace(tf, BP_RADR2_RD, GET_NAME(BP_RADR2_RD));
    sc_trace(tf, block_in_dec_sd, GET_NAME(block_in_dec_sd));
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET, GET_NAME(RESET));
    sc_trace(tf, rdata1_sd, GET_NAME(rdata1_sd));
    sc_trace(tf, rdata2_sd, GET_NAME(rdata2_sd));
    sc_trace(tf, invalid_operand_sd, GET_NAME(invalid_operand_sd));
    sc_trace(tf, stall_sd, GET_NAME(stall_sd));
    sc_trace(tf, dec2if_in_sd, GET_NAME(dec2if_in_sd));
    sc_trace(tf, dec2if_push_sd, GET_NAME(dec2if_push_sd));
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
    sc_trace(tf, jump_sd, GET_NAME(jump_sd));
    sc_trace(tf, adr_dest_sd, GET_NAME(adr_dest_sd));
    sc_trace(tf, exe_op1_sd, GET_NAME(exe_op1_sd));
    sc_trace(tf, exe_op2_sd, GET_NAME(exe_op2_sd));
    sc_trace(tf, mem_data_sd, GET_NAME(mem_data_sd));
    sc_trace(tf, mem_size_sd, GET_NAME(mem_size_sd));
    sc_trace(tf, mem_load_sd, GET_NAME(mem_load_sd));
    sc_trace(tf, mem_store_sd, GET_NAME(mem_store_sd));
    sc_trace(tf, exe_cmd_sd, GET_NAME(exe_cmd_sd));
    sc_trace(tf, optype_sd, GET_NAME(optype_sd));
    sc_trace(tf, exe_neg_op2_sd, GET_NAME(exe_neg_op2_sd));
    sc_trace(tf, exe_wb_sd, GET_NAME(exe_wb_sd));
    sc_trace(tf, mem_sign_extend_sd, GET_NAME(mem_sign_extend_sd));
}