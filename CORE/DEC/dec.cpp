#include "dec.h"


// ---------------------------------------------METHODS FOR DEC2IF GESTION :---------------------------------------------


void decod::dec2if_gestion()
{  
    if(READ_PC_VALID.read() && !dec2if_full.read())
    {
        dec2if_push.write(1) ;
    }
    else
    {
        dec2if_push.write(0) ;
    }
    DEC2IF_EMPTY.write(dec2if_empty.read()) ;
}

// ---------------------------------------------METHODS FOR IF2DEC GESTION :---------------------------------------------

void decod::if2dec_pop_method()
{
    if (add_offset_to_pc.read()) {
        IF2DEC_POP.write(1) ;
        IF2DEC_FLUSH.write(1);
    }
    else if(RADR1_VALID.read() && RADR2_VALID.read() && ! IF2DEC_EMPTY.read() && !dec2exe_full.read())
    {
        IF2DEC_POP.write(1) ;
        IF2DEC_FLUSH.write(0);
    }
    else {
        IF2DEC_POP.write(0) ;
        IF2DEC_FLUSH.write(0);
    }
}

// ---------------------------------------------METHODS FOR DEC2EXE GESTION :---------------------------------------------

void decod::dec2exe_push_method()
{
    if(! RADR1_VALID.read() || ! RADR2_VALID.read() || dec2exe_full.read() || IF2DEC_EMPTY.read())
    {
        dec2exe_push.write(0) ; 
    }
    else 
    {
        dec2exe_push.write(1) ;
    }

}

void decod::concat_dec2exe()
{
    sc_bv<112> dec2exe_in_var ;
    dec2exe_in_var.range(111,80) = dec2exe_op1.read() ;
    dec2exe_in_var.range(79,48)  = dec2exe_op2.read() ;
    dec2exe_in_var[47]  = dec2exe_neg_op1.read() ;
    dec2exe_in_var[46]  = dec2exe_wb.read() ;

    dec2exe_in_var.range(45,14)  = mem_data.read() ;

    dec2exe_in_var[13] = mem_load.read() ;
    dec2exe_in_var[12] = mem_store.read() ;

    dec2exe_in_var[11]   = mem_sign_extend.read() ;
    dec2exe_in_var.range(10,9)   = mem_size.read() ;
    dec2exe_in_var[8]   = select_shift.read() ;
    dec2exe_in_var.range(7,2)   = adr_dest.read() ;
    dec2exe_in_var[1]   = slt_i.read() | slti_i.read() ;
    dec2exe_in_var[0]   = sltu_i.read() | sltiu_i.read() ;
    

    dec2exe_in.write(dec2exe_in_var) ;

}

void decod::unconcat_dec2exe()
{
    sc_bv<112> dec2exe_out_var = DEC2EXE_OUT.read() ;

    DEC2EXE_OP1.write((sc_bv_base) dec2exe_out_var.range(111,80)) ;
    DEC2EXE_OP2.write((sc_bv_base)dec2exe_out_var.range(79,48)) ;
    DEC2EXE_NEG_OP1.write((bool)dec2exe_out_var[47]) ;
    DEC2EXE_WB.write((bool)dec2exe_out_var[46]) ;

    MEM_DATA.write((sc_bv_base)dec2exe_out_var.range(45,14)) ;

    MEM_LOAD.write((bool)dec2exe_out_var[13]) ;
    MEM_STORE.write((bool)dec2exe_out_var[12]) ;

    MEM_SIGN_EXTEND.write((bool)dec2exe_out_var[11])   ;
    MEM_SIZE.write((sc_bv_base)dec2exe_out_var.range(10,9))   ;
    SELECT_SHIFT.write((bool)dec2exe_out_var[8])   ;
    EXE_DEST.write((sc_bv_base)dec2exe_out_var.range(7,2))   ;
    SLT.write((bool)dec2exe_out_var[1])   ;
    SLTU.write((bool)dec2exe_out_var[0])   ;
}

//---------------------------------------------INSTRUCTION TYPE DETECTION :---------------------------------------------


