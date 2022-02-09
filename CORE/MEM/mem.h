#pragma once
#include <systemc.h>
#include <iostream>
#include "../UTIL/fifo_41b/fifo_41b.h"
 
SC_MODULE(mem)
{
    // Mcache Interface :

    sc_out<sc_uint<32> >        MEM_ADR ; // adress in memory
    sc_out<sc_uint<32> >        MEM_DATA ;
    sc_out<bool>                MEM_ADR_VALID,
                                MEM_STORE,
                                MEM_LOAD ; 

    sc_in<sc_uint<32> >         MEM_RESULT ;
    sc_in<bool>                 MEM_STALL ;
    
    //Exe Interface :

    sc_in< sc_uint<32> >        EXE_RES ;
    sc_in< sc_uint<32> >        EXE_MEM_DATA;
    sc_in< sc_uint<4> >         EXE_DEST;
    sc_in< sc_uint<2> >         EXE_MEM_SIZE ;

    sc_in< bool >               EXE_WB,  
                                EXE_MEM_SIGN_EXTEND ; //taille fifo entrée : 74
    sc_in< sc_uint<3> >         EXE_MEM_LOAD, 
                                EXE_MEM_STORE ; 

    //exe2mem interface :
    
    sc_in<bool>                 EXE2MEM_EMPTY ; 
    sc_out<bool>                EXE2MEM_POP ;

    //mem2wbk interface

    sc_in<bool>                 MEM2WBK_POP ;
    sc_signal<bool>             MEM2WBK_PUSH ;
    sc_signal<bool>             MEM2WBK_FULL ;
    sc_out<bool>                MEM2WBK_EMPTY ;
    

    //WBK interface 
    sc_out< sc_uint<32> >       WBK_DATA;
    sc_out< sc_uint<5> >        WBK_DEST;
    sc_out< sc_uint<2> >        WBK_MEM_SIZE ;
    sc_out< bool >              WBK_WB;
    sc_out< bool >              WBK_MEM_SIGN_EXTEND;

    //Internal signals
    sc_signal< sc_uint<41> >    MEM2WBK_FFIN;
    sc_signal< sc_uint<41> >    MEM2WBK_FFOUT;
    sc_signal< sc_uint<32> >    DATA;
    sc_signal< bool >           WB;
    //Global Interface :

    sc_in_clk                   CLK;
    sc_in_clk                   RESET;

    // FIFO
    fifo_41b    fifo_inst;

    void mem2wbk_concat();
    void mem2wbk_unconcat();
    void fifo_gestion();
    void mem_preprocess();

    SC_CTOR(mem) : 
    fifo_inst("mem2wbk")
    {
        fifo_inst.DIN(MEM2WBK_FFIN);
        fifo_inst.DOUT(MEM2WBK_FFOUT);
        fifo_inst.EMPTY(MEM2WBK_EMPTY);
        fifo_inst.FULL(MEM2WBK_FULL);
        fifo_inst.PUSH(MEM2WBK_PUSH);
        fifo_inst.POP(MEM2WBK_POP);
        fifo_inst.CLK(CLK);
        fifo_inst.RESET_N(RESET);
    }
};