#include "dec.h"

//PUSH AND POP MUST FREEZE IS DATA IS NOT VALID

void decod::pushingToIf()
{
    
}

void decod::popFromIf()
{

}

//---------------------------------------------INSTRUCTION TYPE DETECTION :---------------------------------------------


void decod::decoding_instruction_type()
{
    cerr << "enter decoding inst type " << endl ;
    sc_uint<32> if_ir = IF_IR.read() ;

    r_type_inst     = if_ir.range(6,0) == 0b0110011 ? 1 : 0;
    i_type_inst     = if_ir.range(6,0) == 0b0010011 ? 1 : 0; 
    s_type_inst     = if_ir.range(6,0) == 0b0100011 ? 1 : 0; 
    b_type_inst     = if_ir.range(6,0) == 0b1100011 ? 1 : 0;
    u_type_inst     = if_ir.range(6,0) == 0b0110111 ? 1 : 0;
    j_type_inst     = if_ir.range(6,0) == 0b1101111 ? 1 : 0;
    jalr_type_inst  = if_ir.range(6,0) == 0b1100111 ? 1 : 0;

}

//---------------------------------------------INSTRUCTION DETECTION :---------------------------------------------


void decod::decoding_instruction()
{
    sc_uint<32> if_ir = IF_IR.read() ;
    
    //R-type Instruction :

    if(if_ir.range(6,0) == 0b0110011 && if_ir.range(31,25) == 0b0000000 && if_ir.range(14,12) == 0b000) add_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b0110011 && if_ir.range(31,25) == 0b0000000 && if_ir.range(14,12) == 0b010) slt_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b0110011 && if_ir.range(31,25) == 0b0000000 && if_ir.range(14,12) == 0b011) sltu_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b0110011 && if_ir.range(31,25) == 0b0000000 && if_ir.range(14,12) == 0b111) and_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b0110011 && if_ir.range(31,25) == 0b0000000 && if_ir.range(14,12) == 0b110) or_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b0110011 && if_ir.range(31,25) == 0b0000000 && if_ir.range(14,12) == 0b100) xor_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b0110011 && if_ir.range(31,25) == 0b0000000 && if_ir.range(14,12) == 0b001) sll_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b0110011 && if_ir.range(31,25) == 0b0000000 && if_ir.range(14,12) == 0b101) srl_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b0110011 && if_ir.range(31,25) == 0b0100000 && if_ir.range(14,12) == 0b000) sub_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b0110011 && if_ir.range(31,25) == 0b0100000 && if_ir.range(14,12) == 0b101) sra_i = 1 ;
    else 0 ;

    //I-type Instructions :

    if(if_ir.range(6,0) == 0b0010011  && if_ir.range(14,12) == 0b000) addi_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b0010011  && if_ir.range(14,12) == 0b010) slti_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b0010011  && if_ir.range(14,12) == 0b011) sltiu_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b0010011  && if_ir.range(14,12) == 0b111) andi_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b0010011  && if_ir.range(14,12) == 0b110) ori_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b0010011  && if_ir.range(14,12) == 0b100) xori_i = 1 ;
    else 0 ;

    // I-type shift instructions :

    if(if_ir.range(6,0) == 0b0010011 && if_ir.range(31,25) == 0b0000000 && if_ir.range(14,12) == 0b001) slli_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b0010011 && if_ir.range(31,25) == 0b0000000 && if_ir.range(14,12) == 0b101) srli_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b0010011 && if_ir.range(31,25) == 0b0100000 && if_ir.range(14,12) == 0b101) srai_i = 1 ;
    else 0 ;

    // I-type load instructions :

    if(if_ir.range(6,0) == 0b0000011 && if_ir.range(14,12) == 0b010) lw_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b0000011 && if_ir.range(14,12) == 0b001) lh_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b0000011 && if_ir.range(14,12) == 0b101) lhu_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b0000011 && if_ir.range(14,12) == 0b000) lb_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b0000011 && if_ir.range(14,12) == 0b100) lbu_i = 1 ;
    else 0 ;

    //B-type Instructions :


    if(if_ir.range(6,0) == 0b1100011  && if_ir.range(14,12) == 0b000) beq_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b1100011  && if_ir.range(14,12) == 0b001) bne_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b1100011  && if_ir.range(14,12) == 0b100) blt_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b1100011  && if_ir.range(14,12) == 0b101) bge_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b1100011  && if_ir.range(14,12) == 0b110) bltu_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b1100011  && if_ir.range(14,12) == 0b111) bgeu_i = 1 ;
    else 0 ;

    //U-type Instructions :

    if(if_ir.range(6,0) == 0b0110111  ) lui_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b0010111  ) auipc_i = 1 ;
    else 0 ;

    //J-type Instructions :

    if(if_ir.range(6,0) == 0b1101111 ) jal_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b1100111 ) jalr_i = 1 ;
    else 0 ;

    //S-type Instructions :

    if(if_ir.range(6,0) == 0b0100011 && if_ir.range(14,12) == 0b010) sw_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b0100011 && if_ir.range(14,12) == 0b001) sh_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0b0100011 && if_ir.range(14,12) == 0b000) sb_i = 1 ;
    else 0 ;

    
}