void decod::decoding_instruction_type()
{
    sc_uint<32> if_ir = IF_IR.read() ;
    r_type_inst     = if_ir.range(6,0) == 0b0110011 ? 1 : 0;
    i_type_inst     = (if_ir.range(6,0) == 0b0010011 | if_ir.range(6,0) == 0b0000011) ? 1 : 0; 
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

    if(if_ir.range(6,0) == 0b0110011 && if_ir.range(31,25) == 0b0000000 && if_ir.range(14,12) == 0b000) add_i.write(1) ;
    else add_i.write(0) ;
    if(if_ir.range(6,0) == 0b0110011 && if_ir.range(31,25) == 0b0000000 && if_ir.range(14,12) == 0b010) slt_i.write(1) ;
    else slt_i.write(0) ;
    if(if_ir.range(6,0) == 0b0110011 && if_ir.range(31,25) == 0b0000000 && if_ir.range(14,12) == 0b011) sltu_i.write(1) ;
    else sltu_i.write(0) ;
    if(if_ir.range(6,0) == 0b0110011 && if_ir.range(31,25) == 0b0000000 && if_ir.range(14,12) == 0b111) and_i.write(1) ;
    else and_i.write(0) ;
    if(if_ir.range(6,0) == 0b0110011 && if_ir.range(31,25) == 0b0000000 && if_ir.range(14,12) == 0b110) and_i.write(1) ;
    else and_i.write(0) ;
    if(if_ir.range(6,0) == 0b0110011 && if_ir.range(31,25) == 0b0000000 && if_ir.range(14,12) == 0b100) xor_i.write(1) ;
    else xor_i.write(0) ;
    if(if_ir.range(6,0) == 0b0110011 && if_ir.range(31,25) == 0b0000000 && if_ir.range(14,12) == 0b001) xor_i.write(1) ;
    else xor_i.write(0) ;
    if(if_ir.range(6,0) == 0b0110011 && if_ir.range(31,25) == 0b0000000 && if_ir.range(14,12) == 0b101) srl_i.write(1) ;
    else srl_i.write(0) ;
    if(if_ir.range(6,0) == 0b0110011 && if_ir.range(31,25) == 0b0100000 && if_ir.range(14,12) == 0b000) sub_i.write(1) ;
    else sub_i.write(0) ;
    if(if_ir.range(6,0) == 0b0110011 && if_ir.range(31,25) == 0b0100000 && if_ir.range(14,12) == 0b101) sra_i.write(1) ;
    else sra_i.write(0) ;

    //I-type Instructions :

    if(if_ir.range(6,0) == 0b0010011  && if_ir.range(14,12) == 0b000) addi_i.write(1) ;
    else addi_i.write(0) ;
    if(if_ir.range(6,0) == 0b0010011  && if_ir.range(14,12) == 0b010) slti_i.write(1) ;
    else slti_i.write(0) ;
    if(if_ir.range(6,0) == 0b0010011  && if_ir.range(14,12) == 0b011) sltiu_i.write(1) ;
    else sltiu_i.write(0) ;
    if(if_ir.range(6,0) == 0b0010011  && if_ir.range(14,12) == 0b111) andi_i.write(1) ;
    else andi_i.write(0) ;
    if(if_ir.range(6,0) == 0b0010011  && if_ir.range(14,12) == 0b110) ori_i.write(1) ;
    else ori_i.write(0) ;
    if(if_ir.range(6,0) == 0b0010011  && if_ir.range(14,12) == 0b100) xori_i.write(1) ;
    else xori_i.write(0) ;

    // I-type shift instructions :

    if(if_ir.range(6,0) == 0b0010011 && if_ir.range(31,25) == 0b0000000 && if_ir.range(14,12) == 0b001) slli_i.write(1) ;
    else slli_i.write(0) ;
    if(if_ir.range(6,0) == 0b0010011 && if_ir.range(31,25) == 0b0000000 && if_ir.range(14,12) == 0b101) srli_i.write(1) ;
    else srli_i.write(0) ;
    if(if_ir.range(6,0) == 0b0010011 && if_ir.range(31,25) == 0b0100000 && if_ir.range(14,12) == 0b101) srai_i.write(1) ;
    else srai_i.write(0) ;

    // I-type load instructions :

    if(if_ir.range(6,0) == 0b0000011 && if_ir.range(14,12) == 0b010) lw_i.write(1) ;
    else lw_i.write(0) ;
    if(if_ir.range(6,0) == 0b0000011 && if_ir.range(14,12) == 0b001) lh_i.write(1) ;
    else lh_i.write(0) ;
    if(if_ir.range(6,0) == 0b0000011 && if_ir.range(14,12) == 0b101) lhu_i.write(1) ;
    else lhu_i.write(0) ;
    if(if_ir.range(6,0) == 0b0000011 && if_ir.range(14,12) == 0b000) lb_i.write(1) ;
    else lhu_i.write(0) ;
    if(if_ir.range(6,0) == 0b0000011 && if_ir.range(14,12) == 0b100) lbu_i.write(1) ;
    else lbu_i.write(0) ;

    //B-type Instructions :


    if(if_ir.range(6,0) == 0b1100011  && if_ir.range(14,12) == 0b000) beq_i.write(1) ;
    else beq_i.write(0) ;
    if(if_ir.range(6,0) == 0b1100011  && if_ir.range(14,12) == 0b001) bne_i.write(1) ;
    else bne_i.write(0) ;
    if(if_ir.range(6,0) == 0b1100011  && if_ir.range(14,12) == 0b100) blt_i.write(1) ;
    else blt_i.write(0) ;
    if(if_ir.range(6,0) == 0b1100011  && if_ir.range(14,12) == 0b101) bge_i.write(1) ;
    else bge_i.write(0) ;
    if(if_ir.range(6,0) == 0b1100011  && if_ir.range(14,12) == 0b110) bltu_i.write(1) ;
    else bltu_i.write(0) ;
    if(if_ir.range(6,0) == 0b1100011  && if_ir.range(14,12) == 0b111) bgeu_i.write(1) ;
    else bgeu_i.write(0) ;

    //U-type Instructions :

    if(if_ir.range(6,0) == 0b0110111  ) lui_i.write(1) ;
    else lui_i.write(0) ;
    if(if_ir.range(6,0) == 0b0010111  ) auipc_i.write(1) ;
    else auipc_i.write(0) ;

    //J-type Instructions :

    if(if_ir.range(6,0) == 0b1101111 ) jal_i.write(1) ;
    else jal_i.write(0) ;
    if(if_ir.range(6,0) == 0b1100111 ) jalr_i.write(1) ;
    else jalr_i.write(0) ;

    //S-type Instructions :

    if(if_ir.range(6,0) == 0b0100011 && if_ir.range(14,12) == 0b010) sw_i.write(1) ;
    else sw_i.write(0) ;
    if(if_ir.range(6,0) == 0b0100011 && if_ir.range(14,12) == 0b001) sh_i.write(1) ;
    else sh_i.write(0) ;
    if(if_ir.range(6,0) == 0b0100011 && if_ir.range(14,12) == 0b000) sb_i.write(1) ;
    else sb_i.write(0) ;

    
}

