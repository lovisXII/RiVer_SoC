#include "CSR/csr.h"
#include "DEC/dec.h"
#include "EXE/exec.h"
#include "IFETCH/ifetch.h"
#include "MEM/mem.h"
#include "REG/reg.h"
#include "WBK/wbk.h"
#include "systemc.h"

SC_MODULE(core) {
    // Global Interface :

    sc_in_clk   CLK;
    sc_in<bool> RESET;

    // IFETCH-DEC interface
    // DEC2IF :

    sc_signal<bool>      DEC2IF_POP_SI;
    sc_signal<bool>      DEC2IF_EMPTY_SI;
    sc_signal<sc_bv<32>> PC_RD;
    sc_signal<bool>      EXCEPTION_RI;

    // IF2DEC :

    sc_signal<sc_bv<32>>   INSTR_RI;
    sc_signal<bool>        IF2DEC_EMPTY_SI;
    sc_signal<bool>        IF2DEC_POP_SD;
    sc_signal<bool>        IF2DEC_FLUSH_SD;
    sc_signal<sc_uint<32>> PC_IF2DEC_RI;
    // DEC-EXE interface

    sc_signal<sc_uint<32>> PC_DEC2EXE_RD;
    sc_signal<sc_uint<32>> OP1_RD;
    sc_signal<sc_uint<32>> OP2_RD;
    sc_signal<sc_uint<2>>  EXE_CMD_RD;
    sc_signal<bool>        NEG_OP2_RD;
    sc_signal<bool>        WB_RD;
    sc_signal<bool>        SELECT_SHIFT_RD;

    sc_signal<sc_uint<32>> MEM_DATA_RD;
    sc_signal<bool>        MEM_LOAD_RD;
    sc_signal<bool>        MEM_STORE_RD;
    sc_signal<bool>        MEM_SIGN_EXTEND_RD;
    sc_signal<sc_uint<2>>  MEM_SIZE_RD;
    sc_signal<bool>        SLT_RD;
    sc_signal<bool>        SLTU_RD;

    sc_signal<bool> DEC2EXE_POP_SE;
    sc_signal<bool> DEC2EXE_EMPTY_SD;

    sc_signal<bool>       BP_R1_VALID_RD;
    sc_signal<bool>       BP_R2_VALID_RD;
    sc_signal<sc_uint<6>> BP_RADR1_RD;
    sc_signal<sc_uint<6>> BP_RADR2_RD;
    sc_signal<bool>       BLOCK_BP_RD;

    sc_signal<bool>        CSR_WENABLE_RD;
    sc_signal<sc_uint<32>> CSR_RDATA_RD;
    sc_signal<sc_uint<12>> CSR_WADR_RD;
    sc_signal<bool>        INTERRUPTION_SE;
    sc_signal<bool>        EXCEPTION_RD;
    sc_signal<bool>        ECALL_I_RD;
    sc_signal<bool>        EBREAK_I_RD;
    sc_signal<bool>        ILLEGAL_INSTRUCTION_RD;  // accessing stuff in wrong mode
    sc_signal<bool>        ADRESS_MISSALIGNED_RD;   // branch offset is misaligned
    sc_signal<bool>        SYSCALL_U_MODE_RD;
    sc_signal<bool>        SYSCALL_M_MODE_RD;

    // DEC-CSR interface
    sc_signal<sc_uint<12>> CSR_RADR_SD;
    sc_signal<sc_uint<32>> CSR_RDATA_SC;

    // DEC-REG interface
    sc_signal<sc_uint<32>> RDATA1_SR;
    sc_signal<sc_uint<32>> RDATA2_SR;

    sc_signal<sc_uint<6>> RADR1_SD;
    sc_signal<sc_uint<6>> RADR2_SD;

    sc_signal<sc_uint<6>> EXE_DEST_SD;

    sc_signal<sc_uint<32>> READ_PC_SR;
    sc_signal<sc_uint<32>> WRITE_PC_SD;
    sc_signal<bool>        WRITE_PC_ENABLE_SD;

    // EXE-MEM interface
    sc_signal<sc_uint<32>> EXE_RES_RE;
    sc_signal<sc_uint<32>> MEM_DATA_RE;
    sc_signal<sc_uint<6>>  DEST_RE;
    sc_signal<sc_uint<2>>  MEM_SIZE_RE;
    sc_signal<sc_uint<32>> PC_EXE2MEM_RE;
    sc_signal<bool>        MEM_WB, MEM_SIGN_EXTEND_RE;
    sc_signal<bool>        MEM_LOAD_RE, MEM_STORE_RE;
    sc_signal<bool>        EXE2MEM_EMPTY_SE, EXE2MEM_POP_SM;

    sc_signal<bool>        CSR_WENABLE_RE;
    sc_signal<bool>        INTERRUPTION_SX;
    sc_signal<sc_uint<12>> CSR_WADR_RE;
    sc_signal<sc_uint<32>> CSR_RDATA_RE;

    sc_signal<bool> EXCEPTION_RE;
    sc_signal<bool> LOAD_ADRESS_MISSALIGNED_RE;   // adress from store/load isn't aligned
    sc_signal<bool> INSTRUCTION_ACCESS_FAULT_RE;  // trying to access memory in wrong mode
    sc_signal<bool> ECALL_I_RE;
    sc_signal<bool> EBREAK_I_RE;
    sc_signal<bool> ILLEGAL_INSTRUCTION_RE;  // accessing stuff in wrong mode
    sc_signal<bool> ADRESS_MISSALIGNED_RE;   // branch offset is misaligned
    sc_signal<bool> SYSCALL_U_MODE_RE;
    sc_signal<bool> SYSCALL_M_MODE_RE;

    // MEM-WBK interface
    sc_signal<sc_uint<32>> MEM_RES_RM;
    sc_signal<sc_uint<6>>  DEST_RM;
    sc_signal<bool>        WB_RM;
    sc_signal<bool>        WBK_MEM_SIGN_EXTEND;
    sc_signal<bool>        MEM2WBK_EMPTY_SM;
    sc_signal<bool>        MEM2WBK_POP_SW;
    sc_signal<bool>        WBK_MEM_LOAD;
    sc_signal<sc_uint<32>> PC_MEM2WBK_RM;
    sc_signal<bool>        CSR_WENABLE_RM;
    sc_signal<sc_uint<32>> CSR_RDATA_RM;

    // MEM-CSR interface

    sc_signal<sc_uint<12>> CSR_WADR_SM;
    sc_signal<sc_uint<32>> CSR_WDATA_SM;

    sc_signal<sc_uint<32>> MSTATUS_WDATA_RM;
    sc_signal<sc_uint<32>> MIP_WDATA_RM;
    sc_signal<sc_uint<32>> MEPC_WDATA_RM;
    sc_signal<sc_uint<32>> MCAUSE_WDATA_RM;
    sc_signal<sc_uint<32>> MTVEC_VALUE_RC;
    sc_signal<sc_uint<32>> MIP_VALUE_RC;
    sc_signal<bool>        CSR_ENABLE_BEFORE_FIFO_SM;

    // MEM - Pipeline :

    sc_signal<bool>        EXCEPTION_RM;
    sc_signal<bool>        BUS_ERROR_SX;
    // WBK-REG interface

    sc_signal<sc_uint<6>>  WADR_SW;
    sc_signal<sc_uint<32>> WDATA_SW;
    sc_signal<bool>        WENABLE_SW;

    // Mcache interface
    sc_out<sc_uint<32>> MCACHE_ADR_SM;
    sc_out<sc_uint<32>> MCACHE_DATA_SM;
    sc_out<bool>        MCACHE_ADR_VALID_SM, MCACHE_STORE_SM, MCACHE_LOAD_SM;
    sc_out<sc_uint<2>>  MCACHE_MEM_SIZE_SM;

    sc_in<sc_uint<32>> MCACHE_RESULT_SM;
    sc_in<bool>        MCACHE_STALL_SM;

    // Icache interface
    sc_out<sc_uint<32>> ADR_SI;
    sc_out<bool>        ADR_VALID_SI;

    sc_in<sc_bv<32>> IC_INST_SI;
    sc_in<bool>      IC_STALL_SI;

    // Debug
    sc_in<sc_uint<32>>  PC_INIT;
    sc_out<sc_uint<32>> DEBUG_PC_READ;

    // Stage instanciation
    decod  dec_inst;
    exec   exec_inst;
    ifetch ifetch_inst;
    mem    mem_inst;
    reg    reg_inst;
    wbk    wbk_inst;
    csr    csr_inst;

    void core_method();

    void trace(sc_trace_file * tf);
    SC_CTOR(core)
        : dec_inst("decod"),
          exec_inst("exec"),
          ifetch_inst("ifetch"),
          mem_inst("mem"),
          reg_inst("reg"),
          wbk_inst("wbk"),
          csr_inst("csr") {
        SC_METHOD(core_method);
        sensitive << READ_PC_SR;

        ifetch_inst.DEC2IF_POP_SI(DEC2IF_POP_SI);
        ifetch_inst.DEC2IF_EMPTY_SI(DEC2IF_EMPTY_SI);
        ifetch_inst.PC_RD(PC_RD);
        ifetch_inst.INSTR_RI(INSTR_RI);
        ifetch_inst.IF2DEC_EMPTY_SI(IF2DEC_EMPTY_SI);
        ifetch_inst.IF2DEC_POP_SD(IF2DEC_POP_SD);
        ifetch_inst.IF2DEC_FLUSH_SD(IF2DEC_FLUSH_SD);

        ifetch_inst.ADR_SI(ADR_SI);
        ifetch_inst.ADR_VALID_SI(ADR_VALID_SI);

        ifetch_inst.IC_INST_SI(IC_INST_SI);
        ifetch_inst.IC_STALL_SI(IC_STALL_SI);

        ifetch_inst.PC_IF2DEC_RI(PC_IF2DEC_RI);
        ifetch_inst.INTERRUPTION_SE(INTERRUPTION_SE);
        ifetch_inst.EXCEPTION_RI(EXCEPTION_RI);
        ifetch_inst.EXCEPTION_RM(EXCEPTION_RM);

        ifetch_inst.CLK(CLK);
        ifetch_inst.RESET(RESET);

        dec_inst.DEC2IF_POP_SI(DEC2IF_POP_SI);
        dec_inst.DEC2IF_EMPTY_SD(DEC2IF_EMPTY_SI);
        dec_inst.PC_RD(PC_RD);
        dec_inst.PC_DEC2EXE_RD(PC_DEC2EXE_RD);
        dec_inst.PC_IF2DEC_RI(PC_IF2DEC_RI);
        dec_inst.INSTR_RI(INSTR_RI);
        dec_inst.IF2DEC_EMPTY_SI(IF2DEC_EMPTY_SI);
        dec_inst.IF2DEC_POP_SD(IF2DEC_POP_SD);
        dec_inst.IF2DEC_FLUSH_SD(IF2DEC_FLUSH_SD);

        dec_inst.OP1_RD(OP1_RD);
        dec_inst.OP2_RD(OP2_RD);
        dec_inst.EXE_CMD_RD(EXE_CMD_RD);
        dec_inst.NEG_OP2_RD(NEG_OP2_RD);
        dec_inst.WB_RD(WB_RD);
        dec_inst.SELECT_SHIFT_RD(SELECT_SHIFT_RD);
        dec_inst.SLT_RD(SLT_RD);
        dec_inst.SLTU_RD(SLTU_RD);

        dec_inst.MEM_DATA_RD(MEM_DATA_RD);
        dec_inst.MEM_LOAD_RD(MEM_LOAD_RD);
        dec_inst.MEM_STORE_RD(MEM_STORE_RD);
        dec_inst.MEM_SIGN_EXTEND_RD(MEM_SIGN_EXTEND_RD);
        dec_inst.MEM_SIZE_RD(MEM_SIZE_RD);

        dec_inst.DEC2EXE_POP_SE(DEC2EXE_POP_SE);
        dec_inst.DEC2EXE_EMPTY_SD(DEC2EXE_EMPTY_SD);

        dec_inst.RDATA1_SR(RDATA1_SR);
        dec_inst.RDATA2_SR(RDATA2_SR);

        dec_inst.RADR1_SD(RADR1_SD);
        dec_inst.RADR2_SD(RADR2_SD);

        dec_inst.EXE_DEST_SD(EXE_DEST_SD);

        dec_inst.READ_PC_SR(READ_PC_SR);
        dec_inst.WRITE_PC_SD(WRITE_PC_SD);
        dec_inst.WRITE_PC_ENABLE_SD(WRITE_PC_ENABLE_SD);

        dec_inst.BP_DEST_RE(DEST_RE);
        dec_inst.BP_EXE_RES_RE(EXE_RES_RE);
        dec_inst.BP_DEST_RM(DEST_RM);
        dec_inst.BP_MEM_RES_RM(MEM_RES_RM);
        dec_inst.BP_EXE2MEM_EMPTY_SE(EXE2MEM_EMPTY_SE);

        dec_inst.BP_R1_VALID_RD(BP_R1_VALID_RD);
        dec_inst.BP_R2_VALID_RD(BP_R2_VALID_RD);
        dec_inst.BP_RADR1_RD(BP_RADR1_RD);
        dec_inst.BP_RADR2_RD(BP_RADR2_RD);
        dec_inst.BP_MEM_LOAD_RE(MEM_LOAD_RE);

        dec_inst.CSR_WENABLE_RE(CSR_WENABLE_RE);
        dec_inst.CSR_RDATA_RE(CSR_RDATA_RE);
        dec_inst.CSR_WENABLE_RM(CSR_WENABLE_RM);
        dec_inst.CSR_RDATA_RM(CSR_RDATA_RM);

        dec_inst.CSR_WENABLE_RD(CSR_WENABLE_RD);
        dec_inst.CSR_WADR_RD(CSR_WADR_RD);
        dec_inst.CSR_RADR_SD(CSR_RADR_SD);
        dec_inst.CSR_RDATA_SC(CSR_RDATA_SC);
        dec_inst.CSR_RDATA_RD(CSR_RDATA_RD);
        dec_inst.INTERRUPTION_SE(INTERRUPTION_SE);
        dec_inst.EXCEPTION_RI(EXCEPTION_RI);
        dec_inst.EXCEPTION_RD(EXCEPTION_RD);
        dec_inst.ECALL_I_RD(ECALL_I_RD);
        dec_inst.EBREAK_I_RD(EBREAK_I_RD);
        dec_inst.ILLEGAL_INSTRUCTION_RD(ILLEGAL_INSTRUCTION_RD);  // accessing stuff in wrong mode
        dec_inst.ADRESS_MISSALIGNED_RD(ADRESS_MISSALIGNED_RD);    // branch offset is misaligned
        dec_inst.SYSCALL_U_MODE_RD(SYSCALL_U_MODE_RD);
        dec_inst.SYSCALL_M_MODE_RD(SYSCALL_M_MODE_RD);
        dec_inst.EXCEPTION_RM(EXCEPTION_RM);
        dec_inst.MTVEC_VALUE_RC(MTVEC_VALUE_RC);
        dec_inst.BLOCK_BP_RD(BLOCK_BP_RD);

        dec_inst.CLK(CLK);
        dec_inst.RESET_N(RESET);

        exec_inst.RADR1_RD(BP_RADR1_RD);
        exec_inst.RADR2_RD(BP_RADR2_RD);
        exec_inst.OP1_VALID_RD(BP_R1_VALID_RD);
        exec_inst.OP2_VALID_RD(BP_R2_VALID_RD);
        exec_inst.MEM_DEST_RM(DEST_RM);
        exec_inst.MEM_RES_RM(MEM_RES_RM);
        exec_inst.OP1_RD(OP1_RD);
        exec_inst.OP2_RD(OP2_RD);
        exec_inst.CMD_RD(EXE_CMD_RD);
        exec_inst.DEST_RD(EXE_DEST_SD);
        exec_inst.NEG_OP2_RD(NEG_OP2_RD);
        exec_inst.WB_RD(WB_RD);
        exec_inst.SELECT_SHIFT_RD(SELECT_SHIFT_RD);

        exec_inst.PC_DEC2EXE_RD(PC_DEC2EXE_RD);
        exec_inst.PC_EXE2MEM_RE(PC_EXE2MEM_RE);

        exec_inst.MEM_DATA_RD(MEM_DATA_RD);
        exec_inst.MEM_LOAD_RD(MEM_LOAD_RD);
        exec_inst.MEM_STORE_RD(MEM_STORE_RD);
        exec_inst.MEM_SIGN_EXTEND_RD(MEM_SIGN_EXTEND_RD);
        exec_inst.MEM_SIZE_RD(MEM_SIZE_RD);
        exec_inst.SLT_RD(SLT_RD);
        exec_inst.SLTU_RD(SLTU_RD);

        exec_inst.DEC2EXE_POP_SE(DEC2EXE_POP_SE);
        exec_inst.DEC2EXE_EMPTY_SD(DEC2EXE_EMPTY_SD);

        exec_inst.EXE_RES_RE(EXE_RES_RE);
        exec_inst.MEM_DATA_RE(MEM_DATA_RE);
        exec_inst.DEST_RE(DEST_RE);
        exec_inst.MEM_SIZE_RE(MEM_SIZE_RE);

        exec_inst.WB_RE(MEM_WB);
        exec_inst.MEM_SIGN_EXTEND_RE(MEM_SIGN_EXTEND_RE);
        exec_inst.MEM_LOAD_RE(MEM_LOAD_RE);
        exec_inst.MEM_STORE_RE(MEM_STORE_RE);
        exec_inst.EXE2MEM_EMPTY_SE(EXE2MEM_EMPTY_SE);
        exec_inst.EXE2MEM_POP_SM(EXE2MEM_POP_SM);

        exec_inst.CSR_WENABLE_RM(CSR_WENABLE_RM);
        exec_inst.CSR_RDATA_RM(CSR_RDATA_RM);

        exec_inst.INTERRUPTION_SE(INTERRUPTION_SE);
        exec_inst.INTERRUPTION_SX(INTERRUPTION_SX);
        exec_inst.EXCEPTION_RD(EXCEPTION_RD);
        exec_inst.CSR_WENABLE_RD(CSR_WENABLE_RD);
        exec_inst.CSR_WENABLE_RE(CSR_WENABLE_RE);
        exec_inst.CSR_WADR_RD(CSR_WADR_RD);
        exec_inst.CSR_WADR_RE(CSR_WADR_RE);
        exec_inst.CSR_RDATA_RE(CSR_RDATA_RE);
        exec_inst.CSR_RDATA_RD(CSR_RDATA_RD);
        exec_inst.ECALL_I_RD(ECALL_I_RD);
        exec_inst.EBREAK_I_RD(EBREAK_I_RD);
        exec_inst.ILLEGAL_INSTRUCTION_RD(ILLEGAL_INSTRUCTION_RD);  // accessing stuff in wrong mode
        exec_inst.ADRESS_MISSALIGNED_RD(ADRESS_MISSALIGNED_RD);    // branch offset is misaligned
        exec_inst.SYSCALL_U_MODE_RD(SYSCALL_U_MODE_RD);
        exec_inst.SYSCALL_M_MODE_RD(SYSCALL_M_MODE_RD);

        exec_inst.EXCEPTION_RE(EXCEPTION_RE);
        exec_inst.LOAD_ADRESS_MISSALIGNED_RE(LOAD_ADRESS_MISSALIGNED_RE);
        exec_inst.INSTRUCTION_ACCESS_FAULT_RE(INSTRUCTION_ACCESS_FAULT_RE);
        exec_inst.ECALL_I_RE(ECALL_I_RE);
        exec_inst.EBREAK_I_RE(EBREAK_I_RE);
        exec_inst.ILLEGAL_INSTRUCTION_RE(ILLEGAL_INSTRUCTION_RE);
        exec_inst.ADRESS_MISSALIGNED_RE(ADRESS_MISSALIGNED_RE);
        exec_inst.SYSCALL_U_MODE_RE(SYSCALL_U_MODE_RE);
        exec_inst.SYSCALL_M_MODE_RE(SYSCALL_M_MODE_RE);
        exec_inst.EXCEPTION_RM(EXCEPTION_RM);
        exec_inst.BLOCK_BP_RD(BLOCK_BP_RD);

        exec_inst.CLK(CLK);
        exec_inst.RESET(RESET);

        mem_inst.EXE_RES_RE(EXE_RES_RE);
        mem_inst.MEM_DATA_RE(MEM_DATA_RE);
        mem_inst.DEST_RE(DEST_RE);
        mem_inst.MEM_SIZE_RE(MEM_SIZE_RE);

        mem_inst.WB_RE(MEM_WB);
        mem_inst.SIGN_EXTEND_RE(MEM_SIGN_EXTEND_RE);
        mem_inst.LOAD_RE(MEM_LOAD_RE);
        mem_inst.STORE_RE(MEM_STORE_RE);
        mem_inst.EXE2MEM_EMPTY_SE(EXE2MEM_EMPTY_SE);
        mem_inst.EXE2MEM_POP_SM(EXE2MEM_POP_SM);

        mem_inst.MEM_RES_RM(MEM_RES_RM);
        mem_inst.DEST_RM(DEST_RM);
        mem_inst.WB_RM(WB_RM);
        mem_inst.MEM2WBK_EMPTY_SM(MEM2WBK_EMPTY_SM);
        mem_inst.MEM2WBK_POP_SW(MEM2WBK_POP_SW);

        mem_inst.MCACHE_ADR_SM(MCACHE_ADR_SM);
        mem_inst.MCACHE_DATA_SM(MCACHE_DATA_SM);
        mem_inst.MCACHE_ADR_VALID_SM(MCACHE_ADR_VALID_SM);
        mem_inst.MCACHE_STORE_SM(MCACHE_STORE_SM);
        mem_inst.MCACHE_LOAD_SM(MCACHE_LOAD_SM);
        mem_inst.MCACHE_RESULT_SM(MCACHE_RESULT_SM);
        mem_inst.MCACHE_STALL_SM(MCACHE_STALL_SM);
        mem_inst.MCACHE_MEM_SIZE_SM(MCACHE_MEM_SIZE_SM);

        mem_inst.PC_EXE2MEM_RE(PC_EXE2MEM_RE);
        mem_inst.PC_MEM2WBK_RM(PC_MEM2WBK_RM);

        mem_inst.INTERRUPTION_SE(INTERRUPTION_SE);
        mem_inst.CSR_WADR_SE(CSR_WADR_RE);
        mem_inst.CSR_WADR_SM(CSR_WADR_SM);
        mem_inst.CSR_WENABLE_RE(CSR_WENABLE_RE);
        mem_inst.CSR_WENABLE_RM(CSR_WENABLE_RM);
        mem_inst.CSR_WDATA_SM(CSR_WDATA_SM);
        mem_inst.CSR_RDATA_RM(CSR_RDATA_RM);
        mem_inst.CSR_RDATA_RE(CSR_RDATA_RE);

        mem_inst.EXCEPTION_RE(EXCEPTION_RE);
        mem_inst.LOAD_ADRESS_MISSALIGNED_RE(LOAD_ADRESS_MISSALIGNED_RE);
        mem_inst.INSTRUCTION_ACCESS_FAULT_RE(INSTRUCTION_ACCESS_FAULT_RE);
        mem_inst.ECALL_I_RE(ECALL_I_RE);
        mem_inst.EBREAK_I_RE(EBREAK_I_RE);
        mem_inst.ILLEGAL_INSTRUCTION_RE(ILLEGAL_INSTRUCTION_RE);
        mem_inst.ADRESS_MISSALIGNED_RE(ADRESS_MISSALIGNED_RE);
        mem_inst.SYSCALL_U_MODE_RE(SYSCALL_U_MODE_RE);
        mem_inst.SYSCALL_M_MODE_RE(SYSCALL_M_MODE_RE);

        mem_inst.BUS_ERROR_SX(BUS_ERROR_SX);

        mem_inst.EXCEPTION_RM(EXCEPTION_RM);
        mem_inst.MSTATUS_WDATA_RM(MSTATUS_WDATA_RM);
        mem_inst.MIP_WDATA_RM(MIP_WDATA_RM);
        mem_inst.MEPC_WDATA_RM(MEPC_WDATA_RM);
        mem_inst.MCAUSE_WDATA_RM(MCAUSE_WDATA_RM);
        mem_inst.MIP_VALUE_RC(MIP_VALUE_RC);

        mem_inst.CSR_ENABLE_BEFORE_FIFO_SM(CSR_ENABLE_BEFORE_FIFO_SM);

        mem_inst.CLK(CLK);
        mem_inst.RESET(RESET);

        reg_inst.RADR1_SD(RADR1_SD);
        reg_inst.RADR2_SD(RADR2_SD);
        reg_inst.RDATA1_SR(RDATA1_SR);
        reg_inst.RDATA2_SR(RDATA2_SR);

        reg_inst.WADR_SW(WADR_SW);
        reg_inst.WENABLE_SW(WENABLE_SW);
        reg_inst.WDATA_SW(WDATA_SW);

        reg_inst.READ_PC_SR(READ_PC_SR);
        reg_inst.WRITE_PC_SD(WRITE_PC_SD);
        reg_inst.WRITE_PC_ENABLE_SD(WRITE_PC_ENABLE_SD);
        reg_inst.PC_INIT(PC_INIT);

        reg_inst.CLK(CLK);
        reg_inst.RESET_N(RESET);

        wbk_inst.MEM_RES_RM(MEM_RES_RM);
        wbk_inst.DEST_RM(DEST_RM);
        wbk_inst.WB_RM(WB_RM);
        wbk_inst.MEM2WBK_EMPTY_SM(MEM2WBK_EMPTY_SM);
        wbk_inst.MEM2WBK_POP_SW(MEM2WBK_POP_SW);

        wbk_inst.WADR_SW(WADR_SW);
        wbk_inst.WDATA_SW(WDATA_SW);
        wbk_inst.WENABLE_SW(WENABLE_SW);
        wbk_inst.CSR_RDATA_RM(CSR_RDATA_RM);
        wbk_inst.CSR_WENABLE_RM(CSR_WENABLE_RM);

        wbk_inst.PC_MEM2WBK_RM(PC_MEM2WBK_RM);

        wbk_inst.INTERRUPTION_SE(INTERRUPTION_SE);

        wbk_inst.CLK(CLK);
        wbk_inst.RESET(RESET);

        csr_inst.CSR_WADR_SM(CSR_WADR_SM);
        csr_inst.CSR_WDATA_SM(CSR_WDATA_SM);
        csr_inst.CSR_ENABLE_BEFORE_FIFO_SM(CSR_ENABLE_BEFORE_FIFO_SM);

        csr_inst.CSR_RADR_SD(CSR_RADR_SD);
        csr_inst.CSR_RDATA_SC(CSR_RDATA_SC);

        csr_inst.EXCEPTION_RM(EXCEPTION_RM);
        csr_inst.MSTATUS_WDATA_RM(MSTATUS_WDATA_RM);
        csr_inst.MIP_WDATA_RM(MIP_WDATA_RM);
        csr_inst.MEPC_WDATA_RM(MEPC_WDATA_RM);
        csr_inst.MCAUSE_WDATA_RM(MCAUSE_WDATA_RM);
        csr_inst.MTVEC_VALUE_RC(MTVEC_VALUE_RC);
        csr_inst.MIP_VALUE_RC(MIP_VALUE_RC);

        csr_inst.CLK(CLK);
        csr_inst.RESET_N(RESET);
    }
};