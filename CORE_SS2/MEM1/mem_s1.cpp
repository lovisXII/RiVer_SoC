#include "mem_s1.h"

/*
3 memory access are possible :
*  access in byte
*  access in half word
*  access in word
In every situation the adress must be aligned with the size of the access type. Meaning :
* byte      : adress must be aligned on a byte (01)
* half-word : adress must be aligned on a half-word (10)
* word      : adress must be aligned on a word (00)

If a access is made in a specific type and the adress doesnt respect the alignement it must raise an exception.

For load access :
In all the case we send a word-align adress to the ram and we do the masking
operation in mem

For store access :
* byte      : we store the least significant byte of the register in the corresponding part of the adress
* half-word : same thing but with a half word
* word      : the word is directly store inside the memory case
*/

/*
The memory receive a 32 bits adress and ignore the least 2 significant bits.
Meaning it deals with a 30 bits adress.
The Memory must be split in 4 distinct parts, each of these parts
dealing with a byte of a word-align adress.

For example :
li x1,0x1000F101
li x2,12
sb x2,0(x1)

The ram receive the adress 0x1000F101 and ignore the least 2 bits,
so it receive 0x1000F100.
The ram accesses this adress and is informed that a byte access is made
with the least 2 significant bits equal to 01
So it will store the data in the corresponding case adress with is the 2nd one.

*/
void mem_s1::mem2wbk_concat() {
    sc_bv<mem2wbk_size> ff_din;

    ff_din.range(31, 0)   = data_sm.read();
    ff_din.range(36, 32)  = DEST_RE_S1.read();
    ff_din[37]            = wb_sm.read();
    ff_din.range(73, 38)  = PC_EXE2MEM_RE_S1.read();
    ff_din[74]            = CSR_WENABLE_RE_S1.read();
    ff_din.range(106, 75) = CSR_RDATA_RE_S1.read();
    ff_din[107]           = MULT_INST_RE_S1.read();
    mem2wbk_din_sm.write(ff_din);
}
void mem_s1::mem2wbk_unconcat() {
    sc_bv<mem2wbk_size> ff_dout = mem2wbk_dout_sm.read();

    MEM_RES_RM_S1.write((sc_bv_base)ff_dout.range(31, 0));
    DEST_RM_S1.write((sc_bv_base)ff_dout.range(36, 32));
    WB_RM_S1.write((bool)ff_dout[37]);
    PC_MEM2WBK_RM_S1.write((sc_bv_base)ff_dout.range(73, 38));
    CSR_WENABLE_RM_S1.write((bool)ff_dout[74]);
    CSR_RDATA_RM_S1.write((sc_bv_base)ff_dout.range(106, 75));
    MULT_INST_RM_S1.write((bool)ff_dout[107]);
}

void mem_s1::fifo_gestion() {
    bool stall = MCACHE_STALL_SM_S1.read() || mem2wbk_full_sm.read() || EXE2MEM_EMPTY_SE_S1.read();
    mem2wbk_push_sm.write(!stall);
    EXE2MEM_POP_SM_S1.write(!stall);
}
void mem_s1::mem_preprocess() {
    int         size        = MEM_SIZE_RE_S1.read();
    bool        sign_extend = SIGN_EXTEND_RE_S1.read();
    sc_uint<32> din         = MEM_DATA_RE_S1.read();
    int         adr;
    sc_uint<32> dout;
    int         range_size;
    int         range_end;
    int         range_start;  // The beginning of the range of din that should actually
                              // be written to the register
    MEM_SIZE_SM_S1 = MEM_SIZE_RE_S1;
    // Memory slot selection
    switch (EXE_RES_RE_S1.read().range(1, 0)) {
        case 0: range_start = 0; break;
        case 1: range_start = 8; break;
        case 2: range_start = 16; break;
        case 3: range_start = 24; break;
        default: range_start = 0; break;
    }

    // Size of the data manipulate
    switch (size) {
        case 2: range_size = 7; break;   // byte
        case 1: range_size = 15; break;  // half word
        case 0: range_size = 31; break;  // word
        default: range_size = 31; break;
    }
    range_end = std::min(31, range_start + range_size);

    if (sign_extend && din(range_end, range_end)) {
        dout = 0xFFFFFFFF;
    } else {
        dout = 0;
    }

    // size = 2, range_size = 7
    // adresse : 3, range_start = 24
    // range_end = 7+24 = 31
    // din.range(7,0)
    // dout.range(31,24)
    // issue with the store value
    dout.range(range_end, range_start) = din.range(range_size, 0);

    // Whether the register bank shoudl actually write the data
    wb_sm.write(WB_RE_S1.read() || LOAD_RE_S1.read());

    // The data sent to the actual memory/Icache
    MCACHE_DATA_SM_S1.write(dout);
    if (STORE_RE_S1)
        adr = EXE_RES_RE_S1.read();
    else
        adr = EXE_RES_RE_S1.read() & 0xFFFFFFFC;
    MCACHE_ADR_SM_S1.write(adr);
    MCACHE_LOAD_SM_S1.write(LOAD_RE_S1.read());
    MCACHE_STORE_SM_S1.write(STORE_RE_S1.read());
    MCACHE_ADR_VALID_SM_S1.write(!EXE2MEM_EMPTY_SE_S1.read());
    // MCACHE_MEM_SIZE_SM.write(MEM_SIZE_RE_S1.read());
}