//---------------------------------------------REGISTRE & OPERAND DETECTION :---------------------------------------------


void decod::affectation_registres() 
{
    
    sc_uint<32> if_ir = IF_IR.read() ;
    sc_uint<6> radr1_var;
    sc_uint<6> radr2_var;
    sc_uint<6> adr_dest_var;
    sc_uint<32> dec2exe_op1_var;
    sc_uint<32> dec2exe_op2_var;
    sc_uint<32> offset_branch_var ;
    sc_uint<32> mem_data_var ;
    bool inval_adr_dest ;
    bool inc_pc_var ;
    bool invalid_instr = false;

    //R-type Instruction :

    if(r_type_inst == 1)
    {
        radr1_var       = if_ir.range(19,15) ;
        radr2_var       = if_ir.range(24,20) ;
        adr_dest_var    = if_ir.range(11,7) ;
        
        dec2exe_op1_var = (RADR1_DATA.read()) ;
        dec2exe_op2_var = (RADR2_DATA.read()) ;

        offset_branch_var = 0 ;
        mem_data_var = 0 ;
        inc_pc_var = 1 ;
        inval_adr_dest = true ;
    }

    //I-type Instruction :

    else if(i_type_inst == 1)
    {
        radr1_var                       =   if_ir.range(19,15) ;
        radr2_var                       =   0 ;
        adr_dest_var                    =   if_ir.range(11,7) ;

        dec2exe_op1_var                 =   RADR1_DATA.read() ;

        //OP2 is sign extended
        if(if_ir.range(31,31) == 1)
        {
            dec2exe_op2_var.range(31,12) = 0b11111111111111111111 ;
        }
        else
        {
            dec2exe_op2_var.range(31,12) = 0b00000000000000000000 ;
        }
        dec2exe_op2_var.range(11,0)     =   if_ir.range(31,20) ; 
        
        offset_branch_var = 0 ;
        mem_data_var = 0 ;
        inc_pc_var = 1 ;
        inval_adr_dest = true ;


    }

    //S-type Instruction :

    else if(s_type_inst == 1)
    {
        //The adress is obtained by adding rs1 to the 12 bit immediat sign extended 
        // rs2 is copied to the memory

        radr1_var                       =   if_ir.range(19,15) ;
        radr2_var                       =   if_ir.range(24,20) ;
        adr_dest_var                    =   0 ;

        dec2exe_op1_var                 =   RADR1_DATA.read() ;

        if(if_ir.range(31,31) == 1)
        {
            dec2exe_op2_var.range(31,12) = 0b11111111111111111111 ;
        }
        else
        {
            dec2exe_op2_var.range(31,12) = 0b00000000000000000000 ;
        }
        dec2exe_op2_var.range(11,5)     = if_ir.range(31,25) ;
        dec2exe_op2_var.range(4,0)      = if_ir.range(11,7) ;
        
        offset_branch_var = 0 ;
        
        mem_data_var = RADR2_DATA.read() ;
        inc_pc_var = 1 ;
        inval_adr_dest = false ;

    }
    
    //Branch Instruction :

    else if(b_type_inst == 1)
    {
        radr1_var = if_ir.range(19,15) ;
        radr2_var = if_ir.range(24,20) ;

        adr_dest_var = 0 ;

        dec2exe_op1_var = RADR1_DATA.read() ;
        dec2exe_op2_var = RADR2_DATA.read() ;
        
        //Offset must be *4, so he's shift by 2 on the left

        if(if_ir.range(31,31) == 1)
        {
            offset_branch_var.range(31,15) = 0b111111111111111111 ;
        }
        else
        {
            offset_branch_var.range(31,15) = 0b000000000000000000 ;
        }
        offset_branch_var.range(12,12)  = if_ir.range(31,31);
        offset_branch_var.range(11,11)  = if_ir.range(7,7);
        offset_branch_var.range(10,5)   = if_ir.range(30,25);
        offset_branch_var.range(4, 1)    = if_ir.range(11,8);
        offset_branch_var.range(0,0)    = 0;
        mem_data_var = 0 ;
        inval_adr_dest = false ;

        /*BRANCH CONDITION GESTION : */

        sc_uint<32> res = dec2exe_op1_var ^ dec2exe_op2_var ;
        sc_uint<33> res_comparaison ;
        res_comparaison = dec2exe_op1_var - dec2exe_op2_var ;

        if(bne_i.read())
        {  
            inc_pc_var = ((res == 0x0 ? 1 : 0)) ; 
        }
        else if(beq_i.read())
        {  
            inc_pc_var = ((res == 0x0 ? 0 : 1)) ; 
        }
        else if(blt_i.read())
        {  
            inc_pc_var = ((res_comparaison.range(32,32) == 1 | res_comparaison.range(31,31) == 1) ? 0 : 1) ; // if bit 31 == 1, it means rs1 < rs2 
        }
        else if(bltu_i.read())
        {  
            inc_pc_var = ((res_comparaison.range(32,32) == 1 | res_comparaison.range(31,31) == 1) ? 1 : 0) ; 
        }
        else if(bge_i.read())
        {  
            inc_pc_var = ((res_comparaison.range(32,32) == 0 && res_comparaison.range(31,31) == 0) ? 0 : 1) ; // if bit 31 == 1, it means rs1 < rs2 
        }
        else if(bgeu_i.read())
        {  
            inc_pc_var = ((res_comparaison.range(32,32) == 0 && res_comparaison.range(31,31) == 0) ? 1 : 0) ; 
        }

    }
    
    //U-type Instruction :

    else if(u_type_inst == 1)
    {
        radr1_var = 0 ;
        adr_dest_var    = if_ir.range(11,7) ;
        
        dec2exe_op1_var.range(31,12) = if_ir.range(31,12) ;
        dec2exe_op1_var.range(11,0) = 0 ;

        offset_branch_var = 0 ;

        if(auipc_i == 1) // on case of an auipc instruction we need to send PC+imm to rd so we need to get the value from r33
        {
            radr2_var = 0x2F ; 
            dec2exe_op2_var = RADR2_DATA.read() ;
        }
        else // if we don't do an auipc instruction
        {
            radr2_var = 0 ; 
            dec2exe_op2_var = 0 ; 
        }
        mem_data_var = 0 ;
        inc_pc_var = 1 ;
        inval_adr_dest = true ;
    }
    
    //J-type Instruction :

    else if(j_type_inst == 1)
    {
        radr1_var = 0 ;
        radr2_var = 0 ;
        adr_dest_var = if_ir.range(11,7) ;

        dec2exe_op1_var = 0x0 ;
        dec2exe_op2_var = 0x0 ;


        if(if_ir.range(31,31) == 1)
        {
            offset_branch_var.range(31,23) = 0b111111111 ;
        }
        else
        {
            offset_branch_var.range(31,23) = 0b000000000 ;
        }
        offset_branch_var.range(20,20)  = if_ir.range(31,31) ;
        offset_branch_var.range(19,12)  = if_ir.range(19,12) ;
        offset_branch_var.range(11,11)  = if_ir.range(20,20) ;
        offset_branch_var.range(10,1)   = if_ir.range(30,21) ;
        offset_branch_var.range(0,0)    = 0 ;
        mem_data_var = 0 ;
        inc_pc_var = 0 ;
        inval_adr_dest = true ;

    }

    //JALR-type Instruction :
    
    else if(jalr_type_inst == 1)
    {
        radr1_var = if_ir.range(19,15) ;
        radr2_var = 0 ;

        adr_dest_var = if_ir.range(11,7) ;

        dec2exe_op1_var = 0 ;
        dec2exe_op2_var = 0 ;
        
        if(if_ir.range(31,31) == 1)
        {
            offset_branch_var.range(31,12) = 0b11111111111111111111 ;
        }
        else
        {
            offset_branch_var.range(31,12) = 0b00000000000000000000 ;
        }
        offset_branch_var.range(11,0) = if_ir.range(31,20) ;
        mem_data_var = 0 ;
        inc_pc_var = 0 ;
        inval_adr_dest = true ;

    }
    //Default case :
    else
    {
        radr1_var = 0 ;
        radr2_var = 0 ;
        adr_dest_var = 0 ;
        dec2exe_op1_var = 0 ;
        dec2exe_op2_var = 0 ;
        offset_branch_var = 0 ;
        mem_data_var = 0 ;
        inc_pc_var = 0 ;
        inval_adr_dest = false ;
        invalid_instr = true;
    }
    
    invalid_instr = invalid_instr || IF2DEC_EMPTY.read();

    RADR1.write(radr1_var) ;
    RADR2.write(radr2_var) ;
    adr_dest.write(adr_dest_var) ;
    INVAL_DEST.write(adr_dest_var);
    offset_branch.write(offset_branch_var) ;
    dec2exe_op1.write(dec2exe_op1_var) ;
    dec2exe_op2.write(dec2exe_op2_var) ;
    mem_data.write(mem_data_var) ;
    inc_pc.write((inc_pc_var && dec2if_push.read()) || invalid_instr) ;
    add_offset_to_pc.write(!inc_pc_var && dec2if_push.read() && RADR1_VALID.read() && RADR2_VALID.read() && ! invalid_instr);
}



