#include <systemc.h>
#include "../UTIL/fifo_32b/fifo_32b.h"

SC_MODULE(decod)
{
    
    //Interface with REG :

    sc_in   < sc_uint<32> >       RADR1_DATA ; 
    sc_in   < sc_uint<32> >       RADR2_DATA ;
    sc_in   < bool >              RADR1_VALID ; // tells if the data read is valid or no
    sc_in   < bool >              RADR2_VALID ; // same but for rt

    sc_out  < sc_uint<6> >       RADR1 ; // adress of rs
    sc_out  < sc_uint<6> >       RADR2 ; // adress of rt

    sc_out  < sc_uint<6> >       ADR_DEST ; // rd

    sc_in   < sc_uint<32> >      READ_PC ; // value of r32 which is pc
    sc_out  < bool >             INC_PC ; // tells to reg if he does pc+4 or no
    sc_in   < bool >             READ_PC_VALID ; // say if pc is valid or no

    //Interface with EXE :

    sc_out  < sc_uint<32> >      DEC2EXE_OP1 ; // value of op1
    sc_out  < sc_uint<32> >      DEC2EXE_OP2 ; // value of op2
    sc_out  < sc_uint<4> >       DEC2EXE_CMD ; // value of the command sent to exe
    sc_out  < bool >             DEC2EXE_NEG_OP1 ; // say if we take the opposite of the op1 to do a substraction for example
    sc_out  < bool >             DEC2EXE_WB ; // say if we plan to wbk the value of rd or no
    sc_out  < sc_uint<3> >       MEM_LOAD ; // say to mem if we do a load
    sc_out  < sc_uint<3> >       MEM_STORE ; // say to mem if we do a store
    sc_out  < bool >             MEM_SIGN_EXTEND ;
    sc_out  < bool >             SELECT_SHIFT ; //taille fifo entrée : 110
    
    // Interface with DEC2IF : 

    sc_in   < bool >              DEC2IF_EMPTY ;
    sc_in   < bool >              DEC2IF_FULL ;
    sc_out  < bool >              DEC2IF_PUSH ;
    sc_out  < sc_uint<32> >       DEC2IF_PC ;

    //Interface with IF2DEC :

    sc_out  < bool >              IF2DEC_POP ;
    sc_in   < sc_uint<32> >       IF_IR ;
    sc_in   < bool >              IF2DEC_EMPTY ;
    sc_in   < bool >              IF2DEC_FULL ;

    //General Interface :
    sc_in_clk                     CLK ;
    sc_in  <bool>                 RESET_N ;


    //Instance used :
    
    fifo_32b dec2if ;

    // Signals :

    sc_signal < sc_uint<32> >   dec2if_pc_out ;
    sc_signal < bool >          dec2if_pop ;
    sc_signal < bool >          dec2if_empty ;
    sc_signal < bool >          dec2if_full ;

    // Instruction format type :

    sc_signal < bool >          r_type_inst ; // R type format
    sc_signal < bool >          i_type_inst ; // I type format
    sc_signal < bool >          s_type_inst ; // S type format
    sc_signal < bool >          b_type_inst ; // B type format
    sc_signal < bool >          u_type_inst ; // U type format
    sc_signal < bool >          j_type_inst ; // J type format
    sc_signal < bool >          jalr_type_inst ; //JALR has a specific opcode

    //R-type Instructions :


    sc_signal < bool > add_i ;
    sc_signal < bool > slt_i ;
    sc_signal < bool > sltu_i ;
    sc_signal < bool > and_i ;
    sc_signal < bool > or_i ;
    sc_signal < bool > xor_i ;
    sc_signal < bool > sll_i ;
    sc_signal < bool > srl_i ;
    sc_signal < bool > sub_i ;
    sc_signal < bool > sra_i ;

    //I-type Instructions :

    sc_signal < bool > addi_i ;
    sc_signal < bool > slti_i ;
    sc_signal < bool > sltiu_i ;
    sc_signal < bool > andi_i ;
    sc_signal < bool > ori_i ;
    sc_signal < bool > xori_i ;

    sc_signal < bool > jalr_i ;
    
    //S-type Instruction :

    sc_signal < bool > slli_i ;
    sc_signal < bool > srli_i ;
    sc_signal < bool > srai_i ;

    //B-type Instruction :

    sc_signal < bool > beq_i ;
    sc_signal < bool > bne_i ;
    sc_signal < bool > blt_i ;
    sc_signal < bool > bge_i ;
    sc_signal < bool > bltu_i ;
    sc_signal < bool > bgeu_i ;

    //U-type Instruction :

    sc_signal < bool > lui_i ;
    sc_signal < bool > auipc_i ;

    //J-type Instruction :  

    sc_signal < bool > jal_i ;

    void pushingToIf() ;
    void popFromIf() ; 
    void decoding_instruction_type() ;
    void decoding_instruction() ;
    void affectation() ;

    SC_CTOR(decod) :
    dec2if("dec2if")    
    {
        dec2if.DIN(DEC2IF_PC) ;
        dec2if.DOUT(dec2if_pc_out) ;
        dec2if.EMPTY(dec2if_empty) ;
        dec2if.FULL(dec2if_full) ;
        dec2if.PUSH(DEC2IF_PUSH) ;
        dec2if.POP(dec2if_pop) ;
        dec2if.CLK(CLK) ;
        dec2if.RESET_N(RESET_N) ;

        SC_METHOD(pushingToIf) ;
        sensitive << DEC2IF_PC << DEC2IF_PUSH ;
        SC_METHOD(popFromIf) ;
        sensitive << IF2DEC_POP << IF2DEC_EMPTY << IF2DEC_FULL ;
        SC_METHOD(decoding_instruction_type)
        sensitive<<IF_IR ;
        SC_METHOD(decoding_instruction)
        sensitive << r_type_inst << i_type_inst << i_type_inst << s_type_inst << b_type_inst << u_type_inst << j_type_inst ;
        SC_METHOD(affectation)
        sensitive << IF_IR ;
        reset_signal_is(RESET_N,false) ;

    }
};