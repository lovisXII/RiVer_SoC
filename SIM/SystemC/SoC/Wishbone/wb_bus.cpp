#include "wb_bus.h"

void wb_bus::init_mem(std::unordered_map<int,int>* tab)
{
    ram_inst.init_mem(tab);
}
void wb_bus::trace(sc_trace_file* tf)
{
    arbiter_inst.trace(tf);
    ram_inst.trace(tf);
    river0_inst.trace(tf);
    river1_inst.trace(tf);
    multiplexor_inst.trace(tf);

    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET_N, GET_NAME(RESET_N));
    sc_trace(tf, ADR_0, GET_NAME(ADR_0));
    sc_trace(tf, ACK, GET_NAME(ACK));
    sc_trace(tf, DAT_M0, GET_NAME(DAT_M0));
    sc_trace(tf, DAT_S2M, GET_NAME(DAT_S2M));
    sc_trace(tf, CYC, GET_NAME(CYC));

    sc_trace(tf, SEL_0, GET_NAME(SEL_0));
    sc_trace(tf, STB_0, GET_NAME(STB_0));
    sc_trace(tf, WE_0, GET_NAME(WE_0));
    sc_trace(tf, GRANT_0, GET_NAME(GRANT_0));
    sc_trace(tf, SEL_0, GET_NAME(SEL_0));

    sc_trace(tf, PC0_VALUE, GET_NAME(PC0_VALUE));
    sc_trace(tf, PC1_VALUE, GET_NAME(PC1_VALUE));
}