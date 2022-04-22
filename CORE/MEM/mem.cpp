#include "mem.h"

void mem::mem2wbk_concat() {
    sc_bv<mem2wbk_size> ff_din;

    ff_din.range(31, 0)    = data_sm.read();
    ff_din.range(36, 32)   = DEST_RE.read();
    ff_din[37]             = wb_sm.read();
    ff_din.range(73, 38)   = PC_EXE2MEM_RE.read();
    ff_din[74]             = CSR_WENABLE_RE.read();
    ff_din.range(106, 75)  = CSR_RDATA_RE.read();
    ff_din[107]            = exception_sm.read();

    mem2wbk_din_sm.write(ff_din);
}
void mem::mem2wbk_unconcat() {
    sc_bv<mem2wbk_size> ff_dout = mem2wbk_dout_sm.read();

    MEM_RES_RM.write((sc_bv_base)ff_dout.range(31, 0));
    DEST_RM.write((sc_bv_base)ff_dout.range(36, 32));
    WB_RM.write((bool)ff_dout[37]);
    PC_MEM2WBK_RM.write((sc_bv_base)ff_dout.range(73, 38));
    CSR_WENABLE_RM.write((bool)ff_dout[74]);
    CSR_RDATA_RM.write((sc_bv_base)ff_dout.range(106, 75));
    EXCEPTION_RM.write((bool)ff_dout[107]);
}

void mem::fifo_gestion() {
    bool stall = MCACHE_STALL_SM.read() || mem2wbk_full_sm.read() || EXE2MEM_EMPTY_SE.read();
    mem2wbk_push_sm.write(!stall);
    EXE2MEM_POP_SM.write(!stall);
}
void mem::mem_preprocess() {
    // Whether the register bank shoudl actually write the data
    wb_sm.write(WB_RE.read() || LOAD_RE.read());

    // The data sent to the actual memory
    MCACHE_DATA_SM.write(MEM_DATA_RE.read());
    MCACHE_ADR_SM.write(EXE_RES_RE);
    MCACHE_LOAD_SM.write(LOAD_RE.read());
    MCACHE_STORE_SM.write(STORE_RE.read());
    MCACHE_ADR_VALID_SM.write(!EXE2MEM_EMPTY_SE.read());
    MCACHE_MEM_SIZE_SM.write(MEM_SIZE_RE.read());
}

void mem::sign_extend() {
    // sign extend
    int         size        = MEM_SIZE_RE.read();
    bool        sign_extend = SIGN_EXTEND_RE.read();
    sc_uint<32> din         = MCACHE_RESULT_SM.read();
    sc_uint<32> dout;
    int         range_start;  // The beginning of the range of din that should actually
                              // be written to the register
    switch (size) {
        case 2: range_start = 7; break;
        case 1: range_start = 15; break;
        case 0: range_start = 31; break;
        default: range_start = 31; break;
    }

    if (sign_extend && din(range_start, range_start)) {
        dout = 0xFFFFFFFF;
    } else {
        dout = 0;
    }
    dout.range(range_start, 0) = din.range(range_start, 0);

    // Choose which data should be written to the register
    if (LOAD_RE.read()) {
        data_sm.write(dout);
    } else {
        data_sm.write(EXE_RES_RE.read());
    }
}