void mem_s1::sign_extend() {
    // sign extend
    int         size        = MEM_SIZE_RE_S1.read();
    bool        sign_extend = SIGN_EXTEND_RE_S1.read();
    sc_uint<32> din         = MCACHE_RESULT_SM_S1.read();
    sc_uint<32> dout;
    int         range_size;
    int         range_start;  // The beginning of the range of din that should actually
                              // be written to the register
    // MEM_SIZE = 0 -> Word
    // MEM_SIZE = 1 -> Half word
    // MEM_SIZE = 2 -> byte
    switch (EXE_RES_RE_S1.read().range(2, 0)) {
        case 0: range_start = 0; break;
        case 1: range_start = 8; break;
        case 2: range_start = 16; break;
        case 3: range_start = 24; break;
        default: range_start = 0; break;
    }

    switch (size) {
        case 2: range_size = 7; break;
        case 1: range_size = 15; break;
        case 0: range_size = 31; break;
        default: range_size = 31; break;
    }
    int range_end = std::min(31, range_start + range_size);

    if (sign_extend && din(range_end, range_end)) {
        dout = 0xFFFFFFFF;
    } else {
        dout = 0;
    }
    dout.range(range_size, 0) = din.range(range_end, range_start);

    // Choose which data should be written to the register
    if (LOAD_RE_S1.read()) {
        data_sm.write(dout);
    } else {
        data_sm.write(EXE_RES_RE_S1.read());
    }
}

