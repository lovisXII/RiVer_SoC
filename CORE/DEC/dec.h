#pragma once
#include <systemc.h>
#include <iostream>
#include "../UTIL/fifo.h"

#define dec2exe_size        252
#define dec2if_size         64

#define start_kernel_adress 0xF0000000

SC_MODULE(decod) {

    // Interface with REG :

    sc_in<sc_uint<32>> RDATA1_SR;
    sc_in<sc_uint<32>> RDATA2_SR;

    sc_in<sc_uint<32>> READ_PC_SR;  // value of r32 which is pc coming from REG

    sc_out<sc_uint<6>>  RADR1_SD;  // adress of rs
    sc_out<sc_uint<6>>  RADR2_SD;  // adress of rt
    sc_out<sc_uint<32>> WRITE_PC_SD;
    sc_out<bool>        WRITE_PC_ENABLE_SD;

    // Interface with EXE :

    sc_out<sc_uint<32>> OP1_RD;                     // value of op1
    sc_out<sc_uint<32>> OP2_RD;                     // value of op2
    sc_out<sc_uint<2>>  EXE_CMD_RD;                 // value of the command sent to exe
    sc_out<bool>        NEG_OP2_RD;                 // say if we take the opposite of the op1 to do a
                                                    // substraction for example
    sc_out<bool>        WB_RD;                      // say if we plan to wbk the value of rd or no
    sc_out<sc_uint<6>>  EXE_DEST_SD;                // the destination register
    sc_out<sc_uint<4>>  SELECT_TYPE_OPERATIONS_RD;  // taille fifo entrée : 110
    sc_out<bool>        SLT_RD;
    sc_out<bool>        SLTU_RD;
    sc_out<sc_uint<32>> PC_DEC2EXE_RD;       // PC link to the current decoded instruction
    sc_out<sc_uint<32>> PC_BRANCH_VALUE_RD;  // PC of the branch value, will be usefull for exception
    sc_out<sc_uint<32>> MEM_DATA_RD;         // data sent to mem for storage
    sc_out<bool>        MEM_LOAD_RD;         // say to mem if we do a load
    sc_out<bool>        MEM_STORE_RD;        // say to mem if we do a store
    sc_out<bool>        MEM_SIGN_EXTEND_RD;
    sc_out<sc_uint<2>>  MEM_SIZE_RD;  // tells to mem if we do an acces in word, hw or byte

    sc_inout<bool> CSR_WENABLE_RD;     // indicate if we do a csr operation,
                                       // if so need to WBK CSR in rd
    sc_out<sc_uint<12>> CSR_WADR_RD;   // CSR adress sent to EXE, will allow to wbk csr in MEM
    sc_out<sc_uint<32>> CSR_RDATA_RD;  // CSR read data to be wb in register

    // Interface with DEC2IF :

    sc_in<bool>       DEC2IF_POP_SI;    // Ifecth say to decod if it wants a pop or no
    sc_out<bool>      DEC2IF_EMPTY_SD;
    sc_out<sc_bv<32>> PC_RD_S1;          // this value must also be sent to REG
    sc_out<sc_bv<32>> PC_RD_S2;          // this value must also be sent to REG

    // Interface with IF2DEC :

    sc_in<sc_uint<32>> PC_IF2DEC_RI_S1;
    sc_in<sc_uint<32>> PC_IF2DEC_RI_S2;
    sc_in<sc_bv<32>>   INSTR_RI_S1;
    sc_in<sc_bv<32>>   INSTR_RI_S2;
    sc_in<bool>        IF2DEC_EMPTY_SI;
    sc_out<bool>       IF2DEC_POP_SD;  // Decod says to IFETCH if it wants a pop or no
    sc_out<bool>       IF2DEC_FLUSH_SD;

    // Interface with DEC2EXE

    sc_in<bool>                    DEC2EXE_POP_SE;
    sc_out<bool>                   DEC2EXE_EMPTY_SD;
    sc_signal<sc_bv<dec2exe_size>> dec2exe_out_sd;

    // Multiplications signals
    sc_out<bool> MULT_INST_RD;

    sc_in<bool> MULT_INST_RE;
    sc_in<bool> MULT_INST_RM;
    // Interface with CSR :

    sc_out<sc_uint<12>> CSR_RADR_SD;   // CSR adress sent to CSR to get data
    sc_in<sc_uint<32>>  CSR_RDATA_SC;  // data read from CSR

    // Bypasses

    sc_in<sc_uint<6>>  BP_DEST_RE;
    sc_in<sc_uint<32>> BP_EXE_RES_RE;
    sc_in<bool>        BP_MEM_LOAD_RE;
    sc_in<bool>        BP_EXE2MEM_EMPTY_SE;
    sc_in<bool>        BP_MEM2WBK_EMPTY_SM;
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

    sc_out<bool>      ILLEGAL_INSTRUCTION_RD;  // instruction doesnt exist
    sc_out<bool>      ADRESS_MISSALIGNED_RD;   // branch offset is misaligned
    sc_out<bool>      ENV_CALL_U_MODE_RD;
    sc_out<bool>      ENV_CALL_M_MODE_RD;
    sc_out<bool>      ENV_CALL_S_MODE_RD;
    sc_out<bool>      ENV_CALL_WRONG_MODE_RD;
    sc_out<bool>      INSTRUCTION_ACCESS_FAULT_RD;
    sc_out<bool>      MRET_RD;
    sc_out<bool>      EXCEPTION_RD;
    sc_out<bool>      EBREAK_RD;
    sc_in<sc_uint<2>> CURRENT_MODE_SM;
    // General Interface :

    sc_in<bool>        EXCEPTION_SM;
    sc_in<sc_uint<32>> MTVEC_VALUE_RC;
    sc_in<sc_uint<32>> MCAUSE_WDATA_SM;
    sc_in_clk          CLK;
    sc_in<bool>        RESET_N;
    sc_in<bool>        MRET_SM;
    sc_in<sc_uint<32>> RETURN_ADRESS_SM;

    // Interruption :

    sc_in<bool> INTERRUPTION_SE;

    // Instance used :

    fifo<dec2if_size>           dec2if;
    fifo<dec2exe_size>          dec2exe;

    // Signals :

    sc_signal<sc_uint<32>> rdata1_sd;
    sc_signal<sc_uint<32>> rdata2_sd;
    sc_signal<bool>        r1_valid_sd;
    sc_signal<bool>        r2_valid_sd;
    sc_signal<bool>        block_in_dec;

    // fifo dec2if :

    sc_signal<sc_bv<32>> dec2if_pc_sd_s1;  // pc sent to fifo
    sc_signal<sc_bv<32>> dec2if_pc_sd_s2;  // pc sent to fifo
    sc_signal<bool>      dec2if_push_sd;
    sc_signal<bool>      dec2if_empty_sd;
    sc_signal<bool>      dec2if_full_sd;

    // fifo dec2exe :

    sc_signal<sc_bv<dec2exe_size>> dec2exe_in_sd;
    sc_signal<bool>                dec2exe_push_sd;
    sc_signal<bool>                dec2exe_full_sd;


    //fifo dec2if :

    sc_signal<sc_bv<dec2if_size>>         dec2if_in_sd ;
    sc_signal<sc_bv<dec2if_size>>         dec2if_out_sd ;

    // STAGE 1
    
    // Instruction format type :

    sc_signal<bool> r_type_inst_sd_s1;       // R type format
    sc_signal<bool> i_type_inst_sd_s1;       // I type format
    sc_signal<bool> s_type_inst_sd_s1;       // S type format
    sc_signal<bool> b_type_inst_sd_s1;       // B type format
    sc_signal<bool> u_type_inst_sd_s1;       // U type format
    sc_signal<bool> j_type_inst_sd_s1;       // J type format
    sc_signal<bool> jalr_type_inst_sd_s1;    // JALR has a specific opcode
    sc_signal<bool> system_type_inst_sd_s1;  // System instruction
    sc_signal<bool> m_type_inst_sd_s1;       // M type format

    // R-type Instructions :

    sc_signal<bool> add_i_sd_s1;
    sc_signal<bool> slt_i_sd_s1;
    sc_signal<bool> sltu_i_sd_s1;
    sc_signal<bool> and_i_sd_s1;
    sc_signal<bool> or_i_sd_s1;
    sc_signal<bool> xor_i_sd_s1;
    sc_signal<bool> sll_i_sd_s1;
    sc_signal<bool> srl_i_sd_s1;
    sc_signal<bool> sub_i_sd_s1;
    sc_signal<bool> sra_i_sd_s1;

    // I-type Instructions :

    sc_signal<bool> addi_i_sd_s1;
    sc_signal<bool> slti_i_sd_s1;
    sc_signal<bool> sltiu_i_sd_s1;
    sc_signal<bool> andi_i_sd_s1;
    sc_signal<bool> ori_i_sd_s1;
    sc_signal<bool> xori_i_sd_s1;

    sc_signal<bool> jalr_i_sd_s1;

    sc_signal<bool> fence_i_sd_s1;

    // I-type shift instructions :

    sc_signal<bool> slli_i_sd_s1;
    sc_signal<bool> srli_i_sd_s1;
    sc_signal<bool> srai_i_sd_s1;

    // I-type load instructions :

    sc_signal<bool> lw_i_sd_s1;
    sc_signal<bool> lh_i_sd_s1;
    sc_signal<bool> lhu_i_sd_s1;
    sc_signal<bool> lb_i_sd_s1;
    sc_signal<bool> lbu_i_sd_s1;

    // B-type Instruction :

    sc_signal<bool> beq_i_sd_s1;
    sc_signal<bool> bne_i_sd_s1;
    sc_signal<bool> blt_i_sd_s1;
    sc_signal<bool> bge_i_sd_s1;
    sc_signal<bool> bltu_i_sd_s1;
    sc_signal<bool> bgeu_i_sd_s1;

    // U-type Instruction :

    sc_signal<bool> lui_i_sd_s1;
    sc_signal<bool> auipc_i_sd_s1;

    // J-type Instruction :

    sc_signal<bool> jal_i_sd_s1;

    // S-type Instructions :

    sc_signal<bool> sw_i_sd_s1;
    sc_signal<bool> sh_i_sd_s1;
    sc_signal<bool> sb_i_sd_s1;

    // M-type Instructions :

    sc_signal<bool> mul_i_sd_s1;
    sc_signal<bool> mulh_i_sd_s1;
    sc_signal<bool> mulhsu_i_sd_s1;
    sc_signal<bool> mulhu_i_sd_s1;
    sc_signal<bool> div_i_sd_s1;
    sc_signal<bool> divu_i_sd_s1;
    sc_signal<bool> rem_i_sd_s1;
    sc_signal<bool> remu_i_sd_s1;

    // Kernel instruction :

    sc_signal<bool> csrrw_i_sd_s1;
    sc_signal<bool> csrrs_i_sd_s1;
    sc_signal<bool> csrrc_i_sd_s1;
    sc_signal<bool> csrrwi_i_sd_s1;
    sc_signal<bool> csrrsi_i_sd_s1;
    sc_signal<bool> csrrci_i_sd_s1;

    sc_signal<bool> csr_in_progress_s1;

    sc_signal<bool> mret_i_sd_s1;
    sc_signal<bool> sret_i_sd_s1;
    // Signal for Kernel usage

    sc_signal<bool>        csr_wenable_sd_s1;
    sc_signal<sc_uint<12>> csr_radr_sd_s1;
    sc_signal<sc_uint<32>> pc_branch_value_sd_s1;
    // Offset for branch :

    sc_signal<sc_uint<32>> offset_branch_sd_s1;

    // PC gestion :

    sc_signal<bool> inc_pc_sd_s1;
    sc_signal<bool> jump_sd_s1;

    // Pipeline Gestion

    sc_signal<bool> stall_sd_s1;
    // Internal signals :

    sc_signal<sc_uint<6>>  adr_dest_sd_s1;
    sc_signal<sc_uint<32>> exe_op1_sd_s1;
    sc_signal<sc_uint<32>> exe_op2_sd_s1;
    sc_signal<sc_uint<32>> mem_data_sd_s1;

    sc_signal<sc_uint<2>> mem_size_sd_s1;
    sc_signal<bool>       mem_load_sd_s1;
    sc_signal<bool>       mem_store_sd_s1;

    sc_signal<sc_uint<2>> exe_cmd_sd_s1;

    // operation types :
    // 0 : alu
    // 1 : shifter
    // 2 : multiplier
    // 3 : divider
    sc_signal<sc_uint<4>> select_type_operations_sd_s1;

    sc_signal<bool> exe_neg_op2_sd_s1;
    sc_signal<bool> exe_wb_sd_s1;
    sc_signal<bool> mem_sign_extend_sd_s1;
    sc_signal<bool> block_bp_sd_s1;

    // Exception :

    sc_signal<bool> ecall_i_sd_s1;
    sc_signal<bool> ebreak_i_sd_s1;
    sc_signal<bool> illegal_instruction_sd_s1;             // instruction doesnt exist
    sc_signal<bool> instruction_adress_missaligned_sd_s1;  // branch offset is misaligned
    sc_signal<bool> instruction_access_fault_sd_s1;
    sc_signal<bool> env_call_u_mode_sd_s1;
    sc_signal<bool> env_call_s_mode_sd_s1;
    sc_signal<bool> env_call_m_mode_sd_s1;
    sc_signal<bool> env_call_wrong_mode_s1;

    // STAGE 2 


    // Instruction format type :

    sc_signal<bool> r_type_inst_sd_s2;       // R type format
    sc_signal<bool> i_type_inst_sd_s2;       // I type format
    sc_signal<bool> s_type_inst_sd_s2;       // S type format
    sc_signal<bool> b_type_inst_sd_s2;       // B type format
    sc_signal<bool> u_type_inst_sd_s2;       // U type format
    sc_signal<bool> j_type_inst_sd_s2;       // J type format
    sc_signal<bool> jalr_type_inst_sd_s2;    // JALR has a specific opcode
    sc_signal<bool> system_type_inst_sd_s2;  // System instruction
    sc_signal<bool> m_type_inst_sd_s2;       // M type format

    // R-type Instructions :

    sc_signal<bool> add_i_sd_s2;
    sc_signal<bool> slt_i_sd_s2;
    sc_signal<bool> sltu_i_sd_s2;
    sc_signal<bool> and_i_sd_s2;
    sc_signal<bool> or_i_sd_s2;
    sc_signal<bool> xor_i_sd_s2;
    sc_signal<bool> sll_i_sd_s2;
    sc_signal<bool> srl_i_sd_s2;
    sc_signal<bool> sub_i_sd_s2;
    sc_signal<bool> sra_i_sd_s2;

    // I-type Instructions :

    sc_signal<bool> addi_i_sd_s2;
    sc_signal<bool> slti_i_sd_s2;
    sc_signal<bool> sltiu_i_sd_s2;
    sc_signal<bool> andi_i_sd_s2;
    sc_signal<bool> ori_i_sd_s2;
    sc_signal<bool> xori_i_sd_s2;

    sc_signal<bool> jalr_i_sd_s2;

    sc_signal<bool> fence_i_sd_s2;

    // I-type shift instructions :

    sc_signal<bool> slli_i_sd_s2;
    sc_signal<bool> srli_i_sd_s2;
    sc_signal<bool> srai_i_sd_s2;

    // I-type load instructions :

    sc_signal<bool> lw_i_sd_s2;
    sc_signal<bool> lh_i_sd_s2;
    sc_signal<bool> lhu_i_sd_s2;
    sc_signal<bool> lb_i_sd_s2;
    sc_signal<bool> lbu_i_sd_s2;

    // B-type Instruction :

    sc_signal<bool> beq_i_sd_s2;
    sc_signal<bool> bne_i_sd_s2;
    sc_signal<bool> blt_i_sd_s2;
    sc_signal<bool> bge_i_sd_s2;
    sc_signal<bool> bltu_i_sd_s2;
    sc_signal<bool> bgeu_i_sd_s2;

    // U-type Instruction :

    sc_signal<bool> lui_i_sd_s2;
    sc_signal<bool> auipc_i_sd_s2;

    // J-type Instruction :

    sc_signal<bool> jal_i_sd_s2;

    // S-type Instructions :

    sc_signal<bool> sw_i_sd_s2;
    sc_signal<bool> sh_i_sd_s2;
    sc_signal<bool> sb_i_sd_s2;

    // M-type Instructions :

    sc_signal<bool> mul_i_sd_s2;
    sc_signal<bool> mulh_i_sd_s2;
    sc_signal<bool> mulhsu_i_sd_s2;
    sc_signal<bool> mulhu_i_sd_s2;
    sc_signal<bool> div_i_sd_s2;
    sc_signal<bool> divu_i_sd_s2;
    sc_signal<bool> rem_i_sd_s2;
    sc_signal<bool> remu_i_sd_s2;

    // Kernel instruction :

    sc_signal<bool> csrrw_i_sd_s2;
    sc_signal<bool> csrrs_i_sd_s2;
    sc_signal<bool> csrrc_i_sd_s2;
    sc_signal<bool> csrrwi_i_sd_s2;
    sc_signal<bool> csrrsi_i_sd_s2;
    sc_signal<bool> csrrci_i_sd_s2;

    sc_signal<bool> csr_in_progress_s2;

    sc_signal<bool> mret_i_sd_s2;
    sc_signal<bool> sret_i_sd_s2;
    // Signal for Kernel usage

    sc_signal<bool>        csr_wenable_sd_s2;
    sc_signal<sc_uint<12>> csr_radr_sd_s2;
    sc_signal<sc_uint<32>> pc_branch_value_sd_s2;
    // Offset for branch :

    sc_signal<sc_uint<32>> offset_branch_sd_s2;

    // PC gestion :

    sc_signal<bool> inc_pc_sd_s2;
    sc_signal<bool> jump_sd_s2;

    // Pipeline Gestion

    sc_signal<bool> stall_sd_s2;
    // Internal signals :

    sc_signal<sc_uint<6>>  adr_dest_sd_s2;
    sc_signal<sc_uint<32>> exe_op1_sd_s2;
    sc_signal<sc_uint<32>> exe_op2_sd_s2;
    sc_signal<sc_uint<32>> mem_data_sd_s2;

    sc_signal<sc_uint<2>> mem_size_sd_s2;
    sc_signal<bool>       mem_load_sd_s2;
    sc_signal<bool>       mem_store_sd_s2;

    sc_signal<sc_uint<2>> exe_cmd_sd_s2;

    // operation types :
    // 0 : alu
    // 1 : shifter
    // 2 : multiplier
    // 3 : divider
    sc_signal<sc_uint<4>> select_type_operations_sd_s2;

    sc_signal<bool> exe_neg_op2_sd_s2;
    sc_signal<bool> exe_wb_sd_s2;
    sc_signal<bool> mem_sign_extend_sd_s2;
    sc_signal<bool> block_bp_sd_s2;

    // Exception :

    sc_signal<bool> ecall_i_sd_s2;
    sc_signal<bool> ebreak_i_sd_s2;
    sc_signal<bool> illegal_instruction_sd_s2;             // instruction doesnt exist
    sc_signal<bool> instruction_adress_missaligned_sd_s2;  // branch offset is misaligned
    sc_signal<bool> instruction_access_fault_sd_s2;
    sc_signal<bool> env_call_u_mode_sd_s2;
    sc_signal<bool> env_call_s_mode_sd_s2;
    sc_signal<bool> env_call_m_mode_sd_s2;
    sc_signal<bool> env_call_wrong_mode_s2;


    void concat_dec2exe();
    void unconcat_dec2exe();
    void concat_dec2if();
    void unconcat_dec2if();
    void decoding_instruction_type_s1();
    void decoding_instruction_s1();
    void pre_reg_read_decoding_s1();
    void post_reg_read_decoding_s1();
    void decoding_instruction_type_s2();
    void decoding_instruction_s2();
    void pre_reg_read_decoding_s2();
    void post_reg_read_decoding_s2();
    void pc_inc();
    void bypasses();
    void stall_method();
    void trace(sc_trace_file * tf);

    SC_CTOR(decod) : dec2if("dec2if"), dec2exe("dec2exe") {
        dec2if.DIN_S(dec2if_in_sd);
        dec2if.DOUT_R(dec2if_out_sd);
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

        SC_METHOD(concat_dec2exe)
        sensitive << dec2exe_in_sd << exe_op1_sd_s1 << exe_op2_sd_s1 << exe_cmd_sd_s1 << exe_neg_op2_sd_s1 << exe_wb_sd_s1

                  << mem_data_sd_s1 << mem_load_sd_s1 << mem_store_sd_s1

                  << mem_sign_extend_sd_s1 << mem_size_sd_s1 << select_type_operations_sd_s1 << adr_dest_sd_s1 << slti_i_sd_s1
                  << slt_i_sd_s1

                  << sltiu_i_sd_s1 << sltu_i_sd_s1 << RADR1_SD << CSR_RDATA_SC << csr_radr_sd_s1 << RADR2_SD << r1_valid_sd
                  << EXCEPTION_SM << r2_valid_sd << PC_IF2DEC_RI_S1 << csr_wenable_sd_s1 << illegal_instruction_sd_s1
                  << instruction_adress_missaligned_sd_s1 << env_call_m_mode_sd_s1 << block_bp_sd_s1 << env_call_s_mode_sd_s1
                  << env_call_u_mode_sd_s1 << env_call_wrong_mode_s1 << mret_i_sd_s1 << instruction_access_fault_sd_s1 << mul_i_sd_s1
                  << mulh_i_sd_s1 << mulhsu_i_sd_s1 << mulhu_i_sd_s1;
        SC_METHOD(unconcat_dec2exe)
        sensitive << dec2exe_out_sd;

        SC_METHOD(concat_dec2if)
        sensitive   << dec2if_pc_sd_s1 
                    << dec2if_pc_sd_s2;
        SC_METHOD(unconcat_dec2if)
        sensitive   << dec2if_out_sd ;

        SC_METHOD(stall_method)
        sensitive << b_type_inst_sd_s1 << jalr_type_inst_sd_s1 << j_type_inst_sd_s1 << r1_valid_sd << r2_valid_sd
                  << csr_wenable_sd_s1 << DEC2EXE_EMPTY_SD << CSR_WENABLE_RD << CSR_WENABLE_RE << BP_EXE2MEM_EMPTY_SE
                  << csr_in_progress_s1 << block_in_dec << IF2DEC_EMPTY_SI << dec2exe_full_sd << csr_in_progress_s1;

        SC_METHOD(decoding_instruction_type_s1)
        sensitive << INSTR_RI_S1 << READ_PC_SR;

        SC_METHOD(decoding_instruction_type_s2)
        sensitive << INSTR_RI_S2 << READ_PC_SR;
        
        SC_METHOD(decoding_instruction_s1)
        sensitive << INSTR_RI_S1;
        
        SC_METHOD(decoding_instruction_s2)
        sensitive << INSTR_RI_S2;
        
        SC_METHOD(pre_reg_read_decoding_s1)
        sensitive << INSTR_RI_S1 << r_type_inst_sd_s1 << i_type_inst_sd_s1 << i_type_inst_sd_s1 << s_type_inst_sd_s1 << b_type_inst_sd_s1
                  << u_type_inst_sd_s1 << j_type_inst_sd_s1 << m_type_inst_sd_s1 << jalr_type_inst_sd_s1 << beq_i_sd_s1 << bne_i_sd_s1
                  << blt_i_sd_s1 << bge_i_sd_s1 << bltu_i_sd_s1 << bgeu_i_sd_s1 << system_type_inst_sd_s1 << csrrw_i_sd_s1 << csrrs_i_sd_s1
                  << csrrc_i_sd_s1 << csrrwi_i_sd_s1 << csrrsi_i_sd_s1 << csrrci_i_sd_s1 << ecall_i_sd_s1 << ebreak_i_sd_s1 << fence_i_sd_s1
                  << mret_i_sd_s1 << sret_i_sd_s1 << RESET_N << CURRENT_MODE_SM;

        SC_METHOD(pre_reg_read_decoding_s2)
        sensitive << INSTR_RI_S2 << r_type_inst_sd_s1 << i_type_inst_sd_s1 << i_type_inst_sd_s1 << s_type_inst_sd_s1 << b_type_inst_sd_s1
                  << u_type_inst_sd_s1 << j_type_inst_sd_s1 << m_type_inst_sd_s1 << jalr_type_inst_sd_s1 << beq_i_sd_s1 << bne_i_sd_s1
                  << blt_i_sd_s1 << bge_i_sd_s1 << bltu_i_sd_s1 << bgeu_i_sd_s1 << system_type_inst_sd_s1 << csrrw_i_sd_s1 << csrrs_i_sd_s1
                  << csrrc_i_sd_s1 << csrrwi_i_sd_s1 << csrrsi_i_sd_s1 << csrrci_i_sd_s1 << ecall_i_sd_s1 << ebreak_i_sd_s1 << fence_i_sd_s1
                  << mret_i_sd_s1 << sret_i_sd_s1 << RESET_N << CURRENT_MODE_SM;

        SC_METHOD(post_reg_read_decoding_s1)
        sensitive << i_type_inst_sd_s1 << s_type_inst_sd_s1 << b_type_inst_sd_s1 << u_type_inst_sd_s1 << j_type_inst_sd_s1

                  << jalr_type_inst_sd_s1 << beq_i_sd_s1 << bne_i_sd_s1

                  << m_type_inst_sd_s1 << mul_i_sd_s1 << mulh_i_sd_s1 << mulhsu_i_sd_s1 << mulhu_i_sd_s1 << div_i_sd_s1 << divu_i_sd_s1
                  << rem_i_sd_s1 << remu_i_sd_s1

                  << blt_i_sd_s1 << bge_i_sd_s1 << bltu_i_sd_s1 << bgeu_i_sd_s1 << IF2DEC_EMPTY_SI << dec2if_push_sd << READ_PC_SR

                  << stall_sd_s1 << dec2if_push_sd << add_i_sd_s1 << slt_i_sd_s1 << sltu_i_sd_s1 << and_i_sd_s1 << or_i_sd_s1 << xor_i_sd_s1

                  << sll_i_sd_s1 << srl_i_sd_s1 << sub_i_sd_s1 << sra_i_sd_s1 << addi_i_sd_s1

                  << slti_i_sd_s1 << sltiu_i_sd_s1 << andi_i_sd_s1 << ori_i_sd_s1 << xori_i_sd_s1 << jalr_i_sd_s1 << slli_i_sd_s1

                  << srli_i_sd_s1 << srai_i_sd_s1 << lw_i_sd_s1 << lh_i_sd_s1

                  << lhu_i_sd_s1 << lb_i_sd_s1 << lbu_i_sd_s1 << beq_i_sd_s1 << bne_i_sd_s1 << blt_i_sd_s1 << bge_i_sd_s1 << bltu_i_sd_s1

                  << bgeu_i_sd_s1 << lui_i_sd_s1 << auipc_i_sd_s1

                  << jal_i_sd_s1 << sw_i_sd_s1 << sh_i_sd_s1 << sb_i_sd_s1 << j_type_inst_sd_s1 << jalr_type_inst_sd_s1 << dec2exe_push_sd

                  << rdata1_sd << rdata2_sd << r1_valid_sd << r2_valid_sd << system_type_inst_sd_s1 << csrrw_i_sd_s1

                  << csrrs_i_sd_s1 << csrrc_i_sd_s1 << csrrwi_i_sd_s1 << csrrsi_i_sd_s1 << csrrci_i_sd_s1 << CSR_RDATA_SC << ecall_i_sd_s1

                  << ebreak_i_sd_s1 << fence_i_sd_s1 << PC_IF2DEC_RI_S1 << EXCEPTION_SM << mret_i_sd_s1 << sret_i_sd_s1
                  << CSR_RDATA_SC;

        SC_METHOD(post_reg_read_decoding_s2)
        sensitive << i_type_inst_sd_s1 << s_type_inst_sd_s1 << b_type_inst_sd_s1 << u_type_inst_sd_s1 << j_type_inst_sd_s1

                  << jalr_type_inst_sd_s1 << beq_i_sd_s1 << bne_i_sd_s1

                  << m_type_inst_sd_s1 << mul_i_sd_s1 << mulh_i_sd_s1 << mulhsu_i_sd_s1 << mulhu_i_sd_s1 << div_i_sd_s1 << divu_i_sd_s1
                  << rem_i_sd_s1 << remu_i_sd_s1

                  << blt_i_sd_s1 << bge_i_sd_s1 << bltu_i_sd_s1 << bgeu_i_sd_s1 << IF2DEC_EMPTY_SI << dec2if_push_sd << READ_PC_SR

                  << stall_sd_s1 << dec2if_push_sd << add_i_sd_s1 << slt_i_sd_s1 << sltu_i_sd_s1 << and_i_sd_s1 << or_i_sd_s1 << xor_i_sd_s1

                  << sll_i_sd_s1 << srl_i_sd_s1 << sub_i_sd_s1 << sra_i_sd_s1 << addi_i_sd_s1

                  << slti_i_sd_s1 << sltiu_i_sd_s1 << andi_i_sd_s1 << ori_i_sd_s1 << xori_i_sd_s1 << jalr_i_sd_s1 << slli_i_sd_s1

                  << srli_i_sd_s1 << srai_i_sd_s1 << lw_i_sd_s1 << lh_i_sd_s1

                  << lhu_i_sd_s1 << lb_i_sd_s1 << lbu_i_sd_s1 << beq_i_sd_s1 << bne_i_sd_s1 << blt_i_sd_s1 << bge_i_sd_s1 << bltu_i_sd_s1

                  << bgeu_i_sd_s1 << lui_i_sd_s1 << auipc_i_sd_s1

                  << jal_i_sd_s1 << sw_i_sd_s1 << sh_i_sd_s1 << sb_i_sd_s1 << j_type_inst_sd_s1 << jalr_type_inst_sd_s1 << dec2exe_push_sd

                  << rdata1_sd << rdata2_sd << r1_valid_sd << r2_valid_sd << system_type_inst_sd_s1 << csrrw_i_sd_s1

                  << csrrs_i_sd_s1 << csrrc_i_sd_s1 << csrrwi_i_sd_s1 << csrrsi_i_sd_s1 << csrrci_i_sd_s1 << CSR_RDATA_SC << ecall_i_sd_s1

                  << ebreak_i_sd_s1 << fence_i_sd_s1 << PC_IF2DEC_RI_S1 << EXCEPTION_SM << mret_i_sd_s1 << sret_i_sd_s1
                  << CSR_RDATA_SC;

        SC_METHOD(pc_inc)
        sensitive << CLK.pos() << READ_PC_SR << offset_branch_sd_s1 << inc_pc_sd_s1 << jump_sd_s1 << MTVEC_VALUE_RC
                  << EXCEPTION_SM << PC_IF2DEC_RI_S1 << MRET_SM << dec2if_full_sd << IF2DEC_EMPTY_SI << MCAUSE_WDATA_SM
                  << stall_sd_s1;

        SC_METHOD(bypasses);
        sensitive << RDATA1_SR << RDATA2_SR << BP_DEST_RE << BP_EXE_RES_RE

                  << BP_DEST_RM << BP_MEM_RES_RM << RADR1_SD << EXE_DEST_SD

                  << RADR2_SD << BP_EXE2MEM_EMPTY_SE << MULT_INST_RE << MULT_INST_RM

                  << DEC2EXE_EMPTY_SD << BP_MEM_LOAD_RE << BP_MEM2WBK_EMPTY_SM;
        reset_signal_is(RESET_N, false);
    }
};