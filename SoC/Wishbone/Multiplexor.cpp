#include "Multiplexor.h"


void Multiplexor::select()
{
    if(GRANT_0.read())
    {
        ADR.write(ADR_0);
        DAT.write(DAT_0);
        SEL.write(SEL_0);
        STB.write(STB_0);
        WE.write(WE_0);
    }
    else if(GRANT_1.read())
    {
        ADR.write(ADR_1);
        DAT.write(DAT_1);
        SEL.write(SEL_1);
        STB.write(STB_1);
        WE.write(WE_1);
    }
}

void Multiplexor::trace(sc_trace_file* tf)
{
    sc_trace(tf, ADR_0, GET_NAME(ADR_0));
    sc_trace(tf, DAT_0, GET_NAME(DAT_0));

    sc_trace(tf, SEL_0, GET_NAME(SEL_0));
    sc_trace(tf, STB_0, GET_NAME(STB_0));
    sc_trace(tf, WE_0, GET_NAME(WE_0));

    sc_trace(tf, ADR_1, GET_NAME(ADR_1));
    sc_trace(tf, DAT_1, GET_NAME(DAT_1));

    sc_trace(tf, SEL_1, GET_NAME(SEL_1));
    sc_trace(tf, STB_1, GET_NAME(STB_1));
    sc_trace(tf, WE_1, GET_NAME(WE_1));

    sc_trace(tf, ADR, GET_NAME(ADR));
    sc_trace(tf, DAT, GET_NAME(DAT));

    sc_trace(tf, SEL, GET_NAME(SEL));
    sc_trace(tf, STB, GET_NAME(STB));
    sc_trace(tf, WE, GET_NAME(WE));

    sc_trace(tf, GRANT_0, GET_NAME(GRANT_0));
    sc_trace(tf, GRANT_1, GET_NAME(GRANT_1));
}