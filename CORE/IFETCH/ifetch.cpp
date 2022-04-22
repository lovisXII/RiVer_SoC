#include "ifetch.h"

void ifetch::fetch_method() {
    sc_bv<if2dec_size> if2dec_in_var;
    sc_bv<if2dec_size> instr_ri_var = instr_ri.read();
    if (EXCEPTION_RM.read() == 0) {
        ADR_SI.write(PC_RD.read());

        // data sent in if2dec

        if2dec_in_var.range(65,64) =  current_mode_si ;
        if2dec_in_var.range(63, 32) = (sc_bv_base)IC_INST_SI.read();
        if2dec_in_var.range(31, 0)  = (sc_bv_base)PC_RD.read();
        if2dec_in_si.write(if2dec_in_var);
        // data coming out from if2dec :

        CURRENT_MODE_RI.write((sc_bv_base)instr_ri_var.range(65,64 )) ;
        INSTR_RI.write((sc_bv_base)instr_ri_var.range(63, 32));
        PC_IF2DEC_RI.write((sc_bv_base)instr_ri_var.range(31, 0));

    } else {
        // data sent in if2dec

        if2dec_in_var.range(64,65) = 0 ;
        if2dec_in_var.range(63, 32) = nop_encoding;
        if2dec_in_var.range(31, 0)  = (sc_bv_base)PC_RD.read();
        if2dec_in_si.write(if2dec_in_var);
        // data coming out from if2dec :

        INSTR_RI.write((sc_bv_base)instr_ri_var.range(63, 32));
        PC_IF2DEC_RI.write((sc_bv_base)instr_ri_var.range(31, 0));
    }
    if (IF2DEC_FLUSH_SD.read()) {
        IF2DEC_PUSH_SI.write(false);
        DEC2IF_POP_SI.write(true);
        ADR_VALID_SI.write(false);
    } else {
        // stall if the memory stalls, if we can't push to dec, or have no value
        // of pc to pop from dec
        bool stall = IC_STALL_SI.read() || IF2DEC_FULL_SI.read() || DEC2IF_EMPTY_SI.read();
        IF2DEC_PUSH_SI.write(!stall);
        DEC2IF_POP_SI.write(!stall);
        ADR_VALID_SI.write(!DEC2IF_EMPTY_SI.read());
    }
}

void ifetch::exception()  // can be usefull for further use
{
    if(RESET)
        current_mode_si = 0b11 ; // at initialization, strating in M-MODE
    EXCEPTION_RI.write(0);
}


void ifetch::trace(sc_trace_file* tf) {
    sc_trace(tf, ADR_SI, GET_NAME(ADR_SI));
    sc_trace(tf, ADR_VALID_SI, GET_NAME(ADR_VALID_SI));
    sc_trace(tf, IC_INST_SI, GET_NAME(IC_INST_SI));
    sc_trace(tf, IC_STALL_SI, GET_NAME(IC_STALL_SI));
    sc_trace(tf, DEC2IF_EMPTY_SI, GET_NAME(DEC2IF_EMPTY_SI));
    sc_trace(tf, DEC2IF_POP_SI, GET_NAME(DEC2IF_POP_SI));
    sc_trace(tf, IF2DEC_FLUSH_SD, GET_NAME(IF2DEC_FLUSH_SD));
    sc_trace(tf, IF2DEC_POP_SD, GET_NAME(IF2DEC_POP_SD));
    sc_trace(tf, IF2DEC_PUSH_SI, GET_NAME(IF2DEC_PUSH_SI));
    sc_trace(tf, IF2DEC_FULL_SI, GET_NAME(IF2DEC_FULL_SI));
    sc_trace(tf, IF2DEC_EMPTY_SI, GET_NAME(IF2DEC_EMPTY_SI));
    sc_trace(tf, PC_RD, GET_NAME(PC_RD));
    sc_trace(tf, INSTR_RI, GET_NAME(INSTR_RI));
    sc_trace(tf, PC_IF2DEC_RI, GET_NAME(PC_IF2DEC_RI));
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET, GET_NAME(RESET));
    sc_trace(tf, if2dec_in_si, GET_NAME(if2dec_in_si));
    sc_trace(tf, instr_ri, GET_NAME(instr_ri));
    sc_trace(tf, EXCEPTION_RI, GET_NAME(EXCEPTION_RI));
    sc_trace(tf, EXCEPTION_RM, GET_NAME(EXCEPTION_RM));
    sc_trace(tf, INTERRUPTION_SE, GET_NAME(INTERRUPTION_SE));
    fifo_inst.trace(tf);
}