void mem::csr_exception() {
    exception_sm = EXCEPTION_RE.read() || BUS_ERROR_SX.read();
    if ( ! exception_sm) {
        if (CSR_WENABLE_RE.read()) {
            CSR_WADR_SM.write(CSR_WADR_SE.read());
            CSR_WDATA_SM.write(EXE_RES_RE.read());
            CSR_ENABLE_BEFORE_FIFO_SM.write(true);
        } else {
            CSR_WADR_SM.write(0);
            CSR_WDATA_SM.write(0);
            CSR_ENABLE_BEFORE_FIFO_SM.write(0);
        }
    } else {  // Affectation of the cause
        if (LOAD_ADRESS_MISSALIGNED_RE) {
            MSTATUS_WDATA_RM.write(0x1800);  // MPP set to 11
            MIP_WDATA_RM.write(MIP_VALUE_RC.read() && 0xFFFFFFEF);
            MEPC_WDATA_RM.write(PC_EXE2MEM_RE.read());
            MCAUSE_WDATA_RM.write(4);
        }
        if (INSTRUCTION_ACCESS_FAULT_RE) {
            MSTATUS_WDATA_RM.write(0x1800);  // MPP set to 11
            MIP_WDATA_RM.write(MIP_VALUE_RC.read() && 0xFFFFFFFD);
            MEPC_WDATA_RM.write(PC_EXE2MEM_RE.read());
            MCAUSE_WDATA_RM.write(1);
        }
        if (ECALL_I_RE) {
            // MSTATUS_WDATA_RM.write(0x1800); // MPP set to 11
            // MIP_WDATA_RM.write(MIP_VALUE_RC.read() && 0xFFFFFFFE);
            // MEPC_WDATA_RM.write(PC_EXE2MEM_RE.read());
            // MCAUSE_WDATA_RM.write(0);
        }
        if (EBREAK_I_RE) {
            // MSTATUS_WDATA_RM.write(0x1800);// MPP set to 11
            // MIP_WDATA_RM.write(0x1800);
            // MEPC_WDATA_RM.write(PC_EXE2MEM_RE.read());
            // MCAUSE_WDATA_RM.write(0);
        }
        if (ILLEGAL_INSTRUCTION_RE) {
            MSTATUS_WDATA_RM.write(0x1800);  // MPP set to 11
            MIP_WDATA_RM.write(MIP_VALUE_RC.read() && 0xFFFFFFFB);
            MEPC_WDATA_RM.write(PC_EXE2MEM_RE.read());
            MCAUSE_WDATA_RM.write(2);
        }
        if (ADRESS_MISSALIGNED_RE) {
            MSTATUS_WDATA_RM.write(0x1800);  // MPP set to 11
            MIP_WDATA_RM.write(MIP_VALUE_RC.read() && 0xFFFFFFFE);
            MEPC_WDATA_RM.write(PC_EXE2MEM_RE.read());
            MCAUSE_WDATA_RM.write(0);
        }
        if (SYSCALL_U_MODE_RE) {
            MSTATUS_WDATA_RM.write(0x1800);  // MPP set to 11
            MIP_WDATA_RM.write(MIP_VALUE_RC.read() && 0xFFFFFEFF);
            MEPC_WDATA_RM.write(PC_EXE2MEM_RE.read());
            MCAUSE_WDATA_RM.write(8);
        }
        if (SYSCALL_M_MODE_RE) {
            MSTATUS_WDATA_RM.write(0x1800);  // MPP set to 11
            MIP_WDATA_RM.write(MIP_VALUE_RC.read() && 0xFFFFFBFF);
            MEPC_WDATA_RM.write(PC_EXE2MEM_RE.read());
            MCAUSE_WDATA_RM.write(11);
        }
        if (BUS_ERROR_SX) {  // load access fault
            MSTATUS_WDATA_RM.write(0x1800);
            MIP_WDATA_RM.write(MIP_VALUE_RC.read() && 0xFFFFFFEF);
            MEPC_WDATA_RM.write(PC_EXE2MEM_RE.read());
            MCAUSE_WDATA_RM.write(5);
        }
    }
}

