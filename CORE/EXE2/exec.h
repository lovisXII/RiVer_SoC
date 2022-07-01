#pragma once
#include <systemc.h>
#include <iostream>
#include <string>
#include "../UTIL/debug_util.h"
#include "../UTIL/fifo.h"

#define exe2mem_size        200
#define start_kernel_adress 0xF0000000
SC_MODULE(alu_s2) {
    sc_in<sc_uint<32>>  OP1_SE_S1, OP2_SE_S1;
    sc_in<bool>         CIN_SE;
    sc_in<sc_uint<2>>   CMD_SE;
    sc_out<sc_uint<32>> RES_SE;

    void operation();
    void trace(sc_trace_file * tf);

    SC_CTOR(alu_s2) {
        SC_METHOD(operation);
        sensitive << OP1_SE_S1 << OP2_SE_S1 << CIN_SE << CMD_SE;
    }
};

SC_MODULE(shifter_s2) {
    sc_in<sc_uint<32>> DIN_SE;        // input
    sc_in<sc_uint<5>>  SHIFT_VAL_SE;  // shift value
    sc_in<sc_uint<2>>  CMD_SE;        // command

    sc_out<sc_uint<32>> DOUT_SE;  // output

    // Internal signals :

    sc_signal<bool>        sll_se, srl_se, sra_se;  // command, only one must be true
    sc_signal<sc_uint<32>> sll_dout_se,
        sr_dout_se;  // temporary output for each component

    void decode_cmd();
    void shifter_sll();       // component to shift left the input
    void shifter_sr();        // component to shift right the input, arithmetic or logic
    void shifter_agregate();  // component to choose which shift to use
    void trace(sc_trace_file * tf);

    SC_CTOR(shifter_s2) {
        SC_METHOD(decode_cmd);
        sensitive << CMD_SE;
        SC_METHOD(shifter_sll);
        sensitive << DIN_SE << SHIFT_VAL_SE;
        SC_METHOD(shifter_sr);
        sensitive << DIN_SE << SHIFT_VAL_SE << sra_se;
        SC_METHOD(shifter_agregate);
        sensitive << sll_dout_se << sr_dout_se << sll_se << srl_se << sra_se;
    }
};

