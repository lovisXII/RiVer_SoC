#include "alu.h"


void alu::operation()
    {
        sc_uint<33> res_interne ;
        if( cmd.read()[0] == 0 & cmd.read()[0] == 0 )
        {
            res_interne = op1.read() + op2.read() + cin ;
            cout.write(res_interne[32]) ;
        }
        else if(cmd.read()[0] == 0 & cmd.read()[0] == 1 )
        {
            res_interne = op1.read() & op2.read() ;
            cout.write(cin.read()) ;
        }
        else if(cmd.read()[0] == 1 & cmd.read()[0] == 0 )
        {
            res_interne = op1.read() | op2.read() ;
            cout.write(cin.read()) ;
        }
        else if(cmd.read()[0] == 1 & cmd.read()[0] == 1 )
        {
            res_interne = op1.read() ^ op2.read() ;
            cout.write(cin.read()) ;

        }
        res.write(res_interne.range(32,0)) ;
    }