void mem::trace(sc_trace_file* tf) {
    sc_trace(tf, MCACHE_ADR_SM, GET_NAME(MCACHE_ADR_SM));  // adress in memory
    sc_trace(tf, MCACHE_DATA_SM, GET_NAME(MCACHE_DATA_SM));
    sc_trace(tf, MCACHE_ADR_VALID_SM, GET_NAME(MCACHE_ADR_VALID_SM));
    sc_trace(tf, MCACHE_STORE_SM, GET_NAME(MCACHE_STORE_SM));
    sc_trace(tf, MCACHE_LOAD_SM, GET_NAME(MCACHE_LOAD_SM));
    sc_trace(tf, MCACHE_RESULT_SM, GET_NAME(MCACHE_RESULT_SM));
    sc_trace(tf, MCACHE_STALL_SM, GET_NAME(MCACHE_STALL_SM));
    sc_trace(tf, EXE_RES_RE, GET_NAME(EXE_RES_RE));
    sc_trace(tf, MEM_DATA_RE, GET_NAME(MEM_DATA_RE));
    sc_trace(tf, DEST_RE, GET_NAME(DEST_RE));
    sc_trace(tf, MEM_SIZE_RE, GET_NAME(MEM_SIZE_RE));
    sc_trace(tf, WB_RE, GET_NAME(WB_RE));
    sc_trace(tf, SIGN_EXTEND_RE, GET_NAME(SIGN_EXTEND_RE));
    sc_trace(tf, LOAD_RE, GET_NAME(LOAD_RE));
    sc_trace(tf, STORE_RE, GET_NAME(STORE_RE));
    sc_trace(tf, EXE2MEM_EMPTY_SE, GET_NAME(EXE2MEM_EMPTY_SE));
    sc_trace(tf, EXE2MEM_POP_SM, GET_NAME(EXE2MEM_POP_SM));
    sc_trace(tf, MEM2WBK_POP_SW, GET_NAME(MEM2WBK_POP_SW));
    sc_trace(tf, mem2wbk_push_sm, GET_NAME(mem2wbk_push_sm));
    sc_trace(tf, mem2wbk_full_sm, GET_NAME(mem2wbk_full_sm));
    sc_trace(tf, MEM2WBK_EMPTY_SM, GET_NAME(MEM2WBK_EMPTY_SM));
    sc_trace(tf, MEM_RES_RM, GET_NAME(MEM_RES_RM));
    sc_trace(tf, DEST_RM, GET_NAME(DEST_RM));
    sc_trace(tf, WB_RM, GET_NAME(WB_RM));
    sc_trace(tf, mem2wbk_din_sm, GET_NAME(mem2wbk_din_sm));
    sc_trace(tf, mem2wbk_dout_sm, GET_NAME(mem2wbk_dout_sm));
    sc_trace(tf, data_sm, GET_NAME(data_sm));
    sc_trace(tf, wb_sm, GET_NAME(wb_sm));
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET, GET_NAME(RESET));
    sc_trace(tf, CSR_WENABLE_RE, GET_NAME(CSR_WENABLE_RE));
    sc_trace(tf, CSR_WADR_SE, GET_NAME(CSR_WADR_SE));
    sc_trace(tf, CSR_RDATA_RE, GET_NAME(CSR_RDATA_RE));
    sc_trace(tf, CSR_WENABLE_RM, GET_NAME(CSR_WENABLE_RM));
    sc_trace(tf, CSR_RDATA_RM, GET_NAME(CSR_RDATA_RM));
    sc_trace(tf, INTERRUPTION_SE, GET_NAME(INTERRUPTION_SE));
    sc_trace(tf, CSR_WADR_SM, GET_NAME(CSR_WADR_SM));
    sc_trace(tf, CSR_WDATA_SM, GET_NAME(CSR_WDATA_SM));
    sc_trace(tf, EXCEPTION_RE, GET_NAME(EXCEPTION_RE));
    sc_trace(
        tf, LOAD_ADRESS_MISSALIGNED_RE, GET_NAME(LOAD_ADRESS_MISSALIGNED_RE));  // adress from store/load isn't aligned
    sc_trace(tf,
             INSTRUCTION_ACCESS_FAULT_RE,
             GET_NAME(INSTRUCTION_ACCESS_FAULT_RE));  // trying to access memory in wrong mode
    sc_trace(tf, ECALL_I_RE, GET_NAME(ECALL_I_RE));
    sc_trace(tf, EBREAK_I_RE, GET_NAME(EBREAK_I_RE));
    sc_trace(tf, ILLEGAL_INSTRUCTION_RE, GET_NAME(ILLEGAL_INSTRUCTION_RE));  // accessing stuff in wrong mode
    sc_trace(tf, ADRESS_MISSALIGNED_RE, GET_NAME(ADRESS_MISSALIGNED_RE));    // branch offset is misaligned
    sc_trace(tf, SYSCALL_U_MODE_RE, GET_NAME(SYSCALL_U_MODE_RE));
    sc_trace(tf, SYSCALL_M_MODE_RE, GET_NAME(SYSCALL_M_MODE_RE));
    sc_trace(tf, BUS_ERROR_SX, GET_NAME(BUS_ERROR_SX));
    sc_trace(tf, EXCEPTION_RM, GET_NAME(EXCEPTION_RM));
    sc_trace(tf, MSTATUS_WDATA_RM, GET_NAME(MSTATUS_WDATA_RM));
    sc_trace(tf, MIP_WDATA_RM, GET_NAME(MIP_WDATA_RM));
    sc_trace(tf, MEPC_WDATA_RM, GET_NAME(MEPC_WDATA_RM));
    sc_trace(tf, MCAUSE_WDATA_RM, GET_NAME(MCAUSE_WDATA_RM));
    sc_trace(tf, MIP_VALUE_RC, GET_NAME(MIP_VALUE_RC));
    sc_trace(tf, CSR_ENABLE_BEFORE_FIFO_SM, GET_NAME(CSR_ENABLE_BEFORE_FIFO_SM));
    sc_trace(tf, exception_sm, GET_NAME(exception_sm));
    sc_trace(tf, MCACHE_MEM_SIZE_SM, GET_NAME(MCACHE_MEM_SIZE_SM));
    fifo_inst.trace(tf);
}