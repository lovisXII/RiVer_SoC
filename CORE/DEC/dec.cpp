#include "dec.h"

//PUSH AND POP MUST FREEZE IS DATA IS NOT VALID
void decod::pushingToIf()
{
    
}

void decod::popFromIf()
{

}

void decod::decoding_instruction_type()
{
    sc_uint<32> if_ir = IF_IR.read() ;

    if(if_ir.range(6,0) == 0110011) r_type_inst = 1 ;
    else if(if_ir.range(6,0) == 0010011) i_type_inst = 1 ;
    else if(if_ir.range(6,0) == 0010011) s_type_inst = 1 ;
    else if(if_ir.range(6,0) == 1100011) b_type_inst = 1 ;
    else if(if_ir.range(6,0) == 0110111) u_type_inst = 1 ;
    else if(if_ir.range(6,0) == 1101111) j_type_inst = 1 ;
    else if(if_ir.range(6,0) == 1100111) jalr_type_inst = 1 ;
    //Carefull opcode for JALR INST ARE : 1100111
    //Carefull opcode for s and i instruction are the same
    

}

void decod::decoding_instruction()
{
    sc_uint<32> if_ir = IF_IR.read() ;
    
    //R-type Instruction :

    if(if_ir.range(6,0) == 0110011 && if_ir.range(31,25) == 0000000 && if_ir.range(14,12) == 000) add_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0110011 && if_ir.range(31,25) == 0000000 && if_ir.range(14,12) == 010) slt_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0110011 && if_ir.range(31,25) == 0000000 && if_ir.range(14,12) == 011) sltu_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0110011 && if_ir.range(31,25) == 0000000 && if_ir.range(14,12) == 111) and_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0110011 && if_ir.range(31,25) == 0000000 && if_ir.range(14,12) == 110) or_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0110011 && if_ir.range(31,25) == 0000000 && if_ir.range(14,12) == 100) xor_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0110011 && if_ir.range(31,25) == 0000000 && if_ir.range(14,12) == 001) sll_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0110011 && if_ir.range(31,25) == 0000000 && if_ir.range(14,12) == 101) srl_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0110011 && if_ir.range(31,25) == 0100000 && if_ir.range(14,12) == 000) sub_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0110011 && if_ir.range(31,25) == 0100000 && if_ir.range(14,12) == 101) sra_i = 1 ;
    else 0 ;

    //I-type Instructions :

    if(if_ir.range(6,0) == 0010011  && if_ir.range(14,12) == 000) addi_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0010011  && if_ir.range(14,12) == 010) slti_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0010011  && if_ir.range(14,12) == 011) sltiu_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0010011  && if_ir.range(14,12) == 111) andi_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0010011  && if_ir.range(14,12) == 110) ori_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0010011  && if_ir.range(14,12) == 100) xori_i = 1 ;
    else 0 ;

    // I-type shift instructions :

    if(if_ir.range(6,0) == 0010011 && if_ir.range(31,25) == 0000000 && if_ir.range(14,12) == 001) slli_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0010011 && if_ir.range(31,25) == 0000000 && if_ir.range(14,12) == 101) srli_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0010011 && if_ir.range(31,25) == 0100000 && if_ir.range(14,12) == 101) srai_i = 1 ;
    else 0 ;

    // I-type load instructions :

    if(if_ir.range(6,0) == 0000011 && if_ir.range(14,12) == 010) lw_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0000011 && if_ir.range(14,12) == 001) lh_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0000011 && if_ir.range(14,12) == 101) lhu_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0000011 && if_ir.range(14,12) == 000) lb_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0000011 && if_ir.range(14,12) == 100) lbu_i = 1 ;
    else 0 ;

    //B-type Instructions :


    if(if_ir.range(6,0) == 1100011  && if_ir.range(14,12) == 000) beq_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 1100011  && if_ir.range(14,12) == 001) bne_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 1100011  && if_ir.range(14,12) == 100) blt_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 1100011  && if_ir.range(14,12) == 101) bge_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 1100011  && if_ir.range(14,12) == 110) bltu_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 1100011  && if_ir.range(14,12) == 111) bgeu_i = 1 ;
    else 0 ;

    //U-type Instructions :

    if(if_ir.range(6,0) == 0110111  ) lui_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0010111  ) auipc_i = 1 ;
    else 0 ;

    //J-type Instructions :

    if(if_ir.range(6,0) == 1101111 ) jal_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 1100111 ) jalr_i = 1 ;
    else 0 ;

    //S-type Instructions :

    if(if_ir.range(6,0) == 0100011 && if_ir.range(14,12) == 010) sw_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0100011 && if_ir.range(14,12) == 001) sh_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0100011 && if_ir.range(14,12) == 000) sb_i = 1 ;
    else 0 ;

    
}

