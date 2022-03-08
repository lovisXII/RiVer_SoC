#include <systemc.h>
#include "../UTIL/fifo_generic/fifo_generic.h"
SC_MODULE(decod)
{
    
    //Interface with REG :

    sc_in   < sc_uint<32> >       IN_RDATA1_SD ; 
    sc_in   < sc_uint<32> >       IN_RDATA2_SD ;
    sc_in   < bool >              R1_VALID_SD ; // tells if the data read is valid or no
    sc_in   < bool >              R2_VALID_SD ; // same but for rt

    sc_in   < bool >              ADR_DEST_VALID_SD ;
    sc_out  <sc_uint<6>>          ADR_DEST_SD ;

    sc_out  < sc_uint<6> >       RADR1_SD ; // adress of rs
    sc_out  < sc_uint<6> >       RADR2_SD ; // adress of rt

    sc_out  < sc_uint<6> >       INVAL_DEST_SD ; // rd
    sc_out  < bool >             INVAL_ENABLE_SD ;
        
    sc_out  < sc_uint<32> >      WRITE_PC_SD ;
    sc_out  < bool >             WRITE_PC_ENABLE_SD ;

    sc_in   < sc_uint<32> >      READ_PC_SD ; // value of r32 which is pc coming from REG
    sc_in   < bool >             READ_PC_VALID_SD ; // say if pc is valid or no, signal coming from REG

    //Interface with EXE :

    sc_out  < sc_uint<32> >      EXE_OP1_SD ; // value of op1
    sc_out  < sc_uint<32> >      EXE_OP2_SD ; // value of op2
    sc_out  < sc_uint<2> >       EXE_CMD_SD ; // value of the command sent to exe
    sc_out  < bool >             EXE_NEG_OP2_SD ; // say if we take the opposite of the op1 to do a substraction for example
    sc_out  < bool >             EXE_WB_SD ; // say if we plan to wbk the value of rd or no
    sc_out  < sc_uint<6> >       EXE_DEST_SD;    //the destination register
    sc_out  < bool >             EXE_SELECT_SHIFT_SD ; //taille fifo entrée : 110
    sc_out  < bool >             EXE_SLT_SD ;
    sc_out  < bool >             EXE_SLTU_SD ;

    sc_out  < sc_uint<32> >      MEM_DATA_SD ; // data sent to mem for storage
    sc_out  < bool>              MEM_LOAD_SD ; // say to mem if we do a load
    sc_out  < bool >             MEM_STORE_SD ; // say to mem if we do a store
    sc_out  < bool >             MEM_SIGN_EXTEND_SD ; 
    sc_out  < sc_uint<2> >       MEM_SIZE_SD ; // tells to mem if we do an acces in word, hw or byte
    
    // Interface with DEC2IF : 

    sc_in   < bool >              DEC2IF_POP_SD ; // Ifecth say to decod if it wants a pop or no
    sc_out  < bool >              DEC2IF_EMPTY_SD ;
    sc_out  < sc_bv<32> >         IF_PC_SD ; // this value must also be sent to REG

    //Interface with IF2DEC :

    sc_in   < sc_bv<32> >         INSTR_SD ;
    sc_in   < bool >              IF2DEC_EMPTY_SD ;
    sc_out  < bool >              IF2DEC_POP_SD ; //Decod says to IFETCH if it wants a pop or no
    sc_out  < bool >              IF2DEC_FLUSH_SD ;

    //Interface with DEC2EXE

    sc_in< bool >                 DEC2EXE_POP_SD ;
    sc_out< bool >                DEC2EXE_EMPTY_SD ;                    
    sc_signal< sc_bv<114> >       DEC2EXE_OUT_SD ;

    //Bypasses
    sc_in < sc_uint<6> >          BP_EXE_DEST_SD ;
    sc_in < sc_uint<32> >         BP_EXE_RES_SD ;
    sc_in < sc_uint<6> >          BP_MEM_DEST_SD ;
    sc_in < sc_uint<32> >         BP_MEM_RES_SD ;

    //General Interface :
    sc_in_clk                     CLK ;
    sc_in  <bool>                 RESET_N ;


    //Instance used :
    
    fifo_generic<32> dec2if ;
    fifo_generic<114> dec2exe ;

    // Signals :

    sc_signal   < sc_uint<32> >       rdata1_sd ; 
    sc_signal   < sc_uint<32> >       rdata2_sd ;

    //fifo dec2if :
    
    sc_signal < sc_bv<32> >     dec2if_in_sd ; // pc sent to fifo
    sc_signal < bool >          dec2if_push_sd ;
    sc_signal < bool >          dec2if_empty_sd ;
    sc_signal < bool >          dec2if_full_sd ;
    sc_signal < sc_bv<32> >     dec2if_out_sd ;

    //fifo dec2exe :

    sc_signal < sc_bv <114> >   dec2exe_in_sd ;
    sc_signal < bool >          dec2exe_push_sd ;
    sc_signal < bool >          dec2exe_full_sd ;

    // Instruction format type :

    sc_signal < bool >          r_type_inst_sd ; // R type format
    sc_signal < bool >          i_type_inst_sd ; // I type format
    sc_signal < bool >          s_type_inst_sd ; // S type format
    sc_signal < bool >          b_type_inst_sd ; // B type format
    sc_signal < bool >          u_type_inst_sd ; // U type format
    sc_signal < bool >          j_type_inst_sd ; // J type format
    sc_signal < bool >          jalr_type_inst_sd ; //JALR has a specific opcode

    //R-type Instructions :


    sc_signal < bool > add_i_sd ;
    sc_signal < bool > slt_i_sd ;
    sc_signal < bool > sltu_i_sd ;
    sc_signal < bool > and_i_sd ;
    sc_signal < bool > or_i_sd ;
    sc_signal < bool > xor_i_sd ;
    sc_signal < bool > sll_i_sd ;
    sc_signal < bool > srl_i_sd ;
    sc_signal < bool > sub_i_sd ;
    sc_signal < bool > sra_i_sd ;

    //I-type Instructions :

    sc_signal < bool > addi_i_sd ;
    sc_signal < bool > slti_i_sd ;
    sc_signal < bool > sltiu_i_sd ;
    sc_signal < bool > andi_i_sd ;
    sc_signal < bool > ori_i_sd ;
    sc_signal < bool > xori_i_sd ;

    sc_signal < bool > jalr_i_sd ;

    // I-type shift instructions :

    sc_signal < bool > slli_i_sd ;
    sc_signal < bool > srli_i_sd ;
    sc_signal < bool > srai_i_sd ;

   // I-type load instructions :

   sc_signal < bool > lw_i_sd ;
   sc_signal < bool > lh_i_sd ;
   sc_signal < bool > lhu_i_sd ;
   sc_signal < bool > lb_i_sd ;
   sc_signal < bool > lbu_i_sd ;

    //B-type Instruction :

    sc_signal < bool > beq_i_sd ;
    sc_signal < bool > bne_i_sd ;
    sc_signal < bool > blt_i_sd ;
    sc_signal < bool > bge_i_sd ;
    sc_signal < bool > bltu_i_sd ;
    sc_signal < bool > bgeu_i_sd ;

    //U-type Instruction :

    sc_signal < bool > lui_i_sd ;
    sc_signal < bool > auipc_i_sd ;

    //J-type Instruction :  

    sc_signal < bool > jal_i_sd ;

    //S-type Instructions :

    sc_signal < bool > sw_i_sd ;
    sc_signal < bool > sh_i_sd ;
    sc_signal < bool > sb_i_sd ;

    //Offset for branch :

    sc_signal < sc_uint<32> > offset_branch_sd ;

    //PC gestion :

    sc_signal <bool> inc_pc_sd ;
    sc_signal <bool> add_offset_to_pc_sd ;


    //Internal signals :

    sc_signal< sc_uint<6>>  adr_dest_sd;
    sc_signal< sc_uint<32>> exe_op1_sd;
    sc_signal< sc_uint<32>> exe_op2_sd;
    sc_signal< sc_uint<32>> mem_data_sd ;
    
    sc_signal<sc_uint<2>>   mem_size_sd ;
    sc_signal<bool>   mem_load_sd ;
    sc_signal<bool>   mem_store_sd ;

    sc_signal<sc_uint<2>>   exe_cmd_sd ;
    sc_signal<bool>         select_shift_sd ;
    sc_signal<bool>         exe_neg_op2_sd ;
    sc_signal<bool>         exe_wb_sd ;
    sc_signal<bool>         mem_sign_extend_sd ;

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
    void bypasses() ;
    void trace(sc_trace_file* tf);

    SC_CTOR(decod) :
    dec2if("dec2if"),
    dec2exe("dec2exe")    
    {
        dec2if.DIN(dec2if_in_sd) ;
        dec2if.DOUT(IF_PC_SD) ;
        dec2if.EMPTY(dec2if_empty_sd) ;
        dec2if.FULL(dec2if_full_sd) ;
        dec2if.PUSH(dec2if_push_sd) ;
        dec2if.POP(DEC2IF_POP_SD) ;
        dec2if.CLK(CLK) ;
        dec2if.RESET_N(RESET_N) ;
    
        dec2exe.DIN(dec2exe_in_sd) ;
        dec2exe.DOUT(DEC2EXE_OUT_SD) ;
        dec2exe.EMPTY(DEC2EXE_EMPTY_SD) ;
        dec2exe.FULL(dec2exe_full_sd) ;
        dec2exe.PUSH(dec2exe_push_sd) ;
        dec2exe.POP(DEC2EXE_POP_SD) ;
        dec2exe.CLK(CLK) ;
        dec2exe.RESET_N(RESET_N) ;

        SC_METHOD(dec2if_gestion)
        sensitive   << dec2if_empty_sd 
                    << READ_PC_VALID_SD
                    << dec2if_full_sd ;

        SC_METHOD(concat_dec2exe)
        sensitive   << dec2exe_in_sd 
                    << exe_op1_sd 
                    << exe_op2_sd 
                    << exe_cmd_sd
                    << exe_neg_op2_sd
                    << exe_wb_sd 
                    << mem_data_sd 
                    << mem_load_sd 
                    << mem_store_sd 
                    << mem_sign_extend_sd 
                    << mem_size_sd 
                    << select_shift_sd 
                    << adr_dest_sd
                    << slti_i_sd 
                    << slt_i_sd 
                    << sltiu_i_sd 
                    << sltu_i_sd;
        SC_METHOD(unconcat_dec2exe)
        sensitive << DEC2EXE_OUT_SD ;       
        SC_METHOD(dec2exe_push_method)
        sensitive   << R1_VALID_SD 
                    << R2_VALID_SD 
                    << ADR_DEST_VALID_SD
                    << dec2exe_full_sd 
                    << IF2DEC_EMPTY_SD ;

        SC_METHOD(if2dec_pop_method)
        sensitive   << ADR_DEST_VALID_SD 
                    << R1_VALID_SD 
                    << R2_VALID_SD 
                    << IF2DEC_EMPTY_SD 
                    << dec2exe_full_sd
                    << add_offset_to_pc_sd ;

        SC_METHOD(decoding_instruction_type)
        sensitive   << INSTR_SD 
                    << READ_PC_SD ;
        SC_METHOD(decoding_instruction)
        sensitive   << INSTR_SD ;
        SC_METHOD(affectation_registres)
        sensitive   << INSTR_SD
                    << rdata1_sd
                    << rdata2_sd
                    << R1_VALID_SD
                    << R2_VALID_SD
                    << IF2DEC_EMPTY_SD
                    << dec2if_push_sd
                    << r_type_inst_sd 
                    << i_type_inst_sd 
                    << i_type_inst_sd 
                    << s_type_inst_sd 
                    << b_type_inst_sd 
                    << u_type_inst_sd 
                    << j_type_inst_sd 
                    << jalr_type_inst_sd 
                    << beq_i_sd
                    << bne_i_sd
                    << blt_i_sd
                    << bge_i_sd
                    << bltu_i_sd
                    << bgeu_i_sd  
                    << dec2if_push_sd
                    << READ_PC_SD ;
        SC_METHOD(affectation_calcul)
        sensitive   << add_i_sd
                    << slt_i_sd
                    << sltu_i_sd
                    << and_i_sd
                    << or_i_sd
                    << xor_i_sd
                    << sll_i_sd
                    << srl_i_sd
                    << sub_i_sd
                    << sra_i_sd
                    << addi_i_sd
                    << slti_i_sd
                    << sltiu_i_sd
                    << andi_i_sd
                    << ori_i_sd
                    << xori_i_sd
                    << jalr_i_sd
                    << slli_i_sd
                    << srli_i_sd
                    << srai_i_sd
                    << lw_i_sd
                    << lh_i_sd
                    << lhu_i_sd
                    << lb_i_sd
                    << lbu_i_sd
                    << beq_i_sd
                    << bne_i_sd
                    << blt_i_sd
                    << bge_i_sd
                    << bltu_i_sd
                    << bgeu_i_sd
                    << lui_i_sd
                    << auipc_i_sd
                    << jal_i_sd
                    << sw_i_sd
                    << sh_i_sd
                    << sb_i_sd  
                    << j_type_inst_sd
                    << jalr_type_inst_sd
                    << dec2exe_push_sd;
        SC_METHOD(pc_inc)
        sensitive   << CLK.pos()
                    << READ_PC_SD
                    << offset_branch_sd
                    << inc_pc_sd
                    << add_offset_to_pc_sd
                    << READ_PC_VALID_SD ;
        reset_signal_is(RESET_N,false) ;

    }
};