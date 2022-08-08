#include "IP_RIVER.h"


void IP_RIVER::trace(sc_trace_file* tf)
{
    core_inst.trace(tf);
    icache_inst.trace(tf);
    dcache_inst.trace(tf);
    wrapper_inst.trace(tf);

    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET_N, GET_NAME(RESET_N));

    sc_trace(tf, ACK, GET_NAME(ACK));
    sc_trace(tf, DAT_I, GET_NAME(DAT_I));
    sc_trace(tf, GRANT, GET_NAME(GRANT));

    sc_trace(tf, ADR, GET_NAME(ADR));
    sc_trace(tf, DAT_O, GET_NAME(DAT_O));
    sc_trace(tf, SEL, GET_NAME(SEL));
    sc_trace(tf, STB, GET_NAME(STB));
    sc_trace(tf, WE, GET_NAME(WE));
    sc_trace(tf, CYC, GET_NAME(CYC));
}