void decod::affectation_registres() 
{
    //MISSING VERIFICATION OF VALIDATION NEED TO ADD A KIND OF WAIT TO CHECK IF DATA IS VALID
    
    sc_uint<32> if_ir = IF_IR.read() ;
    sc_uint<6> radr1;
    sc_uint<6> radr2;
    sc_uint<6> adr_dest;
    sc_uint<32> dec2exe_op1;
    sc_uint<32> dec2exe_op2;
    sc_uint<32> offset_branch_var ;
    sc_uint<32> mem_data ;
    bool inc_pc ;

    if(r_type_inst == 1)
    {
        radr1       = if_ir.range(19,15) ;
        radr2       = if_ir.range(24,20) ;
        adr_dest    = if_ir.range(11,7) ;
        
        dec2exe_op1 = (RADR1_DATA.read()) ;
        dec2exe_op2 = (RADR2_DATA.read()) ;

        offset_branch_var = 0 ;
        mem_data = 0 ;
        inc_pc = 1 ;
    }
    if(i_type_inst == 1)
    {
        radr1                       =   if_ir.range(19,15) ;
        radr2                       =   0 ;
        adr_dest                    =   if_ir.range(11,7) ;

        dec2exe_op1                 =   RADR1_DATA.read() ;

        //OP2 is sign extended
        dec2exe_op2.range(31,12)    =   if_ir.range(31,31) ; 
        dec2exe_op2.range(11,0)     =   if_ir.range(31,20) ; 
        
        offset_branch_var = 0 ;
        mem_data = 0 ;
        inc_pc = 1 ;


    }
    if(s_type_inst == 1)
    {
        //The adress is obtained by adding rs1 to the 12 bit immediat sign extended 
        // rs2 is copied to the memory

        radr1                       =   if_ir.range(19,15) ;
        radr2                       =   if_ir.range(24,20) ;
        adr_dest                    =   0 ;

        dec2exe_op1                 =   RADR1_DATA.read() ;

        dec2exe_op2.range(31,12)    = if_ir.range(31,31) ;
        dec2exe_op2.range(11,5)     = if_ir.range(31,25) ;
        dec2exe_op2.range(4,0)      = if_ir.range(11,7) ;
        
        offset_branch_var = 0 ;
        
        mem_data = RADR2_DATA.read() ;
        inc_pc = 1 ;

    }
    
    if(b_type_inst == 1)
    {
        radr1 = if_ir.range(19,15) ;
        radr2 = if_ir.range(24,20) ;

        adr_dest = 0 ;

        dec2exe_op1 = RADR1_DATA.read() ;
        dec2exe_op2 = 
        
        //Offset must be *4, so he's shift by 2 on the left

        offset_branch_var.range(31,15)  = if_ir.range(31,31) ;
        offset_branch_var.range(14,14)  = if_ir.range(31,31);
        offset_branch_var.range(13,13)  = if_ir.range(7,7);
        offset_branch_var.range(12,7)   = if_ir.range(30,25);
        offset_branch_var.range(5,3)    = if_ir.range(11,8);
        offset_branch_var.range(2,0)    = 0 ;
        mem_data = 0 ;

        //for branch instruction inc_pc is setup depending of if the branch condition is allowed or no
    }
    
    if(u_type_inst == 1)
    {
        radr1 = 0 ;
        adr_dest    = if_ir.range(11,7) ;
        
        dec2exe_op1.range(31,12) = if_ir.range(31,12) ;
        dec2exe_op1.range(11,0) = 0 ;

        offset_branch_var = 0 ;

        if(auipc_i == 1) // on case of an auipc instruction we need to send PC+imm to rd so we need to get the value from r33
        {
            radr2 = 0x2F ; 
            dec2exe_op2 = RADR2_DATA.read() ;
        }
        else // if we don't do an auipc instruction
        {
            radr2 = 0 ; 
            dec2exe_op2 = 0 ; 
        }
        mem_data = 0 ;
        inc_pc = 1 ;
    }
    
    if(j_type_inst == 1)
    {
        radr1 = 0 ;
        radr2 = 0 ;
        adr_dest = if_ir.range(11,7) ;

        dec2exe_op1 = 0 ;
        dec2exe_op2 = 0 ;

        offset_branch_var.range(31,23)  = if_ir.range(31,31) ;
        offset_branch_var.range(22,22)  = if_ir.range(31,31) ;
        offset_branch_var.range(21,14)  = if_ir.range(19,12) ;
        offset_branch_var.range(13,13)  = if_ir.range(20,20) ;
        offset_branch_var.range(12,3)   = if_ir.range(30,21) ;
        offset_branch_var.range(2,0)    = 0 ;
        mem_data = 0 ;
        inc_pc = 0 ;

    }
    
    if(jalr_type_inst == 1)
    {
        radr1 = if_ir.range(19,15) ;
        radr2 = 0 ;

        adr_dest = if_ir.range(11,7) ;

        dec2exe_op1 = 0 ;
        dec2exe_op2 = 0 ;

        offset_branch_var.range(31,12) = if_ir.range(31,31) ;
        offset_branch_var.range(11,0) = if_ir.range(31,20) ;
        mem_data = 0 ;
        inc_pc = 0 ;

    }
    
    RADR1.write(radr1) ;
    RADR2.write(radr2) ;
    ADR_DEST.write(adr_dest) ;
    offset_branch.write(offset_branch_var) ;
    DEC2EXE_OP1.write(dec2exe_op1) ;
    DEC2EXE_OP2.write(dec2exe_op2) ;
    MEM_DATA.write(mem_data) ;
    INC_PC.write(inc_pc) ;
}

