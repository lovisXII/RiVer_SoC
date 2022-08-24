#include "mem.h"

/*
3 memory access are possible :
*  access in byte
*  access in half word
*  access in word
In every situation the adress must be aligned with the size of the access type. Meaning :
* byte      : adress must be aligned on a byte (01)
* half-word : adress must be aligned on a half-word (10)
* word      : adress must be aligned on a word (00)

If a access is made in a specific type and the adress doesnt respect the alignement it must raise an
exception.

For load access :
In all the case we send a word-align adress to the ram and we do the masking
operation in mem

For store access :
* byte      : we store the least significant byte of the register in the corresponding part of the
adress
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
void mem::mem2wbk_concat() {
    sc_bv<MEM2WBK_SIZE> ff_din;

    ff_din.range(31, 0)  = data_sm.read();
    ff_din.range(36, 32) = DEST_RE.read();
    ff_din[37]           = wb_sm.read();
    mem2wbk_din_sm.write(ff_din);
}
void mem::mem2wbk_unconcat() {
    sc_bv<MEM2WBK_SIZE> ff_dout = mem2wbk_dout_sm.read();

    MEM_RES_RM.write((sc_bv_base)ff_dout.range(31, 0));
    DEST_RM.write((sc_bv_base)ff_dout.range(36, 32));
    WB_RM.write((bool)ff_dout[37]);
}

void mem::fifo_gestion() {
    bool stall = MCACHE_STALL_SM.read() || mem2wbk_full_sm.read() || EXE2MEM_EMPTY_SE.read();
    mem2wbk_push_sm.write(!stall);
    EXE2MEM_POP_SM.write(!stall);
}
void mem::mem_preprocess() {
    int         size        = MEM_SIZE_RE.read();
    bool        sign_extend = SIGN_EXTEND_RE.read();
    sc_uint<32> din         = MEM_DATA_RE.read();
    int         adr;
    sc_uint<32> dout;
    int         range_size;
    int         range_end;
    int         range_start;  // The beginning of the range of din that should actually
                              // be written to the register
    MEM_SIZE_SM = MEM_SIZE_RE;
    // Memory slot selection
    switch (EXE_RES_RE.read().range(1, 0)) {
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

    dout.range(range_end, range_start) = din.range(range_size, 0);

    // Whether the register bank should actually write the data
    wb_sm.write(WB_RE.read() || LOAD_RE.read());

    // The data sent to the actual memory/Icache
    MCACHE_DATA_SM.write(dout);
    if (STORE_RE)
        adr = EXE_RES_RE.read();
    else
        adr = EXE_RES_RE.read() & 0xFFFFFFFC;
    MCACHE_ADR_SM.write(adr);
    MCACHE_LOAD_SM.write(LOAD_RE.read());
    MCACHE_STORE_SM.write(STORE_RE.read());
    MCACHE_ADR_VALID_SM.write(!EXE2MEM_EMPTY_SE.read());
}

void mem::sign_extend() {
    // sign extend
    int         size        = MEM_SIZE_RE.read();
    bool        sign_extend = SIGN_EXTEND_RE.read();
    sc_uint<32> din         = MCACHE_RESULT_SM.read();
    sc_uint<32> dout;
    int         range_size;
    int         range_start;  // The beginning of the range of din that should actually
                              // be written to the register
    // MEM_SIZE = 0 -> Word
    // MEM_SIZE = 1 -> Half word
    // MEM_SIZE = 2 -> byte
    switch (EXE_RES_RE.read().range(2, 0)) {
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
    if (LOAD_RE.read()) {
        data_sm.write(dout);
    } else {
        data_sm.write(EXE_RES_RE.read());
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
    sc_trace(tf, MEM_SIZE_SM, GET_NAME(MEM_SIZE_SM));
    fifo_inst.trace(tf);
}