#include <systemc.h>
#include "../UTIL/fifo_generic/fifo_generic.h"
SC_MODULE(decod)
{
    
    //Interface with REG :

    sc_in   < sc_uint<32> >       RADR1_DATA ; 
    sc_in   < sc_uint<32> >       RADR2_DATA ;
    sc_in   < bool >              RADR1_VALID ; // tells if the data read is valid or no
    sc_in   < bool >              RADR2_VALID ; // same but for rt

    sc_in   < bool >              ADR_DEST_VALID ;
    sc_out  <sc_uint<6>>          ADR_DEST_DECOD ;

    sc_out  < sc_uint<6> >       RADR1 ; // adress of rs
    sc_out  < sc_uint<6> >       RADR2 ; // adress of rt

    sc_out  < sc_uint<6> >       INVAL_DEST ; // rd
    sc_out  < bool >             INVAL_ENABLE ;
        
    sc_out  < sc_uint<32> >      WRITE_PC ;
    sc_out  < bool >             WRITE_PC_ENABLE ;

    sc_in   < sc_uint<32> >      READ_PC ; // value of r32 which is pc coming from REG
    sc_in   < bool >             READ_PC_VALID ; // say if pc is valid or no, signal coming from REG

    //Interface with EXE :

    sc_out  < sc_uint<32> >      DEC2EXE_OP1 ; // value of op1
    sc_out  < sc_uint<32> >      DEC2EXE_OP2 ; // value of op2
    sc_out  < sc_uint<2> >       DEC2EXE_CMD ; // value of the command sent to exe
    sc_out  < bool >             DEC2EXE_NEG_OP1 ; // say if we take the opposite of the op1 to do a substraction for example
    sc_out  < bool >             DEC2EXE_WB ; // say if we plan to wbk the value of rd or no
    sc_out  < sc_uint<6> >       EXE_DEST;    //the destination register

    sc_out  < sc_uint<32> >      MEM_DATA ; // data sent to mem for storage
    sc_out  < bool>              MEM_LOAD ; // say to mem if we do a load
    sc_out  < bool >             MEM_STORE ; // say to mem if we do a store
    sc_out  < bool >             MEM_SIGN_EXTEND ; 
    sc_out  < sc_uint<2> >       MEM_SIZE ; // tells to mem if we do an acces in word, hw or byte
    sc_out  < bool >             SELECT_SHIFT ; //taille fifo entrée : 110
    sc_out  < bool >             SLT ;
    sc_out  < bool >             SLTU ;
    
    // Interface with DEC2IF : 

    sc_in  < bool >               DEC2IF_POP ; // Ifecth say to decod if it wants a pop or no
    sc_out   < bool >             DEC2IF_EMPTY ;
    sc_out  < sc_bv<32> >         DEC2IF_PC ; // this value must also be sent to REG

    //Interface with IF2DEC :

    sc_in   < sc_bv<32> >         IF_IR ;
    sc_in   < bool >              IF2DEC_EMPTY ;
    sc_out  < bool >              IF2DEC_POP ; //Decod says to IFETCH if it wants a pop or no
    sc_out  < bool >              IF2DEC_FLUSH ;

    //Interface with DEC2EXE

    sc_in< bool >                 DEC2EXE_POP ;
    sc_out< bool >                DEC2EXE_EMPTY ;                    
    sc_signal< sc_bv<114> >       DEC2EXE_OUT ;

    //General Interface :
    sc_in_clk                     CLK ;
    sc_in  <bool>                 RESET_N ;


    //Instance used :
    
    fifo_generic<32> dec2if ;
    fifo_generic<114> dec2exe ;

    // Signals :

    //fifo dec2if :
    
    sc_signal < sc_bv<32> >   dec2if_pc_in ; // pc sent to fifo
    sc_signal < bool >          dec2if_push ;
    sc_signal < bool >          dec2if_empty ;
    sc_signal < bool >          dec2if_full ;
    sc_signal < sc_bv<32> >   dec2if_pc_out ;

    //fifo dec2exe :

    sc_signal < sc_bv <114> >   dec2exe_in ;
    sc_signal < bool >          dec2exe_push ;
    sc_signal < bool >          dec2exe_full ;

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

    // I-type shift instructions :

    sc_signal < bool > slli_i ;
    sc_signal < bool > srli_i ;
    sc_signal < bool > srai_i ;

   // I-type load instructions :

   sc_signal < bool > lw_i ;
   sc_signal < bool > lh_i ;
   sc_signal < bool > lhu_i ;
   sc_signal < bool > lb_i ;
   sc_signal < bool > lbu_i ;

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

    //S-type Instructions :

    sc_signal < bool > sw_i ;
    sc_signal < bool > sh_i ;
    sc_signal < bool > sb_i ;

    //Offset for branch :

    sc_signal < sc_uint<32> > offset_branch ;

    //PC gestion :

    sc_signal <bool> inc_pc ;
    sc_signal <bool> add_offset_to_pc ;


    //Internal signals :

    sc_signal< sc_uint<6>>  adr_dest;
    sc_signal< sc_uint<32>> dec2exe_op1;
    sc_signal< sc_uint<32>> dec2exe_op2;
    sc_signal< sc_uint<32>> offset_branch_var ;
    sc_signal< sc_uint<32>> mem_data ;
    
    sc_signal<sc_uint<2>>   mem_size ;
    sc_signal<bool>   mem_load ;
    sc_signal<bool>   mem_store ;

    sc_signal<sc_uint<2>>   dec2exe_cmd ;
    sc_signal<bool>         select_shift ;
    sc_signal<bool>         dec2exe_neg_op1 ;
    sc_signal<bool>         dec2exe_wb ;
    sc_signal<bool>         mem_sign_extend ;

    void dec2if_gestion() ;
    void concat_dec2exe() ;
    void unconcat_dec2exe() ;
    void dec2exe_push_method() ;
    void if2dec_pop_method() ;
    void decoding_instruction_type() ;
    void decoding_instruction() ;
    void affectation_registres() ;
    void affectation_calcul() ;
    void pc_inc() ;
    void trace(sc_trace_file* tf);

    SC_CTOR(decod) :
    dec2if("dec2if"),
    dec2exe("dec2exe")    
    {
        dec2if.DIN(dec2if_pc_in) ;
        dec2if.DOUT(DEC2IF_PC) ;
        dec2if.EMPTY(dec2if_empty) ;
        dec2if.FULL(dec2if_full) ;
        dec2if.PUSH(dec2if_push) ;
        dec2if.POP(DEC2IF_POP) ;
        dec2if.CLK(CLK) ;
        dec2if.RESET_N(RESET_N) ;
    
        dec2exe.DIN(dec2exe_in) ;
        dec2exe.DOUT(DEC2EXE_OUT) ;
        dec2exe.EMPTY(DEC2EXE_EMPTY) ;
        dec2exe.FULL(dec2exe_full) ;
        dec2exe.PUSH(dec2exe_push) ;
        dec2exe.POP(DEC2EXE_POP) ;
        dec2exe.CLK(CLK) ;
        dec2exe.RESET_N(RESET_N) ;

        SC_METHOD(dec2if_gestion)
        sensitive   << dec2if_empty 
                    << READ_PC_VALID
                    << dec2if_full ;

        SC_METHOD(concat_dec2exe)
        sensitive   << dec2exe_in 
                    << dec2exe_op1 
                    << dec2exe_op2 
                    << dec2exe_cmd
                    << dec2exe_neg_op1
                    << dec2exe_wb 
                    << mem_data 
                    << mem_load 
                    << mem_store 
                    << mem_sign_extend 
                    << mem_size 
                    << select_shift 
                    << adr_dest
                    << slti_i 
                    << slt_i 
                    << sltiu_i 
                    << sltu_i;
        SC_METHOD(unconcat_dec2exe)
        sensitive << DEC2EXE_OUT ;       
        SC_METHOD(dec2exe_push_method)
        sensitive   << RADR1_VALID 
                    << RADR2_VALID 
                    << ADR_DEST_VALID
                    << dec2exe_full 
                    << IF2DEC_EMPTY ;

        SC_METHOD(if2dec_pop_method)
        sensitive   << ADR_DEST_VALID 
                    << RADR1_VALID 
                    << RADR2_VALID 
                    << IF2DEC_EMPTY 
                    << dec2exe_full
                    << add_offset_to_pc ;

        SC_METHOD(decoding_instruction_type)
        sensitive   << IF_IR 
                    << READ_PC ;
        SC_METHOD(decoding_instruction)
        sensitive   << IF_IR ;
        SC_METHOD(affectation_registres)
        sensitive   << IF_IR
                    << RADR1_DATA
                    << RADR2_DATA
                    << RADR1_VALID
                    << RADR2_VALID
                    << IF2DEC_EMPTY
                    << dec2if_push
                    << r_type_inst 
                    << i_type_inst 
                    << i_type_inst 
                    << s_type_inst 
                    << b_type_inst 
                    << u_type_inst 
                    << j_type_inst 
                    << jalr_type_inst 
                    << beq_i
                    << bne_i
                    << blt_i
                    << bge_i
                    << bltu_i
                    << bgeu_i  
                    << dec2if_push
                    << READ_PC ;
        SC_METHOD(affectation_calcul)
        sensitive   << add_i
                    << slt_i
                    << sltu_i
                    << and_i
                    << or_i
                    << xor_i
                    << sll_i
                    << srl_i
                    << sub_i
                    << sra_i
                    << addi_i
                    << slti_i
                    << sltiu_i
                    << andi_i
                    << ori_i
                    << xori_i
                    << jalr_i
                    << slli_i
                    << srli_i
                    << srai_i
                    << lw_i
                    << lh_i
                    << lhu_i
                    << lb_i
                    << lbu_i
                    << beq_i
                    << bne_i
                    << blt_i
                    << bge_i
                    << bltu_i
                    << bgeu_i
                    << lui_i
                    << auipc_i
                    << jal_i
                    << sw_i
                    << sh_i
                    << sb_i  
                    << j_type_inst
                    << jalr_type_inst
                    << dec2exe_push;
        SC_METHOD(pc_inc)
        sensitive   << CLK.pos()
                    << READ_PC
                    << offset_branch
                    << inc_pc
                    << add_offset_to_pc
                    << READ_PC_VALID ;
        reset_signal_is(RESET_N,false) ;

    }
};