void decod::affectation_calcul()
{

    //We are going to setup commands sent to EXE here, so each if will be execute for one type of command :

    //CMD : +

    if(add_i & sub_i & addi_i & lw_i & lh_i & lhu_i & lb_i & lbu_i & sw_i & sh_i & sb_i & auipc_i)
    {
        DEC2EXE_CMD.write(0) ;
        SELECT_SHIFT.write(0) ;
        if(sub_i)
        {
            DEC2EXE_NEG_OP1.write(1) ;
        }

    }

    //CMD : &
    if(and_i & and_i)
    {
        DEC2EXE_CMD.write(1) ;
        SELECT_SHIFT.write(0) ;

    }
    //CMD : |
    if(or_i & ori_i)
    {
        DEC2EXE_CMD.write(2) ;
        SELECT_SHIFT.write(0) ;
    }
    //CMD : ^
    if(xor_i & xori_i)
    {
        DEC2EXE_CMD.write(3) ;
        SELECT_SHIFT.write(0) ;
    }
    //SHIFT : SLL
    if(sll_i & slli_i)
    {
        DEC2EXE_CMD.write(0) ;
        SELECT_SHIFT.write(1) ;

    }
    //SHIFT : SRL
    if(srl_i & srli_i)
    {
        DEC2EXE_CMD.write(1) ;
        SELECT_SHIFT.write(1) ;

    }
    //SHIFT SRA
    if(sra_i & srai_i)
    {
        DEC2EXE_CMD.write(2) ;
        SELECT_SHIFT.write(1) ;

    }
} 

void decod::pc_inc()
{
    sc_uint<32> pc = READ_PC.read() ;
    sc_uint<32> pc_out ;
    sc_uint<32> offset_branch_var = offset_branch.read() ;

    if(inc_pc & READ_PC_VALID.read())
    {
        pc_out = pc + 4 ;
    }
    else if (! inc_pc  & READ_PC_VALID.read())
    {
        pc_out = pc + offset_branch_var ;
    }
    DEC2IF_PC.write(pc_out) ;
}