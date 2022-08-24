#include "ifetch.h"

void ifetch::fetch_method() {
    // FIFO concat & unconcat
    // data sent in if2dec

    sc_bv<IF2DEC_SIZE> if2dec_in_var;
    if2dec_in_var.range(63, 32) = (sc_bv_base)IC_INST_SI.read();
    if2dec_in_var.range(31, 0)  = (sc_bv_base)PC_RD.read();
    if2dec_in_si.write(if2dec_in_var);

    // data coming out from if2dec :

    sc_bv<IF2DEC_SIZE> if2dec_out_var = if2dec_out_si;
    INSTR_RI.write((sc_bv_base)if2dec_out_var.range(63, 32));
    PC_RI.write((sc_bv_base)if2dec_out_var.range(31, 0));

    ADR_SI = PC_RD.read();
    // FIFO gestion
    if (IF2DEC_FLUSH_SD.read()) {
        IF2DEC_PUSH_SI = false;
        DEC2IF_POP_SI  = true;
        ADR_VALID_SI   = false;
    } else {
        // stall if the memory stalls, if we can't push to dec, or have no value
        // of pc to pop from dec
        bool stall     = IC_STALL_SI || IF2DEC_FULL_SI || DEC2IF_EMPTY_SD;
        IF2DEC_PUSH_SI = !stall;
        DEC2IF_POP_SI  = !stall;
        ADR_VALID_SI   = !DEC2IF_EMPTY_SD;
    }
}

void ifetch::trace(sc_trace_file* tf) {
    sc_trace(tf, ADR_SI, GET_NAME(ADR_SI));
    sc_trace(tf, ADR_VALID_SI, GET_NAME(ADR_VALID_SI));
    sc_trace(tf, IC_INST_SI, GET_NAME(IC_INST_SI));
    sc_trace(tf, IC_STALL_SI, GET_NAME(IC_STALL_SI));
    sc_trace(tf, DEC2IF_EMPTY_SD, GET_NAME(DEC2IF_EMPTY_SD));
    sc_trace(tf, DEC2IF_POP_SI, GET_NAME(DEC2IF_POP_SI));
    sc_trace(tf, IF2DEC_FLUSH_SD, GET_NAME(IF2DEC_FLUSH_SD));
    sc_trace(tf, IF2DEC_POP_SD, GET_NAME(IF2DEC_POP_SD));
    sc_trace(tf, IF2DEC_PUSH_SI, GET_NAME(IF2DEC_PUSH_SI));
    sc_trace(tf, IF2DEC_FULL_SI, GET_NAME(IF2DEC_FULL_SI));
    sc_trace(tf, IF2DEC_EMPTY_SI, GET_NAME(IF2DEC_EMPTY_SI));
    sc_trace(tf, PC_RD, GET_NAME(PC_RD));
    sc_trace(tf, INSTR_RI, GET_NAME(INSTR_RI));
    sc_trace(tf, PC_RI, GET_NAME(PC_RI));
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET, GET_NAME(RESET));
    sc_trace(tf, if2dec_in_si, GET_NAME(if2dec_in_si));
    sc_trace(tf, if2dec_out_si, GET_NAME(if2dec_out_si));
    fifo_inst.trace(tf);
}