#pragma once
#include <systemc.h>
#include <iostream>
#include "../UTIL/fifo_generic/fifo_generic.h"
#include "../UTIL/debug_util.h"

/*
Assuming the following stuff :

MEM_SIZE = 00 -> access in word 
MEM_SIZE = 01 -> access in half word
MEM_SIZE = 10  -> acces in byte

*/
SC_MODULE(mem)
{
    // Mcache Interface :

    sc_out<sc_uint<32> >        MCACHE_ADR_SM ; // adress in memory
    sc_out<sc_uint<32> >        MCACHE_DATA_SM ;
    sc_out<bool>                MCACHE_ADR_VALID_SM,
                                MCACHE_STORE_SM,
                                MCACHE_LOAD_SM ; 

    sc_in<sc_uint<32> >         MCACHE_RESULT_SM ;
    sc_in<bool>                 MCACHE_STALL_SM ;
    
    //Exe Interface :

    sc_in< sc_uint<32> >        EXE_RES_SM ;
    sc_in< sc_uint<32> >        MEM_DATA_SM;
    sc_in< sc_uint<6> >         EXE_DEST_SM;
    sc_in< sc_uint<2> >         EXE_MEM_SIZE_SM ;

    sc_in< bool >               EXE_WB_SM,  
                                SIGN_EXTEND_SM ; //taille fifo entrée : 74
    sc_in< bool >               LOAD_SM, 
                                STORE_SM ; 

    //exe2mem interface :
    
    sc_in<bool>                 EXE2MEM_EMPTY_SM ; 
    sc_out<bool>                EXE2MEM_POP_SM ;

    //mem2wbk interface

    sc_in<bool>                 MEM2WBK_POP_SM ;
    sc_signal<bool>             mem2wbk_push_sm ;
    sc_signal<bool>             mem2wbk_full_sm ;
    sc_out<bool>                MEM2WBK_EMPTY_SM ;
    

    //WBK interface 
    sc_out< sc_uint<32> >       WBK_DATA_SM;
    sc_out< sc_uint<6> >        WBK_DEST_SM;
    sc_out< sc_uint<2> >        WBK_MEM_SIZE_SM ;
    sc_out< bool >              WBK_WB_SM;
    sc_out< bool >              WBK_SIGN_EXTEND_SM;
    sc_out< bool >              WBK_LOAD_SM;

    //Internal signals
    sc_signal< sc_bv<42> >      mem2wbk_din_sm;
    sc_signal< sc_bv<42> >      mem2wbk_dout_sm;
    sc_signal< sc_uint<32> >    data_sm;
    sc_signal< bool >           wb_sm;
    //Global Interface :

    sc_in_clk                   CLK;
    sc_in_clk                   RESET;

    // FIFO
    fifo_generic<42>    fifo_inst;

    void mem2wbk_concat();
    void mem2wbk_unconcat();
    void fifo_gestion();
    void mem_preprocess();

    void trace(sc_trace_file* tf);

    SC_CTOR(mem) : 
    fifo_inst("mem2wbk")
    {
        fifo_inst.DIN(mem2wbk_din_sm);
        fifo_inst.DOUT(mem2wbk_dout_sm);
        fifo_inst.EMPTY(MEM2WBK_EMPTY_SM);
        fifo_inst.FULL(mem2wbk_full_sm);
        fifo_inst.PUSH(mem2wbk_push_sm);
        fifo_inst.POP(MEM2WBK_POP_SM);
        fifo_inst.CLK(CLK);
        fifo_inst.RESET_N(RESET);

        SC_METHOD(mem2wbk_concat);
        sensitive << data_sm << EXE_DEST_SM << EXE_MEM_SIZE_SM << wb_sm << SIGN_EXTEND_SM;
        SC_METHOD(mem2wbk_unconcat);
        sensitive << mem2wbk_dout_sm;
        SC_METHOD(fifo_gestion);
        sensitive << MCACHE_STALL_SM << mem2wbk_full_sm << EXE2MEM_EMPTY_SM << wb_sm;
        SC_METHOD(mem_preprocess);
        sensitive << EXE_WB_SM << LOAD_SM << MCACHE_RESULT_SM << EXE_RES_SM << MEM_DATA_SM << STORE_SM << EXE2MEM_EMPTY_SM;
    }
};