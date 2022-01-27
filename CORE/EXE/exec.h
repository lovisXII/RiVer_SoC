#pragma once
#include <systemc.h>
#include <iostream>
#include <string>
#include "alu.h"
#include "shifter.h"
#include "../UTIL/fifo_generic/fifo_generic.h"
#include "../UTIL/fifo_72b/fifo_72b.h"

typedef fifo_generic<74> fifo_74b;

SC_MODULE(exec)
{
    // Input/Output of EXE : 

    sc_in< sc_uint<32> >    OP1 ;
    sc_in< sc_uint<32> >    OP2 ;
    sc_in< sc_uint<32> >    MEM_DATA;
    sc_in< sc_uint<4> >     DEST;
    sc_in< sc_uint<2> >     CMD ;
    sc_in< sc_uint<2> >     MEM_SIZE ;
    sc_in< bool >           NEG_OP2, WB, MEM_LOAD, MEM_STORE, MEM_SIGN_EXTEND, SELECT_SHIFT ; //taille fifo entrée : 110
    sc_in< bool >           NEG_OP1, WB, MEM_LOAD, MEM_STORE, MEM_SIGN_EXTEND, SELECT_SHIFT ; //taille fifo entrée : 110
    sc_in< bool >           EXE2MEM_POP, DEC2EXE_EMPTY;
    sc_in_clk               CLK;
    sc_in< bool >           RESET;

    sc_out< sc_uint<32> >  FFOUT_EXE_RES ;
    sc_out< sc_uint<32> >  FFOUT_MEM_DATA;
    sc_out< sc_uint<4> >   FFOUT_DEST;
    sc_out< sc_uint<2> >   FFOUT_MEM_SIZE ;

    //Internals signals :

    sc_out< bool >   FFOUT_WB, FFOUT_MEM_LOAD, FFOUT_MEM_STORE, FFOUT_MEM_SIGN_EXTEND ; //taille fifo sortie : 74
    sc_out< bool >   EXE2MEM_EMPTY, DEC2EXE_POP;
    
    sc_signal< sc_uint<32> >  FFIN_EXE_RES ;

    sc_signal< sc_bv<74> >    FF_DIN;
    sc_signal< sc_bv<74> >    FF_DOUT;

    sc_signal< sc_uint<32> > ALU_IN_OP1;
    sc_signal< sc_uint<32> > ALU_OUT;
    sc_signal< sc_uint<32> > SHIFTER_OUT;
    sc_signal< sc_uint<5> > SHIFT_VAL;  
    sc_signal< bool > EXE2MEM_PUSH, EXE2MEM_FULL;  

    //Instance used :

    alu         alu_inst;
    shifter     shifter_inst;
    fifo_74b    fifo_inst;
    
    void preprocess_op();   // send op2 or ~op2 in ALU_IN_OP2
    void select_exec_res(); // setup FFIN_EXE_RES as ALU_OUT or SHIFTER_OUT
    void fifo_concat();     // setup result  in fifo exe2mem
    void fifo_unconcat();   // 
    void manage_fifo();

    SC_CTOR(exec) :
        alu_inst("alu"), 
        shifter_inst("shifter"),
        fifo_inst("exe2mem")
    {
        //ALU port map :

        alu_inst.OP1(ALU_IN_OP1);
        alu_inst.OP2(OP2);
        alu_inst.CMD(CMD);
        alu_inst.CIN(NEG_OP1);
        alu_inst.RES(ALU_OUT);

        //Shifter port map :

        shifter_inst.DIN(OP1);
        shifter_inst.SHIFT_VAL(SHIFT_VAL);
        shifter_inst.CMD(CMD);
        shifter_inst.DOUT(SHIFTER_OUT);
        
        fifo_inst.DIN(FF_DIN);
        fifo_inst.DOUT(FF_DOUT);
        fifo_inst.EMPTY(EXE2MEM_EMPTY);
        fifo_inst.FULL(EXE2MEM_FULL);
        fifo_inst.PUSH(EXE2MEM_PUSH);
        fifo_inst.POP(EXE2MEM_POP);
        fifo_inst.CLK(CLK);
        fifo_inst.RESET_N(RESET);

        SC_METHOD(preprocess_op);
        sensitive << OP1 << NEG_OP1;
        SC_METHOD(select_exec_res);
        sensitive << ALU_OUT << SHIFTER_OUT << SELECT_SHIFT;
        SC_METHOD(fifo_concat);
        sensitive << MEM_DATA << DEST << MEM_SIZE << MEM_LOAD << MEM_SIGN_EXTEND << MEM_STORE << WB << FFIN_EXE_RES;
        SC_METHOD(fifo_unconcat);
        sensitive << FF_DOUT;
        SC_METHOD(manage_fifo);
        sensitive << EXE2MEM_FULL << DEC2EXE_EMPTY;
    }
};

