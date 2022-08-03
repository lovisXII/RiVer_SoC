#pragma once
#include <systemc.h>
#include <iostream>
#include "../UTIL/fifo.h"

#define dec2exe_size_s1 254
#define dec2exe_size_s2 252
#define dec2if_size     64

#define start_kernel_adress 0xF0000000

SC_MODULE(decod) {
    // Instance used :

    fifo<dec2if_size>     dec2if;
    fifo<dec2exe_size_s1> dec2exe_s1;
    fifo<dec2exe_size_s2> dec2exe_s2;
    // Interface with DEC2IF :

    sc_in<bool>       DEC2IF_POP_SI;  // Ifecth say to decod if it wants a pop or no
    sc_out<bool>      DEC2IF_EMPTY_SD;
    sc_out<sc_bv<32>> PC_DEC2IF_RD_S1;  // this value must also be sent to REG
    sc_out<sc_bv<32>> PC_DEC2IF_RD_S2;  // this value must also be sent to REG

    // Interface with IF2DEC :

    sc_in<sc_uint<32>> PC_IF2DEC_RI_S1;
    sc_in<sc_uint<32>> PC_IF2DEC_RI_S2;
    sc_in<sc_bv<32>>   INSTR_RI_S1;
    sc_in<sc_bv<32>>   INSTR_RI_S2;
    sc_in<bool>        IF2DEC_EMPTY_SI_S1;
    sc_in<bool>        IF2DEC_EMPTY_SI_S2;
    sc_out<bool>       IF2DEC_POP_SD_S1;  // Decod says to IFETCH if it wants a pop or no
    sc_out<bool>       IF2DEC_POP_SD_S2;
    sc_out<bool>       IF2DEC_FLUSH_SD;//12

    // Interface with IFTECH
    
    sc_out<sc_uint<2>> PRIORITARY_PIPELINE_RD ; // indicate which fifo is the most recent

    // Interface with CSR :

    sc_out<sc_uint<12>> CSR_RADR_SD_S1;   // CSR adress sent to CSR to get data
    sc_out<sc_uint<12>> CSR_RADR_SD_S2;   // CSR adress sent to CSR to get data
    sc_in<sc_uint<32>>  CSR_RDATA_SC_S1;  // data read from CSR
    sc_in<sc_uint<32>>  CSR_RDATA_SC_S2;  // data read from CSR
    sc_in<sc_uint<32>>  KERNEL_ADR_SC;
    // Interface with REG :

    sc_in<sc_uint<32>> RDATA1_SR_S1;
    sc_in<sc_uint<32>> RDATA2_SR_S1;

    sc_in<sc_uint<32>> RDATA1_SR_S2;
    sc_in<sc_uint<32>> RDATA2_SR_S2;

    sc_in<sc_uint<32>> READ_PC_SR;  // value of r32 which is pc coming from REG

    sc_out<sc_uint<6>> RADR1_SD_S1;  // adress of rs
    sc_out<sc_uint<6>> RADR2_SD_S1;  // adress of rt

    sc_out<sc_uint<6>> RADR1_SD_S2;  // adress of rs
    sc_out<sc_uint<6>> RADR2_SD_S2;  // adress of rt

    sc_out<sc_uint<32>> WRITE_PC_SD;
    sc_out<bool>        WRITE_PC_ENABLE_SD;

    // Interface with EXE_S1 :

    sc_out<sc_uint<32>> OP1_RD_S1;                     // value of op1
    sc_out<sc_uint<32>> OP2_RD_S1;         //30            // value of op2
    sc_out<sc_uint<2>>  EXE_CMD_RD_S1;                 // value of the command sent to exe
    sc_out<bool>        NEG_OP2_RD_S1;                 // allow to take not(op2) to do substraction
    sc_out<bool>        WB_RD_S1;                      // say if we plan to wbk the value of rd or no
    sc_out<sc_uint<6>>  EXE_DEST_RD_S1;                // the destination register
    sc_out<sc_uint<4>>  SELECT_TYPE_OPERATIONS_RD_S1;  // taille fifo entrée : 110
    sc_out<bool>        SLT_RD_S1;
    sc_out<bool>        SLTU_RD_S1;
    sc_out<sc_uint<32>> PC_DEC2EXE_RD_S1;       // PC link to the current decoded instruction
    sc_out<sc_uint<32>> PC_BRANCH_VALUE_RD_S1;  // PC of the branch value, will be usefull for exception
    sc_out<sc_uint<32>> MEM_DATA_RD_S1;        //40 // data sent to mem for storage
    sc_out<bool>        MEM_LOAD_RD_S1;         // say to mem if we do a load
    sc_out<bool>        MEM_STORE_RD_S1;        // say to mem if we do a store
    sc_out<bool>        MEM_SIGN_EXTEND_RD_S1;
    sc_out<sc_uint<2>>  MEM_SIZE_RD_S1;  // tells to mem if we do an acces in word, hw or byte

    sc_inout<bool> CSR_WENABLE_RD_S1;     // indicate if we do a csr operation,
                                          // if so need to WBK CSR in rd
    sc_out<sc_uint<12>> CSR_WADR_RD_S1;   // CSR adress sent to EXE, will allow to wbk csr in MEM
    sc_out<sc_uint<32>> CSR_RDATA_RD_S1;  // CSR read data to be wb in register

    // Interface with EXE_S2 :

    sc_out<sc_uint<32>> OP1_RD_S2;      // value of op1
    sc_out<sc_uint<32>> OP2_RD_S2;      // value of op2
    sc_out<sc_uint<2>>  EXE_CMD_RD_S2;  // value of the command sent to exe
    sc_out<bool>        NEG_OP2_RD_S2;  // say if we take the opposite of the op1 to do a
                                        // substraction for example

    sc_out<bool>        WB_RD_S2;                      // say if we plan to wbk the value of rd or no
    sc_out<sc_uint<6>>  EXE_DEST_RD_S2;                // the destination register
    sc_out<sc_uint<4>>  SELECT_TYPE_OPERATIONS_RD_S2;  // taille fifo entrée : 110
    sc_out<bool>        SLT_RD_S2;
    sc_out<bool>        SLTU_RD_S2;             //56
    sc_out<sc_uint<32>> PC_DEC2EXE_RD_S2;       // PC link to the current decoded instruction
    sc_out<sc_uint<32>> PC_BRANCH_VALUE_RD_S2;  // PC of the branch value, will be usefull for exception
    sc_out<sc_uint<32>> MEM_DATA_RD_S2;         // data sent to mem for storage
    sc_out<bool>        MEM_LOAD_RD_S2;         // say to mem if we do a load
    sc_out<bool>        MEM_STORE_RD_S2;        // say to mem if we do a store
    sc_out<bool>        MEM_SIGN_EXTEND_RD_S2;
    sc_out<sc_uint<2>>  MEM_SIZE_RD_S2;  // tells to mem if we do an acces in word, hw or byte

    sc_inout<bool> CSR_WENABLE_RD_S2;     // indicate if we do a csr operation,
                                          // if so need to WBK CSR in rd
    sc_out<sc_uint<12>> CSR_WADR_RD_S2;   // CSR adress sent to EXE, will allow to wbk csr in MEM
    sc_out<sc_uint<32>> CSR_RDATA_RD_S2;  // CSR read data to be wb in register

    // Interface with DEC2EXE

    sc_in<bool>                       DEC2EXE_POP_SE_S1;
    sc_out<bool>                      DEC2EXE_EMPTY_SD_S1;//69
    sc_signal<sc_bv<dec2exe_size_s1>> dec2exe_out_sd_s1;

    sc_in<bool>                       DEC2EXE_POP_SE_S2;
    sc_out<bool>                      DEC2EXE_EMPTY_SD_S2;//71
    sc_signal<sc_bv<dec2exe_size_s2>> dec2exe_out_sd_s2;

    // Multiplications signals
    sc_out<bool> MULT_INST_RD_S1;
    sc_out<bool> MULT_INST_RD_S2;

    sc_in<bool> MULT_INST_RM_S1;//75
    sc_in<bool> MULT_INST_RE_S1;

    // Bypasses with S1

    sc_in<sc_uint<6>>  DEST_RE_S1;
    sc_in<sc_uint<32>> EXE_RES_RE_S1;
    sc_in<bool>        MEM_LOAD_RE_S1;
    sc_in<bool>        EXE2MEM_EMPTY_SE_S1;
    sc_in<bool>        MEM2WBK_EMPTY_SM_S1;
    sc_in<sc_uint<6>>  DEST_RM_S1;
    sc_in<sc_uint<32>> MEM_RES_RM_S1;

    sc_in<bool>        CSR_WENABLE_RE_S1;
    sc_in<sc_uint<32>> CSR_RDATA_RE_S1;
    sc_in<bool>        CSR_WENABLE_RM_S1;
    sc_in<sc_uint<32>> CSR_RDATA_RM_S1;

    sc_out<bool>       BP_R1_VALID_RD_S1;
    sc_out<bool>       BP_R2_VALID_RD_S1;
    sc_out<bool>       BP_R1_VALID_RD_S2;
    sc_out<bool>       BP_R2_VALID_RD_S2;
    sc_out<sc_uint<6>> BP_RADR1_RD_S1;
    sc_out<sc_uint<6>> BP_RADR2_RD_S1;
    sc_out<sc_uint<6>> BP_RADR1_RD_S2;
    sc_out<sc_uint<6>> BP_RADR2_RD_S2;
    sc_out<bool>       BLOCK_BP_RD_S1;
    sc_out<bool>       BLOCK_BP_RD_S2;


    // Bypasses with S2

    sc_in<sc_uint<6>>   DEST_RE_S2 ;
    sc_in<bool>         MEM_LOAD_RE_S2;
    sc_in<bool>         EXE2MEM_EMPTY_SE_S2;
    sc_in<bool>         CSR_WENABLE_RE_S2;
    sc_in<sc_uint<32>>  CSR_RDATA_RE_S2;

    sc_in<sc_uint<6>>   DEST_RM_S2;
    sc_in<bool>         CSR_WENABLE_RM_S2;
    sc_in<sc_uint<32>>  CSR_RDATA_RM_S2;
    sc_in<bool>         MEM2WBK_EMPTY_SM_S2;
    sc_in<sc_uint<32>>  EXE_RES_RE_S2;
    sc_in<sc_uint<32>>  MEM_RES_RM_S2;



    // Exception/Interruption :

    sc_in<bool> EXCEPTION_RI;  // this signal will be at 0 considering there is no exception in IFETCH

    sc_out<bool>      ILLEGAL_INSTRUCTION_RD_S1;  // instruction doesnt exist
    sc_out<bool>      ADRESS_MISSALIGNED_RD_S1;   // branch offset is misaligned
    sc_out<bool>      ENV_CALL_U_MODE_RD_S1;
    sc_out<bool>      ENV_CALL_M_MODE_RD_S1;
    sc_out<bool>      ENV_CALL_S_MODE_RD_S1;
    sc_out<bool>      ENV_CALL_WRONG_MODE_RD_S1;
    sc_out<bool>      INSTRUCTION_ACCESS_FAULT_RD_S1;
    sc_out<bool>      MRET_RD_S1;
    sc_out<bool>      EXCEPTION_RD_S1;
    sc_out<bool>      EBREAK_RD_S1;

    sc_out<bool>      ILLEGAL_INSTRUCTION_RD_S2;  // instruction doesnt exist
    sc_out<bool>      ADRESS_MISSALIGNED_RD_S2;   // branch offset is misaligned
    sc_out<bool>      ENV_CALL_U_MODE_RD_S2;
    sc_out<bool>      ENV_CALL_M_MODE_RD_S2;
    sc_out<bool>      ENV_CALL_S_MODE_RD_S2;
    sc_out<bool>      ENV_CALL_WRONG_MODE_RD_S2;
    sc_out<bool>      INSTRUCTION_ACCESS_FAULT_RD_S2;
    sc_out<bool>      MRET_RD_S2;
    sc_out<bool>      EXCEPTION_RD_S2;
    sc_out<bool>      EBREAK_RD_S2;
    sc_in<sc_uint<2>> CURRENT_MODE_SM;

    // Interruption :

    sc_in<bool> INTERRUPTION_SE_S1;

    // General Interface :

    sc_in<bool>        EXCEPTION_SM;
    sc_in<sc_uint<32>> MTVEC_VALUE_RC;
    sc_in<sc_uint<32>> MCAUSE_WDATA_SM_S1;
    sc_in_clk          CLK;
    sc_in<bool>        RESET_N;
    sc_in<bool>        MRET_SM;
    sc_in<sc_uint<32>> RETURN_ADRESS_SM;

    // Signals :

    sc_signal<sc_uint<32>> rdata1_sd_s1;
    sc_signal<sc_uint<32>> rdata2_sd_s1;
    sc_signal<sc_uint<32>> rdata1_sd_s2;
    sc_signal<sc_uint<32>> rdata2_sd_s2;
    sc_signal<bool>        r1_valid_sd_s1;
    sc_signal<bool>        r2_valid_sd_s1;
    sc_signal<bool>        r1_valid_sd_s2;
    sc_signal<bool>        r2_valid_sd_s2;
    sc_signal<bool>        block_in_dec;

    // fifo dec2if :

    sc_signal<sc_bv<32>> dec2if_pc_sd_s1;  // pc sent to fifo
    sc_signal<sc_bv<32>> dec2if_pc_sd_s2;  // pc sent to fifo
    sc_signal<bool>      dec2if_push_sd;
    sc_signal<bool>      dec2if_empty_sd;
    sc_signal<bool>      dec2if_full_sd;

    // fifo dec2exe_s1 :

    sc_signal<sc_bv<dec2exe_size_s1>> dec2exe_in_sd_s1;
    sc_signal<bool>                   dec2exe_push_sd_s1;
    sc_signal<bool>                   dec2exe_full_sd_s1;

    sc_signal<sc_bv<dec2exe_size_s2>> dec2exe_in_sd_s2;
    sc_signal<bool>                   dec2exe_push_sd_s2;
    sc_signal<bool>                   dec2exe_full_sd_s2;

    // fifo dec2if :

    sc_signal<sc_bv<dec2if_size>> dec2if_in_sd;
    sc_signal<sc_bv<dec2if_size>> dec2if_out_sd;

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
    sc_signal<bool> add_offset_to_pc_s1;

    // Pipeline Gestion

    sc_signal<bool> stall_sd;
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
    sc_signal<bool> add_offset_to_pc_s2;

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

    // Super-scalar parameters :

    sc_signal<bool> reg_dependencies_sd ; 
    sc_signal<sc_uint<2>> prioritary_pipeline_sd ; // indicate which fifo is the most recent
    sc_signal<sc_uint<2>> prioritary_pipeline_rd;
    sc_signal<bool> flushing_inst_s2 ;

    void concat_dec2exe_s1();
    void unconcat_dec2exe_s1();
    
    void concat_dec2exe_s2() ;
    void unconcat_dec2exe_s2();
    
    void prio_pipeline_reg_gestion();

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
    void dependencies() ;
    void prio_pipeline_signal();
    //FSM :


    void trace(sc_trace_file * tf);

    SC_CTOR(decod) : dec2if("dec2if"), dec2exe_s1("dec2exe_s1"), dec2exe_s2("dec2exe_s2") {
        dec2if.DIN_S(dec2if_in_sd);
        dec2if.DOUT_R(dec2if_out_sd);
        dec2if.EMPTY_S(dec2if_empty_sd);
        dec2if.FULL_S(dec2if_full_sd);
        dec2if.PUSH_S(dec2if_push_sd);
        dec2if.POP_S(DEC2IF_POP_SI);
        dec2if.CLK(CLK);
        dec2if.RESET_N(RESET_N);

        dec2exe_s1.DIN_S(dec2exe_in_sd_s1);
        dec2exe_s1.DOUT_R(dec2exe_out_sd_s1);
        dec2exe_s1.EMPTY_S(DEC2EXE_EMPTY_SD_S1);
        dec2exe_s1.FULL_S(dec2exe_full_sd_s1);
        dec2exe_s1.PUSH_S(dec2exe_push_sd_s1);
        dec2exe_s1.POP_S(DEC2EXE_POP_SE_S1);
        dec2exe_s1.CLK(CLK);
        dec2exe_s1.RESET_N(RESET_N);

        dec2exe_s2.DIN_S(dec2exe_in_sd_s2);
        dec2exe_s2.DOUT_R(dec2exe_out_sd_s2);
        dec2exe_s2.EMPTY_S(DEC2EXE_EMPTY_SD_S2);
        dec2exe_s2.FULL_S(dec2exe_full_sd_s2);
        dec2exe_s2.PUSH_S(dec2exe_push_sd_s2);
        dec2exe_s2.POP_S(DEC2EXE_POP_SE_S2);
        dec2exe_s2.CLK(CLK);
        dec2exe_s2.RESET_N(RESET_N);

        SC_METHOD(dependencies)
        sensitive   
            << CSR_WENABLE_RD_S1
            << DEC2EXE_EMPTY_SD_S1
            << CSR_WENABLE_RE_S1
            << EXE2MEM_EMPTY_SE_S1

            << CSR_WENABLE_RD_S2
            << DEC2EXE_EMPTY_SD_S2
            << CSR_WENABLE_RE_S2

            << csr_wenable_sd_s2

            << EXE2MEM_EMPTY_SE_S2
			<< adr_dest_sd_s1
			<< RADR1_SD_S2
			<< RADR2_SD_S2;
        SC_METHOD(prio_pipeline_signal)
        sensitive   << reg_dependencies_sd
                    << stall_sd
                    << jump_sd_s1
                    << jump_sd_s2
                    << dec2exe_full_sd_s1
                    << dec2exe_full_sd_s2
                    << prioritary_pipeline_rd
                    << EXCEPTION_SM;
        SC_METHOD(prio_pipeline_reg_gestion)
        sensitive 
			<< CLK.pos();
        SC_METHOD(concat_dec2exe_s1)
        sensitive 
			<< dec2exe_in_sd_s1 
			<< exe_op1_sd_s1 
			<< exe_op2_sd_s1 
			<< exe_cmd_sd_s1 
			<< exe_neg_op2_sd_s1
                  
			<< exe_wb_sd_s1

                  
			<< mem_data_sd_s1 
			<< mem_load_sd_s1 
			<< mem_store_sd_s1

                  
			<< mem_sign_extend_sd_s1 
			<< mem_size_sd_s1 
			<< select_type_operations_sd_s1 
			<< adr_dest_sd_s1
                  
			<< slti_i_sd_s1 
			<< slt_i_sd_s1

                  
			<< sltiu_i_sd_s1 
			<< sltu_i_sd_s1 
			<< RADR1_SD_S1 
			<< CSR_RDATA_SC_S1 
			<< csr_radr_sd_s1 
			<< RADR2_SD_S1
                  
			<< r1_valid_sd_s1 
			<< EXCEPTION_SM 
			<< r2_valid_sd_s1 
			<< PC_IF2DEC_RI_S1 
			<< csr_wenable_sd_s1
                  
			<< illegal_instruction_sd_s1 
			<< instruction_adress_missaligned_sd_s1 
			<< env_call_m_mode_sd_s1
                  
			<< block_bp_sd_s1 
			<< env_call_s_mode_sd_s1 
			<< env_call_u_mode_sd_s1 
			<< env_call_wrong_mode_s1
                  
			<< mret_i_sd_s1 
			<< instruction_access_fault_sd_s1 
			<< mul_i_sd_s1 
			<< mulh_i_sd_s1 
			<< mulhsu_i_sd_s1
                  
			<< mulhu_i_sd_s1;


        SC_METHOD(concat_dec2exe_s2)
        sensitive 
			<< dec2exe_in_sd_s2 
			<< exe_op1_sd_s2 
			<< exe_op2_sd_s2 
			<< exe_cmd_sd_s2 
			<< exe_neg_op2_sd_s2
                  
			<< exe_wb_sd_s2

                  
			<< mem_data_sd_s2 
			<< mem_load_sd_s2 
			<< mem_store_sd_s2

                  
			<< mem_sign_extend_sd_s2 
			<< mem_size_sd_s2 
			<< select_type_operations_sd_s2 
			<< adr_dest_sd_s2
                  
			<< slti_i_sd_s2 
			<< slt_i_sd_s2

                  
			<< sltiu_i_sd_s2 
			<< sltu_i_sd_s2 
			<< RADR1_SD_S2 
			<< CSR_RDATA_SC_S2 
			<< csr_radr_sd_s2 
			<< RADR2_SD_S2
                  
			<< r1_valid_sd_s1 
			<< EXCEPTION_SM 
			<< r2_valid_sd_s1 
			<< PC_IF2DEC_RI_S2 
			<< csr_wenable_sd_s2
                  
			<< illegal_instruction_sd_s2 
			<< instruction_adress_missaligned_sd_s2 
			<< env_call_m_mode_sd_s2
                  
			<< block_bp_sd_s2 
			<< env_call_s_mode_sd_s2 
			<< env_call_u_mode_sd_s2 
			<< env_call_wrong_mode_s2
                  
			<< mret_i_sd_s2 
			<< instruction_access_fault_sd_s2 
			<< mul_i_sd_s2 
			<< mulh_i_sd_s2 
			<< mulhsu_i_sd_s2
                  
			<< mulhu_i_sd_s2 
			<< reg_dependencies_sd;
        SC_METHOD(unconcat_dec2exe_s1)
        sensitive 
			<< dec2exe_out_sd_s1;

        SC_METHOD(unconcat_dec2exe_s2)
        sensitive 
			<< dec2exe_out_sd_s2;
        SC_METHOD(concat_dec2if)
        sensitive 
			<< dec2if_pc_sd_s1 
			<< dec2if_pc_sd_s2;
        SC_METHOD(unconcat_dec2if)
        sensitive 
			<< dec2if_out_sd;

        SC_METHOD(stall_method)
        sensitive 
            << csr_in_progress_s1
            << csr_in_progress_s2
            
            << csr_wenable_sd_s1
            << csr_wenable_sd_s2
            
            << r1_valid_sd_s1
            << r2_valid_sd_s1
            << r1_valid_sd_s2
            << r2_valid_sd_s2
            
            << block_in_dec

            << b_type_inst_sd_s1
            << jalr_type_inst_sd_s1
            << j_type_inst_sd_s1
            << b_type_inst_sd_s2
            << jalr_type_inst_sd_s2
            << j_type_inst_sd_s2
            
            << IF2DEC_EMPTY_SI_S1
            << IF2DEC_EMPTY_SI_S2
            << dec2exe_full_sd_s1
            << dec2exe_full_sd_s2;

        SC_METHOD(decoding_instruction_type_s1)
        sensitive 
			<< INSTR_RI_S1 
			<< READ_PC_SR;

        SC_METHOD(decoding_instruction_type_s2)
        sensitive 
			<< INSTR_RI_S2 
			<< READ_PC_SR;

        SC_METHOD(decoding_instruction_s1)
        sensitive 
			<< INSTR_RI_S1;

        SC_METHOD(decoding_instruction_s2)
        sensitive 
			<< INSTR_RI_S2;

        SC_METHOD(pre_reg_read_decoding_s1)
        sensitive 
			<< INSTR_RI_S1 
			<< r_type_inst_sd_s1 
			<< i_type_inst_sd_s1 
			<< i_type_inst_sd_s1 
			<< s_type_inst_sd_s1
                  
			<< b_type_inst_sd_s1 
			<< u_type_inst_sd_s1 
			<< j_type_inst_sd_s1 
			<< m_type_inst_sd_s1
                  
			<< jalr_type_inst_sd_s1 
			<< beq_i_sd_s1 
			<< bne_i_sd_s1 
			<< blt_i_sd_s1 
			<< bge_i_sd_s1 
			<< bltu_i_sd_s1
                  
			<< bgeu_i_sd_s1 
			<< system_type_inst_sd_s1 
			<< csrrw_i_sd_s1 
			<< csrrs_i_sd_s1 
			<< csrrc_i_sd_s1
                  
			<< csrrwi_i_sd_s1 
			<< csrrsi_i_sd_s1 
			<< csrrci_i_sd_s1 
			<< ecall_i_sd_s1 
			<< ebreak_i_sd_s1
                  
			<< fence_i_sd_s1 
			<< mret_i_sd_s1 
			<< sret_i_sd_s1 
			<< RESET_N 
			<< CURRENT_MODE_SM;

        SC_METHOD(pre_reg_read_decoding_s2)
        sensitive 
			<< INSTR_RI_S2 
			<< r_type_inst_sd_s2 
			<< i_type_inst_sd_s2 
			<< i_type_inst_sd_s2 
			<< s_type_inst_sd_s2
                  
			<< b_type_inst_sd_s2 
			<< u_type_inst_sd_s2 
			<< j_type_inst_sd_s2 
			<< m_type_inst_sd_s2
                  
			<< jalr_type_inst_sd_s2 
			<< beq_i_sd_s2 
			<< bne_i_sd_s2 
			<< blt_i_sd_s2 
			<< bge_i_sd_s2 
			<< bltu_i_sd_s2
                  
			<< bgeu_i_sd_s2 
			<< system_type_inst_sd_s2 
			<< csrrw_i_sd_s2 
			<< csrrs_i_sd_s2 
			<< csrrc_i_sd_s2
                  
			<< csrrwi_i_sd_s2 
			<< csrrsi_i_sd_s2 
			<< csrrci_i_sd_s2 
			<< ecall_i_sd_s2 
			<< ebreak_i_sd_s2
                  
			<< fence_i_sd_s2 
			<< mret_i_sd_s2 
			<< sret_i_sd_s2 
			<< RESET_N 
			<< CURRENT_MODE_SM;

        SC_METHOD(post_reg_read_decoding_s1)
        sensitive 
			<< i_type_inst_sd_s1 
			<< s_type_inst_sd_s1 
			<< b_type_inst_sd_s1 
			<< u_type_inst_sd_s1
                  
			<< j_type_inst_sd_s1

                  
			<< jalr_type_inst_sd_s1 
			<< beq_i_sd_s1 
			<< bne_i_sd_s1

                  
			<< m_type_inst_sd_s1 
			<< mul_i_sd_s1 
			<< mulh_i_sd_s1 
			<< mulhsu_i_sd_s1 
			<< mulhu_i_sd_s1 
			<< div_i_sd_s1
                  
			<< divu_i_sd_s1 
			<< rem_i_sd_s1 
			<< remu_i_sd_s1

                  
			<< blt_i_sd_s1 
			<< bge_i_sd_s1 
			<< bltu_i_sd_s1 
			<< bgeu_i_sd_s1 
			<< IF2DEC_EMPTY_SI_S1 
			<< dec2if_push_sd
                  
			<< READ_PC_SR

                  
			<< stall_sd 
			<< dec2if_push_sd 
			<< add_i_sd_s1 
			<< slt_i_sd_s1 
			<< sltu_i_sd_s1 
			<< and_i_sd_s1
                  
			<< or_i_sd_s1 
			<< xor_i_sd_s1

                  
			<< sll_i_sd_s1 
			<< srl_i_sd_s1 
			<< sub_i_sd_s1 
			<< sra_i_sd_s1 
			<< addi_i_sd_s1

                  
			<< slti_i_sd_s1 
			<< sltiu_i_sd_s1 
			<< andi_i_sd_s1 
			<< ori_i_sd_s1 
			<< xori_i_sd_s1 
			<< jalr_i_sd_s1
                  
			<< slli_i_sd_s1

                  
			<< srli_i_sd_s1 
			<< srai_i_sd_s1 
			<< lw_i_sd_s1 
			<< lh_i_sd_s1

                  
			<< lhu_i_sd_s1 
			<< lb_i_sd_s1 
			<< lbu_i_sd_s1 
			<< beq_i_sd_s1 
			<< bne_i_sd_s1 
			<< blt_i_sd_s1
                  
			<< bge_i_sd_s1 
			<< bltu_i_sd_s1

                  
			<< bgeu_i_sd_s1 
			<< lui_i_sd_s1 
			<< auipc_i_sd_s1

                  
			<< jal_i_sd_s1 
			<< sw_i_sd_s1 
			<< sh_i_sd_s1 
			<< sb_i_sd_s1 
			<< j_type_inst_sd_s1 
			<< jalr_type_inst_sd_s1
                  
			<< dec2exe_push_sd_s1

                  
			<< rdata1_sd_s1 
			<< rdata2_sd_s1 
			<< r1_valid_sd_s1 
			<< r2_valid_sd_s1 
			<< system_type_inst_sd_s1
                  
			<< csrrw_i_sd_s1

                  
			<< csrrs_i_sd_s1 
			<< csrrc_i_sd_s1 
			<< csrrwi_i_sd_s1 
			<< csrrsi_i_sd_s1 
			<< csrrci_i_sd_s1
                  
			<< CSR_RDATA_SC_S1 
			<< ecall_i_sd_s1

                  
			<< ebreak_i_sd_s1 
			<< fence_i_sd_s1 
			<< PC_IF2DEC_RI_S1 
			<< EXCEPTION_SM 
			<< mret_i_sd_s1 
			<< sret_i_sd_s1
            << PC_IF2DEC_RI_S1
			<< CSR_RDATA_SC_S1;

        SC_METHOD(post_reg_read_decoding_s2)
        sensitive 
			<< i_type_inst_sd_s2 
			<< s_type_inst_sd_s2 
			<< b_type_inst_sd_s2 
			<< u_type_inst_sd_s2
                  
			<< j_type_inst_sd_s2

                  
			<< jalr_type_inst_sd_s2 
			<< beq_i_sd_s2 
			<< bne_i_sd_s2

                  
			<< m_type_inst_sd_s2 
			<< mul_i_sd_s2 
			<< mulh_i_sd_s2 
			<< mulhsu_i_sd_s2 
			<< mulhu_i_sd_s2 
			<< div_i_sd_s2
                  
			<< divu_i_sd_s2 
			<< rem_i_sd_s2 
			<< remu_i_sd_s2

                  
			<< blt_i_sd_s2 
			<< bge_i_sd_s2 
			<< bltu_i_sd_s2 
			<< bgeu_i_sd_s2 
			<< IF2DEC_EMPTY_SI_S2 
			<< dec2if_push_sd
                  
			<< READ_PC_SR

                  
			<< stall_sd 
			<< dec2if_push_sd 
			<< add_i_sd_s2 
			<< slt_i_sd_s2 
			<< sltu_i_sd_s2 
			<< and_i_sd_s2
                  
			<< or_i_sd_s2 
			<< xor_i_sd_s2

                  
			<< sll_i_sd_s2 
			<< srl_i_sd_s2 
			<< sub_i_sd_s2 
			<< sra_i_sd_s2 
			<< addi_i_sd_s2

                  
			<< slti_i_sd_s2 
			<< sltiu_i_sd_s2 
			<< andi_i_sd_s2 
			<< ori_i_sd_s2 
			<< xori_i_sd_s2 
			<< jalr_i_sd_s2
                  
			<< slli_i_sd_s2

                  
			<< srli_i_sd_s2 
			<< srai_i_sd_s2 
			<< lw_i_sd_s2 
			<< lh_i_sd_s2

                  
			<< lhu_i_sd_s2 
			<< lb_i_sd_s2 
			<< lbu_i_sd_s2 
			<< beq_i_sd_s2 
			<< bne_i_sd_s2 
			<< blt_i_sd_s2
                  
			<< bge_i_sd_s2 
			<< bltu_i_sd_s2

                  
			<< bgeu_i_sd_s2 
			<< lui_i_sd_s2 
			<< auipc_i_sd_s2

                  
			<< jal_i_sd_s2 
			<< sw_i_sd_s2 
			<< sh_i_sd_s2 
			<< sb_i_sd_s2 
			<< j_type_inst_sd_s2 
			<< jalr_type_inst_sd_s2
                  
			<< dec2exe_push_sd_s2

                  
			<< rdata1_sd_s2 
			<< rdata2_sd_s2 
			<< r1_valid_sd_s2 
			<< r2_valid_sd_s2 
			<< system_type_inst_sd_s2
                  
			<< csrrw_i_sd_s2

                  
			<< csrrs_i_sd_s2 
			<< csrrc_i_sd_s2 
			<< csrrwi_i_sd_s2 
			<< csrrsi_i_sd_s2 
			<< csrrci_i_sd_s2
                  
			<< CSR_RDATA_SC_S2 
			<< ecall_i_sd_s2

                  
			<< ebreak_i_sd_s2 
			<< fence_i_sd_s2 
			<< PC_IF2DEC_RI_S2 
			<< EXCEPTION_SM 
			<< mret_i_sd_s2 
			<< sret_i_sd_s2
            << jump_sd_s1
			<< CSR_RDATA_SC_S2;

        SC_METHOD(pc_inc)
        sensitive 
			<< CLK.pos() 
                  
			<< READ_PC_SR 
                  
			<< offset_branch_sd_s1 
                  
			<< offset_branch_sd_s2 
                  
			<< jump_sd_s1  
                  
			<< jump_sd_s2 
                  
			<< MTVEC_VALUE_RC
                  
			<< EXCEPTION_SM 
                  
			<< PC_IF2DEC_RI_S1 
                  
			<< MRET_SM 
                  
			<< dec2if_full_sd 
                  
			<< IF2DEC_EMPTY_SI_S1 
                  
			<< MCAUSE_WDATA_SM_S1
                  
			<< stall_sd
                  
			<< add_offset_to_pc_s1
                  
			<< add_offset_to_pc_s2
                  
			<< reg_dependencies_sd
            << PC_IF2DEC_RI_S2
            << PRIORITARY_PIPELINE_RD;

        SC_METHOD(bypasses);
        sensitive   
			<< RDATA1_SR_S1 
                    
			<< RDATA2_SR_S1 
                    
			<< DEST_RE_S1 
                    
			<< EXE_RES_RE_S1
                    
			<< DEST_RM_S1 
                    
			<< MEM_RES_RM_S1 
                    
			<< RADR1_SD_S1 
                    
			<< EXE_DEST_RD_S1
                    
			<< RADR2_SD_S1 
                    
			<< EXE2MEM_EMPTY_SE_S1 
                    
			<< MULT_INST_RE_S1 
                    
			<< MULT_INST_RM_S1
                    
			<< DEC2EXE_EMPTY_SD_S1 
                    
			<< MEM_LOAD_RE_S1 
                    
			<< MEM2WBK_EMPTY_SM_S1

                    
			<< RDATA1_SR_S2 
                    
			<< RDATA2_SR_S2 
                    
			<< DEST_RE_S2 
                    
			<< EXE_RES_RE_S2
                    
			<< DEST_RM_S2 
                    
			<< MEM_RES_RM_S1 
                    
			<< RADR1_SD_S2 
                    
			<< EXE_DEST_RD_S2
                    
			<< RADR2_SD_S2 
                    
			<< EXE2MEM_EMPTY_SE_S2 
                    
			<< DEC2EXE_EMPTY_SD_S2 
                    
			<< MEM_LOAD_RE_S2 
            << reg_dependencies_sd
			<< MEM2WBK_EMPTY_SM_S2;
        reset_signal_is(RESET_N, false);
    }
};