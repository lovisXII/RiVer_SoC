#pragma once
#include <systemc.h>
#include <iostream>


SC_MODULE(ifetch)
{
    // Icache Interface :

    sc_out<sc_uint<32> > IF_ADR ; // @ which we search a data block from memory
    sc_out<bool> IF_ADR_VALID ; 

    sc_in<sc_uint<32> > IC_INST ;
    sc_in<bool> IC_STALL ;
    
    //Dec Interface :


    //dec2if interface :
    
    sc_in<bool> DEC2IF_EMPTY ; 
    sc_in<bool> DEC2IF_POP ;

    //if2dec interface
    
    sc_in<bool> IF2DEC_FLUSH ; // allow to flush if2dec in case of a branch
    sc_out<bool> IF2DEC_POP ;
    sc_out<bool> IF2DEC_EMPTY ;
    
    sc_in<sc_uint<32>  > DEC_PC ; // PC coming to fetch an instruction
    sc_out<sc_uint<32> > IF_IR ; // instruction sent to if2dec 
    
    //Global Interface :

    sc_in_clk CLK;

    SC_CTOR(ifetch)
    {
        
    }
}