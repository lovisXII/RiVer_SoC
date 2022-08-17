#pragma once
#include <systemc.h>
#include <iostream>
#include "../UTIL/debug_util.h"
#include "../UTIL/fifo.h"
#define mem2wbk_size 108
/*
Assuming the following stuff :

MEM_SIZE = 00 -> access in word
MEM_SIZE = 01 -> access in half word
MEM_SIZE = 10  -> acces in byte

*/
SC_MODULE(mem_s2) {
    // Mcache Interface :

    sc_out<sc_uint<32>> MCACHE_ADR_SM_S2;  // adress in memory
    sc_out<sc_uint<32>> MCACHE_DATA_SM_S2;
    sc_out<bool>        MCACHE_ADR_VALID_SM_S2;
    sc_out<bool>        MCACHE_STORE_SM_S2;
    sc_out<bool>        MCACHE_LOAD_SM_S2;
    sc_in<sc_uint<32>> MCACHE_RESULT_SM_S2;
    sc_in<bool>        MCACHE_STALL_SM_S2;
    sc_out<sc_uint<2>> MEM_SIZE_SM_S2;
    // Exe Interface :

    sc_in<sc_uint<32>> EXE_RES_RE_S2;
    sc_in<sc_uint<32>> MEM_DATA_RE_S2;
    sc_in<sc_uint<6>>  DEST_RE_S2;
    sc_in<sc_uint<2>>  MEM_SIZE_RE_S2;

    sc_in<sc_uint<32>> PC_EXE2MEM_RE_S2;
    sc_in<bool>        WB_RE_S2;
    sc_in<bool>        SIGN_EXTEND_RE_S2;     // taille fifo entrée : 74
    sc_in<bool>        LOAD_RE_S2, STORE_RE_S2;  // 15


    sc_in<bool>        LOAD_RE_S1;
    sc_in<bool>        STORE_RE_S1;

    sc_in<bool>        MULT_INST_RE_S2;

    sc_in<bool>        CSR_WENABLE_RE_S2;
    sc_in<sc_uint<12>> CSR_WADR_SE_S2;
    sc_in<sc_uint<32>> CSR_RDATA_RE_S2;

    sc_in<bool> EXCEPTION_RE_S2;
    sc_in<bool> LOAD_ADRESS_MISSALIGNED_RE_S2;         // adress from store/load isn't aligned
    sc_in<bool> LOAD_ACCESS_FAULT_RE_S2;               // trying to access memory in wrong mode
    sc_in<bool> ILLEGAL_INSTRUCTION_RE_S2;             // accessing stuff in wrong mode
    sc_in<bool> INSTRUCTION_ADRESS_MISSALIGNED_RE_S2;  // branch offset is misaligned
    sc_in<bool> STORE_ADRESS_MISSALIGNED_RE_S2;
    sc_in<bool> STORE_ACCESS_FAULT_RE_S2;
    sc_in<bool> ENV_CALL_S_MODE_RE_S2;
    sc_in<bool> ENV_CALL_M_MODE_RE_S2;
    sc_in<bool> ENV_CALL_U_MODE_RE_S2;
    sc_in<bool> ENV_CALL_WRONG_MODE_RE_S2;
    sc_in<bool> INSTRUCTION_ACCESS_FAULT_RE_S2;
    sc_in<bool> MRET_RE_S2;  // 35

    sc_in<sc_uint<32>> PC_BRANCH_VALUE_RE_S2;

    // Bus Interface : // No bus in our implemation but can be use for further use

    sc_in<bool> BUS_ERROR_SX;

    // exe2mem interface :

    sc_in<bool>  EXE2MEM_EMPTY_SE_S2;
    sc_out<bool> EXE2MEM_POP_SM_S2;

    // mem2wbk interface

    sc_in<bool>         MEM2WBK_POP_SW_S2;  
    sc_out<bool>        MEM2WBK_EMPTY_SM_S2;
    sc_out<sc_uint<32>> PC_MEM2WBK_RM_S2;
    sc_out<bool>        CSR_WENABLE_RM_S2;  

    // WBK interface
    sc_out<sc_uint<32>> MEM_RES_RM_S2;
    sc_out<sc_uint<6>>  DEST_RM_S2;
    sc_out<bool>        WB_RM_S2;
    sc_out<sc_uint<32>> CSR_RDATA_RM_S2;
    sc_out<bool>        MULT_INST_RM_S2; //48
    // Global Interface :

    sc_in<bool>         EXCEPTION_SM_S1;
    sc_out<bool>        EXCEPTION_SM_S2;
    sc_out<bool>        EXCEPTION_SM;

    
    sc_in<sc_uint<2>>   CURRENT_MODE_SM_S1;

    sc_out<sc_uint<2>>  CURRENT_MODE_SM_S2;
    sc_out<sc_uint<32>> RETURN_ADRESS_SM_S2;
    sc_out<bool>        MRET_SM_S2;  // 54
    sc_in<bool>        MRET_SM_S1;  // 55

    sc_in_clk   CLK;
    sc_in<bool> RESET;

    // Interruption :

    sc_in<bool> INTERRUPTION_SE_S2;
    sc_in<bool> MACHINE_SOFTWARE_INTERRUPT_SE_S2;
    sc_in<bool> MACHINE_TIMER_INTERRUPT_SE_S2;
    sc_in<bool> MACHINE_EXTERNAL_INTERRUPT_SE_S2;
    sc_in<bool> EBREAK_RE_S2;
    // CSR Interface :

    sc_out<sc_uint<12>> CSR_WADR_SM_S2;
    sc_out<sc_uint<32>> CSR_WDATA_SM_S2;
    sc_out<bool>        CSR_ENABLE_SM_S2;


    sc_in<sc_uint<32>> MSTATUS_WDATA_SM_S1;
    sc_in<sc_uint<32>> MIP_WDATA_SM_S1;
    sc_in<sc_uint<32>> MEPC_WDATA_SM_S1;
    sc_in<sc_uint<32>> MCAUSE_WDATA_SM_S1;
    sc_in<sc_uint<32>> MTVAL_WDATA_SM_S1;

    sc_out<sc_uint<32>> MSTATUS_WDATA_RM_S2;
    sc_out<sc_uint<32>> MIP_WDATA_RM_S2;
    sc_out<sc_uint<32>> MEPC_WDATA_RM_S2;
    sc_out<sc_uint<32>> MCAUSE_WDATA_SM_S2;
    sc_out<sc_uint<32>> MTVAL_WDATA_SM_S2;
    
    sc_in<sc_uint<32>>  MEPC_SC;
    sc_in<sc_uint<32>>  MSTATUS_RC;  // 71
    sc_in<sc_uint<32>>  MTVEC_VALUE_RC;
    sc_in<sc_uint<32>>  MIP_VALUE_RC;
    sc_in<sc_uint<32>>  MCAUSE_SC;

    // SS2 :

    sc_out<bool> MEM_ACCESS_IS_PRIO_RD_S2; //74

    // Internal signals

    sc_signal<bool>     mem2wbk_push_sm;
    sc_signal<bool>     mem2wbk_full_sm;

    sc_signal<sc_bv<mem2wbk_size>> mem2wbk_din_sm;
    sc_signal<sc_bv<mem2wbk_size>> mem2wbk_dout_sm;
    sc_signal<bool>                exception_sm;
    sc_signal<sc_uint<32>>         data_sm;
    sc_signal<bool>                wb_sm;

    // Exception signals

    sc_signal<bool>       save_restore_sm;  // sd bit from mstatus
    sc_signal<sc_uint<2>> mpp_sm;           // MPP bit from mstatus
    sc_signal<bool>       mpie_sm;          // MPIE bit from mstatus
    sc_signal<bool>       mie_sm;           // MIE bit from mstatus

    sc_signal<sc_uint<32>> return_adress_sm;
    sc_signal<bool>        mret_sm;

    // Superscalar-signals

    sc_signal<bool> mem_access_is_prio_sd_s2;
    sc_signal<bool> mem_access_is_prio_rd_s2;
    sc_signal<sc_uint<2>> current_mode_rm_s2;

    // FIFO
    fifo<mem2wbk_size> fifo_inst;

    void mem2wbk_concat();
    void mem2wbk_unconcat();
    void fifo_gestion();
    void mem_preprocess();
    void sign_extend();
    void interruption();
    void csr_exception();
    void memory_access_prio() ;
    void reg_mem_prio();
    void current_mode_reg();
    void trace(sc_trace_file * tf);

    SC_CTOR(mem_s2) : fifo_inst("mem2wbk") {
        fifo_inst.DIN_S(mem2wbk_din_sm);
        fifo_inst.DOUT_R(mem2wbk_dout_sm);
        fifo_inst.EMPTY_S(MEM2WBK_EMPTY_SM_S2);
        fifo_inst.FULL_S(mem2wbk_full_sm);
        fifo_inst.PUSH_S(mem2wbk_push_sm);
        fifo_inst.POP_S(MEM2WBK_POP_SW_S2);
        fifo_inst.CLK(CLK);
        fifo_inst.RESET_N(RESET);

        SC_METHOD(current_mode_reg)
        sensitive << CLK.pos();
        SC_METHOD(memory_access_prio);
        sensitive   << LOAD_RE_S1
                    << LOAD_RE_S2
                    << STORE_RE_S1
                    << STORE_RE_S2;

        SC_METHOD(reg_mem_prio);
        sensitive << CLK.pos();
        SC_METHOD(mem2wbk_concat);
        sensitive << data_sm << DEST_RE_S2 << wb_sm << CSR_WENABLE_RE_S2 << CSR_RDATA_RE_S2 << exception_sm << mret_sm
                  << return_adress_sm;
        SC_METHOD(mem2wbk_unconcat);
        sensitive << mem2wbk_dout_sm;
        SC_METHOD(fifo_gestion);
        sensitive << MCACHE_STALL_SM_S2 << mem2wbk_full_sm << EXE2MEM_EMPTY_SE_S2 << wb_sm;
        SC_METHOD(mem_preprocess);
        sensitive << WB_RE_S2 << LOAD_RE_S2 << MEM_SIZE_RE_S2 << MCACHE_RESULT_SM_S2 << EXE_RES_RE_S2 << MEM_DATA_RE_S2 << STORE_RE_S2
                  << SIGN_EXTEND_RE_S2 << EXE2MEM_EMPTY_SE_S2;
        SC_METHOD(sign_extend);
        sensitive << MEM_SIZE_RE_S2 << SIGN_EXTEND_RE_S2 << MCACHE_RESULT_SM_S2 << EXE_RES_RE_S2 << LOAD_RE_S2;
        SC_METHOD(csr_exception);
        sensitive   << EXCEPTION_RE_S2 
                    << CSR_WENABLE_RE_S2 
                    << MIP_VALUE_RC 
                    << PC_EXE2MEM_RE_S2 
                    << CSR_WADR_SE_S2 
                    << EXE_RES_RE_S2
                    << INSTRUCTION_ACCESS_FAULT_RE_S2 
                    << ILLEGAL_INSTRUCTION_RE_S2 
                    << INSTRUCTION_ADRESS_MISSALIGNED_RE_S2
                    << ENV_CALL_U_MODE_RE_S2 
                    << ENV_CALL_S_MODE_RE_S2 
                    << ENV_CALL_M_MODE_RE_S2 
                    << LOAD_ADRESS_MISSALIGNED_RE_S2
                    << STORE_ADRESS_MISSALIGNED_RE_S2 
                    << LOAD_ACCESS_FAULT_RE_S2 
                    << STORE_ACCESS_FAULT_RE_S2 
                    << MRET_RE_S2
                    << EXCEPTION_SM_S2 
                    << ENV_CALL_WRONG_MODE_RE_S2 
                    << BUS_ERROR_SX 
                    << EXCEPTION_SM_S2 
                    <<  MSTATUS_RC
                    << EXE_RES_RE_S2 
                    << MEPC_SC 
                    << current_mode_rm_s2 
                    << MRET_SM_S1 
                    << EXCEPTION_SM_S1 
                    << mem_access_is_prio_rd_s2
                    << EXE2MEM_EMPTY_SE_S2
                    << MSTATUS_WDATA_SM_S1
                    << MIP_WDATA_SM_S1
                    << MEPC_WDATA_SM_S1
                    << MCAUSE_SC
                    << MIP_VALUE_RC
                    << MCAUSE_WDATA_SM_S1
                    << MTVAL_WDATA_SM_S1;
    }
};