void mem_s1::csr_exception() {
    EXCEPTION_SM_S1            = ((EXCEPTION_RE_S1.read() || BUS_ERROR_SX.read()) && !EXE2MEM_EMPTY_SE_S1);
    sc_uint<32> mstatus_new = MSTATUS_RC.read();

    if (!RESET) CURRENT_MODE_SM_S1 = 3;

    if (!EXCEPTION_SM_S1 && !EXCEPTION_SM_S2) {
        if (CSR_WENABLE_RE_S1.read()) {
            CSR_WADR_SM_S1.write(CSR_WADR_SE_S1.read());
            CSR_WDATA_SM_S1.write(EXE_RES_RE_S1.read());
            CSR_ENABLE_SM_S1.write(true);
        } else {
            CSR_WADR_SM_S1.write(0);
            CSR_WDATA_SM_S1.write(0);
            CSR_ENABLE_SM_S1.write(0);
        }
        MRET_SM_S1 = 0;
    } 
    else if (EXCEPTION_SM_S1) 
    {
        // Affectation of the cause
        // PLEASE DO NOT MOVE THE IF ORDER
        // THEY ARE IN A SPECIFIC ORDER
        // TO RESPECT PRIORITY IN CASE OF SEVERAL
        // EXCEPTION OCCURING AT THE SAME CYCLE
        bool machine_interrupt_enable = mstatus_new[3];
        if (BUS_ERROR_SX) {       // load access fault
            save_restore_sm = 0;  // Need to save context
            mpp_sm          = CURRENT_MODE_SM_S1;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_SM_S1          = mstatus_new;

            // MCAUSE_WDATA_SM_S1.write(5);
        } else if (ENV_CALL_WRONG_MODE_RE_S1) {
            save_restore_sm = 0;  // Need to save context
            mpp_sm          = CURRENT_MODE_SM_S1;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_SM_S1          = mstatus_new;

            MEPC_WDATA_SM_S1.write(PC_EXE2MEM_RE_S1.read());
            MTVAL_WDATA_SM_S1 = 0;
            MCAUSE_WDATA_SM_S1.write(24);
            CURRENT_MODE_SM_S1 = 3;
        } else if (MRET_RE_S1) {
            save_restore_sm = 0;
            mpp_sm          = CURRENT_MODE_SM_S1;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 1;

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_SM_S1          = mstatus_new;

            CURRENT_MODE_SM_S1 = 0;  // Retrun in user mode

            // loading return value (main) from EPC to PC :
            // The adress will be send to ifetch

            RETURN_ADRESS_SM_S1 = MEPC_SC;

            // Informing IFETCH that a return instruction have been received

            MEPC_WDATA_SM_S1.write(PC_EXE2MEM_RE_S1.read());
            MTVAL_WDATA_SM_S1 = 0;
            MRET_SM_S1        = 1;
        } else if (STORE_ACCESS_FAULT_RE_S1) {
            save_restore_sm = 0;  // Need to save context
            mpp_sm          = CURRENT_MODE_SM_S1;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_SM_S1          = mstatus_new;

            MEPC_WDATA_SM_S1.write(PC_EXE2MEM_RE_S1.read());
            MTVAL_WDATA_SM_S1 = EXE_RES_RE_S1;
            MCAUSE_WDATA_SM_S1.write(7);
            CURRENT_MODE_SM_S1 = 3;
        } else if (LOAD_ACCESS_FAULT_RE_S1) {
            save_restore_sm = 0;  // Need to save context
            mpp_sm          = CURRENT_MODE_SM_S1;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_SM_S1          = mstatus_new;

            MEPC_WDATA_SM_S1.write(PC_EXE2MEM_RE_S1.read());
            MTVAL_WDATA_SM_S1 = EXE_RES_RE_S1;
            MCAUSE_WDATA_SM_S1.write(5);
            CURRENT_MODE_SM_S1 = 3;
        } else if (STORE_ADRESS_MISSALIGNED_RE_S1) {
            save_restore_sm = 0;
            mpp_sm          = CURRENT_MODE_SM_S1;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_SM_S1          = mstatus_new;

            MEPC_WDATA_SM_S1.write(PC_EXE2MEM_RE_S1.read());
            MTVAL_WDATA_SM_S1 = EXE_RES_RE_S1;
            MCAUSE_WDATA_SM_S1.write(6);
            CURRENT_MODE_SM_S1 = 3;
        } else if (LOAD_ADRESS_MISSALIGNED_RE_S1) {
            save_restore_sm = 0;
            mpp_sm          = CURRENT_MODE_SM_S1;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_SM_S1          = mstatus_new;

            MEPC_WDATA_SM_S1.write(PC_EXE2MEM_RE_S1.read());
            MTVAL_WDATA_SM_S1 = EXE_RES_RE_S1;
            MCAUSE_WDATA_SM_S1.write(4);
            CURRENT_MODE_SM_S1 = 3;
        } else if (ENV_CALL_M_MODE_RE_S1) {
            save_restore_sm = 1;  // Need to save context
            mpp_sm          = CURRENT_MODE_SM_S1;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_SM_S1          = mstatus_new;

            MEPC_WDATA_SM_S1.write(PC_EXE2MEM_RE_S1.read());
            MTVAL_WDATA_SM_S1 = 0;
            MCAUSE_WDATA_SM_S1.write(11);
            CURRENT_MODE_SM_S1 = 3;
        } else if (ENV_CALL_S_MODE_RE_S1) {
            save_restore_sm = 1;  // Need to save context
            mpp_sm          = CURRENT_MODE_SM_S1;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_SM_S1          = mstatus_new;

            MEPC_WDATA_SM_S1.write(PC_EXE2MEM_RE_S1.read());
            MTVAL_WDATA_SM_S1 = 0;
            MCAUSE_WDATA_SM_S1.write(9);
            CURRENT_MODE_SM_S1 = 3;
        } else if (ENV_CALL_U_MODE_RE_S1) {
            save_restore_sm = 1;  // Need to save context
            mpp_sm          = CURRENT_MODE_SM_S1;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_SM_S1          = mstatus_new;

            MEPC_WDATA_SM_S1.write(PC_EXE2MEM_RE_S1.read());
            MTVAL_WDATA_SM_S1 = 0;
            MCAUSE_WDATA_SM_S1.write(8);
            CURRENT_MODE_SM_S1 = 3;
        } else if (EBREAK_RE_S1) {
            save_restore_sm = 1;  // Need to save context
            mpp_sm          = CURRENT_MODE_SM_S1;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_SM_S1          = mstatus_new;

            MEPC_WDATA_SM_S1.write(PC_EXE2MEM_RE_S1.read());
            MTVAL_WDATA_SM_S1 = PC_EXE2MEM_RE_S1;
            MCAUSE_WDATA_SM_S1.write(3);
            CURRENT_MODE_SM_S1 = 3;
        } else if (INSTRUCTION_ADRESS_MISSALIGNED_RE_S1) {
            save_restore_sm = 0;  // Need to save context
            mpp_sm          = CURRENT_MODE_SM_S1;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_SM_S1          = mstatus_new;

            MEPC_WDATA_SM_S1.write(PC_EXE2MEM_RE_S1.read());
            MTVAL_WDATA_SM_S1 = PC_BRANCH_VALUE_RE_S1;
            MCAUSE_WDATA_SM_S1.write(0);
            CURRENT_MODE_SM_S1 = 3;
        } else if (ILLEGAL_INSTRUCTION_RE_S1) {
            save_restore_sm = 0;  // Need to save context
            mpp_sm          = CURRENT_MODE_SM_S1;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_SM_S1          = mstatus_new;

            MEPC_WDATA_SM_S1.write(PC_EXE2MEM_RE_S1.read());
            MTVAL_WDATA_SM_S1 = 0;
            MCAUSE_WDATA_SM_S1.write(2);
            CURRENT_MODE_SM_S1 = 3;
        } else if (INSTRUCTION_ACCESS_FAULT_RE_S1) {
            save_restore_sm = 0;  // Need to save context
            mpp_sm          = CURRENT_MODE_SM_S1;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_SM_S1          = mstatus_new;

            MEPC_WDATA_SM_S1.write(PC_EXE2MEM_RE_S1.read());
            MTVAL_WDATA_SM_S1 = 0;
            MCAUSE_WDATA_SM_S1.write(1);
            CURRENT_MODE_SM_S1 = 3;
        }
        if (!MRET_RE_S1.read()) MRET_SM_S1 = 0;
    }
}

