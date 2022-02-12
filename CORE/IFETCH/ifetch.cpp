#include "ifetch.h"

void ifetch::fetch_method() 
{
    IF_ADR.write(DEC_PC.read());
    IF_ADR_VALID.write(!DEC2IF_EMPTY.read());
    if (!IF2DEC_FLUSH.read()) {
        bool stall = IC_STALL.read() || IF2DEC_FULL.read() || DEC2IF_EMPTY.read();
        IF2DEC_PUSH.write(!stall);
        DEC2IF_POP.write(!stall);
    }
    else {
        IF2DEC_PUSH.write(false);
        DEC2IF_POP.write(true);
    }
}
void ifetch::trace(sc_trace_file* tf) {
        sc_trace(tf, IF_ADR, GET_NAME(IF_ADR));
        sc_trace(tf, IF_ADR_VALID, GET_NAME(IF_ADR_VALID)); 
        sc_trace(tf, IC_INST, GET_NAME(IC_INST));
        sc_trace(tf, IC_STALL, GET_NAME(IC_STALL));
        sc_trace(tf, DEC2IF_EMPTY, GET_NAME(DEC2IF_EMPTY)); 
        sc_trace(tf, DEC2IF_POP, GET_NAME(DEC2IF_POP));
        sc_trace(tf, IF2DEC_FLUSH, GET_NAME(IF2DEC_FLUSH)); 
        sc_trace(tf, IF2DEC_POP, GET_NAME(IF2DEC_POP));
        sc_trace(tf, IF2DEC_PUSH, GET_NAME(IF2DEC_PUSH));
        sc_trace(tf, IF2DEC_FULL, GET_NAME(IF2DEC_FULL));
        sc_trace(tf, IF2DEC_EMPTY, GET_NAME(IF2DEC_EMPTY));
        sc_trace(tf, DEC_PC, GET_NAME(DEC_PC)); 
        sc_trace(tf, IF_IR, GET_NAME(IF_IR));
        sc_trace(tf, CLK, GET_NAME(CLK));
        sc_trace(tf, RESET, GET_NAME(RESET));
        fifo_inst.trace(tf);
}