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

    if(if_ir.range(6,7) == 0110011) r_type_inst = 1 ;
    else if(if_ir.range(6,7) == 0110011) r_type_inst = 1 ;
    else if(if_ir.range(6,7) == 0010011) i_type_inst = 1 ;
    else if(if_ir.range(6,7) == 0010011) s_type_inst = 1 ;
    else if(if_ir.range(6,7) == 1100011) b_type_inst = 1 ;
    else if(if_ir.range(6,7) == 0110111) u_type_inst = 1 ;
    else if(if_ir.range(6,7) == 1101111) j_type_inst = 1 ;
    //Carefull opcode for JALR INST ARE : 1100111
    //Carefull opcode for s and i instruction are the same
    

}

void decod::decoding_instruction()
{
    
}