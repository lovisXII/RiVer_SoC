#include "wb_bus.h"

void wb_bus::init_mem(std::unordered_map<int,int>* tab)
{
    ram_inst.init_mem(tab);
}
void wb_bus::trace(sc_trace_file* tf)
{
    arbiter_inst.trace(tf);
    ram_inst.trace(tf);
    river_inst.trace(tf);

    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET_N, GET_NAME(RESET_N));
    sc_trace(tf, ADR, GET_NAME(ADR));
    sc_trace(tf, ACK, GET_NAME(ACK));
    sc_trace(tf, DAT_M2S, GET_NAME(DAT_M2S));
    sc_trace(tf, DAT_S2M, GET_NAME(DAT_S2M));
    sc_trace(tf, CYC, GET_NAME(CYC));

    sc_trace(tf, SEL, GET_NAME(SEL));
    sc_trace(tf, STB, GET_NAME(STB));
    sc_trace(tf, WE, GET_NAME(WE));
    sc_trace(tf, CYC_0, GET_NAME(CYC_0));
    sc_trace(tf, GRANT_0, GET_NAME(GRANT_0));
    sc_trace(tf, SEL_0, GET_NAME(SEL_0));
}