void mem_s1::trace(sc_trace_file* tf) {
    sc_trace(tf, MCACHE_ADR_SM_S1, GET_NAME(MCACHE_ADR_SM_S1));  // adress in memory
    sc_trace(tf, MCACHE_DATA_SM_S1, GET_NAME(MCACHE_DATA_SM_S1));
    sc_trace(tf, EXCEPTION_SM_S2, GET_NAME(EXCEPTION_SM_S2));
    sc_trace(tf, MEM_ACCESS_IS_PRIO_RD_S2, GET_NAME(MEM_ACCESS_IS_PRIO_RD_S2));
    sc_trace(tf, CURRENT_MODE_SM_S2, GET_NAME(CURRENT_MODE_SM_S2));
    sc_trace(tf, MCACHE_ADR_VALID_SM_S1, GET_NAME(MCACHE_ADR_VALID_SM_S1));
    sc_trace(tf, MCACHE_STORE_SM_S1, GET_NAME(MCACHE_STORE_SM_S1));
    sc_trace(tf, MCACHE_LOAD_SM_S1, GET_NAME(MCACHE_LOAD_SM_S1));
    sc_trace(tf, MCACHE_RESULT_SM_S1, GET_NAME(MCACHE_RESULT_SM_S1));
    sc_trace(tf, MCACHE_STALL_SM_S1, GET_NAME(MCACHE_STALL_SM_S1));
    sc_trace(tf, EXE_RES_RE_S1, GET_NAME(EXE_RES_RE_S1));
    sc_trace(tf, MEM_DATA_RE_S1, GET_NAME(MEM_DATA_RE_S1));
    sc_trace(tf, DEST_RE_S1, GET_NAME(DEST_RE_S1));
    sc_trace(tf, MEM_SIZE_RE_S1, GET_NAME(MEM_SIZE_RE_S1));
    sc_trace(tf, WB_RE_S1, GET_NAME(WB_RE_S1));
    sc_trace(tf, SIGN_EXTEND_RE_S1, GET_NAME(SIGN_EXTEND_RE_S1));
    sc_trace(tf, LOAD_RE_S1, GET_NAME(LOAD_RE_S1));
    sc_trace(tf, STORE_RE_S1, GET_NAME(STORE_RE_S1));
    sc_trace(tf, EXE2MEM_EMPTY_SE_S1, GET_NAME(EXE2MEM_EMPTY_SE_S1));
    sc_trace(tf, EXE2MEM_POP_SM_S1, GET_NAME(EXE2MEM_POP_SM_S1));
    sc_trace(tf, MEM2WBK_POP_SW_S1, GET_NAME(MEM2WBK_POP_SW_S1));
    sc_trace(tf, mem2wbk_push_sm, GET_NAME(mem2wbk_push_sm));
    sc_trace(tf, mem2wbk_full_sm, GET_NAME(mem2wbk_full_sm));
    sc_trace(tf, MEM2WBK_EMPTY_SM_S1, GET_NAME(MEM2WBK_EMPTY_SM_S1));
    sc_trace(tf, MEM_RES_RM_S1, GET_NAME(MEM_RES_RM_S1));
    sc_trace(tf, DEST_RM_S1, GET_NAME(DEST_RM_S1));
    sc_trace(tf, WB_RM_S1, GET_NAME(WB_RM_S1));
    sc_trace(tf, mem2wbk_din_sm, GET_NAME(mem2wbk_din_sm));
    sc_trace(tf, mem2wbk_dout_sm, GET_NAME(mem2wbk_dout_sm));
    sc_trace(tf, data_sm, GET_NAME(data_sm));
    sc_trace(tf, wb_sm, GET_NAME(wb_sm));
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET, GET_NAME(RESET));
    sc_trace(tf, CSR_WENABLE_RE_S1, GET_NAME(CSR_WENABLE_RE_S1));
    sc_trace(tf, CSR_WADR_SE_S1, GET_NAME(CSR_WADR_SE_S1));
    sc_trace(tf, CSR_RDATA_RE_S1, GET_NAME(CSR_RDATA_RE_S1));
    sc_trace(tf, CSR_WENABLE_RM_S1, GET_NAME(CSR_WENABLE_RM_S1));
    sc_trace(tf, CSR_RDATA_RM_S1, GET_NAME(CSR_RDATA_RM_S1));
    sc_trace(tf, INTERRUPTION_SE_S1, GET_NAME(INTERRUPTION_SE_S1));
    sc_trace(tf, CSR_WADR_SM_S1, GET_NAME(CSR_WADR_SM_S1));
    sc_trace(tf, CSR_WDATA_SM_S1, GET_NAME(CSR_WDATA_SM_S1));
    sc_trace(tf, EXCEPTION_RE_S1, GET_NAME(EXCEPTION_RE_S1));
    sc_trace(
        tf, LOAD_ADRESS_MISSALIGNED_RE_S1, GET_NAME(LOAD_ADRESS_MISSALIGNED_RE_S1));  // adress from store/load isn't aligned
    sc_trace(tf, LOAD_ACCESS_FAULT_RE_S1,
             GET_NAME(LOAD_ACCESS_FAULT_RE_S1));  // trying to access memory in wrong mode
    sc_trace(tf, ENV_CALL_U_MODE_RE_S1, GET_NAME(ENV_CALL_U_MODE_RE_S1));
    sc_trace(tf, ENV_CALL_WRONG_MODE_RE_S1, GET_NAME(ENV_CALL_WRONG_MODE_RE_S1));
    sc_trace(tf, ILLEGAL_INSTRUCTION_RE_S1, GET_NAME(ILLEGAL_INSTRUCTION_RE_S1));  // accessing stuff in wrong mode
    sc_trace(tf,
             INSTRUCTION_ADRESS_MISSALIGNED_RE_S1,
             GET_NAME(INSTRUCTION_ADRESS_MISSALIGNED_RE_S1));  // branch offset is misaligned
    sc_trace(tf, ENV_CALL_S_MODE_RE_S1, GET_NAME(ENV_CALL_S_MODE_RE_S1));
    sc_trace(tf, ENV_CALL_M_MODE_RE_S1, GET_NAME(ENV_CALL_M_MODE_RE_S1));
    sc_trace(tf, PC_EXE2MEM_RE_S1, GET_NAME(PC_EXE2MEM_RE_S1));
    sc_trace(tf, BUS_ERROR_SX, GET_NAME(BUS_ERROR_SX));
    sc_trace(tf, EXCEPTION_SM_S1, GET_NAME(EXCEPTION_SM_S1));
    sc_trace(tf, MSTATUS_WDATA_SM_S1, GET_NAME(MSTATUS_WDATA_SM_S1));
    sc_trace(tf, MIP_WDATA_SM_S1, GET_NAME(MIP_WDATA_SM_S1));
    sc_trace(tf, MEPC_WDATA_SM_S1, GET_NAME(MEPC_WDATA_SM_S1));
    sc_trace(tf, MCAUSE_WDATA_SM_S1, GET_NAME(MCAUSE_WDATA_SM_S1));
    sc_trace(tf, MIP_VALUE_RC, GET_NAME(MIP_VALUE_RC));
    sc_trace(tf, CSR_ENABLE_SM_S1, GET_NAME(CSR_ENABLE_SM_S1));
    sc_trace(tf, EBREAK_RE_S1, GET_NAME(EBREAK_RE_S1));
    sc_trace(tf, exception_sm, GET_NAME(exception_sm));
    sc_trace(tf, MULT_INST_RM_S1, GET_NAME(MULT_INST_RM_S1));
    // sc_trace(tf, MCACHE_MEM_SIZE_SM, GET_NAME(MCACHE_MEM_SIZE_SM));
    sc_trace(tf, CURRENT_MODE_SM_S1, GET_NAME(CURRENT_MODE_SM_S1));
    sc_trace(tf, MRET_RE_S1, GET_NAME(MRET_RE_S1));
    sc_trace(tf, MRET_SM_S1, GET_NAME(MRET_SM_S1));
    sc_trace(tf, RETURN_ADRESS_SM_S1, GET_NAME(RETURN_ADRESS_SM_S1));
    sc_trace(tf, mret_sm, GET_NAME(mret_sm));
    sc_trace(tf, MEM_SIZE_SM_S1, GET_NAME(MEM_SIZE_SM_S1));
    sc_trace(tf, PC_BRANCH_VALUE_RE_S1, GET_NAME(PC_BRANCH_VALUE_RE_S1));
    sc_trace(tf, STORE_ACCESS_FAULT_RE_S1, GET_NAME(STORE_ACCESS_FAULT_RE_S1));
    sc_trace(tf, STORE_ADRESS_MISSALIGNED_RE_S1, GET_NAME(STORE_ADRESS_MISSALIGNED_RE_S1));
    sc_trace(tf, INSTRUCTION_ACCESS_FAULT_RE_S1, GET_NAME(INSTRUCTION_ACCESS_FAULT_RE_S1));
    fifo_inst.trace(tf);
}