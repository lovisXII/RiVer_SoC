#include "alu.h"


void alu::operation()
    {
        sc_uint<32> res_interne ;
        if( cmd.read() == 0 )
        {
            res_interne = op1.read() + op2.read() + cin ;
        }
        else if(cmd.read() == 1 )
        {
            res_interne = op1.read() & op2.read() ;
        }
        else if(cmd.read() == 2 )
        {
            res_interne = op1.read() | op2.read() ;
        }
        else if(cmd.read() == 3 )
        {
            res_interne = op1.read() ^ op2.read() ;

        }
        res.write(res_interne) ;
    }