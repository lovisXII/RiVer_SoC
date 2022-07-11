#include "IFETCH/ifetch.h"
#include "DEC/dec.h"
#include "EXE1/exec_s1.h"
#include "MEM1/mem_s1.h"
#include "WBK1/wbk_s1.h"

#include "EXE2/exec_s2.h"
#include "MEM2/mem_s2.h"
#include "WBK2/wbk_s2.h"

#include "CSR/csr.h"
#include "REG/reg.h"

#include "systemc.h"

#include "EXE1/x0_multiplier.h"
#include "MEM1/x1_multiplier.h"
#include "WBK1/x2_multiplier.h"
#include "EXE2/exec_s2.h"

SC_MODULE(core) {
    
    // Stage instanciation
    
    ifetch      ifetch_inst;
    decod       dec_inst;

    exec_s1     exec_inst_s1;
    exec_s2     exec_inst_s2;
    
    mem_s1      mem_inst_s1;
    mem_s2      mem_inst_s2;
    
    wbk_s1      wbk_inst_s1;
    wbk_s2      wbk_inst_s2;
    
    reg         reg_inst;
    csr         csr_inst;
    


    x0_multiplier      x0_multiplier_inst;
    x1_multiplier      x1_multiplier_inst;

    // Mcache interface

    sc_out<sc_uint<2>>  MEM_SIZE_SM_S1;
    sc_out<sc_uint<2>>  MEM_SIZE_SM_S2;
    sc_out<sc_uint<32>> MCACHE_ADR_SM_S1;
    sc_out<sc_uint<32>> MCACHE_ADR_SM_S2;

    sc_out<sc_uint<32>> MCACHE_DATA_SM_S1;
    sc_out<sc_uint<32>> MCACHE_DATA_SM_S2;
    sc_out<bool>        MCACHE_ADR_VALID_SM_S1;
    sc_out<bool>        MCACHE_ADR_VALID_SM_S2;
    sc_out<bool>        MCACHE_STORE_SM_S1;
    sc_out<bool>        MCACHE_STORE_SM_S2;
    sc_out<bool>        MCACHE_LOAD_SM_S1;
    sc_out<bool>        MCACHE_LOAD_SM_S2;
    sc_in<sc_uint<32>>  MCACHE_RESULT_SM_S1;
    sc_in<sc_uint<32>>  MCACHE_RESULT_SM_S2;
    sc_in<bool>         MCACHE_STALL_SM_S1;
    sc_in<bool>         MCACHE_STALL_SM_S2;

    // Icache interface
    sc_out<sc_uint<32>> ADR_SI_S1;
    sc_out<bool>        ADR_VALID_SI_S1;

    sc_in<sc_bv<32>>    IC_INST_SI_S1;

    sc_out<sc_uint<32>> ADR_SI_S2;
    sc_out<bool>        ADR_VALID_SI_S2;

    sc_in<sc_bv<32>>    IC_INST_SI_S2;
    sc_in<bool>         IC_STALL_SI;

    // Debug
    sc_in<sc_uint<32>>  PC_INIT;
    sc_out<sc_uint<32>> DEBUG_PC_READ;


    // Global Interface :

    sc_in_clk   CLK;
    sc_in<bool> RESET;

    // IFETCH-DEC interface
    // DEC2IF :

    sc_signal<bool>      DEC2IF_POP_SI;
    sc_signal<bool>      DEC2IF_EMPTY_SI;
    sc_signal<sc_bv<32>> PC_DEC2IF_RD_S1;
    sc_signal<sc_bv<32>> PC_DEC2IF_RD_S2;
    sc_signal<bool>      EXCEPTION_RI;

    sc_signal<sc_uint<2>> PRIORITARY_PIPELINE_RD;
    // IF2DEC :

    sc_signal<sc_bv<32>>   INSTR_RI_S1;
    sc_signal<sc_bv<32>>   INSTR_RI_S2;
    sc_signal<bool>        IF2DEC_EMPTY_SI_S1;
    sc_signal<bool>        IF2DEC_POP_SD_S1;
    sc_signal<bool>        IF2DEC_FLUSH_SD;
    sc_signal<bool>        IF2DEC_EMPTY_SI_S2;
    sc_signal<bool>        IF2DEC_POP_SD_S2;
    sc_signal<sc_uint<32>> PC_IF2DEC_RI_S1;
    sc_signal<sc_uint<32>> PC_IF2DEC_RI_S2;
    // DEC-EXE interface

    sc_signal<sc_uint<32>> PC_DEC2EXE_RD_S1;
    sc_signal<sc_uint<32>> OP1_RD_S1;
    sc_signal<sc_uint<32>> OP2_RD_S1;
    sc_signal<sc_uint<2>>  EXE_CMD_RD_S1;
    sc_signal<bool>        NEG_OP2_RD_S1;
    sc_signal<bool>        WB_RD_S1;
    sc_signal<sc_uint<4>>  SELECT_TYPE_OPERATIONS_RD_S1;
    sc_signal<sc_uint<32>>  PC_BRANCH_VALUE_RD_S1;

    sc_signal<sc_uint<32>> MEM_DATA_RD_S1;
    sc_signal<bool>        MEM_LOAD_RD_S1;
    sc_signal<bool>        MEM_STORE_RD_S1;
    sc_signal<bool>        MEM_SIGN_EXTEND_RD_S1;
    sc_signal<sc_uint<2>>  MEM_SIZE_RD_S1;
    sc_signal<bool>        SLT_RD_S1;
    sc_signal<bool>        SLTU_RD_S1;


    // Interface with EXE_S2 :

    sc_signal<sc_uint<32>> OP1_RD_S2;                    
    sc_signal<sc_uint<32>> OP2_RD_S2;                     
    sc_signal<sc_uint<2>>  EXE_CMD_RD_S2;                
    sc_signal<bool>        NEG_OP2_RD_S2;                 

    sc_signal<bool>        WB_RD_S2;                     
    sc_signal<sc_uint<6>>  EXE_DEST_RD_S2;               
    sc_signal<sc_uint<4>>  SELECT_TYPE_OPERATIONS_RD_S2;  
    sc_signal<bool>        SLT_RD_S2;
    sc_signal<bool>        SLTU_RD_S2;
    sc_signal<sc_uint<32>> PC_DEC2EXE_RD_S2;      
    sc_signal<sc_uint<32>> PC_BRANCH_VALUE_RD_S2;  
    sc_signal<sc_uint<32>> MEM_DATA_RD_S2;         
    sc_signal<bool>        MEM_LOAD_RD_S2;   
    sc_signal<bool>        MEM_STORE_RD_S2;       
    sc_signal<bool>        MEM_SIGN_EXTEND_RD_S2;
    sc_signal<sc_uint<2>>  MEM_SIZE_RD_S2;  

    sc_signal<bool> CSR_WENABLE_RD_S2;     
    sc_signal<sc_uint<12>> CSR_WADR_RD_S2;   
    sc_signal<sc_uint<32>> CSR_RDATA_RD_S2;  
    

    sc_signal<sc_uint<6>>        DEST_RM_S2;
    sc_signal<sc_uint<32>>        MEM_RES_RM_S2;
    sc_signal<bool>        EBREAK_RD_S2;
    sc_signal<bool>        EBREAK_RE_S2;
    sc_signal<bool>        MULT_INST_RD_S2;
    sc_signal<sc_uint<32>>        PC_EXE2MEM_RE_S2;
    sc_signal<sc_uint<32>>        EXE_RES_RE_S2;
    sc_signal<sc_uint<32>>        MEM_DATA_RE_S2;
    sc_signal<sc_uint<6>>        DEST_RE_S2;
    sc_signal<sc_uint<2>>        MEM_SIZE_RE_S2;
    sc_signal<bool>        MEM_SIGN_EXTEND_RE_S2;
    sc_signal<bool>        MEM_LOAD_RE_S2;
    sc_signal<bool>        MEM_STORE_RE_S2;
    sc_signal<bool>        MULT_INST_RE_S2;
    sc_signal<bool>        MULT_SEL_HIGH_RE_S2;
    sc_signal<bool>        EXE2MEM_EMPTY_SE_S2;
    sc_signal<bool>        EXE2MEM_POP_SM_S2;
    sc_signal<bool>        CSR_WENABLE_RM_S2;
    sc_signal<sc_uint<32>>        CSR_RDATA_RM_S2;
    sc_signal<bool>        MACHINE_SOFTWARE_INTERRUPT_SE_S2;
    sc_signal<bool>        EXCEPTION_RD_S2;
    sc_signal<bool>        CSR_WENABLE_RE_S2;
    sc_signal<sc_uint<32>>        CSR_RDATA_RE_S2;
    sc_signal<bool>        ENV_CALL_S_MODE_RD_S2;
    sc_signal<bool>        ENV_CALL_WRONG_MODE_RD_S2;
    sc_signal<bool>        ILLEGAL_INSTRUCTION_RD_S2;
    sc_signal<bool>        ADRESS_MISSALIGNED_RD_S2;
    sc_signal<bool>        ENV_CALL_U_MODE_RD_S2;
    sc_signal<bool>        ENV_CALL_M_MODE_RD_S2;
    sc_signal<bool>        EXCEPTION_RE_S2;
    sc_signal<bool>        LOAD_ADRESS_MISSALIGNED_RE_S2;
    sc_signal<bool>        LOAD_ACCESS_FAULT_RE_S2;
    sc_signal<bool>        STORE_ADRESS_MISSALIGNED_RE_S2;
    sc_signal<bool>        STORE_ACCESS_FAULT_RE_S2;
    sc_signal<bool>        MRET_RD_S2;
    sc_signal<bool>        ENV_CALL_U_MODE_RE_S2;
    sc_signal<bool>        ILLEGAL_INSTRUCTION_RE_S2;
    sc_signal<bool>        INSTRUCTION_ADRESS_MISSALIGNED_RE_S2;
    sc_signal<bool>        ENV_CALL_S_MODE_RE_S2;
    sc_signal<bool>        ENV_CALL_M_MODE_RE_S2;
    sc_signal<bool>        BLOCK_BP_RD_S2;
    sc_signal<sc_uint<2>>        CURRENT_MODE_SM_S2;
    sc_signal<bool>        INSTRUCTION_ACCESS_FAULT_RD_S2;
    sc_signal<bool>        INSTRUCTION_ACCESS_FAULT_RE_S2;
    sc_signal<sc_uint<32>>        PC_BRANCH_VALUE_RE_S2;
    sc_signal<sc_uint<32>>        op1_se_s2;
    sc_signal<sc_uint<32>>        op2_se_s2;
    sc_signal<bool>        MULT_INST_RM_S2;
    sc_signal<bool>        MRET_RE_S2;
    sc_signal<bool>        ENV_CALL_WRONG_MODE_RE_S2;
    sc_signal<bool>        MEM2WBK_EMPTY_SM_S2;

    sc_signal<bool> DEC2EXE_POP_SE_S1;
    sc_signal<bool> DEC2EXE_EMPTY_SD_S1;

    sc_signal<bool> DEC2EXE_POP_SE_S2;
    sc_signal<bool> DEC2EXE_EMPTY_SD_S2;

    sc_signal<bool>       BP_R1_VALID_RD_S1;
    sc_signal<bool>       BP_R2_VALID_RD_S1;
    sc_signal<bool>       BP_R1_VALID_RD_S2;
    sc_signal<bool>       BP_R2_VALID_RD_S2;
    sc_signal<sc_uint<6>> BP_RADR1_RD_S1;
    sc_signal<sc_uint<6>> BP_RADR2_RD_S1;
    sc_signal<sc_uint<6>> BP_RADR1_RD_S2;
    sc_signal<sc_uint<6>> BP_RADR2_RD_S2;
    sc_signal<bool>       BLOCK_BP_RD_S1;

    sc_signal<bool>        CSR_WENABLE_RD_S1;
    sc_signal<sc_uint<32>> CSR_RDATA_RD_S1;
    sc_signal<sc_uint<12>> CSR_WADR_RD_S1;
    sc_signal<bool>        INTERRUPTION_SE_S1;
    sc_signal<bool>        INTERRUPTION_SE_S2;
    sc_signal<bool>        EXCEPTION_RD_S1;
    sc_signal<bool>        ECALL_I_RD;
    sc_signal<bool>        EBREAK_RD_S1;
    sc_signal<bool>        EBREAK_RE_S1;
    sc_signal<bool>        ILLEGAL_INSTRUCTION_RD_S1;  // accessing stuff in wrong mode
    sc_signal<bool>        ADRESS_MISSALIGNED_RD_S1;   // branch offset is misaligned
    sc_signal<bool>        ENV_CALL_U_MODE_RD_S1;
    sc_signal<bool>        ENV_CALL_M_MODE_RD_S1;

    // DEC-CSR interface
    sc_signal<sc_uint<12>> CSR_RADR_SD_S1;
    sc_signal<sc_uint<12>> CSR_RADR_SD_S2;
    sc_signal<sc_uint<32>> CSR_RDATA_SC_S1;
    sc_signal<sc_uint<32>> CSR_RDATA_SC_S2;
    sc_signal<sc_uint<32>> MCAUSE_SC;
    // DEC-REG interface
    sc_signal<sc_uint<32>> RDATA1_SR_S1;
    sc_signal<sc_uint<32>> RDATA2_SR_S1;

    sc_signal<sc_uint<6>> RADR1_SD_S1;
    sc_signal<sc_uint<6>> RADR2_SD_S1;

    sc_signal<sc_uint<32>> RDATA1_SR_S2;
    sc_signal<sc_uint<32>> RDATA2_SR_S2;

    sc_signal<sc_uint<6>> RADR1_SD_S2;
    sc_signal<sc_uint<6>> RADR2_SD_S2;

    sc_signal<sc_uint<6>> EXE_DEST_RD_S1;

    sc_signal<sc_uint<32>> READ_PC_SR;
    sc_signal<sc_uint<32>> WRITE_PC_SD;
    sc_signal<bool>        WRITE_PC_ENABLE_SD;

    //EXE-X0
    sc_signal<sc_uint<32>> op1_se_s1;
    sc_signal<sc_uint<32>> op2_se;

    // X0-X1 interface
    sc_signal<sc_bv<384>> multiplier_out_sx0;
    sc_signal<bool>       signed_op_rx0;
    sc_signal<bool>       x02x1_EMPTY_SX0, x02x1_POP_SX1;
    sc_signal<bool>       carry_rx0;
    sc_signal<bool>       carry_rx1;
    // EXE-MEM interface
    sc_signal<sc_uint<32>> PC_BRANCH_VALUE_RE_S1;
    sc_signal<sc_uint<32>> EXE_RES_RE_S1;
    sc_signal<sc_uint<32>> MEM_DATA_RE_S1;
    sc_signal<sc_uint<6>>  DEST_RE_S1;
    sc_signal<sc_uint<2>>  MEM_SIZE_RE_S1;
    sc_signal<sc_uint<32>> PC_EXE2MEM_RE_S1;
    sc_signal<bool>        MEM_WB_S1, MEM_SIGN_EXTEND_RE_S1;
    sc_signal<bool>        MEM_WB_S2;
    sc_signal<bool>        MEM_LOAD_RE_S1, MEM_STORE_RE_S1;
    sc_signal<bool>        MEM_MULT_RE, MULT_SEL_HIGH_RE_S1;

    sc_signal<bool> EXE2MEM_EMPTY_SE_S1, EXE2MEM_POP_SM_S1;
    sc_signal<bool> CSR_WENABLE_RE_S1;
    sc_signal<bool> MACHINE_SOFTWARE_INTERRUPT_SX;
    sc_signal<bool> MACHINE_TIMER_INTERRUPT_SX;
    sc_signal<bool> MACHINE_EXTERNAL_INTERRUPT_SX;

    sc_signal<bool>        MACHINE_SOFTWARE_INTERRUPT_SE_S1;
    sc_signal<bool>        MACHINE_TIMER_INTERRUPT_SE_S1;
    sc_signal<bool>        MACHINE_TIMER_INTERRUPT_SE_S2;
    sc_signal<bool>        MACHINE_EXTERNAL_INTERRUPT_SE_S1;
    sc_signal<bool>        MACHINE_EXTERNAL_INTERRUPT_SE_S2;
    sc_signal<sc_uint<12>> CSR_WADR_RE_S1;
    sc_signal<sc_uint<12>> CSR_WADR_RE_S2;
    sc_signal<sc_uint<32>> CSR_RDATA_RE_S1;

    sc_signal<bool> EXCEPTION_RE_S1;
    sc_signal<bool> LOAD_ADRESS_MISSALIGNED_RE_S1;  // adress from store/load isn't aligned
    sc_signal<bool> LOAD_ACCESS_FAULT_RE_S1;        // trying to access memory in wrong mode

    sc_signal<bool> STORE_ADRESS_MISSALIGNED_RE_S1;
    sc_signal<bool> STORE_ACCESS_FAULT_RE_S1;
    sc_signal<bool> ECALL_I_RE;
    sc_signal<bool> EBREAK_I_RE;
    sc_signal<bool> ILLEGAL_INSTRUCTION_RE_S1;             // accessing stuff in wrong mode
    sc_signal<bool> INSTRUCTION_ADRESS_MISSALIGNED_RE_S1;  // branch offset is misaligned
    sc_signal<bool> ENV_CALL_S_MODE_RE_S1;
    sc_signal<bool> ENV_CALL_M_MODE_RE_S1;
    sc_signal<bool> ENV_CALL_S_MODE_RD_S1;
    sc_signal<bool> ENV_CALL_WRONG_MODE_RD_S1;
    sc_signal<bool> MRET_RD_S1;
    sc_signal<bool> ENV_CALL_WRONG_MODE_RE_S1;
    sc_signal<bool> ENV_CALL_U_MODE_RE_S1;
    sc_signal<bool> MRET_RE_S1;
    sc_signal<bool> INSTRUCTION_ACCESS_FAULT_RD_S1;
    sc_signal<bool> INSTRUCTION_ACCESS_FAULT_RE_S1;
    sc_signal<bool> MULT_INST_RD_S1;
    sc_signal<bool> MULT_INST_RE_S1;
    sc_signal<bool> MULT_INST_RM_S1;
    // X1-X2 interface
    sc_signal<sc_bv<128>> multiplier_out_sx1;
    sc_signal<sc_uint<32>>  multiplier_out_sx2;
    sc_signal<bool>       signed_op_rx1;
    sc_signal<bool>       x12x2_EMPTY_SX1, x12x2_POP_SX2;

    // MEM-WBK interface
    sc_signal<sc_uint<32>> MEM_RES_RM_S1;
    sc_signal<sc_uint<6>>  DEST_RM_S1;
    sc_signal<bool>        WB_RM_S1;
    sc_signal<bool>        WB_RM_S2;
    sc_signal<bool>        WBK_MEM_SIGN_EXTEND;
    sc_signal<bool>        MEM2WBK_EMPTY_SM_S1;
    sc_signal<bool>        MEM2WBK_POP_SW_S1;
    sc_signal<bool>        MEM2WBK_POP_SW_S2;
    sc_signal<bool>        WBK_MEM_LOAD;
    sc_signal<sc_uint<32>> PC_MEM2WBK_RM_S1;
    sc_signal<sc_uint<32>> PC_MEM2WBK_RM_S2;
    sc_signal<bool>        CSR_WENABLE_RM_S1;
    sc_signal<sc_uint<32>> CSR_RDATA_RM_S1;

    // MEM-CSR interface

    sc_signal<sc_uint<12>> CSR_WADR_SM_S1;
    sc_signal<sc_uint<32>> CSR_WDATA_SM_S1;
    sc_signal<sc_uint<12>> CSR_WADR_SM_S2;
    sc_signal<sc_uint<32>> CSR_WDATA_SM_S2;

    sc_signal<sc_uint<32>> MSTATUS_WDATA_RM_S1;
    sc_signal<sc_uint<32>> MIP_WDATA_RM_S1;
    sc_signal<sc_uint<32>> MEPC_WDATA_RM_S1;
    sc_signal<sc_uint<32>> MCAUSE_WDATA_SM_S1;
    sc_signal<sc_uint<32>> MEPC_SC_S1;
    sc_signal<sc_uint<32>> MSTATUS_RC_S1;
    sc_signal<sc_uint<32>> MTVEC_VALUE_RC_S1;
    sc_signal<sc_uint<32>> MIP_VALUE_RC_S1;
    sc_signal<sc_uint<32>> MTVAL_WDATA_SM_S1;
    sc_signal<bool>        CSR_ENABLE_BEFORE_FIFO_SM_S1;

    sc_signal<sc_uint<32>> MSTATUS_WDATA_RM_S2;
    sc_signal<sc_uint<32>> MIP_WDATA_RM_S2;
    sc_signal<sc_uint<32>> MEPC_WDATA_RM_S2;
    sc_signal<sc_uint<32>> MCAUSE_WDATA_SM_S2;
    sc_signal<sc_uint<32>> MEPC_SC_S2;
    sc_signal<sc_uint<32>> MSTATUS_RC_S2;
    sc_signal<sc_uint<32>> MTVEC_VALUE_RC_S2;
    sc_signal<sc_uint<32>> MIP_VALUE_RC_S2;
    sc_signal<sc_uint<32>> MTVAL_WDATA_SM_S2;
    sc_signal<bool>        CSR_ENABLE_BEFORE_FIFO_SM_S2;

    // MEM-IFETCH

    sc_signal<bool>        MRET_SM_S1;
    sc_signal<bool>        MRET_SM_S2;
    sc_signal<sc_uint<32>> RETURN_ADRESS_SM_S1;
    sc_signal<sc_uint<32>> RETURN_ADRESS_SM_S2;

    // MEM - Pipeline :

    sc_signal<bool> EXCEPTION_SM_S1;
    sc_signal<bool> EXCEPTION_SM_S2;
    sc_signal<bool> BUS_ERROR_SX;
    // WBK-REG interface

    sc_signal<sc_uint<6>>  WADR_SW_S1;
    sc_signal<sc_uint<32>> WDATA_SW_S1;
    sc_signal<bool>        WENABLE_SW_S1;

    sc_signal<sc_uint<6>>  WADR_SW_S2;
    sc_signal<sc_uint<32>> WDATA_SW_S2;
    sc_signal<bool>        WENABLE_SW_S2;


    // Pipeline Mode

    sc_signal<sc_uint<2>> CURRENT_MODE_SM_S1;


    x2_multiplier      x2_multiplier_inst;

    void core_method();

    void trace(sc_trace_file * tf);
    SC_CTOR(core) : 
          ifetch_inst("ifetch"),
          dec_inst("decod"),
          exec_inst_s1("exec_s1"),
          exec_inst_s2("exec_s2"),
          mem_inst_s1("mem_s1"),
          mem_inst_s2("mem_s2"),
          wbk_inst_s1("wbk_s1"),
          wbk_inst_s2("wbk_s2"),
          x0_multiplier_inst("x0_multiplier"),
          x1_multiplier_inst("x1_multiplier"),
          x2_multiplier_inst("x2_multiplier"),
          reg_inst("reg"),
          csr_inst("csr") {
        SC_METHOD(core_method);
        sensitive << READ_PC_SR;

        ifetch_inst.DEC2IF_POP_SI(DEC2IF_POP_SI);
        ifetch_inst.DEC2IF_EMPTY_SI(DEC2IF_EMPTY_SI);
        ifetch_inst.PC_DEC2IF_RD_S1(PC_DEC2IF_RD_S1);
        ifetch_inst.PC_DEC2IF_RD_S2(PC_DEC2IF_RD_S2);
        ifetch_inst.INSTR_RI_S1(INSTR_RI_S1);
        ifetch_inst.INSTR_RI_S2(INSTR_RI_S2);
        ifetch_inst.IF2DEC_EMPTY_SI_S1(IF2DEC_EMPTY_SI_S1);
        ifetch_inst.IF2DEC_POP_SD_S1(IF2DEC_POP_SD_S1);
        ifetch_inst.IF2DEC_FLUSH_SD(IF2DEC_FLUSH_SD);

        ifetch_inst.IF2DEC_EMPTY_SI_S2(IF2DEC_EMPTY_SI_S2);
        ifetch_inst.IF2DEC_POP_SD_S2(IF2DEC_POP_SD_S2);

        ifetch_inst.ADR_SI_S1(ADR_SI_S1);
        ifetch_inst.ADR_SI_S2(ADR_SI_S2);
        ifetch_inst.ADR_VALID_SI_S1(ADR_VALID_SI_S1);
        ifetch_inst.ADR_VALID_SI_S2(ADR_VALID_SI_S2);

        ifetch_inst.IC_INST_SI_S1(IC_INST_SI_S1);
        ifetch_inst.IC_INST_SI_S2(IC_INST_SI_S2);
        ifetch_inst.IC_STALL_SI(IC_STALL_SI);

        ifetch_inst.PC_IF2DEC_RI_S1(PC_IF2DEC_RI_S1);
        ifetch_inst.PC_IF2DEC_RI_S2(PC_IF2DEC_RI_S2);
        ifetch_inst.INTERRUPTION_SE_S1(INTERRUPTION_SE_S1);
        ifetch_inst.EXCEPTION_RI(EXCEPTION_RI);
        ifetch_inst.EXCEPTION_SM_S1(EXCEPTION_SM_S1);
        ifetch_inst.CURRENT_MODE_SM_S1(CURRENT_MODE_SM_S1);
        ifetch_inst.MRET_SM_S1(MRET_SM_S1);
        ifetch_inst.RETURN_ADRESS_SM_S1(RETURN_ADRESS_SM_S1);
        ifetch_inst.PRIORITARY_PIPELINE_RD(PRIORITARY_PIPELINE_RD);

 



        ifetch_inst.CLK(CLK);
        ifetch_inst.RESET(RESET);

        dec_inst.DEC2IF_POP_SI(DEC2IF_POP_SI);
        dec_inst.DEC2IF_EMPTY_SD(DEC2IF_EMPTY_SI);
        dec_inst.PC_DEC2IF_RD_S1(PC_DEC2IF_RD_S1);
        dec_inst.PC_DEC2IF_RD_S2(PC_DEC2IF_RD_S2);
        dec_inst.PC_DEC2EXE_RD_S1(PC_DEC2EXE_RD_S1);
        dec_inst.PC_IF2DEC_RI_S1(PC_IF2DEC_RI_S1);
        dec_inst.PC_IF2DEC_RI_S2(PC_IF2DEC_RI_S2);
        dec_inst.INSTR_RI_S1(INSTR_RI_S1);
        dec_inst.INSTR_RI_S2(INSTR_RI_S2);
        dec_inst.IF2DEC_EMPTY_SI_S1(IF2DEC_EMPTY_SI_S1);
        dec_inst.IF2DEC_EMPTY_SI_S2(IF2DEC_EMPTY_SI_S2);
        dec_inst.IF2DEC_POP_SD_S1(IF2DEC_POP_SD_S1);
        dec_inst.IF2DEC_POP_SD_S2(IF2DEC_POP_SD_S2);
        dec_inst.IF2DEC_FLUSH_SD(IF2DEC_FLUSH_SD);

        dec_inst.OP1_RD_S1(OP1_RD_S1);
        dec_inst.OP2_RD_S1(OP2_RD_S1);
        dec_inst.EXE_CMD_RD_S1(EXE_CMD_RD_S1);
        dec_inst.NEG_OP2_RD_S1(NEG_OP2_RD_S1);
        dec_inst.WB_RD_S1(WB_RD_S1);
        dec_inst.SELECT_TYPE_OPERATIONS_RD_S1(SELECT_TYPE_OPERATIONS_RD_S1);
        dec_inst.SLT_RD_S1(SLT_RD_S1);
        dec_inst.SLTU_RD_S1(SLTU_RD_S1);

        dec_inst.MEM_DATA_RD_S1(MEM_DATA_RD_S1);
        dec_inst.MEM_LOAD_RD_S1(MEM_LOAD_RD_S1);
        dec_inst.MEM_STORE_RD_S1(MEM_STORE_RD_S1);
        dec_inst.MEM_SIGN_EXTEND_RD_S1(MEM_SIGN_EXTEND_RD_S1);
        dec_inst.MEM_SIZE_RD_S1(MEM_SIZE_RD_S1);


        dec_inst.OP1_RD_S2(OP1_RD_S2);
        dec_inst.OP2_RD_S2(OP2_RD_S2);
        dec_inst.EXE_CMD_RD_S2(EXE_CMD_RD_S2);
        dec_inst.NEG_OP2_RD_S2(NEG_OP2_RD_S2);
        dec_inst.WB_RD_S2(WB_RD_S2);
        dec_inst.EXE_DEST_RD_S2(EXE_DEST_RD_S2);
        dec_inst.SELECT_TYPE_OPERATIONS_RD_S2(SELECT_TYPE_OPERATIONS_RD_S2);
        dec_inst.SLT_RD_S2(SLT_RD_S2);
        dec_inst.SLTU_RD_S2(SLTU_RD_S2);
        dec_inst.PC_DEC2EXE_RD_S2(PC_DEC2EXE_RD_S2);
        dec_inst.PC_BRANCH_VALUE_RD_S2(PC_BRANCH_VALUE_RD_S2);

        dec_inst.MEM_DATA_RD_S2(MEM_DATA_RD_S2);
        dec_inst.MEM_LOAD_RD_S2(MEM_LOAD_RD_S2);
        dec_inst.MEM_STORE_RD_S2(MEM_STORE_RD_S2);
        dec_inst.MEM_SIGN_EXTEND_RD_S2(MEM_SIGN_EXTEND_RD_S2);
        dec_inst.MEM_SIZE_RD_S2(MEM_SIZE_RD_S2);
        dec_inst.CSR_WENABLE_RD_S2(CSR_WENABLE_RD_S2);
        dec_inst.CSR_WADR_RD_S2(CSR_WADR_RD_S2);
        dec_inst.CSR_RDATA_RD_S2(CSR_RDATA_RD_S2);

        dec_inst.DEC2EXE_POP_SE_S1(DEC2EXE_POP_SE_S1);
        dec_inst.DEC2EXE_EMPTY_SD_S1(DEC2EXE_EMPTY_SD_S1);

        dec_inst.DEC2EXE_POP_SE_S2(DEC2EXE_POP_SE_S2);
        dec_inst.DEC2EXE_EMPTY_SD_S2(DEC2EXE_EMPTY_SD_S2);

        dec_inst.RADR1_SD_S1(RADR1_SD_S1);
        dec_inst.RADR2_SD_S1(RADR2_SD_S1);

        dec_inst.RDATA1_SR_S1(RDATA1_SR_S1);
        dec_inst.RDATA2_SR_S1(RDATA2_SR_S1);

        dec_inst.RADR1_SD_S2(RADR1_SD_S2);
        dec_inst.RADR2_SD_S2(RADR2_SD_S2);


        dec_inst.RDATA1_SR_S2(RDATA1_SR_S2);
        dec_inst.RDATA2_SR_S2(RDATA2_SR_S2);

        dec_inst.EXE_DEST_RD_S1(EXE_DEST_RD_S1);

        dec_inst.READ_PC_SR(READ_PC_SR);
        dec_inst.WRITE_PC_SD(WRITE_PC_SD);
        dec_inst.WRITE_PC_ENABLE_SD(WRITE_PC_ENABLE_SD);

        dec_inst.DEST_RE_S1(DEST_RE_S1);
        dec_inst.EXE_RES_RE_S1(EXE_RES_RE_S1);
        dec_inst.DEST_RM_S1(DEST_RM_S1);
        dec_inst.MEM_RES_RM_S1(MEM_RES_RM_S1);
        dec_inst.EXE2MEM_EMPTY_SE_S1(EXE2MEM_EMPTY_SE_S1);
        dec_inst.MEM2WBK_EMPTY_SM_S1(MEM2WBK_EMPTY_SM_S1);

        dec_inst.BP_R1_VALID_RD_S1(BP_R1_VALID_RD_S1);
        dec_inst.BP_R2_VALID_RD_S1(BP_R2_VALID_RD_S1);
        dec_inst.BP_R1_VALID_RD_S2(BP_R1_VALID_RD_S2);
        dec_inst.BP_R2_VALID_RD_S2(BP_R2_VALID_RD_S2);
        dec_inst.BP_RADR1_RD_S1(BP_RADR1_RD_S1);
        dec_inst.BP_RADR2_RD_S1(BP_RADR2_RD_S1);
        dec_inst.BP_RADR1_RD_S2(BP_RADR1_RD_S2);
        dec_inst.BP_RADR2_RD_S2(BP_RADR2_RD_S2);
        dec_inst.MEM_LOAD_RE_S1(MEM_LOAD_RE_S1);

        dec_inst.CSR_WENABLE_RE_S1(CSR_WENABLE_RE_S1);
        dec_inst.CSR_RDATA_RE_S1(CSR_RDATA_RE_S1);
        dec_inst.CSR_WENABLE_RM_S1(CSR_WENABLE_RM_S1);
        dec_inst.CSR_RDATA_RM_S1(CSR_RDATA_RM_S1);
        dec_inst.EBREAK_RD_S1(EBREAK_RD_S1);



        dec_inst.DEST_RE_S2(DEST_RE_S2);
        dec_inst.MEM_LOAD_RE_S2(MEM_LOAD_RE_S2);
        dec_inst.EXE2MEM_EMPTY_SE_S2(EXE2MEM_EMPTY_SE_S2);
        dec_inst.CSR_WENABLE_RE_S2(CSR_WENABLE_RE_S2);
        dec_inst.CSR_RDATA_RE_S2(CSR_RDATA_RE_S2);
        dec_inst.DEST_RM_S2(DEST_RM_S2);
        dec_inst.CSR_WENABLE_RM_S2(CSR_WENABLE_RM_S2);
        dec_inst.CSR_RDATA_RM_S2(CSR_RDATA_RM_S2);
        dec_inst.BP_MEM2WBK_EMPTY_SM_S2(MEM2WBK_EMPTY_SM_S2);
        dec_inst.EXE_RES_RE_S2(EXE_RES_RE_S2);
        dec_inst.MEM_RES_RM_S2(MEM_RES_RM_S2);


        dec_inst.CSR_WENABLE_RD_S1(CSR_WENABLE_RD_S1);
        dec_inst.CSR_WADR_RD_S1(CSR_WADR_RD_S1);
        dec_inst.CSR_RADR_SD_S1(CSR_RADR_SD_S1);
        dec_inst.CSR_RADR_SD_S2(CSR_RADR_SD_S2);
        dec_inst.CSR_RDATA_SC_S1(CSR_RDATA_SC_S1);
        dec_inst.CSR_RDATA_SC_S2(CSR_RDATA_SC_S2);
        dec_inst.CSR_RDATA_RD_S1(CSR_RDATA_RD_S1);
        dec_inst.INTERRUPTION_SE_S1(INTERRUPTION_SE_S1);
        dec_inst.EXCEPTION_RI(EXCEPTION_RI);
        dec_inst.EXCEPTION_RD_S1(EXCEPTION_RD_S1);
        dec_inst.ENV_CALL_S_MODE_RD_S1(ENV_CALL_S_MODE_RD_S1);
        dec_inst.ENV_CALL_WRONG_MODE_RD_S1(ENV_CALL_WRONG_MODE_RD_S1);
        dec_inst.ILLEGAL_INSTRUCTION_RD_S1(ILLEGAL_INSTRUCTION_RD_S1);  // accessing stuff in wrong mode
        dec_inst.ADRESS_MISSALIGNED_RD_S1(ADRESS_MISSALIGNED_RD_S1);    // branch offset is misaligned
        dec_inst.ENV_CALL_U_MODE_RD_S1(ENV_CALL_U_MODE_RD_S1);
        dec_inst.ENV_CALL_M_MODE_RD_S1(ENV_CALL_M_MODE_RD_S1);
        dec_inst.EXCEPTION_SM_S1(EXCEPTION_SM_S1);


        dec_inst.ENV_CALL_S_MODE_RD_S2(ENV_CALL_S_MODE_RD_S2);
        dec_inst.ENV_CALL_WRONG_MODE_RD_S2(ENV_CALL_WRONG_MODE_RD_S2);
        dec_inst.ILLEGAL_INSTRUCTION_RD_S2(ILLEGAL_INSTRUCTION_RD_S2);  // accessing stuff in wrong mode
        dec_inst.ADRESS_MISSALIGNED_RD_S2(ADRESS_MISSALIGNED_RD_S2);    // branch offset is misaligned
        dec_inst.ENV_CALL_U_MODE_RD_S2(ENV_CALL_U_MODE_RD_S2);
        dec_inst.ENV_CALL_M_MODE_RD_S2(ENV_CALL_M_MODE_RD_S2);
        dec_inst.INSTRUCTION_ACCESS_FAULT_RD_S2(INSTRUCTION_ACCESS_FAULT_RD_S2);
        dec_inst.MRET_RD_S2(MRET_RD_S2);
        dec_inst.EXCEPTION_RD_S2(EXCEPTION_RD_S2);
        dec_inst.EBREAK_RD_S2(EBREAK_RD_S2);

        dec_inst.BLOCK_BP_RD_S2(BLOCK_BP_RD_S2);

        dec_inst.MTVEC_VALUE_RC_S1(MTVEC_VALUE_RC_S1);
        dec_inst.MRET_SM_S1(MRET_SM_S1);
        dec_inst.BLOCK_BP_RD_S1(BLOCK_BP_RD_S1);
        dec_inst.CURRENT_MODE_SM_S1(CURRENT_MODE_SM_S1);
        dec_inst.MRET_RD_S1(MRET_RD_S1);
        dec_inst.RETURN_ADRESS_SM_S1(RETURN_ADRESS_SM_S1);
        dec_inst.INSTRUCTION_ACCESS_FAULT_RD_S1(INSTRUCTION_ACCESS_FAULT_RD_S1);
        dec_inst.MCAUSE_WDATA_SM_S1(MCAUSE_WDATA_SM_S1);
        dec_inst.MULT_INST_RD_S1(MULT_INST_RD_S1);
        dec_inst.MULT_INST_RD_S2(MULT_INST_RD_S2);
        dec_inst.MULT_INST_RE_S1(MULT_INST_RE_S1);
        dec_inst.MULT_INST_RM_S1(MULT_INST_RM_S1);
        dec_inst.PC_BRANCH_VALUE_RD_S1(PC_BRANCH_VALUE_RD_S1);
        dec_inst.PRIORITARY_PIPELINE_RD(PRIORITARY_PIPELINE_RD);

        dec_inst.CLK(CLK);
        dec_inst.RESET_N(RESET);

        exec_inst_s1.RADR1_RD_S1(BP_RADR1_RD_S1);
        exec_inst_s1.RADR2_RD_S1(BP_RADR2_RD_S1);
        exec_inst_s1.OP1_VALID_RD_S1(BP_R1_VALID_RD_S1);
        exec_inst_s1.OP2_VALID_RD_S1(BP_R2_VALID_RD_S1);
        exec_inst_s1.MEM_DEST_RM_S1(DEST_RM_S1);
        exec_inst_s1.MEM_DEST_RM_S2(DEST_RM_S2);
        exec_inst_s1.MEM_RES_RM_S1(MEM_RES_RM_S1);
        exec_inst_s1.MEM_RES_RM_S2(MEM_RES_RM_S2);
        exec_inst_s1.OP1_RD_S1(OP1_RD_S1);
        exec_inst_s1.OP2_RD_S1(OP2_RD_S1);
        exec_inst_s1.CMD_RD_S1(EXE_CMD_RD_S1);
        exec_inst_s1.DEST_RD_S1(EXE_DEST_RD_S1);
        exec_inst_s1.NEG_OP2_RD_S1(NEG_OP2_RD_S1);
        exec_inst_s1.WB_RD_S1(WB_RD_S1);
        exec_inst_s1.SELECT_TYPE_OPERATIONS_RD_S1(SELECT_TYPE_OPERATIONS_RD_S1);
        exec_inst_s1.EBREAK_RD_S1(EBREAK_RD_S1);
        exec_inst_s1.EBREAK_RE_S1(EBREAK_RE_S1);

        exec_inst_s1.MULT_INST_RD_S1(MULT_INST_RD_S1);
        exec_inst_s1.PC_DEC2EXE_RD_S1(PC_DEC2EXE_RD_S1);
        exec_inst_s1.PC_EXE2MEM_RE_S1(PC_EXE2MEM_RE_S1);

        exec_inst_s1.MEM_DATA_RD_S1(MEM_DATA_RD_S1);
        exec_inst_s1.MEM_LOAD_RD_S1(MEM_LOAD_RD_S1);
        exec_inst_s1.MEM_STORE_RD_S1(MEM_STORE_RD_S1);
        exec_inst_s1.MEM_SIGN_EXTEND_RD_S1(MEM_SIGN_EXTEND_RD_S1);
        exec_inst_s1.MEM_SIZE_RD_S1(MEM_SIZE_RD_S1);
        exec_inst_s1.SLT_RD_S1(SLT_RD_S1);
        exec_inst_s1.SLTU_RD_S1(SLTU_RD_S1);

        exec_inst_s1.DEC2EXE_POP_SE_S1(DEC2EXE_POP_SE_S1);
        exec_inst_s1.DEC2EXE_EMPTY_SD_S1(DEC2EXE_EMPTY_SD_S1);

        exec_inst_s1.EXE_RES_RE_S1(EXE_RES_RE_S1);
        exec_inst_s1.EXE_RES_RE_S2(EXE_RES_RE_S2);
        exec_inst_s1.MEM_DATA_RE_S1(MEM_DATA_RE_S1);
        exec_inst_s1.DEST_RE_S1(DEST_RE_S1);
        exec_inst_s1.DEST_RE_S2(DEST_RE_S2);
        exec_inst_s1.MEM_SIZE_RE_S1(MEM_SIZE_RE_S1);

        exec_inst_s1.WB_RE_S1(MEM_WB_S1);
        exec_inst_s1.MEM_SIGN_EXTEND_RE_S1(MEM_SIGN_EXTEND_RE_S1);
        exec_inst_s1.MEM_LOAD_RE_S1(MEM_LOAD_RE_S1);
        exec_inst_s1.MEM_LOAD_RE_S2(MEM_LOAD_RE_S2);
        exec_inst_s1.MEM_STORE_RE_S1(MEM_STORE_RE_S1);
        exec_inst_s1.MULT_INST_RE_S1(MULT_INST_RE_S1);
        exec_inst_s1.MULT_SEL_HIGH_RE_S1(MULT_SEL_HIGH_RE_S1);
        exec_inst_s1.EXE2MEM_EMPTY_SE_S1(EXE2MEM_EMPTY_SE_S1);
        exec_inst_s1.EXE2MEM_POP_SM_S1(EXE2MEM_POP_SM_S1);

        exec_inst_s1.CSR_WENABLE_RM_S1(CSR_WENABLE_RM_S1);
        exec_inst_s1.CSR_WENABLE_RM_S2(CSR_WENABLE_RM_S2);
        exec_inst_s1.CSR_RDATA_RM_S1(CSR_RDATA_RM_S1);
        exec_inst_s1.CSR_RDATA_RM_S2(CSR_RDATA_RM_S2);

        exec_inst_s1.INTERRUPTION_SE_S1(INTERRUPTION_SE_S1);
        exec_inst_s1.MACHINE_SOFTWARE_INTERRUPT_SX(MACHINE_SOFTWARE_INTERRUPT_SX);
        exec_inst_s1.MACHINE_TIMER_INTERRUPT_SX(MACHINE_TIMER_INTERRUPT_SX);
        exec_inst_s1.MACHINE_EXTERNAL_INTERRUPT_SX(MACHINE_EXTERNAL_INTERRUPT_SX);
        exec_inst_s1.MACHINE_SOFTWARE_INTERRUPT_SE_S1(MACHINE_SOFTWARE_INTERRUPT_SE_S1);
        exec_inst_s1.MACHINE_TIMER_INTERRUPT_SE_S1(MACHINE_TIMER_INTERRUPT_SE_S1);
        exec_inst_s1.MACHINE_EXTERNAL_INTERRUPT_SE_S1(MACHINE_EXTERNAL_INTERRUPT_SE_S1);
        exec_inst_s1.EXCEPTION_RD_S1(EXCEPTION_RD_S1);
        exec_inst_s1.CSR_WENABLE_RD_S1(CSR_WENABLE_RD_S1);
        exec_inst_s1.CSR_WENABLE_RE_S1(CSR_WENABLE_RE_S1);
        exec_inst_s1.CSR_WENABLE_RE_S2(CSR_WENABLE_RE_S2);
        exec_inst_s1.CSR_WADR_RD_S1(CSR_WADR_RD_S1);
        exec_inst_s1.CSR_WADR_RE_S1(CSR_WADR_RE_S1);
        exec_inst_s1.CSR_RDATA_RE_S1(CSR_RDATA_RE_S1);
        exec_inst_s1.CSR_RDATA_RE_S2(CSR_RDATA_RE_S2);
        exec_inst_s1.CSR_RDATA_RD_S1(CSR_RDATA_RD_S1);
        exec_inst_s1.ENV_CALL_S_MODE_RD_S1(ENV_CALL_S_MODE_RD_S1);
        exec_inst_s1.ENV_CALL_WRONG_MODE_RD_S1(ENV_CALL_WRONG_MODE_RD_S1);
        exec_inst_s1.ILLEGAL_INSTRUCTION_RD_S1(ILLEGAL_INSTRUCTION_RD_S1);  // accessing stuff in wrong mode
        exec_inst_s1.ADRESS_MISSALIGNED_RD_S1(ADRESS_MISSALIGNED_RD_S1);    // branch offset is misaligned
        exec_inst_s1.ENV_CALL_U_MODE_RD_S1(ENV_CALL_U_MODE_RD_S1);
        exec_inst_s1.ENV_CALL_M_MODE_RD_S1(ENV_CALL_M_MODE_RD_S1);
        exec_inst_s1.MRET_RD_S1(MRET_RD_S1);

        exec_inst_s1.EXCEPTION_RE_S1(EXCEPTION_RE_S1);
        exec_inst_s1.LOAD_ADRESS_MISSALIGNED_RE_S1(LOAD_ADRESS_MISSALIGNED_RE_S1);
        exec_inst_s1.LOAD_ACCESS_FAULT_RE_S1(LOAD_ACCESS_FAULT_RE_S1);
        exec_inst_s1.STORE_ADRESS_MISSALIGNED_RE_S1(STORE_ADRESS_MISSALIGNED_RE_S1);
        exec_inst_s1.STORE_ACCESS_FAULT_RE_S1(STORE_ACCESS_FAULT_RE_S1);
        exec_inst_s1.ENV_CALL_WRONG_MODE_RE_S1(ENV_CALL_WRONG_MODE_RE_S1);
        exec_inst_s1.ENV_CALL_U_MODE_RE_S1(ENV_CALL_U_MODE_RE_S1);
        exec_inst_s1.ILLEGAL_INSTRUCTION_RE_S1(ILLEGAL_INSTRUCTION_RE_S1);
        exec_inst_s1.INSTRUCTION_ADRESS_MISSALIGNED_RE_S1(INSTRUCTION_ADRESS_MISSALIGNED_RE_S1);
        exec_inst_s1.ENV_CALL_S_MODE_RE_S1(ENV_CALL_S_MODE_RE_S1);
        exec_inst_s1.ENV_CALL_M_MODE_RE_S1(ENV_CALL_M_MODE_RE_S1);
        exec_inst_s1.EXCEPTION_SM_S1(EXCEPTION_SM_S1);
        exec_inst_s1.BLOCK_BP_RD_S1(BLOCK_BP_RD_S1);
        exec_inst_s1.CURRENT_MODE_SM_S1(CURRENT_MODE_SM_S1);
        exec_inst_s1.MRET_RE_S1(MRET_RE_S1);
        exec_inst_s1.INSTRUCTION_ACCESS_FAULT_RD_S1(INSTRUCTION_ACCESS_FAULT_RD_S1);
        exec_inst_s1.INSTRUCTION_ACCESS_FAULT_RE_S1(INSTRUCTION_ACCESS_FAULT_RE_S1);
        exec_inst_s1.PC_BRANCH_VALUE_RD_S1(PC_BRANCH_VALUE_RD_S1);
        exec_inst_s1.PC_BRANCH_VALUE_RE_S1(PC_BRANCH_VALUE_RE_S1);

        exec_inst_s1.OP1_SE_S1(op1_se_s1);
        exec_inst_s1.OP2_SE_S1(op2_se);

        exec_inst_s1.MULT_INST_RM_S1(MULT_INST_RM_S1);
        exec_inst_s1.MEM2WBK_EMPTY_SM_S1(MEM2WBK_EMPTY_SM_S1);

        exec_inst_s1.CLK(CLK);
        exec_inst_s1.RESET(RESET);
    

        // EXE_S2 port map :

        exec_inst_s2.RADR1_RD_S2(BP_RADR1_RD_S1);
        exec_inst_s2.RADR2_RD_S2(BP_RADR2_RD_S1);
        exec_inst_s2.OP1_VALID_RD_S2(BP_R1_VALID_RD_S1);
        exec_inst_s2.OP2_VALID_RD_S2(BP_R2_VALID_RD_S1);
        exec_inst_s2.MEM_DEST_RM_S2(DEST_RM_S2);
        exec_inst_s2.MEM_RES_RM_S2(MEM_RES_RM_S2);
        exec_inst_s2.OP1_RD_S2(OP1_RD_S2);
        exec_inst_s2.OP2_RD_S2(OP2_RD_S2);
        exec_inst_s2.CMD_RD_S2(EXE_CMD_RD_S2);
        exec_inst_s2.DEST_RD_S2(EXE_DEST_RD_S2);
        exec_inst_s2.NEG_OP2_RD_S2(NEG_OP2_RD_S2);
        exec_inst_s2.WB_RD_S2(WB_RD_S2);
        exec_inst_s2.SELECT_TYPE_OPERATIONS_RD_S2(SELECT_TYPE_OPERATIONS_RD_S2);
        exec_inst_s2.EBREAK_RD_S2(EBREAK_RD_S2);
        exec_inst_s2.EBREAK_RE_S2(EBREAK_RE_S2);

        exec_inst_s2.MULT_INST_RD_S2(MULT_INST_RD_S2);
        exec_inst_s2.PC_DEC2EXE_RD_S2(PC_DEC2EXE_RD_S2);
        exec_inst_s2.PC_EXE2MEM_RE_S2(PC_EXE2MEM_RE_S2);

        exec_inst_s2.MEM_DATA_RD_S2(MEM_DATA_RD_S2);
        exec_inst_s2.MEM_LOAD_RD_S2(MEM_LOAD_RD_S2);
        exec_inst_s2.MEM_STORE_RD_S2(MEM_STORE_RD_S2);
        exec_inst_s2.MEM_SIGN_EXTEND_RD_S2(MEM_SIGN_EXTEND_RD_S2);
        exec_inst_s2.MEM_SIZE_RD_S2(MEM_SIZE_RD_S2);
        exec_inst_s2.SLT_RD_S2(SLT_RD_S2);
        exec_inst_s2.SLTU_RD_S2(SLTU_RD_S2);

        exec_inst_s2.DEC2EXE_POP_SE_S2(DEC2EXE_POP_SE_S2);
        exec_inst_s2.DEC2EXE_EMPTY_SD_S2(DEC2EXE_EMPTY_SD_S2);

        exec_inst_s2.EXE_RES_RE_S2(EXE_RES_RE_S2);
        exec_inst_s2.MEM_DATA_RE_S2(MEM_DATA_RE_S2);
        exec_inst_s2.DEST_RE_S2(DEST_RE_S2);
        exec_inst_s2.MEM_SIZE_RE_S2(MEM_SIZE_RE_S2);

        exec_inst_s2.WB_RE_S2(MEM_WB_S2);
        exec_inst_s2.MEM_SIGN_EXTEND_RE_S2(MEM_SIGN_EXTEND_RE_S2);
        exec_inst_s2.MEM_LOAD_RE_S2(MEM_LOAD_RE_S2);
        exec_inst_s2.MEM_STORE_RE_S2(MEM_STORE_RE_S2);
        exec_inst_s2.MULT_INST_RE_S2(MULT_INST_RE_S2);
        exec_inst_s2.MULT_SEL_HIGH_RE_S2(MULT_SEL_HIGH_RE_S2);
        exec_inst_s2.EXE2MEM_EMPTY_SE_S2(EXE2MEM_EMPTY_SE_S2);
        exec_inst_s2.EXE2MEM_POP_SM_S2(EXE2MEM_POP_SM_S2);

        exec_inst_s2.CSR_WENABLE_RM_S2(CSR_WENABLE_RM_S2);
        exec_inst_s2.CSR_RDATA_RM_S2(CSR_RDATA_RM_S2);

        exec_inst_s2.INTERRUPTION_SE_S2(INTERRUPTION_SE_S2);
        exec_inst_s2.MACHINE_SOFTWARE_INTERRUPT_SX(MACHINE_SOFTWARE_INTERRUPT_SX);
        exec_inst_s2.MACHINE_TIMER_INTERRUPT_SX(MACHINE_TIMER_INTERRUPT_SX);
        exec_inst_s2.MACHINE_EXTERNAL_INTERRUPT_SX(MACHINE_EXTERNAL_INTERRUPT_SX);
        exec_inst_s2.MACHINE_SOFTWARE_INTERRUPT_SE_S2(MACHINE_SOFTWARE_INTERRUPT_SE_S2);
        exec_inst_s2.MACHINE_TIMER_INTERRUPT_SE_S2(MACHINE_TIMER_INTERRUPT_SE_S2);
        exec_inst_s2.MACHINE_EXTERNAL_INTERRUPT_SE_S2(MACHINE_EXTERNAL_INTERRUPT_SE_S2);
        exec_inst_s2.EXCEPTION_RD_S2(EXCEPTION_RD_S2);
        exec_inst_s2.CSR_WENABLE_RD_S2(CSR_WENABLE_RD_S2);
        exec_inst_s2.CSR_WENABLE_RE_S2(CSR_WENABLE_RE_S2);
        exec_inst_s2.CSR_WADR_RD_S2(CSR_WADR_RD_S2);
        exec_inst_s2.CSR_RDATA_RD_S2(CSR_RDATA_RD_S2);
        exec_inst_s2.CSR_WADR_RE_S2(CSR_WADR_RE_S2);
        exec_inst_s2.CSR_RDATA_RE_S2(CSR_RDATA_RE_S2);
        exec_inst_s2.ENV_CALL_S_MODE_RD_S2(ENV_CALL_S_MODE_RD_S2);
        exec_inst_s2.ENV_CALL_WRONG_MODE_RD_S2(ENV_CALL_WRONG_MODE_RD_S2);
        exec_inst_s2.ILLEGAL_INSTRUCTION_RD_S2(ILLEGAL_INSTRUCTION_RD_S2);  // accessing stuff in wrong mode
        exec_inst_s2.ADRESS_MISSALIGNED_RD_S2(ADRESS_MISSALIGNED_RD_S2);    // branch offset is misaligned
        exec_inst_s2.ENV_CALL_U_MODE_RD_S2(ENV_CALL_U_MODE_RD_S2);
        exec_inst_s2.ENV_CALL_M_MODE_RD_S2(ENV_CALL_M_MODE_RD_S2);
        exec_inst_s2.MRET_RD_S2(MRET_RD_S2);

        exec_inst_s2.EXCEPTION_RE_S2(EXCEPTION_RE_S2);
        exec_inst_s2.LOAD_ADRESS_MISSALIGNED_RE_S2(LOAD_ADRESS_MISSALIGNED_RE_S2);
        exec_inst_s2.LOAD_ACCESS_FAULT_RE_S2(LOAD_ACCESS_FAULT_RE_S2);
        exec_inst_s2.STORE_ADRESS_MISSALIGNED_RE_S2(STORE_ADRESS_MISSALIGNED_RE_S2);
        exec_inst_s2.STORE_ACCESS_FAULT_RE_S2(STORE_ACCESS_FAULT_RE_S2);
        exec_inst_s2.ENV_CALL_WRONG_MODE_RE_S2(ENV_CALL_WRONG_MODE_RE_S2);
        exec_inst_s2.ENV_CALL_U_MODE_RE_S2(ENV_CALL_U_MODE_RE_S2);
        exec_inst_s2.ILLEGAL_INSTRUCTION_RE_S2(ILLEGAL_INSTRUCTION_RE_S2);
        exec_inst_s2.INSTRUCTION_ADRESS_MISSALIGNED_RE_S2(INSTRUCTION_ADRESS_MISSALIGNED_RE_S2);
        exec_inst_s2.ENV_CALL_S_MODE_RE_S2(ENV_CALL_S_MODE_RE_S2);
        exec_inst_s2.ENV_CALL_M_MODE_RE_S2(ENV_CALL_M_MODE_RE_S2);
        exec_inst_s2.EXCEPTION_SM_S1(EXCEPTION_SM_S1);
        exec_inst_s2.BLOCK_BP_RD_S2(BLOCK_BP_RD_S2);
        exec_inst_s2.CURRENT_MODE_SM_S2(CURRENT_MODE_SM_S2);
        exec_inst_s2.MRET_RE_S2(MRET_RE_S2);
        exec_inst_s2.INSTRUCTION_ACCESS_FAULT_RD_S2(INSTRUCTION_ACCESS_FAULT_RD_S2);
        exec_inst_s2.INSTRUCTION_ACCESS_FAULT_RE_S2(INSTRUCTION_ACCESS_FAULT_RE_S2);
        exec_inst_s2.PC_BRANCH_VALUE_RD_S2(PC_BRANCH_VALUE_RD_S2);
        exec_inst_s2.PC_BRANCH_VALUE_RE_S2(PC_BRANCH_VALUE_RE_S2);

        exec_inst_s2.OP1_SE_S2(op1_se_s2);
        exec_inst_s2.OP2_SE_S2(op2_se_s2);

        exec_inst_s2.MULT_INST_RM_S2(MULT_INST_RM_S2);
        exec_inst_s2.BP_MEM2WBK_EMPTY_SM_S2(MEM2WBK_EMPTY_SM_S2);

        exec_inst_s2.CLK(CLK);
        exec_inst_s2.RESET(RESET);



        //X0 - MULTIPLIER port map :

        x0_multiplier_inst.OP1_SE_S1(op1_se_s1);
        x0_multiplier_inst.OP2_SE_S1(op2_se);
        x0_multiplier_inst.EXE_CMD_RD_S1(EXE_CMD_RD_S1);
        x0_multiplier_inst.X02X1_POP_SX1(x02x1_POP_SX1);
        x0_multiplier_inst.DEC2X0_EMPTY_SD(DEC2EXE_EMPTY_SD_S1);

        x0_multiplier_inst.RES_RX0(multiplier_out_sx0);
        x0_multiplier_inst.SIGNED_OP_RX0(signed_op_rx0);
        x0_multiplier_inst.CARRY_RX0(carry_rx0);
        x0_multiplier_inst.X02X1_EMPTY_SX0(x02x1_EMPTY_SX0);

        x0_multiplier_inst.CLK(CLK);
        x0_multiplier_inst.RESET(RESET);

        // MEM port map :

        mem_inst_s1.EXE_RES_RE_S1(EXE_RES_RE_S1);  // 0
        mem_inst_s1.MEM_DATA_RE_S1(MEM_DATA_RE_S1);
        mem_inst_s1.DEST_RE_S1(DEST_RE_S1);
        mem_inst_s1.MEM_SIZE_RE_S1(MEM_SIZE_RE_S1);

        mem_inst_s1.WB_RE_S1(MEM_WB_S1);
        mem_inst_s1.SIGN_EXTEND_RE_S1(MEM_SIGN_EXTEND_RE_S1);
        mem_inst_s1.LOAD_RE_S1(MEM_LOAD_RE_S1);
        mem_inst_s1.STORE_RE_S1(MEM_STORE_RE_S1);
        mem_inst_s1.MULT_INST_RE_S1(MULT_INST_RE_S1);
        mem_inst_s1.MULT_INST_RM_S1(MULT_INST_RM_S1);
        mem_inst_s1.EXE2MEM_EMPTY_SE_S1(EXE2MEM_EMPTY_SE_S1);
        mem_inst_s1.EXE2MEM_POP_SM_S1(EXE2MEM_POP_SM_S1);

        mem_inst_s1.MEM_RES_RM_S1(MEM_RES_RM_S1);
        mem_inst_s1.DEST_RM_S1(DEST_RM_S1);
        mem_inst_s1.WB_RM_S1(WB_RM_S1);
        mem_inst_s1.EBREAK_RE_S1(EBREAK_RE_S1);

        mem_inst_s1.MEM2WBK_EMPTY_SM_S1(MEM2WBK_EMPTY_SM_S1);
        mem_inst_s1.MEM2WBK_POP_SW_S1(MEM2WBK_POP_SW_S1);

        mem_inst_s1.MCACHE_ADR_SM_S1(MCACHE_ADR_SM_S1);
        mem_inst_s1.MCACHE_DATA_SM_S1(MCACHE_DATA_SM_S1);
        mem_inst_s1.MCACHE_ADR_VALID_SM_S1(MCACHE_ADR_VALID_SM_S1);
        mem_inst_s1.MCACHE_STORE_SM_S1(MCACHE_STORE_SM_S1);
        mem_inst_s1.MCACHE_LOAD_SM_S1(MCACHE_LOAD_SM_S1);  // 19
        mem_inst_s1.MEM_SIZE_SM_S1(MEM_SIZE_SM_S1);  

        mem_inst_s1.MCACHE_RESULT_SM_S1(MCACHE_RESULT_SM_S1);
        mem_inst_s1.MCACHE_STALL_SM_S1(MCACHE_STALL_SM_S1);

        mem_inst_s1.PC_EXE2MEM_RE_S1(PC_EXE2MEM_RE_S1);
        mem_inst_s1.PC_MEM2WBK_RM_S1(PC_MEM2WBK_RM_S1);

        mem_inst_s1.INTERRUPTION_SE_S1(INTERRUPTION_SE_S1);
        mem_inst_s1.MACHINE_SOFTWARE_INTERRUPT_SE_S1(MACHINE_SOFTWARE_INTERRUPT_SE_S1);
        mem_inst_s1.MACHINE_TIMER_INTERRUPT_SE_S1(MACHINE_TIMER_INTERRUPT_SE_S1);
        mem_inst_s1.MACHINE_EXTERNAL_INTERRUPT_SE_S1(MACHINE_EXTERNAL_INTERRUPT_SE_S1);
        mem_inst_s1.CSR_WADR_SE_S1(CSR_WADR_RE_S1);
        mem_inst_s1.CSR_WADR_SM_S1(CSR_WADR_SM_S1);
        mem_inst_s1.CSR_WENABLE_RE_S1(CSR_WENABLE_RE_S1);
        mem_inst_s1.CSR_WENABLE_RM_S1(CSR_WENABLE_RM_S1);
        mem_inst_s1.CSR_WDATA_SM_S1(CSR_WDATA_SM_S1);
        mem_inst_s1.CSR_RDATA_RM_S1(CSR_RDATA_RM_S1);
        mem_inst_s1.CSR_RDATA_RE_S1(CSR_RDATA_RE_S1);

        mem_inst_s1.EXCEPTION_RE_S1(EXCEPTION_RE_S1);
        mem_inst_s1.LOAD_ADRESS_MISSALIGNED_RE_S1(LOAD_ADRESS_MISSALIGNED_RE_S1);
        mem_inst_s1.LOAD_ACCESS_FAULT_RE_S1(LOAD_ACCESS_FAULT_RE_S1);
        mem_inst_s1.STORE_ADRESS_MISSALIGNED_RE_S1(STORE_ADRESS_MISSALIGNED_RE_S1);
        mem_inst_s1.STORE_ACCESS_FAULT_RE_S1(STORE_ACCESS_FAULT_RE_S1);
        mem_inst_s1.ENV_CALL_U_MODE_RE_S1(ENV_CALL_U_MODE_RE_S1);
        mem_inst_s1.ENV_CALL_WRONG_MODE_RE_S1(ENV_CALL_WRONG_MODE_RE_S1);
        mem_inst_s1.ILLEGAL_INSTRUCTION_RE_S1(ILLEGAL_INSTRUCTION_RE_S1);
        mem_inst_s1.INSTRUCTION_ADRESS_MISSALIGNED_RE_S1(INSTRUCTION_ADRESS_MISSALIGNED_RE_S1);
        mem_inst_s1.ENV_CALL_S_MODE_RE_S1(ENV_CALL_S_MODE_RE_S1);  // 39
        mem_inst_s1.ENV_CALL_M_MODE_RE_S1(ENV_CALL_M_MODE_RE_S1);
        mem_inst_s1.MRET_RE_S1(MRET_RE_S1);
        mem_inst_s1.INSTRUCTION_ACCESS_FAULT_RE_S1(INSTRUCTION_ACCESS_FAULT_RE_S1);

        mem_inst_s1.BUS_ERROR_SX(BUS_ERROR_SX);

        mem_inst_s1.EXCEPTION_SM_S1(EXCEPTION_SM_S1);
        mem_inst_s1.CURRENT_MODE_SM_S1(CURRENT_MODE_SM_S1);
        mem_inst_s1.RETURN_ADRESS_SM_S1(RETURN_ADRESS_SM_S1);
        mem_inst_s1.MRET_SM_S1(MRET_SM_S1);

        mem_inst_s1.MSTATUS_WDATA_RM_S1(MSTATUS_WDATA_RM_S1);
        mem_inst_s1.MIP_WDATA_RM_S1(MIP_WDATA_RM_S1);
        mem_inst_s1.MEPC_WDATA_RM_S1(MEPC_WDATA_RM_S1);
        mem_inst_s1.MCAUSE_WDATA_SM_S1(MCAUSE_WDATA_SM_S1);

        mem_inst_s1.MEPC_SC_S1(MEPC_SC_S1);
        mem_inst_s1.MSTATUS_RC_S1(MSTATUS_RC_S1);
        mem_inst_s1.MTVEC_VALUE_RC_S1(MTVEC_VALUE_RC_S1);
        mem_inst_s1.MIP_VALUE_RC_S1(MIP_VALUE_RC_S1);      // 54
        mem_inst_s1.MTVAL_WDATA_SM_S1(MTVAL_WDATA_SM_S1);  // 54

        mem_inst_s1.CSR_ENABLE_BEFORE_FIFO_SM_S1(CSR_ENABLE_BEFORE_FIFO_SM_S1);  // 55
        mem_inst_s1.PC_BRANCH_VALUE_RE_S1(PC_BRANCH_VALUE_RE_S1);  

        mem_inst_s1.CLK(CLK);
        mem_inst_s1.RESET(RESET);  // 58

        // MEM_S2 port map :


        mem_inst_s2.EXE_RES_RE_S2(EXE_RES_RE_S2);  // 0
        mem_inst_s2.MEM_DATA_RE_S2(MEM_DATA_RE_S2);
        mem_inst_s2.DEST_RE_S2(DEST_RE_S2);
        mem_inst_s2.MEM_SIZE_RE_S2(MEM_SIZE_RE_S2);

        mem_inst_s2.WB_RE_S2(MEM_WB_S2);
        mem_inst_s2.SIGN_EXTEND_RE_S2(MEM_SIGN_EXTEND_RE_S2);
        mem_inst_s2.LOAD_RE_S2(MEM_LOAD_RE_S2);
        mem_inst_s2.STORE_RE_S2(MEM_STORE_RE_S2);
        mem_inst_s2.MULT_INST_RE_S2(MULT_INST_RE_S2);
        mem_inst_s2.MULT_INST_RM_S2(MULT_INST_RM_S2);
        mem_inst_s2.EXE2MEM_EMPTY_SE_S2(EXE2MEM_EMPTY_SE_S2);
        mem_inst_s2.EXE2MEM_POP_SM_S2(EXE2MEM_POP_SM_S2);

        mem_inst_s2.MEM_RES_RM_S2(MEM_RES_RM_S2);
        mem_inst_s2.DEST_RM_S2(DEST_RM_S2);
        mem_inst_s2.WB_RM_S2(WB_RM_S2);
        mem_inst_s2.EBREAK_RE_S2(EBREAK_RE_S2);

        mem_inst_s2.MEM2WBK_EMPTY_SM_S2(MEM2WBK_EMPTY_SM_S2);
        mem_inst_s2.MEM2WBK_POP_SW_S2(MEM2WBK_POP_SW_S2);

        mem_inst_s2.MCACHE_ADR_SM_S2(MCACHE_ADR_SM_S2);
        mem_inst_s2.MCACHE_DATA_SM_S2(MCACHE_DATA_SM_S2);
        mem_inst_s2.MCACHE_ADR_VALID_SM_S2(MCACHE_ADR_VALID_SM_S2);
        mem_inst_s2.MCACHE_STORE_SM_S2(MCACHE_STORE_SM_S2);
        mem_inst_s2.MCACHE_LOAD_SM_S2(MCACHE_LOAD_SM_S2);  // 19
        mem_inst_s2.MEM_SIZE_SM_S2(MEM_SIZE_SM_S2);  

        mem_inst_s2.MCACHE_RESULT_SM_S2(MCACHE_RESULT_SM_S2);
        mem_inst_s2.MCACHE_STALL_SM_S2(MCACHE_STALL_SM_S2);

        mem_inst_s2.PC_EXE2MEM_RE_S2(PC_EXE2MEM_RE_S2);
        mem_inst_s2.PC_MEM2WBK_RM_S2(PC_MEM2WBK_RM_S2);

        mem_inst_s2.INTERRUPTION_SE_S2(INTERRUPTION_SE_S2);
        mem_inst_s2.MACHINE_SOFTWARE_INTERRUPT_SE_S2(MACHINE_SOFTWARE_INTERRUPT_SE_S2);
        mem_inst_s2.MACHINE_TIMER_INTERRUPT_SE_S2(MACHINE_TIMER_INTERRUPT_SE_S2);
        mem_inst_s2.MACHINE_EXTERNAL_INTERRUPT_SE_S2(MACHINE_EXTERNAL_INTERRUPT_SE_S2);
        mem_inst_s2.CSR_WADR_SE_S2(CSR_WADR_RE_S2);
        mem_inst_s2.CSR_WADR_SM_S2(CSR_WADR_SM_S2);
        mem_inst_s2.CSR_WENABLE_RE_S2(CSR_WENABLE_RE_S2);
        mem_inst_s2.CSR_WENABLE_RM_S2(CSR_WENABLE_RM_S2);
        mem_inst_s2.CSR_WDATA_SM_S2(CSR_WDATA_SM_S2);
        mem_inst_s2.CSR_RDATA_RM_S2(CSR_RDATA_RM_S2);
        mem_inst_s2.CSR_RDATA_RE_S2(CSR_RDATA_RE_S2);

        mem_inst_s2.EXCEPTION_RE_S2(EXCEPTION_RE_S2);
        mem_inst_s2.LOAD_ADRESS_MISSALIGNED_RE_S2(LOAD_ADRESS_MISSALIGNED_RE_S2);
        mem_inst_s2.LOAD_ACCESS_FAULT_RE_S2(LOAD_ACCESS_FAULT_RE_S2);
        mem_inst_s2.STORE_ADRESS_MISSALIGNED_RE_S2(STORE_ADRESS_MISSALIGNED_RE_S2);
        mem_inst_s2.STORE_ACCESS_FAULT_RE_S2(STORE_ACCESS_FAULT_RE_S2);
        mem_inst_s2.ENV_CALL_U_MODE_RE_S2(ENV_CALL_U_MODE_RE_S2);
        mem_inst_s2.ENV_CALL_WRONG_MODE_RE_S2(ENV_CALL_WRONG_MODE_RE_S2);
        mem_inst_s2.ILLEGAL_INSTRUCTION_RE_S2(ILLEGAL_INSTRUCTION_RE_S2);
        mem_inst_s2.INSTRUCTION_ADRESS_MISSALIGNED_RE_S2(INSTRUCTION_ADRESS_MISSALIGNED_RE_S2);
        mem_inst_s2.ENV_CALL_S_MODE_RE_S2(ENV_CALL_S_MODE_RE_S2);  // 39
        mem_inst_s2.ENV_CALL_M_MODE_RE_S2(ENV_CALL_M_MODE_RE_S2);
        mem_inst_s2.MRET_RE_S2(MRET_RE_S2);
        mem_inst_s2.INSTRUCTION_ACCESS_FAULT_RE_S2(INSTRUCTION_ACCESS_FAULT_RE_S2);

        mem_inst_s2.BUS_ERROR_SX(BUS_ERROR_SX);

        mem_inst_s2.EXCEPTION_SM_S2(EXCEPTION_SM_S2);
        mem_inst_s2.CURRENT_MODE_SM_S2(CURRENT_MODE_SM_S2);
        mem_inst_s2.RETURN_ADRESS_SM_S2(RETURN_ADRESS_SM_S2);
        mem_inst_s2.MRET_SM_S2(MRET_SM_S2);

        mem_inst_s2.MSTATUS_WDATA_RM_S2(MSTATUS_WDATA_RM_S2);
        mem_inst_s2.MIP_WDATA_RM_S2(MIP_WDATA_RM_S2);
        mem_inst_s2.MEPC_WDATA_RM_S2(MEPC_WDATA_RM_S2);
        mem_inst_s2.MCAUSE_WDATA_SM_S2(MCAUSE_WDATA_SM_S2);

        mem_inst_s2.MEPC_SC_S2(MEPC_SC_S2);
        mem_inst_s2.MSTATUS_RC_S2(MSTATUS_RC_S2);
        mem_inst_s2.MTVEC_VALUE_RC_S2(MTVEC_VALUE_RC_S2);
        mem_inst_s2.MIP_VALUE_RC_S2(MIP_VALUE_RC_S2);      // 54
        mem_inst_s2.MTVAL_WDATA_SM_S2(MTVAL_WDATA_SM_S2);  // 54

        mem_inst_s2.CSR_ENABLE_BEFORE_FIFO_SM_S2(CSR_ENABLE_BEFORE_FIFO_SM_S2);  // 55
        mem_inst_s2.PC_BRANCH_VALUE_RE_S2(PC_BRANCH_VALUE_RE_S2);  

        mem_inst_s2.CLK(CLK);
        mem_inst_s2.RESET(RESET);  // 58


        //X1 - MULTIPLIER port map :

        x1_multiplier_inst.IN_RX0(multiplier_out_sx0);
        x1_multiplier_inst.SIGNED_OP_RX0(signed_op_rx0);
        x1_multiplier_inst.CARRY_RX0(carry_rx0);
        x1_multiplier_inst.X12X2_POP_SX2(x12x2_POP_SX2);
        x1_multiplier_inst.RES_RX1(multiplier_out_sx1);
        x1_multiplier_inst.SIGNED_OP_RX1(signed_op_rx1);
        x1_multiplier_inst.CARRY_RX1(carry_rx1);
        x1_multiplier_inst.X12X2_EMPTY_SX1(x12x2_EMPTY_SX1);
        x1_multiplier_inst.X02X1_EMPTY_SX0(x02x1_EMPTY_SX0);
        x1_multiplier_inst.X02X1_POP_SX1(x02x1_POP_SX1);
        x1_multiplier_inst.CLK(CLK);
        x1_multiplier_inst.RESET(RESET);
        // REG port map :

        reg_inst.RADR1_SD_S1(RADR1_SD_S1);
        reg_inst.RADR2_SD_S1(RADR2_SD_S1);
        reg_inst.RDATA1_SR_S1(RDATA1_SR_S1);
        reg_inst.RDATA2_SR_S1(RDATA2_SR_S1);

        reg_inst.RADR1_SD_S2(RADR1_SD_S2);
        reg_inst.RADR2_SD_S2(RADR2_SD_S2);
        reg_inst.RDATA1_SR_S2(RDATA1_SR_S2);
        reg_inst.RDATA2_SR_S2(RDATA2_SR_S2);

        reg_inst.WADR_SW_S1(WADR_SW_S1);
        reg_inst.WENABLE_SW_S1(WENABLE_SW_S1);
        reg_inst.WDATA_SW_S1(WDATA_SW_S1);

        reg_inst.WADR_SW_S2(WADR_SW_S2);
        reg_inst.WENABLE_SW_S2(WENABLE_SW_S2);
        reg_inst.WDATA_SW_S2(WDATA_SW_S2);

        reg_inst.READ_PC_SR(READ_PC_SR);
        reg_inst.WRITE_PC_SD(WRITE_PC_SD);
        reg_inst.WRITE_PC_ENABLE_SD(WRITE_PC_ENABLE_SD);
        reg_inst.PC_INIT(PC_INIT);

        reg_inst.CLK(CLK);
        reg_inst.RESET_N(RESET);

        // WBK_S1 port map :

        wbk_inst_s1.MEM_RES_RM_S1(MEM_RES_RM_S1);
        wbk_inst_s1.DEST_RM_S1(DEST_RM_S1);
        wbk_inst_s1.WB_RM_S1(WB_RM_S1);
        wbk_inst_s1.MEM2WBK_EMPTY_SM_S1(MEM2WBK_EMPTY_SM_S1);
        wbk_inst_s1.MEM2WBK_POP_SW_S1(MEM2WBK_POP_SW_S1);

        wbk_inst_s1.WADR_SW_S1(WADR_SW_S1);
        wbk_inst_s1.WDATA_SW_S1(WDATA_SW_S1);
        wbk_inst_s1.WENABLE_SW_S1(WENABLE_SW_S1);

        wbk_inst_s1.CSR_RDATA_RM_S1(CSR_RDATA_RM_S1);
        wbk_inst_s1.CSR_WENABLE_RM_S1(CSR_WENABLE_RM_S1);

        wbk_inst_s1.PC_MEM2WBK_RM_S1(PC_MEM2WBK_RM_S1);

        wbk_inst_s1.INTERRUPTION_SE_S1(INTERRUPTION_SE_S1);
        wbk_inst_s1.CURRENT_MODE_SM_S1(CURRENT_MODE_SM_S1);
        
        wbk_inst_s1.MULT_INST_RM_S1(MULT_INST_RM_S1);
        wbk_inst_s1.X2_RES_RX2(multiplier_out_sx2);

        wbk_inst_s1.CLK(CLK);
        wbk_inst_s1.RESET(RESET);


        // WBK_S2 port map :

        wbk_inst_s2.MEM_RES_RM_S2(MEM_RES_RM_S2);
        wbk_inst_s2.DEST_RM_S2(DEST_RM_S2);
        wbk_inst_s2.WB_RM_S2(WB_RM_S2);
        wbk_inst_s2.MEM2WBK_EMPTY_SM_S2(MEM2WBK_EMPTY_SM_S2);
        wbk_inst_s2.MEM2WBK_POP_SW_S2(MEM2WBK_POP_SW_S2);

        wbk_inst_s2.WADR_SW_S2(WADR_SW_S2);
        wbk_inst_s2.WDATA_SW_S2(WDATA_SW_S2);
        wbk_inst_s2.WENABLE_SW_S2(WENABLE_SW_S2);
        wbk_inst_s2.CSR_RDATA_RM_S2(CSR_RDATA_RM_S2);
        wbk_inst_s2.CSR_WENABLE_RM_S2(CSR_WENABLE_RM_S2);

        wbk_inst_s2.PC_MEM2WBK_RM_S2(PC_MEM2WBK_RM_S2);

        wbk_inst_s2.INTERRUPTION_SE_S2(INTERRUPTION_SE_S2);
        wbk_inst_s2.CURRENT_MODE_SM_S2(CURRENT_MODE_SM_S2);
        
        wbk_inst_s2.MULT_INST_RM_S2(MULT_INST_RM_S2);
        wbk_inst_s2.X2_RES_RX2(multiplier_out_sx2);

        wbk_inst_s2.CLK(CLK);
        wbk_inst_s2.RESET(RESET);

        //X1 - MULTIPLIER port map :

        x2_multiplier_inst.IN_RX1(multiplier_out_sx1);
        x2_multiplier_inst.SIGNED_OP_RX1(signed_op_rx1);
        x2_multiplier_inst.CARRY_RX1(carry_rx1);
        x2_multiplier_inst.X12X2_POP_SX2(x12x2_POP_SX2);
        x2_multiplier_inst.RES_RX2(multiplier_out_sx2);
        x2_multiplier_inst.X12X2_EMPTY_SX1(x12x2_EMPTY_SX1);
        x2_multiplier_inst.CLK(CLK);
        x2_multiplier_inst.RESET(RESET);

        csr_inst.CSR_WADR_SM_S1(CSR_WADR_SM_S1);
        csr_inst.CSR_WDATA_SM_S1(CSR_WDATA_SM_S1);
        csr_inst.CSR_ENABLE_BEFORE_FIFO_SM_S1(CSR_ENABLE_BEFORE_FIFO_SM_S1);

        csr_inst.CSR_RADR_SD_S1(CSR_RADR_SD_S1);
        csr_inst.CSR_RDATA_SC_S1(CSR_RDATA_SC_S1);

        csr_inst.EXCEPTION_SM_S1(EXCEPTION_SM_S1);
        csr_inst.MSTATUS_WDATA_RM_S1(MSTATUS_WDATA_RM_S1);
        csr_inst.MIP_WDATA_RM_S1(MIP_WDATA_RM_S1);
        csr_inst.MEPC_WDATA_RM_S1(MEPC_WDATA_RM_S1);
        csr_inst.MCAUSE_WDATA_SM_S1(MCAUSE_WDATA_SM_S1);

        csr_inst.MEPC_SC_S1(MEPC_SC_S1);
        csr_inst.MSTATUS_RC_S1(MSTATUS_RC_S1);
        csr_inst.MTVEC_VALUE_RC_S1(MTVEC_VALUE_RC_S1);
        csr_inst.MIP_VALUE_RC_S1(MIP_VALUE_RC_S1);
        csr_inst.MCAUSE_SC(MCAUSE_SC);
        csr_inst.MTVAL_WDATA_SM_S1(MTVAL_WDATA_SM_S1);

        csr_inst.CLK(CLK);
        csr_inst.RESET_N(RESET);
    }
};