//---------------------------------------------EXE & MEM SIGNAL DETECTION :---------------------------------------------



void decod::affectation_calcul()
{

    //We are going to setup commands sent to EXE here, so each if will be execute for one type of command :

    //CMD : +
    bool dec2exe_wb_var;
    if(add_i | sub_i | addi_i | lw_i | lh_i | lhu_i | lb_i | lbu_i | sw_i | sh_i | sb_i | auipc_i | lui_i | slti_i | slt_i | sltiu_i | sltu_i)
    {
        dec2exe_cmd.write(0) ;
        select_shift.write(0) ;

        //NEG OP1 GESTION :

        if(sub_i | slt_i | slti_i | sltu_i | sltiu_i)
        {
            dec2exe_neg_op1.write(1) ;
        }
        else
        {
            dec2exe_neg_op1.write(0) ;
        }

        //WBK GESTION :

        if(add_i | sub_i | addi_i | lw_i | lh_i | lhu_i | lb_i | lbu_i | auipc_i | lui_i | slt_i | slti_i | sltu_i | sltiu_i) dec2exe_wb_var = 1 ;
        else if(sw_i | sh_i | sb_i)  dec2exe_wb_var = 0 ;
        
        //MEMORY GESTION :

        //LOAD :

        if(lw_i | lh_i | lhu_i | lb_i | lbu_i) 
        {
            mem_load.write(1) ;
            if(lw_i)
            {
                mem_size.write(0) ;
                mem_sign_extend.write(0) ;
            }
            else if(lh_i | lhu_i)
            {
                mem_size.write(1) ;
                if(lhu_i) mem_sign_extend.write(1) ;
                else mem_sign_extend.write(0) ;
            }
            else if(lb_i | lbu_i)
            {
                mem_size.write(2) ;
                if(lbu_i) mem_sign_extend.write(1) ;
                else mem_sign_extend.write(0) ;
            }
        }
        else
        {
            mem_load.write(0) ;
            mem_sign_extend.write(0) ;
        }

        //STORE :

        if(sw_i | sh_i | sb_i) 
        {
            mem_store.write(1) ;
            if(sw_i)
            {
                mem_size.write(0) ;
            }
            else if(sh_i)
            {
                mem_size.write(1) ;
            }
            else if(sb_i)
            {
                mem_size.write(2) ;
            }
        }
        else
        {
            mem_store.write(0) ;
        }



    }

    //CMD : &
    else if(and_i | and_i)
    {
        dec2exe_cmd.write(1) ;
        dec2exe_neg_op1.write(0) ;
        dec2exe_wb_var = 1 ;
        mem_load.write(0) ;
        mem_store.write(0) ;
        mem_sign_extend.write(0) ;
        mem_size.write(0) ;

        select_shift.write(0) ;

    }
    //CMD : |
    else if(or_i | ori_i)
    {
        dec2exe_cmd.write(2) ;
        dec2exe_neg_op1.write(0) ;
        dec2exe_wb_var = 1 ;
        mem_load.write(0) ;
        mem_store.write(0) ;
        mem_sign_extend.write(0) ;
        mem_size.write(0) ;
        select_shift.write(0) ;
    }
    //CMD : ^
    else if(xor_i | xori_i)
    {
        dec2exe_cmd.write(3) ;
        dec2exe_neg_op1.write(0) ;
        dec2exe_wb_var = 1 ;
        mem_load.write(0) ;
        mem_store.write(0) ;
        mem_sign_extend.write(0) ;
        mem_size.write(0) ;
        select_shift.write(0) ;
    }
    //SHIFT : SLL
    else if(sll_i | slli_i)
    {
        dec2exe_cmd.write(0) ;
        dec2exe_neg_op1.write(0) ;
        dec2exe_wb_var = 1 ;
        mem_load.write(0) ;
        mem_store.write(0) ;
        mem_sign_extend.write(0) ;
        mem_size.write(0) ;
        select_shift.write(1) ;

    }
    //SHIFT : SRL
    else if(srl_i | srli_i)
    {
        dec2exe_cmd.write(1) ;
        dec2exe_neg_op1.write(0) ;
        dec2exe_wb_var = 1 ;
        mem_load.write(0) ;
        mem_store.write(0) ;
        mem_sign_extend.write(0) ;
        mem_size.write(0) ;
        select_shift.write(1) ;

    }
    //SHIFT SRA
    else if(sra_i | srai_i)
    {
        dec2exe_cmd.write(2) ;
        dec2exe_neg_op1.write(0) ;
        dec2exe_wb_var = 1 ;
        mem_load.write(0) ;
        mem_store.write(0) ;
        mem_sign_extend.write(0) ;
        mem_size.write(0) ;
        select_shift.write(1) ;

    }
    else
    {
        dec2exe_cmd.write(0) ;
        dec2exe_neg_op1.write(0) ;
        dec2exe_wb_var = 0 ;
        mem_load.write(0) ;
        mem_store.write(0) ;
        mem_sign_extend.write(0) ;
        mem_size.write(0) ;
        select_shift.write(0) ;   
    }
    INVAL_ENABLE.write(dec2exe_wb_var);
    dec2exe_wb.write(dec2exe_wb_var);
} 

