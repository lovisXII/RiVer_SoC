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