SC_MODULE(exec_s2) {
    // Input/Output of EXE :

    sc_in<sc_uint<32>> OP1_RD_S2;  // can contains CSR if CSR_type_operation_RD == 1
    sc_in<sc_uint<32>> OP2_RD_S2;
    sc_in<bool>        OP1_VALID_RD_S2;
    sc_in<bool>        OP2_VALID_RD_S2;
    sc_in<sc_uint<6>>  RADR1_RD_S2;
    sc_in<sc_uint<6>>  RADR2_RD_S2;
    sc_in<bool>        BLOCK_BP_RD_S2;

    sc_in<sc_uint<32>> PC_DEC2EXE_RD_S2;
    sc_in<sc_uint<32>> MEM_DATA_RD_S2;
    sc_in<sc_uint<6>>  DEST_RD_S2;
    sc_in<sc_uint<2>>  CMD_RD_S2;
    sc_in<sc_uint<2>>  MEM_SIZE_RD_S2;
    sc_in<bool>        NEG_OP2_RD_S2, WB_RD_S2;
    sc_in<bool>        MEM_SIGN_EXTEND_RD_S2;
    sc_in<sc_uint<4>>  SELECT_TYPE_OPERATIONS_RD_S2;  // taille fifo entrée : 110
    sc_in<bool>        MEM_LOAD_RD_S2, MEM_STORE_RD_S2;

    sc_in<bool> EXE2MEM_POP_SM_S2;
    sc_in<bool> MULT_INST_RD_S2;
    sc_in<bool> DEC2EXE_EMPTY_SD_S2;
    sc_in<bool> SLT_RD_S2, SLTU_RD_S2;

    sc_in<bool>        CSR_WENABLE_RD_S2;
    sc_in<sc_uint<12>> CSR_WADR_RD_S2;
    sc_in<sc_uint<32>> CSR_RDATA_RD_S2;

    sc_in<bool> MULT_INST_RM_S2;
    sc_in<bool> BP_MEM2WBK_EMPTY_SM_S2;
    // Exception coming from Decod :

    sc_in<bool>         EXCEPTION_RD_S2;  // tells if an instruction have been made in DEC
    sc_in<sc_uint<2>>   CURRENT_MODE_SM_S2;
    sc_in<sc_uint<32>>  PC_BRANCH_VALUE_RD_S2;
    sc_out<sc_uint<32>> PC_BRANCH_VALUE_RE_S2;

    sc_in<bool> ILLEGAL_INSTRUCTION_RD_S2;  // accessing stuff in wrong mode
    sc_in<bool> ADRESS_MISSALIGNED_RD_S2;   // branch offset is misaligned
    sc_in<bool> ENV_CALL_U_MODE_RD_S2;
    sc_in<bool> ENV_CALL_M_MODE_RD_S2;
    sc_in<bool> ENV_CALL_S_MODE_RD_S2;
    sc_in<bool> ENV_CALL_WRONG_MODE_RD_S2;
    sc_in<bool> INSTRUCTION_ACCESS_FAULT_RD_S2;
    sc_in<bool> MRET_RD_S2;
    sc_in<bool> EBREAK_RD_S2;

    sc_out<bool> EXCEPTION_RE_S2;
    sc_out<bool> LOAD_ADRESS_MISSALIGNED_RE_S2;  // adress from store/load isn't aligned
    sc_out<bool> LOAD_ACCESS_FAULT_RE_S2;        // trying to access memory in wrong mode
    sc_out<bool> STORE_ADRESS_MISSALIGNED_RE_S2;
    sc_out<bool> STORE_ACCESS_FAULT_RE_S2;
    sc_out<bool> ILLEGAL_INSTRUCTION_RE_S2;             // accessing stuff in wrong mode
    sc_out<bool> INSTRUCTION_ADRESS_MISSALIGNED_RE_S2;  // branch offset is misaligned
    sc_out<bool> ENV_CALL_S_MODE_REEBREAK_RD_S2_S2;
    sc_out<bool> ENV_CALL_M_MODE_RE_S2;
    sc_out<bool> ENV_CALL_U_MODE_RE_S2;
    sc_out<bool> ENV_CALL_WRONG_MODE_RE_S2;
    sc_out<bool> INSTRUCTION_ACCESS_FAULT_RE_S2;
    sc_out<bool> MRET_RE_S2;
    sc_out<bool> EBREAK_RE_S2;

    sc_out<sc_uint<32>> OP1_SE_S2;
    sc_out<sc_uint<32>> OP2_SE_S2;
    // Interruption :

    sc_out<bool> MACHINE_SOFTWARE_INTERRUPT_SE_S2;
    sc_out<bool> MACHINE_TIMER_INTERRUPT_SE;
    sc_out<bool> MACHINE_EXTERNAL_INTERRUPT_SE;
    sc_out<bool> INTERRUPTION_SE;
    // bypasses

    sc_in<sc_uint<6>>  MEM_DEST_RM_S2;
    sc_in<sc_uint<32>> MEM_RES_RM_S2;
    sc_in<bool>        CSR_WENABLE_RM_S2;
    sc_in<sc_uint<32>> CSR_RDATA_RM_S2;

    // General Interface :

    sc_in<bool> EXCEPTION_SM;
    sc_in_clk   CLK;
    sc_in<bool> RESET;
    sc_in<bool> MACHINE_SOFTWARE_INTERRUPT_SX;
    sc_in<bool> MACHINE_TIMER_INTERRUPT_SX;
    sc_in<bool> MACHINE_EXTERNAL_INTERRUPT_SX;

    // Fifo exe2mem interface :

    sc_out<sc_uint<32>> EXE_RES_RE_S2;
    sc_out<sc_uint<32>> MEM_DATA_RE_S2;
    sc_out<sc_uint<6>>  DEST_RE_S2;
    sc_out<sc_uint<2>>  MEM_SIZE_RE_S2;
    sc_out<sc_uint<32>> PC_EXE2MEM_RE_S2;

    sc_out<bool> WB_RE_S2, MEM_SIGN_EXTEND_RE_S2;  // taille fifo sortie : 7
    sc_out<bool> MEM_LOAD_RE_S2, MEM_STORE_RE_S2;
    sc_out<bool> MULT_INST_RE_S2;      // multiplication instruction
    sc_out<bool> MULT_SEL_HIGH_RE_S2;  // select higher bits of multiplication

    sc_out<bool> EXE2MEM_EMPTY_SE_S2, DEC2EXE_POP_SE_S2;

    sc_out<bool>        CSR_WENABLE_RE_S2;
    sc_out<sc_uint<12>> CSR_WADR_RE;
    sc_out<sc_uint<32>> CSR_RDATA_RE_S2;

    // Internals signals :

    sc_signal<sc_uint<32>> exe_res_se_s2;

    sc_signal<sc_bv<exe2mem_size>> exe2mem_din_se_s2;  // concatenation of exe_res, mem_data...etc
    sc_signal<sc_bv<exe2mem_size>> exe2mem_dout_se_s2;

    sc_signal<sc_uint<32>> op1_se_s2;
    sc_signal<sc_uint<32>> op2_se;
    sc_signal<sc_uint<32>> alu_in_op2_se;
    sc_signal<sc_uint<32>> alu_out_se;
    sc_signal<sc_uint<32>> shifter_out_se;
    sc_signal<sc_uint<32>> divider_out_se;
    sc_signal<sc_uint<32>> bp_mem_data_sd;
    sc_signal<sc_uint<5>>  shift_val_se;

    sc_signal<bool> exe2mem_push_se, exe2mem_full_se;

    sc_signal<bool> blocked;

    sc_signal<bool> wb_re;
    sc_signal<bool> mem_load_re;
    sc_signal<bool> mem_store_re;

    sc_signal<bool> stall_se;
    sc_signal<bool> r1_valid_se;
    sc_signal<bool> r2_valid_se;
    // Exception :

    sc_signal<bool> exception_se;
    sc_signal<bool> load_adress_missaligned_se;  // adress from store/load isn't aligned
    sc_signal<bool> load_access_fault_se;        // trying to access memory in wrong mode
    sc_signal<bool> store_access_fault_se;
    sc_signal<bool> store_adress_missaligned_se;
    // Instance used :

    alu_s2     alu_inst;
    shifter_s2 shifter_inst;
    // divider          divider_inst;
    fifo<exe2mem_size> fifo_inst;

    void preprocess_op();    // send op2 or ~op2 in ALU_IN_OP2
    void select_exec_res();  // setup FFIN_EXE_RES as ALU_OUT or SHIFTER_OUT
    void fifo_concat();      // setup result  in fifo exe2mem
    void fifo_unconcat();    // unconcatenet result from the fifo
    void manage_fifo();      // allow the push/pop of fifo exe2mem

    void bypasses();  // allow the push/pop of fifo exe2mem
    void exception();

    void trace(sc_trace_file * tf);
    SC_CTOR(exec_s2)
        : alu_inst("alu_s2"),
          shifter_inst("shifter_s2"),
          // divider_inst("divider"),
          fifo_inst("exe2mem") {
        // ALU port map :

        alu_inst.OP1_SE_S1(op1_se_s2);
        alu_inst.OP2_SE_S1(alu_in_op2_se);
        alu_inst.CMD_SE(CMD_RD_S2);
        alu_inst.CIN_SE(NEG_OP2_RD_S2);
        alu_inst.RES_SE(alu_out_se);

        // Shifter port map :

        shifter_inst.DIN_SE(op1_se_s2);
        shifter_inst.SHIFT_VAL_SE(shift_val_se);
        shifter_inst.CMD_SE(CMD_RD_S2);
        shifter_inst.DOUT_SE(shifter_out_se);

        // DIVIDER port map :

        /*divider_inst.OP1_SE_S2(op1_se_s2);
        divider_inst.OP2_SE_S2(alu_in_op2_se);
        divider_inst.CMD_SE(CMD_RD_S2);
        divider_inst.CIN_SE(NEG_OP2_RD_S2);
        divider_inst.RES_SE(divider_out_se);*/

        // fifo port map

        fifo_inst.DIN_S(exe2mem_din_se_s2);
        fifo_inst.DOUT_R(exe2mem_dout_se_s2);
        fifo_inst.EMPTY_S(EXE2MEM_EMPTY_SE_S2);
        fifo_inst.FULL_S(exe2mem_full_se);
        fifo_inst.PUSH_S(exe2mem_push_se);
        fifo_inst.POP_S(EXE2MEM_POP_SM_S2);
        fifo_inst.CLK(CLK);
        fifo_inst.RESET_N(RESET);

        SC_METHOD(preprocess_op);
        sensitive << op1_se_s2 << NEG_OP2_RD_S2 << op2_se;
        SC_METHOD(select_exec_res);
        sensitive << alu_out_se << divider_out_se << shifter_out_se << SLT_RD_S2 << SLTU_RD_S2
                  << SELECT_TYPE_OPERATIONS_RD_S2 << CURRENT_MODE_SM_S2 << MEM_LOAD_RD_S2 << MEM_STORE_RD_S2
                  << exception_se << RESET;
        SC_METHOD(fifo_concat);
        sensitive << bp_mem_data_sd << DEST_RD_S2 << MEM_SIZE_RD_S2 << MEM_LOAD_RD_S2 << MEM_SIGN_EXTEND_RD_S2
                  << MEM_STORE_RD_S2 << WB_RD_S2 << exe_res_se_s2 << mem_load_re << mem_store_re << wb_re
                  << CSR_WENABLE_RD_S2 << CSR_WADR_RD_S2 << CSR_RDATA_RD_S2 << ILLEGAL_INSTRUCTION_RD_S2
                  << ADRESS_MISSALIGNED_RD_S2 << ENV_CALL_U_MODE_RD_S2 << ENV_CALL_M_MODE_RD_S2 << exception_se
                  << load_adress_missaligned_se << load_access_fault_se << store_access_fault_se
                  << store_adress_missaligned_se << EXCEPTION_SM << MRET_RD_S2 << INSTRUCTION_ACCESS_FAULT_RD_S2
                  << MULT_INST_RD_S2;
        SC_METHOD(fifo_unconcat);
        sensitive << exe2mem_dout_se_s2;
        SC_METHOD(manage_fifo);
        sensitive << exe2mem_full_se << DEC2EXE_EMPTY_SD_S2 << OP1_VALID_RD_S2 << OP2_VALID_RD_S2 << exception_se << blocked
                  << r1_valid_se << r2_valid_se;
        SC_METHOD(bypasses);
        sensitive << OP1_VALID_RD_S2 << OP2_VALID_RD_S2 << MEM_DEST_RM_S2 << MEM_RES_RM_S2 << DEST_RE_S2 << EXE_RES_RE_S2 << RADR1_RD_S2
                  << CSR_WENABLE_RE_S2 << BLOCK_BP_RD_S2 << DEST_RE_S2 << MEM_LOAD_RE_S2 << CSR_WENABLE_RM_S2 << CSR_RDATA_RM_S2
                  << RADR2_RD_S2 << OP1_RD_S2 << OP2_RD_S2 << exception_se << MEM_DATA_RD_S2 << MEM_STORE_RD_S2
                  << MULT_INST_RE_S2 << MULT_INST_RM_S2 << BP_MEM2WBK_EMPTY_SM_S2 << EXE2MEM_EMPTY_SE_S2;
        SC_METHOD(exception);
        sensitive << WB_RD_S2 << MEM_LOAD_RD_S2 << MEM_STORE_RD_S2 << WB_RD_S2 << EXCEPTION_RD_S2
                  << load_adress_missaligned_se << exception_se << load_access_fault_se << store_access_fault_se
                  << store_adress_missaligned_se;
    }
};
