#include "dec.h"

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

    //S-Type Instructions :

    if(if_ir.range(6,0) == 0010011 && if_ir.range(31,25) == 0000000 && if_ir.range(14,12) == 001) slli_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0010011 && if_ir.range(31,25) == 0000000 && if_ir.range(14,12) == 101) srli_i = 1 ;
    else 0 ;
    if(if_ir.range(6,0) == 0010011 && if_ir.range(31,25) == 0100000 && if_ir.range(14,12) == 101) srai_i = 1 ;
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

    //Affectation RADR1 :

    
}

void decod::affectation()
{
    sc_uint<32> if_ir = IF_IR.read() ;
    sc_uint<6> radr1;
    sc_uint<6> radr2;
    sc_uint<6> adr_dest;
    sc_uint<32> dec2exe_op1;
    sc_uint<32> dec2exe_op2;


    if(r_type_inst == 1)
    {
        radr1       = if_ir.range(19,15) ;
        radr2       = if_ir.range(24,20) ;
        adr_dest    = if_ir.range(11,7) ;
        
        dec2exe_op1 = (RADR1_DATA.read()) ;
        dec2exe_op2 = (RADR2_DATA.read()) ;
    }
    if(i_type_inst == 1)
    {
        radr1                       =   if_ir.range(19,15) ;
        adr_dest                    =   if_ir.range(11,7) ;

        dec2exe_op1                 =   RADR1_DATA.read() ;

        dec2exe_op2.range(31,12)    =   if_ir.range(31,31) ; 
        dec2exe_op2.range(11,0)     =   if_ir.range(31,20) ;       

    }
    if(s_type_inst == 1)
    {
        radr1 = if_ir.range(19,15) ;
        radr2 = if_ir.range(24,20) ;

        dec2exe_op1 = RADR1_DATA.read() ;
        dec2exe_op2 = RADR2_DATA.read() ; 
    }
    
    if(b_type_inst == 1)
    {
        radr1 = if_ir.range(19,15) ;
        radr2 = if_ir.range(24,20) ;

        dec2exe_op1 = RADR1_DATA.read() ;
        dec2exe_op2 = RADR2_DATA.read() ; 
    }
    
    if(u_type_inst == 1)
    {
        adr_dest = if_ir.range(11,7) ;
        dec2exe_op1 = RADR1_DATA.read() ;
        dec2exe_op2 = RADR2_DATA.read() ; 
    }
    
    if(j_type_inst == 1)
    {
        adr_dest = if_ir.range(11,7) ;

        dec2exe_op1 = RADR1_DATA.read() ;
        dec2exe_op2 = RADR2_DATA.read() ; 
    }
    
    if(jalr_type_inst == 1)
    {

    }
    
    
}