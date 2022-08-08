#include "wb_arbiter.h"


void wb_arbiter::master_selector()
{
    if(!RESET_N)
    {
        CYC_O = false;
    }
    else
    {
        // if add more masters change this part of the code by a 
        // round robin arbiter
        CYC_O = CYC_0_I; //CYC_0_I | CYC_1_I | CYC_2_I | ../..
        if(CYC_0_I)
        {
            GRANT_0_O = true;
        }
    }
}
void wb_arbiter::slave_selector()
{
    sc_uint<nb_slaves> slave_selected = 0;

    int index = 1;
    for(int i = 0; i < nb_slaves; i++)
    {
        if(slave_deb_adr[i] <= ADR_I.read() &&
           slave_end_adr[i] >= ADR_I.read())
            slave_selected = index;
        index <<= 1;
    }
    switch(slave_selected)
    {
        case 1:
            SEL_0_O = 1;
    }
}

void wb_arbiter::init_memory()
{
    slave_deb_adr[0] = ram_adr_deb;
    slave_end_adr[0] = ram_adr_end;
}

void wb_arbiter::trace(sc_trace_file* tf)
{
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET_N, GET_NAME(RESET_N));

    sc_trace(tf, ADR_I, GET_NAME(ADR_I));
    sc_trace(tf, CYC_0_I, GET_NAME(CYC_0_I));
    sc_trace(tf, CYC_O, GET_NAME(CYC_O));
    sc_trace(tf, GRANT_0_O, GET_NAME(GRANT_0_O));
    sc_trace(tf, SEL_0_O, GET_NAME(SEL_0_O));
}