//---------------------------------------------PC GESTION :---------------------------------------------

void decod::pc_inc()
{
    sc_uint<32> pc = READ_PC.read() ;
    sc_uint<32> pc_out = pc;
    sc_uint<32> offset_branch_var = offset_branch.read() ;

    if(inc_pc && READ_PC_VALID.read())
    {
        pc_out = pc + 4 ;
        WRITE_PC.write(pc_out);
        WRITE_PC_ENABLE.write(1);
    }
    else if (!inc_pc  && READ_PC_VALID.read() && add_offset_to_pc.read())
    {
        pc_out = pc + offset_branch_var - 4 ;
        WRITE_PC.write(pc_out);
        WRITE_PC_ENABLE.write(1);
    }
    else {
        WRITE_PC_ENABLE.write(0);
    }
    dec2if_pc_in.write(pc_out) ;
}



//---------------------------------------------METHOD TO TRACE SIGNALS :---------------------------------------------


void decod::trace(sc_trace_file* tf)
{
    dec2if.trace(tf);
    dec2exe.trace(tf); 
    sc_trace(tf,RADR1_DATA,GET_NAME(RADR1_DATA)); 
    sc_trace(tf,RADR2_DATA,GET_NAME(RADR2_DATA));
    sc_trace(tf,RADR1_VALID,GET_NAME(RADR1_VALID)); 
    sc_trace(tf,RADR2_VALID,GET_NAME(RADR2_VALID)); 
    sc_trace(tf,RADR1,GET_NAME(RADR1)); 
    sc_trace(tf,RADR2,GET_NAME(RADR2)); 
    sc_trace(tf,EXE_DEST,GET_NAME(EXE_DEST)); 
    sc_trace(tf,READ_PC,GET_NAME(READ_PC)); 
    sc_trace(tf,READ_PC_VALID,GET_NAME(READ_PC_VALID));   
    sc_trace(tf,DEC2EXE_OP1,GET_NAME(DEC2EXE_OP1)); 
    sc_trace(tf,DEC2EXE_OP2,GET_NAME(DEC2EXE_OP2)); 
    sc_trace(tf,DEC2EXE_CMD,GET_NAME(DEC2EXE_CMD)); 
    sc_trace(tf,DEC2EXE_NEG_OP1,GET_NAME(DEC2EXE_NEG_OP1)); 
    sc_trace(tf,DEC2EXE_WB,GET_NAME(DEC2EXE_WB)); 
    sc_trace(tf,MEM_DATA,GET_NAME(MEM_DATA));
    sc_trace(tf,MEM_LOAD,GET_NAME(MEM_LOAD)); 
    sc_trace(tf,MEM_STORE,GET_NAME(MEM_STORE)); 
    sc_trace(tf,MEM_SIGN_EXTEND,GET_NAME(MEM_SIGN_EXTEND)); 
    sc_trace(tf,MEM_SIZE,GET_NAME(MEM_SIZE)); 
    sc_trace(tf,SELECT_SHIFT,GET_NAME(SELECT_SHIFT)); 
    sc_trace(tf,DEC2IF_POP,GET_NAME(DEC2IF_POP)); 
    sc_trace(tf,DEC2IF_EMPTY,GET_NAME(DEC2IF_EMPTY));
    sc_trace(tf,DEC2IF_PC,GET_NAME(DEC2IF_PC));
    sc_trace(tf,IF_IR,GET_NAME(IF_IR));
    sc_trace(tf,IF2DEC_EMPTY,GET_NAME(IF2DEC_EMPTY));
    sc_trace(tf,IF2DEC_POP,GET_NAME(IF2DEC_POP)); 
    sc_trace(tf,DEC2EXE_POP,GET_NAME(DEC2EXE_POP));
    sc_trace(tf,DEC2EXE_EMPTY,GET_NAME(DEC2EXE_EMPTY));
    sc_trace(tf,DEC2EXE_OUT,GET_NAME(DEC2EXE_OUT));
    sc_trace(tf,CLK,GET_NAME(CLK));
    sc_trace(tf,RESET_N,GET_NAME(RESET_N));
    sc_trace(tf,dec2if_pc_in,GET_NAME(dec2if_pc_in)); 
    sc_trace(tf,dec2if_push,GET_NAME(dec2if_push));
    sc_trace(tf,dec2if_empty,GET_NAME(dec2if_empty));
    sc_trace(tf,dec2if_full,GET_NAME(dec2if_full));
    sc_trace(tf,dec2if_pc_out,GET_NAME(dec2if_pc_out));
    sc_trace(tf,dec2exe_in,GET_NAME(dec2exe_in));
    sc_trace(tf,dec2exe_push,GET_NAME(dec2exe_push));
    sc_trace(tf,dec2exe_full,GET_NAME(dec2exe_full));
    sc_trace(tf,r_type_inst,GET_NAME(r_type_inst)); 
    sc_trace(tf,i_type_inst,GET_NAME(i_type_inst)); 
    sc_trace(tf,s_type_inst,GET_NAME(s_type_inst)); 
    sc_trace(tf,b_type_inst,GET_NAME(b_type_inst)); 
    sc_trace(tf,u_type_inst,GET_NAME(u_type_inst)); 
    sc_trace(tf,j_type_inst,GET_NAME(j_type_inst)); 
    sc_trace(tf,jalr_type_inst,GET_NAME(jalr_type_inst)); 
    sc_trace(tf,add_i,GET_NAME(add_i));
    sc_trace(tf,slt_i,GET_NAME(slt_i));
    sc_trace(tf,sltu_i,GET_NAME(sltu_i));
    sc_trace(tf,and_i,GET_NAME(and_i));
    sc_trace(tf,or_i,GET_NAME(or_i));
    sc_trace(tf,xor_i,GET_NAME(xor_i));
    sc_trace(tf,sll_i,GET_NAME(sll_i));
    sc_trace(tf,srl_i,GET_NAME(srl_i));
    sc_trace(tf,sub_i,GET_NAME(sub_i));
    sc_trace(tf,sra_i,GET_NAME(sra_i));
    sc_trace(tf,addi_i,GET_NAME(addi_i));
    sc_trace(tf,slti_i,GET_NAME(slti_i));
    sc_trace(tf,sltiu_i,GET_NAME(sltiu_i));
    sc_trace(tf,andi_i,GET_NAME(andi_i));
    sc_trace(tf,ori_i,GET_NAME(ori_i));
    sc_trace(tf,xori_i,GET_NAME(xori_i));
    sc_trace(tf,jalr_i,GET_NAME(jalr_i));
    sc_trace(tf,slli_i,GET_NAME(slli_i));
    sc_trace(tf,srli_i,GET_NAME(srli_i));
    sc_trace(tf,srai_i,GET_NAME(srai_i));
    sc_trace(tf,lw_i,GET_NAME(lw_i));
    sc_trace(tf,lh_i,GET_NAME(lh_i));
    sc_trace(tf,lhu_i,GET_NAME(lhu_i));
    sc_trace(tf,lb_i,GET_NAME(lb_i));
    sc_trace(tf,lbu_i,GET_NAME(lbu_i));
    sc_trace(tf,beq_i,GET_NAME(beq_i));
    sc_trace(tf,bne_i,GET_NAME(bne_i));
    sc_trace(tf,blt_i,GET_NAME(blt_i));
    sc_trace(tf,bge_i,GET_NAME(bge_i));
    sc_trace(tf,bltu_i,GET_NAME(bltu_i));
    sc_trace(tf,bgeu_i,GET_NAME(bgeu_i));
    sc_trace(tf,lui_i,GET_NAME(lui_i));
    sc_trace(tf,auipc_i,GET_NAME(auipc_i));
    sc_trace(tf,jal_i,GET_NAME(jal_i));
    sc_trace(tf,sw_i,GET_NAME(sw_i));
    sc_trace(tf,sh_i,GET_NAME(sh_i));
    sc_trace(tf,sb_i,GET_NAME(sb_i));
    sc_trace(tf,offset_branch,GET_NAME(offset_branch));
    sc_trace(tf,inc_pc,GET_NAME(inc_pc));
    sc_trace(tf,adr_dest,GET_NAME(adr_dest));
    sc_trace(tf,dec2exe_op1,GET_NAME(dec2exe_op1));
    sc_trace(tf,dec2exe_op2,GET_NAME(dec2exe_op2));
    sc_trace(tf,offset_branch_var,GET_NAME(offset_branch_var));
    sc_trace(tf,mem_size,GET_NAME(mem_size));
    sc_trace(tf,mem_load,GET_NAME(mem_load));
    sc_trace(tf,mem_store,GET_NAME(mem_store));
    sc_trace(tf,dec2exe_cmd,GET_NAME(dec2exe_cmd));
    sc_trace(tf,select_shift,GET_NAME(select_shift));
    sc_trace(tf,dec2exe_neg_op1,GET_NAME(dec2exe_neg_op1));
    sc_trace(tf,dec2exe_wb,GET_NAME(dec2exe_wb));
    sc_trace(tf,mem_sign_extend,GET_NAME(mem_sign_extend));
    sc_trace(tf,WRITE_PC,GET_NAME(WRITE_PC));
    sc_trace(tf,WRITE_PC_ENABLE,GET_NAME(WRITE_PC_ENABLE));
    sc_trace(tf,INVAL_ENABLE,GET_NAME(INVAL_ENABLE));
    sc_trace(tf,INVAL_DEST,GET_NAME(INVAL_DEST));
    sc_trace(tf,add_offset_to_pc,GET_NAME(add_offset_to_pc));
    sc_trace(tf,IF2DEC_FLUSH, GET_NAME(IF2DEC_FLUSH));
}