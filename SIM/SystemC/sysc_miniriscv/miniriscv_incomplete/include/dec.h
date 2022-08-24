#include <systemc.h>
#include <iostream>
#include "constants.h"
#include "fifo.h"

SC_MODULE(decod) {
    /*****************************************************
                    Interface with REG
    ******************************************************/

    // adresses of Rs and Rd
    sc_out<sc_uint<6>> RADR1_SD;
    sc_out<sc_uint<6>> RADR2_SD;
    // Data read in registers
    sc_in<sc_uint<32>> RDATA1_SR;
    sc_in<sc_uint<32>> RDATA2_SR;

    // Current value of PC
    sc_in<sc_uint<32>> READ_PC_SR;

    // Port to write into PC
    sc_out<sc_uint<32>> WRITE_PC_SD;
    sc_out<bool>        WRITE_PC_ENABLE_SD;

    /*****************************************************
                    Interface with EXE
    ******************************************************/

    sc_out<sc_uint<32>> OP1_RD;
    sc_out<sc_uint<32>> OP2_RD;

    /* Command sent to EXE.
    In case of a shift :
        - 0 : Shift Left Logical (sll)
        - 1 : Shift Right Logical (srl)
        - 2 : Shift Right Arithmetic (sra)

    In case of an ALU operation :
        - 0 : add
        - 1 : and
        - 2 : or
        - 3 : xor
    In case of an SLT operation
        - 0 : slt
        - 1 : sltu
    */
    sc_out<sc_uint<2>> EXE_CMD_RD;

    sc_out<bool>       NEG_OP2_RD;   // negates the second operator, to do substractions
    sc_out<bool>       WB_RD;        // write back the result in reg
    sc_out<sc_uint<6>> EXE_DEST_SD;  // the destination register
    /* operation types :
        - 0 : alu
        - 1 : shifter
        - 2 : slt
    */
    sc_out<sc_uint<3>>  OPTYPE_RD;
    sc_out<sc_uint<32>> MEM_DATA_RD;         // data sent to mem for storage
    sc_out<bool>        MEM_LOAD_RD;         // a memory load
    sc_out<bool>        MEM_STORE_RD;        // a memory store
    sc_out<bool>        MEM_SIGN_EXTEND_RD;  // loads and stores can be signed or unsigned
    sc_out<sc_uint<2>>  MEM_SIZE_RD;         // mem accesses can be words, half words or bytes

    /*****************************************************
                    Interface with DEC2IF
    ******************************************************/

    sc_in<bool>       DEC2IF_POP_SI;  // The POP signal coming from Ifetch
    sc_out<bool>      DEC2IF_EMPTY_SD;
    sc_out<sc_bv<32>> PC_RD;  // the output of the fifo, containing the PC of the next instruction

    /*****************************************************
                    Interface with IF2DEC
    ******************************************************/

    sc_in<sc_bv<32>>   INSTR_RI;  // The instruction coming from Ifetch
    sc_in<sc_uint<32>> PC_RI;     // The pc of the instruction being executed
    sc_in<bool>        IF2DEC_EMPTY_SI;
    sc_out<bool>       IF2DEC_POP_SD;    // the POP signal sent to Ifetch
    sc_out<bool>       IF2DEC_FLUSH_SD;  // Signal sent to IFETCH to flush all instructions

    /*****************************************************
                    Interface with DEC2EXE
    ******************************************************/

    sc_in<bool>                    DEC2EXE_POP_SE;  // The POP signal coming from EXE
    sc_out<bool>                   DEC2EXE_EMPTY_SD;
    sc_signal<sc_bv<DEC2EXE_SIZE>> dec2exe_out_sd;

    /*****************************************************
                            BYPASSES
    ******************************************************/
    // see the bypasses process in dec.cpp, or the documentation, for an explanation

    // pipeline data coming from the end of EXE
    sc_in<sc_uint<6>>  DEST_RE;           // the destination register of the data
    sc_in<sc_uint<32>> EXE_RES_RE;        // the data
    sc_in<bool>        MEM_LOAD_RE;       // whether it is a memory load
    sc_in<bool>        EXE2MEM_EMPTY_SE;  // whether the data is valid

    // pipeline data coming from the end of MEM
    sc_in<sc_uint<6>>  DEST_RM;           // the destination register of the data
    sc_in<sc_uint<32>> MEM_RES_RM;        // the data
    sc_in<bool>        MEM2WBK_EMPTY_SM;  // Whether the data is valid

    // Bypass outputs for EXE
    sc_out<sc_uint<6>> BP_RADR1_RD;  // the register of the data in OP1
    sc_out<sc_uint<6>> BP_RADR2_RD;  // the register of the data in OP2
    sc_out<bool>       BLOCK_BP_RD;  // prevent any further bypasses

    // General Interface :

    sc_in_clk   CLK;
    sc_in<bool> RESET;

    // Signals :

    sc_signal<sc_uint<32>> rdata1_sd;
    sc_signal<sc_uint<32>> rdata2_sd;
    sc_signal<bool>        invalid_operand_sd;

    // fifo dec2if :

    sc_signal<sc_bv<32>> dec2if_in_sd;  // pc sent to fifo
    sc_signal<bool>      dec2if_push_sd;
    sc_signal<bool>      dec2if_full_sd;
    sc_signal<sc_bv<32>> dec2if_out_sd;

    // fifo dec2exe :

    sc_signal<sc_bv<DEC2EXE_SIZE>> dec2exe_in_sd;
    sc_signal<bool>                dec2exe_push_sd;
    sc_signal<bool>                dec2exe_full_sd;

    // Instruction format type :

    sc_signal<bool> r_type_inst_sd;     // R type format
    sc_signal<bool> i_type_inst_sd;     // I type format
    sc_signal<bool> s_type_inst_sd;     // S type format
    sc_signal<bool> b_type_inst_sd;     // B type format
    sc_signal<bool> u_type_inst_sd;     // U type format
    sc_signal<bool> j_type_inst_sd;     // J type format
    sc_signal<bool> jalr_type_inst_sd;  // JALR has a specific opcode

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

    // Offset for branch :

    sc_signal<sc_uint<32>> offset_branch_sd;

    // PC gestion :

    sc_signal<bool> inc_pc_sd;
    sc_signal<bool> jump_sd;

    // Pipeline Gestion

    sc_signal<bool> stall_sd;
    sc_signal<bool> normal_sd;
    sc_signal<bool> block_in_dec_sd;

    // Internal signals :

    sc_signal<sc_uint<6>>  adr_dest_sd;
    sc_signal<sc_uint<32>> exe_op1_sd;
    sc_signal<sc_uint<32>> exe_op2_sd;
    sc_signal<sc_uint<32>> mem_data_sd;

    sc_signal<sc_uint<2>> mem_size_sd;
    sc_signal<bool>       mem_load_sd;
    sc_signal<bool>       mem_store_sd;

    sc_signal<sc_uint<2>> exe_cmd_sd;

    sc_signal<sc_uint<2>> optype_sd;

    sc_signal<bool> exe_neg_op2_sd;
    sc_signal<bool> exe_wb_sd;
    sc_signal<bool> mem_sign_extend_sd;
    sc_signal<bool> block_bp_sd;

    // Instance used :

    fifo<32>           dec2if;
    fifo<DEC2EXE_SIZE> dec2exe;

    void concat_dec2exe();
    void unconcat_dec2exe();
    void decoding_instruction_type();
    void decoding_instruction();
    void pre_reg_read_decoding();
    void post_reg_read_decoding();
    void pc_inc();
    void bypasses();
    void stall_method();
    void trace(sc_trace_file * tf);

    SC_CTOR(decod) : dec2if("dec2if"), dec2exe("dec2exe") {
        dec2if.DATAIN_S(dec2if_in_sd);
        dec2if.DATAOUT_R(PC_RD);
        dec2if.EMPTY_S(DEC2IF_EMPTY_SD);
        dec2if.FULL_S(dec2if_full_sd);
        dec2if.PUSH_S(dec2if_push_sd);
        dec2if.POP_S(DEC2IF_POP_SI);
        dec2if.CLK(CLK);
        dec2if.RESET(RESET);

        dec2exe.DATAIN_S(dec2exe_in_sd);
        dec2exe.DATAOUT_R(dec2exe_out_sd);
        dec2exe.EMPTY_S(DEC2EXE_EMPTY_SD);
        dec2exe.FULL_S(dec2exe_full_sd);
        dec2exe.PUSH_S(dec2exe_push_sd);
        dec2exe.POP_S(DEC2EXE_POP_SE);
        dec2exe.CLK(CLK);
        dec2exe.RESET(RESET);

        SC_METHOD(concat_dec2exe)
        sensitive << dec2exe_in_sd << exe_op1_sd << exe_op2_sd << exe_cmd_sd << exe_neg_op2_sd
                  << exe_wb_sd << mem_data_sd << mem_load_sd << mem_store_sd << mem_sign_extend_sd
                  << mem_size_sd << optype_sd << adr_dest_sd << slti_i_sd << slt_i_sd << sltiu_i_sd
                  << sltu_i_sd << RADR1_SD << RADR2_SD << block_bp_sd;
        SC_METHOD(unconcat_dec2exe)
        sensitive << dec2exe_out_sd;

        SC_METHOD(stall_method)
        sensitive << b_type_inst_sd << jalr_type_inst_sd << j_type_inst_sd << invalid_operand_sd
                  << DEC2EXE_EMPTY_SD << EXE2MEM_EMPTY_SE << IF2DEC_EMPTY_SI << dec2exe_full_sd
                  << block_in_dec_sd;

        SC_METHOD(decoding_instruction_type)
        sensitive << INSTR_RI << READ_PC_SR;
        SC_METHOD(decoding_instruction)
        sensitive << INSTR_RI;
        SC_METHOD(pre_reg_read_decoding)
        sensitive << INSTR_RI << r_type_inst_sd << i_type_inst_sd << i_type_inst_sd
                  << s_type_inst_sd << b_type_inst_sd << u_type_inst_sd << j_type_inst_sd
                  << jalr_type_inst_sd << beq_i_sd << bne_i_sd << blt_i_sd << bge_i_sd << bltu_i_sd
                  << bgeu_i_sd << fence_i_sd << RESET;
        SC_METHOD(post_reg_read_decoding)
        sensitive << i_type_inst_sd << s_type_inst_sd << b_type_inst_sd << u_type_inst_sd
                  << j_type_inst_sd << jalr_type_inst_sd << beq_i_sd << bne_i_sd << blt_i_sd
                  << bge_i_sd << bltu_i_sd << bgeu_i_sd << IF2DEC_EMPTY_SI << dec2if_push_sd
                  << READ_PC_SR << stall_sd << dec2if_push_sd << add_i_sd << slt_i_sd << sltu_i_sd
                  << and_i_sd << or_i_sd << xor_i_sd << sll_i_sd << srl_i_sd << sub_i_sd << sra_i_sd
                  << addi_i_sd << slti_i_sd << sltiu_i_sd << andi_i_sd << ori_i_sd << xori_i_sd
                  << jalr_i_sd << slli_i_sd << srli_i_sd << srai_i_sd << lw_i_sd << lh_i_sd
                  << lhu_i_sd << lb_i_sd << lbu_i_sd << beq_i_sd << bne_i_sd << blt_i_sd << bge_i_sd
                  << bltu_i_sd << bgeu_i_sd << lui_i_sd << auipc_i_sd << jal_i_sd << sw_i_sd
                  << sh_i_sd << sb_i_sd << j_type_inst_sd << jalr_type_inst_sd << dec2exe_push_sd
                  << rdata1_sd << rdata2_sd << fence_i_sd << PC_RI;
        SC_METHOD(pc_inc)
        sensitive << CLK.pos() << READ_PC_SR << offset_branch_sd << inc_pc_sd << jump_sd << PC_RI
                  << dec2if_full_sd << IF2DEC_EMPTY_SI << stall_sd;

        SC_METHOD(bypasses);
        sensitive << RDATA1_SR << RDATA2_SR << DEST_RE << EXE_RES_RE << DEST_RM
                  << MEM_RES_RM << RADR1_SD << EXE_DEST_SD << RADR2_SD << EXE2MEM_EMPTY_SE
                  << DEC2EXE_EMPTY_SD << MEM_LOAD_RE << MEM2WBK_EMPTY_SM;
        reset_signal_is(RESET, false);
    }
};