//---------------------------------------------REGISTRE & OPERAND DETECTION :---------------------------------------------


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

    //R-type Instruction :

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

    //I-type Instruction :

    else if(i_type_inst == 1)
    {
        radr1                       =   if_ir.range(19,15) ;
        radr2                       =   0 ;
        adr_dest                    =   if_ir.range(11,7) ;

        dec2exe_op1                 =   RADR1_DATA.read() ;

        //OP2 is sign extended
        if(if_ir.range(31,31) == 1)
        {
            dec2exe_op2.range(31,12) = 0b11111111111111111111 ;
        }
        else
        {
            dec2exe_op2.range(31,12) = 0b00000000000000000000 ;
        }
        dec2exe_op2.range(11,0)     =   if_ir.range(31,20) ; 
        
        offset_branch_var = 0 ;
        mem_data = 0 ;
        inc_pc = 1 ;


    }

    //S-type Instruction :

    else if(s_type_inst == 1)
    {
        //The adress is obtained by adding rs1 to the 12 bit immediat sign extended 
        // rs2 is copied to the memory

        radr1                       =   if_ir.range(19,15) ;
        radr2                       =   if_ir.range(24,20) ;
        adr_dest                    =   0 ;

        dec2exe_op1                 =   RADR1_DATA.read() ;

        if(if_ir.range(31,31) == 1)
        {
            dec2exe_op2.range(31,12) = 0b11111111111111111111 ;
        }
        else
        {
            dec2exe_op2.range(31,12) = 0b00000000000000000000 ;
        }
        dec2exe_op2.range(11,5)     = if_ir.range(31,25) ;
        dec2exe_op2.range(4,0)      = if_ir.range(11,7) ;
        
        offset_branch_var = 0 ;
        
        mem_data = RADR2_DATA.read() ;
        inc_pc = 1 ;

    }
    
    //Branch Instruction :

    else if(b_type_inst == 1)
    {
        radr1 = if_ir.range(19,15) ;
        radr2 = if_ir.range(24,20) ;

        adr_dest = 0 ;

        dec2exe_op1 = RADR1_DATA.read() ;
        dec2exe_op2 = RADR2_DATA.read() ;
        
        //Offset must be *4, so he's shift by 2 on the left

        if(if_ir.range(31,31) == 1)
        {
            offset_branch_var.range(31,15) = 0b111111111111111111 ;
        }
        else
        {
            offset_branch_var.range(31,15) = 0b000000000000000000 ;
        }
        offset_branch_var.range(14,14)  = if_ir.range(31,31);
        offset_branch_var.range(13,13)  = if_ir.range(7,7);
        offset_branch_var.range(12,7)   = if_ir.range(30,25);
        offset_branch_var.range(5,3)    = if_ir.range(11,8);
        offset_branch_var.range(2,0)    = 0 ;
        mem_data = 0 ;

        //for branch instruction inc_pc is setup depending of if the branch condition is allowed or no
    }
    
    //U-type Instruction :

    else if(u_type_inst == 1)
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
    
    //J-type Instruction :

    else if(j_type_inst == 1)
    {
        radr1 = 0 ;
        radr2 = 0 ;
        adr_dest = if_ir.range(11,7) ;

        dec2exe_op1 = 0 ;
        dec2exe_op2 = 0 ;


        if(if_ir.range(31,31) == 1)
        {
            offset_branch_var.range(31,23) = 0b111111111 ;
        }
        else
        {
            offset_branch_var.range(31,23) = 0b000000000 ;
        }
        offset_branch_var.range(22,22)  = if_ir.range(31,31) ;
        offset_branch_var.range(21,14)  = if_ir.range(19,12) ;
        offset_branch_var.range(13,13)  = if_ir.range(20,20) ;
        offset_branch_var.range(12,3)   = if_ir.range(30,21) ;
        offset_branch_var.range(2,0)    = 0 ;
        mem_data = 0 ;
        inc_pc = 0 ;

    }

    //JALR-type Instruction :
    
    else if(jalr_type_inst == 1)
    {
        radr1 = if_ir.range(19,15) ;
        radr2 = 0 ;

        adr_dest = if_ir.range(11,7) ;

        dec2exe_op1 = 0 ;
        dec2exe_op2 = 0 ;
        
        if(if_ir.range(31,31) == 1)
        {
            offset_branch_var.range(31,12) = 0b11111111111111111111 ;
        }
        else
        {
            offset_branch_var.range(31,12) = 0b00000000000000000000 ;
        }
        offset_branch_var.range(11,0) = if_ir.range(31,20) ;
        mem_data = 0 ;
        inc_pc = 0 ;

    }
    //Default case :
    else
    {
        radr1 = 0 ;
        radr2 = 0 ;
        adr_dest = 0 ;
        dec2exe_op1 = 0 ;
        dec2exe_op2 = 0 ;
        offset_branch_var = 0 ;
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



//---------------------------------------------EXE & MEM SIGNAL DETECTION :---------------------------------------------



void decod::affectation_calcul()
{

    /*
    Missing Slt instructions
    */

    sc_uint<2> mem_size ;
    sc_uint<3> mem_load ;
    sc_uint<3> mem_store ;
    //We are going to setup commands sent to EXE here, so each if will be execute for one type of command :

    //CMD : +

    if(add_i | sub_i | addi_i | lw_i | lh_i | lhu_i | lb_i | lbu_i | sw_i | sh_i | sb_i | auipc_i)
    {
        DEC2EXE_CMD.write(0) ;
        SELECT_SHIFT.write(0) ;

        //NEG OP1 GESTION :

        if(sub_i)
        {
            DEC2EXE_NEG_OP1.write(1) ;
        }
        else
        {
            DEC2EXE_NEG_OP1.write(0) ;
        }

        //WBK GESTION :

        if(add_i | sub_i | addi_i | lw_i | lh_i | lhu_i | lb_i | lbu_i | auipc_i) DEC2EXE_WB.write(1) ;
        else if(sw_i | sh_i | sb_i)  DEC2EXE_WB.write(0) ;
        
        //MEMORY GESTION :

        //LOAD :

        if(lw_i | lh_i | lhu_i | lb_i | lbu_i) 
        {
            MEM_LOAD.write(1) ;
            if(lw_i)
            {
                MEM_SIZE.write(0) ;
                MEM_SIGN_EXTEND.write(0) ;
            }
            else if(lh_i | lhu_i)
            {
                MEM_SIZE.write(1) ;
                if(lhu_i) MEM_SIGN_EXTEND.write(1) ;
                else MEM_SIGN_EXTEND.write(0) ;
            }
            else if(lb_i | lbu_i)
            {
                MEM_SIZE.write(2) ;
                if(lbu_i) MEM_SIGN_EXTEND.write(1) ;
                else MEM_SIGN_EXTEND.write(0) ;
            }
        }
        else
        {
            MEM_LOAD.write(0) ;
            MEM_SIGN_EXTEND.write(0) ;
        }

        //STORE :

        if(sw_i | sh_i | sb_i) 
        {
            MEM_STORE.write(1) ;
            if(sw_i)
            {
                MEM_SIZE.write(0) ;
            }
            else if(sh_i)
            {
                MEM_SIZE.write(1) ;
            }
            else if(sb_i)
            {
                MEM_SIZE.write(2) ;
            }
        }
        else
        {
            MEM_STORE.write(0) ;
        }



    }

    //CMD : &
    else if(and_i | and_i)
    {
        DEC2EXE_CMD.write(1) ;
        DEC2EXE_NEG_OP1.write(0) ;
        DEC2EXE_WB.write(1) ;
        MEM_LOAD.write(0) ;
        MEM_STORE.write(0) ;
        MEM_SIGN_EXTEND.write(0) ;
        MEM_SIZE.write(0) ;

        SELECT_SHIFT.write(0) ;

    }
    //CMD : |
    else if(or_i | ori_i)
    {
        DEC2EXE_CMD.write(2) ;
        DEC2EXE_NEG_OP1.write(0) ;
        DEC2EXE_WB.write(1) ;
        MEM_LOAD.write(0) ;
        MEM_STORE.write(0) ;
        MEM_SIGN_EXTEND.write(0) ;
        MEM_SIZE.write(0) ;
        SELECT_SHIFT.write(0) ;
    }
    //CMD : ^
    else if(xor_i | xori_i)
    {
        DEC2EXE_CMD.write(3) ;
        DEC2EXE_NEG_OP1.write(0) ;
        DEC2EXE_WB.write(1) ;
        MEM_LOAD.write(0) ;
        MEM_STORE.write(0) ;
        MEM_SIGN_EXTEND.write(0) ;
        MEM_SIZE.write(0) ;
        SELECT_SHIFT.write(0) ;
    }
    //SHIFT : SLL
    else if(sll_i | slli_i)
    {
        DEC2EXE_CMD.write(0) ;
        DEC2EXE_NEG_OP1.write(0) ;
        DEC2EXE_WB.write(1) ;
        MEM_LOAD.write(0) ;
        MEM_STORE.write(0) ;
        MEM_SIGN_EXTEND.write(0) ;
        MEM_SIZE.write(0) ;
        SELECT_SHIFT.write(1) ;

    }
    //SHIFT : SRL
    else if(srl_i | srli_i)
    {
        DEC2EXE_CMD.write(1) ;
        DEC2EXE_NEG_OP1.write(0) ;
        DEC2EXE_WB.write(1) ;
        MEM_LOAD.write(0) ;
        MEM_STORE.write(0) ;
        MEM_SIGN_EXTEND.write(0) ;
        MEM_SIZE.write(0) ;
        SELECT_SHIFT.write(1) ;

    }
    //SHIFT SRA
    else if(sra_i | srai_i)
    {
        DEC2EXE_CMD.write(2) ;
        DEC2EXE_NEG_OP1.write(0) ;
        DEC2EXE_WB.write(1) ;
        MEM_LOAD.write(0) ;
        MEM_STORE.write(0) ;
        MEM_SIGN_EXTEND.write(0) ;
        MEM_SIZE.write(0) ;
        SELECT_SHIFT.write(1) ;

    }
} 

//---------------------------------------------PC GESTION :---------------------------------------------

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

