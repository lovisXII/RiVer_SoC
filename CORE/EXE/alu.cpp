#include "alu.h"


void alu::operation()
    {
        sc_uint<32> res_interne ;
        if( CMD.read() == 0 )
        {
            res_interne = OP1.read() + OP2.read() + CIN ;
        }
        else if(CMD.read() == 1 )
        {
            res_interne = OP1.read() && OP2.read() ;
        }
        else if(CMD.read() == 2 )
        {
            res_interne = OP1.read() | OP2.read() ;
        }
        else if(CMD.read() == 3 )
        {
            res_interne = OP1.read() ^ OP2.read() ;

        }
        RES.write(res_interne) ;
    }


void alu::trace(sc_trace_file* tf) {
        sc_trace(tf, OP1, GET_NAME(OP1));
        sc_trace(tf, OP2, GET_NAME(OP2));
        sc_trace(tf, CIN, GET_NAME(CIN));
        sc_trace(tf, CMD, GET_NAME(CMD));
        sc_trace(tf, RES, GET_NAME(RES));
    }