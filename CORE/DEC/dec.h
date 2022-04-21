#include <systemc.h>
#include <iostream>
#include "../UTIL/fifo.h"
#define dec2exe_size 213

SC_MODULE(decod) {
    // Interface with REG :

    sc_in<sc_uint<32>> RDATA1_SR;
    sc_in<sc_uint<32>> RDATA2_SR;

    sc_in<sc_uint<32>> READ_PC_SR;  // value of r32 which is pc coming from REG

    sc_out<sc_uint<6>> RADR1_SD;  // adress of rs
    sc_out<sc_uint<6>> RADR2_SD;  // adress of rt

    sc_out<sc_uint<32>> WRITE_PC_SD;
    sc_out<bool>        WRITE_PC_ENABLE_SD;

    // Interface with EXE :

    sc_out<sc_uint<32>> OP1_RD;           // value of op1
    sc_out<sc_uint<32>> OP2_RD;           // value of op2
    sc_out<sc_uint<2>>  EXE_CMD_RD;       // value of the command sent to exe
    sc_out<bool>        NEG_OP2_RD;       // say if we take the opposite of the op1 to do a
                                          // substraction for example
    sc_out<bool>        WB_RD;            // say if we plan to wbk the value of rd or no
    sc_out<sc_uint<6>>  EXE_DEST_SD;      // the destination register
    sc_out<bool>        SELECT_SHIFT_RD;  // taille fifo entrée : 110
    sc_out<bool>        SLT_RD;
    sc_out<bool>        SLTU_RD;
    sc_out<sc_uint<32>> PC_DEC2EXE_RD;  // PC link to the current decoded instruction

    sc_out<sc_uint<32>> MEM_DATA_RD;   // data sent to mem for storage
    sc_out<bool>        MEM_LOAD_RD;   // say to mem if we do a load
    sc_out<bool>        MEM_STORE_RD;  // say to mem if we do a store
    sc_out<bool>        MEM_SIGN_EXTEND_RD;
    sc_out<sc_uint<2>>  MEM_SIZE_RD;  // tells to mem if we do an acces in word, hw or byte

    sc_inout<bool> CSR_WENABLE_RD;       // indicate if we do a csr operation,
                                       // if so need to WBK CSR in rd
    sc_out<sc_uint<12>> CSR_WADR_RD;   // CSR adress sent to EXE, will allow to wbk csr in MEM
    sc_out<sc_uint<32>> CSR_RDATA_RD;  // CSR read data to be wb in register

    // Interface with DEC2IF :

    sc_in<bool>       DEC2IF_POP_SI;  // Ifecth say to decod if it wants a pop or no
    sc_out<bool>      DEC2IF_EMPTY_SD;
    sc_out<sc_bv<32>> PC_RD;  // this value must also be sent to REG

    // Interface with IF2DEC :

    sc_in<sc_uint<32>> PC_IF2DEC_RI;
    sc_in<sc_bv<32>>   INSTR_RI;
    sc_in<bool>        IF2DEC_EMPTY_SI;
    sc_out<bool>       IF2DEC_POP_SD;  // Decod says to IFETCH if it wants a pop or no
    sc_out<bool>       IF2DEC_FLUSH_SD;

    // Interface with DEC2EXE

    sc_in<bool>                    DEC2EXE_POP_SE;
    sc_out<bool>                   DEC2EXE_EMPTY_SD;
    sc_signal<sc_bv<dec2exe_size>> dec2exe_out_sd;

    // Interface with CSR :

    sc_out<sc_uint<12>> CSR_RADR_SD;   // CSR adress sent to CSR to get data
    sc_in<sc_uint<32>>  CSR_RDATA_SC;  // data read from CSR

    // Bypasses

    sc_in<sc_uint<6>>  BP_DEST_RE;
    sc_in<sc_uint<32>> BP_EXE_RES_RE;
    sc_in<bool>        BP_MEM_LOAD_RE;
    sc_in<bool>        BP_EXE2MEM_EMPTY_SE;
    sc_in<sc_uint<6>>  BP_DEST_RM;
    sc_in<sc_uint<32>> BP_MEM_RES_RM;

    sc_in<bool>        CSR_WENABLE_RE;
    sc_in<sc_uint<32>> CSR_RDATA_RE;
    sc_in<bool>        CSR_WENABLE_RM;
    sc_in<sc_uint<32>> CSR_RDATA_RM;

    sc_out<bool>       BP_R1_VALID_RD;
    sc_out<bool>       BP_R2_VALID_RD;
    sc_out<sc_uint<6>> BP_RADR1_RD;
    sc_out<sc_uint<6>> BP_RADR2_RD;
    sc_out<bool>       BLOCK_BP_RD;

    // Exception :

    sc_in<bool> EXCEPTION_RI;  // this signal will be at 0 considering there is no exception in IFETCH

    sc_out<bool> ECALL_I_RD;
    sc_out<bool> EBREAK_I_RD;
    sc_out<bool> ILLEGAL_INSTRUCTION_RD;  // instruction doesnt exist
    sc_out<bool> ADRESS_MISSALIGNED_RD;   // branch offset is misaligned
    sc_out<bool> SYSCALL_U_MODE_RD;
    sc_out<bool> SYSCALL_M_MODE_RD;

    sc_out<bool> EXCEPTION_RD;
    // General Interface :

    sc_in<bool>        EXCEPTION_RM;
    sc_in<sc_uint<32>> MTVEC_VALUE_RC;
    sc_in_clk          CLK;
    sc_in<bool>        RESET_N;

    // Interruption :

    sc_in<bool> INTERRUPTION_SE;

    // Instance used :

    fifo<32>           dec2if;
    fifo<dec2exe_size> dec2exe;

    // Signals :

    sc_signal<sc_uint<32>> rdata1_sd;
    sc_signal<sc_uint<32>> rdata2_sd;
    sc_signal<bool>        r1_valid_sd;
    sc_signal<bool>        r2_valid_sd;
    sc_signal<bool>        stall;
    sc_signal<bool>        block_in_dec;

    // fifo dec2if :

    sc_signal<sc_bv<32>> dec2if_in_sd;  // pc sent to fifo
    sc_signal<bool>      dec2if_push_sd;
    sc_signal<bool>      dec2if_empty_sd;
    sc_signal<bool>      dec2if_full_sd;
    sc_signal<sc_bv<32>> dec2if_out_sd;

    // fifo dec2exe :

    sc_signal<sc_bv<dec2exe_size>> dec2exe_in_sd;
    sc_signal<bool>                dec2exe_push_sd;
    sc_signal<bool>                dec2exe_full_sd;

    // Instruction format type :

    sc_signal<bool> r_type_inst_sd;       // R type format
    sc_signal<bool> i_type_inst_sd;       // I type format
    sc_signal<bool> s_type_inst_sd;       // S type format
    sc_signal<bool> b_type_inst_sd;       // B type format
    sc_signal<bool> u_type_inst_sd;       // U type format
    sc_signal<bool> j_type_inst_sd;       // J type format
    sc_signal<bool> jalr_type_inst_sd;    // JALR has a specific opcode
    sc_signal<bool> system_type_inst_sd;  // System instruction
    // R-type Instructions :

    sc_signal<bool> add_i_sd;
    sc_signal<bool> slt_i_sd;
    sc_signal<bool> sltu_i_sd;
    sc_signal<bool> and_i_sd;
    sc_signal<bool> or_i_sd;
    sc_signal<bool> xor_i_sd;
    sc_signal<bool> sll_i_sd;
    sc_signal<bool> srl_i_sd;
    sc_signal<bool> sub_i_sd;
    sc_signal<bool> sra_i_sd;

    // I-type Instructions :

    sc_signal<bool> addi_i_sd;
    sc_signal<bool> slti_i_sd;
    sc_signal<bool> sltiu_i_sd;
    sc_signal<bool> andi_i_sd;
    sc_signal<bool> ori_i_sd;
    sc_signal<bool> xori_i_sd;

    sc_signal<bool> jalr_i_sd;

    sc_signal<bool> fence_i_sd;

    // I-type shift instructions :

    sc_signal<bool> slli_i_sd;
    sc_signal<bool> srli_i_sd;
    sc_signal<bool> srai_i_sd;

    // I-type load instructions :

    sc_signal<bool> lw_i_sd;
    sc_signal<bool> lh_i_sd;
    sc_signal<bool> lhu_i_sd;
    sc_signal<bool> lb_i_sd;
    sc_signal<bool> lbu_i_sd;

    // B-type Instruction :

    sc_signal<bool> beq_i_sd;
    sc_signal<bool> bne_i_sd;
    sc_signal<bool> blt_i_sd;
    sc_signal<bool> bge_i_sd;
    sc_signal<bool> bltu_i_sd;
    sc_signal<bool> bgeu_i_sd;

    // U-type Instruction :

    sc_signal<bool> lui_i_sd;
    sc_signal<bool> auipc_i_sd;

    // J-type Instruction :

    sc_signal<bool> jal_i_sd;

    // S-type Instructions :

    sc_signal<bool> sw_i_sd;
    sc_signal<bool> sh_i_sd;
    sc_signal<bool> sb_i_sd;

    // Kernel instruction :

    sc_signal<bool> csrrw_i_sd;
    sc_signal<bool> csrrs_i_sd;
    sc_signal<bool> csrrc_i_sd;
    sc_signal<bool> csrrwi_i_sd;
    sc_signal<bool> csrrsi_i_sd;
    sc_signal<bool> csrrci_i_sd;

    sc_signal<bool> csr_in_progress;

    // Signal for Kernel usage

    sc_signal<bool>        csr_wenable_sd;
    sc_signal<sc_uint<12>> csr_radr_sd;

    // Offset for branch :

    sc_signal<sc_uint<32>> offset_branch_sd;

    // PC gestion :

    sc_signal<bool> inc_pc_sd;
    sc_signal<bool> add_offset_to_pc_sd;

    // Internal signals :

    sc_signal<sc_uint<6>>  adr_dest_sd;
    sc_signal<sc_uint<32>> exe_op1_sd;
    sc_signal<sc_uint<32>> exe_op2_sd;
    sc_signal<sc_uint<32>> mem_data_sd;

    sc_signal<sc_uint<2>> mem_size_sd;
    sc_signal<bool>       mem_load_sd;
    sc_signal<bool>       mem_store_sd;

    sc_signal<sc_uint<2>> exe_cmd_sd;
    sc_signal<bool>       select_shift_sd;
    sc_signal<bool>       exe_neg_op2_sd;
    sc_signal<bool>       exe_wb_sd;
    sc_signal<bool>       mem_sign_extend_sd;
    sc_signal<bool>       block_bp_sd;

    // Exception :

    sc_signal<bool> ecall_i_sd;
    sc_signal<bool> ebreak_i_sd;
    sc_signal<bool> illegal_instruction_sd;  // instruction doesnt exist
    sc_signal<bool> adress_missaligned_sd;   // branch offset is misaligned
    sc_signal<bool> syscall_u_mode_sd;
    sc_signal<bool> syscall_s_mode_sd;

    void dec2if_gestion();
    void concat_dec2exe();
    void unconcat_dec2exe();
    void dec2exe_push_method();
    void if2dec_pop_method();
    void decoding_instruction_type();
    void decoding_instruction();
    void pre_reg_read_decoding();
    void post_reg_read_decoding();
    void pc_inc();
    void bypasses();
    void stall_method();
    void trace(sc_trace_file * tf);

    SC_CTOR(decod) : dec2if("dec2if"), dec2exe("dec2exe") {
        dec2if.DIN_S(dec2if_in_sd);
        dec2if.DOUT_R(PC_RD);
        dec2if.EMPTY_S(dec2if_empty_sd);
        dec2if.FULL_S(dec2if_full_sd);
        dec2if.PUSH_S(dec2if_push_sd);
        dec2if.POP_S(DEC2IF_POP_SI);
        dec2if.CLK(CLK);
        dec2if.RESET_N(RESET_N);

        dec2exe.DIN_S(dec2exe_in_sd);
        dec2exe.DOUT_R(dec2exe_out_sd);
        dec2exe.EMPTY_S(DEC2EXE_EMPTY_SD);
        dec2exe.FULL_S(dec2exe_full_sd);
        dec2exe.PUSH_S(dec2exe_push_sd);
        dec2exe.POP_S(DEC2EXE_POP_SE);
        dec2exe.CLK(CLK);
        dec2exe.RESET_N(RESET_N);

        SC_METHOD(dec2if_gestion)
        sensitive

            << dec2if_empty_sd << dec2if_full_sd << stall << EXCEPTION_RM;

        SC_METHOD(concat_dec2exe)
        sensitive << dec2exe_in_sd << exe_op1_sd << exe_op2_sd << exe_cmd_sd << exe_neg_op2_sd << exe_wb_sd

                  << mem_data_sd << mem_load_sd << mem_store_sd

                  << mem_sign_extend_sd << mem_size_sd << select_shift_sd << adr_dest_sd << slti_i_sd << slt_i_sd

                  << sltiu_i_sd << sltu_i_sd << RADR1_SD

                  << RADR2_SD << r1_valid_sd << EXCEPTION_RM << r2_valid_sd << PC_IF2DEC_RI << csr_wenable_sd
                  << ecall_i_sd << ebreak_i_sd << illegal_instruction_sd << adress_missaligned_sd << syscall_u_mode_sd
                  << block_bp_sd << syscall_s_mode_sd;
        SC_METHOD(unconcat_dec2exe)
        sensitive << dec2exe_out_sd;
        SC_METHOD(dec2exe_push_method)
        sensitive << dec2exe_full_sd << IF2DEC_EMPTY_SI << stall << EXCEPTION_RM;

        SC_METHOD(if2dec_pop_method)
        sensitive << IF2DEC_EMPTY_SI << dec2exe_full_sd << add_offset_to_pc_sd << stall << EXCEPTION_RM;

        SC_METHOD(stall_method)
        sensitive << b_type_inst_sd << jalr_type_inst_sd << j_type_inst_sd << r1_valid_sd << r2_valid_sd
                  << csr_wenable_sd << DEC2EXE_EMPTY_SD << CSR_WENABLE_RE << BP_EXE2MEM_EMPTY_SE << csr_in_progress

                  << block_in_dec;

        SC_METHOD(decoding_instruction_type)
        sensitive << INSTR_RI << READ_PC_SR;
        SC_METHOD(decoding_instruction)
        sensitive << INSTR_RI;
        SC_METHOD(pre_reg_read_decoding)
        sensitive << INSTR_RI << r_type_inst_sd << i_type_inst_sd

                  << i_type_inst_sd << s_type_inst_sd << b_type_inst_sd << u_type_inst_sd << j_type_inst_sd

                  << jalr_type_inst_sd << beq_i_sd << bne_i_sd

                  << blt_i_sd << bge_i_sd << bltu_i_sd << bgeu_i_sd << system_type_inst_sd << csrrw_i_sd << csrrs_i_sd

                  << csrrc_i_sd << csrrwi_i_sd << csrrsi_i_sd << csrrci_i_sd << ecall_i_sd << ebreak_i_sd << fence_i_sd;
        SC_METHOD(post_reg_read_decoding)
        sensitive << i_type_inst_sd << s_type_inst_sd << b_type_inst_sd << u_type_inst_sd << j_type_inst_sd

                  << jalr_type_inst_sd << beq_i_sd << bne_i_sd

                  << blt_i_sd << bge_i_sd << bltu_i_sd << bgeu_i_sd << IF2DEC_EMPTY_SI << dec2if_push_sd << READ_PC_SR

                  << stall << dec2if_push_sd << add_i_sd << slt_i_sd << sltu_i_sd << and_i_sd << or_i_sd << xor_i_sd

                  << sll_i_sd << srl_i_sd << sub_i_sd << sra_i_sd << addi_i_sd

                  << slti_i_sd << sltiu_i_sd << andi_i_sd << ori_i_sd << xori_i_sd << jalr_i_sd << slli_i_sd

                  << srli_i_sd << srai_i_sd << lw_i_sd << lh_i_sd

                  << lhu_i_sd << lb_i_sd << lbu_i_sd << beq_i_sd << bne_i_sd << blt_i_sd << bge_i_sd << bltu_i_sd

                  << bgeu_i_sd << lui_i_sd << auipc_i_sd

                  << jal_i_sd << sw_i_sd << sh_i_sd << sb_i_sd << j_type_inst_sd << jalr_type_inst_sd << dec2exe_push_sd

                  << rdata1_sd << rdata2_sd << r1_valid_sd << r2_valid_sd << system_type_inst_sd << csrrw_i_sd

                  << csrrs_i_sd << csrrc_i_sd << csrrwi_i_sd << csrrsi_i_sd << csrrci_i_sd << CSR_RDATA_SC << ecall_i_sd

                  << ebreak_i_sd << fence_i_sd << PC_IF2DEC_RI << EXCEPTION_RM;
        SC_METHOD(pc_inc)
        sensitive << CLK.pos() << READ_PC_SR << offset_branch_sd << inc_pc_sd << add_offset_to_pc_sd << MTVEC_VALUE_RC

                  << EXCEPTION_RM << PC_IF2DEC_RI;

        SC_METHOD(bypasses);
        sensitive << RDATA1_SR << RDATA2_SR << BP_DEST_RE << BP_EXE_RES_RE << BP_DEST_RM << BP_MEM_RES_RM << RADR1_SD

                  << RADR2_SD << BP_EXE2MEM_EMPTY_SE;
        reset_signal_is(RESET_N, false);
    }
};