#include "ifetch.h"

void ifetch::fetch_method() 
{
    sc_bv<64> if2dec_in_var ;
    sc_bv<64> instr_ri_var  = instr_ri.read();

    ADR_SI.write(PC_RD.read());

    // data sent in if2dec

    if2dec_in_var.range(63,32) = IC_INST_SI.read() ;
    if2dec_in_var.range(31,0) = PC_RD.read() ;

    if2dec_in.write(if2dec_in_var) ;
    //data coming out from if2dec :

    INSTR_RI.write((sc_bv_base)instr_ri_var.range(63,32)) ;
    PC_OUT_RI.write((sc_bv_base)instr_ri_var.range(31,0)) ;
    
    if (IF2DEC_FLUSH_SD.read()) {
        IF2DEC_PUSH_SI.write(false);
        DEC2IF_POP_SI.write(true);
        ADR_VALID_SI.write(false);
    }
    else {
        //stall if the memory stalls, if we can't push to dec, or have no value of pc to pop from dec
        bool stall = IC_STALL_SI.read() || IF2DEC_FULL_SI.read() || DEC2IF_EMPTY_SI.read();
        IF2DEC_PUSH_SI.write(!stall);
        DEC2IF_POP_SI.write(!stall);
        ADR_VALID_SI.write(!DEC2IF_EMPTY_SI.read());    
    }
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
        sc_trace(tf, CLK, GET_NAME(PC_OUT_RI));
        sc_trace(tf, CLK, GET_NAME(CLK));
        sc_trace(tf, RESET, GET_NAME(RESET));
        sc_trace(tf, RESET, GET_NAME(if2dec_in));
        sc_trace(tf, RESET, GET_NAME(instr_ri));
        fifo_inst.trace(tf);
}