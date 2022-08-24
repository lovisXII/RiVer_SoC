#include "IP_RAM.h"


void IP_RAM::init_mem(std::unordered_map<int,int>* tab)
{
    ram_inst.init_mem(tab);
}

void IP_RAM::trace(sc_trace_file* tf)
{
    wrapper_inst.trace(tf);
    ram_inst.trace(tf);

    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET_N, GET_NAME(RESET_N));

    sc_trace(tf, DAT_I, GET_NAME(DAT_I));
    sc_trace(tf, ADR_I, GET_NAME(ADR_I));
    sc_trace(tf, CYC_I, GET_NAME(CYC_I));
    sc_trace(tf, SEL_I, GET_NAME(SEL_I));
    sc_trace(tf, STB_I, GET_NAME(STB_I));
    sc_trace(tf, WE_I, GET_NAME(WE_I));

    sc_trace(tf, DAT_O, GET_NAME(DAT_O));
    sc_trace(tf, ACK_O, GET_NAME(ACK_O));
    
    sc_trace(tf, ARBITER_SEL_I, GET_NAME(ARBITER_SEL_I));

    //signals
    sc_trace(tf, ADR, GET_NAME(ADR));
    sc_trace(tf, RAM_I, GET_NAME(RAM_I));
    sc_trace(tf, RAM_O, GET_NAME(RAM_O));
    sc_trace(tf, MEM_SIZE, GET_NAME(MEM_SIZE));
    sc_trace(tf, WE, GET_NAME(WE));
    sc_trace(tf, RAM2WRAPPER_VALID, GET_NAME(RAM2WRAPPER_VALID));
    sc_trace(tf, WRAPPER2RAM_VALID, GET_NAME(WRAPPER2RAM_VALID));
}