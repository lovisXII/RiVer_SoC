#include "ifetch.h"

void ifetch::fetch_method() 
{
    ADR_SI.write(PC_SI.read());
    
    if (!IF2DEC_FLUSH_SI.read()) {
        bool stall = IC_STALL_SI.read() || IF2DEC_FULL_SI.read() || DEC2IF_EMPTY_SI.read();
        IF2DEC_PUSH_SI.write(!stall);
        DEC2IF_POP_SI.write(!stall);
        ADR_VALID_SI.write(!DEC2IF_EMPTY_SI.read());    }
    else {
        IF2DEC_PUSH_SI.write(false);
        DEC2IF_POP_SI.write(true);
        ADR_VALID_SI.write(false);
    }
}
void ifetch::trace(sc_trace_file* tf) {
        sc_trace(tf, ADR_SI, GET_NAME(ADR_SI));
        sc_trace(tf, ADR_VALID_SI, GET_NAME(ADR_VALID_SI)); 
        sc_trace(tf, IC_INST_SI, GET_NAME(IC_INST_SI));
        sc_trace(tf, IC_STALL_SI, GET_NAME(IC_STALL_SI));
        sc_trace(tf, DEC2IF_EMPTY_SI, GET_NAME(DEC2IF_EMPTY_SI)); 
        sc_trace(tf, DEC2IF_POP_SI, GET_NAME(DEC2IF_POP_SI));
        sc_trace(tf, IF2DEC_FLUSH_SI, GET_NAME(IF2DEC_FLUSH_SI)); 
        sc_trace(tf, IF2DEC_POP_SI, GET_NAME(IF2DEC_POP_SI));
        sc_trace(tf, IF2DEC_PUSH_SI, GET_NAME(IF2DEC_PUSH_SI));
        sc_trace(tf, IF2DEC_FULL_SI, GET_NAME(IF2DEC_FULL_SI));
        sc_trace(tf, IF2DEC_EMPTY_SI, GET_NAME(IF2DEC_EMPTY_SI));
        sc_trace(tf, PC_SI, GET_NAME(PC_SI)); 
        sc_trace(tf, INSTR_SI, GET_NAME(INSTR_SI));
        sc_trace(tf, CLK, GET_NAME(CLK));
        sc_trace(tf, RESET, GET_NAME(RESET));
        fifo_inst.trace(tf);
}