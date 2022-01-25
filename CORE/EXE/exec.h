#pragma once
#include <systemc.h>
#include <iostream>
#include <string>
#include "alu.h"
#include "shifter.h"
#include "../UTIL/fifo_generic/fifo_generic.h"

typedef fifo_generic<74> fifo_74b;

SC_MODULE(exec)
{
    sc_in< sc_uint<32> >  OP1 ;
    sc_in< sc_uint<32> >  OP2 ;
    sc_in< sc_uint<32> >  MEM_DATA;
    sc_in< sc_uint<4> >   DEST;
    sc_in< sc_uint<2> >   CMD ;
    sc_in< sc_uint<2> >   MEM_SIZE ;
    sc_in< bool >   NEG_OP2, WB, MEM_LOAD, MEM_STORE, MEM_SIGN_EXTEND, SELECT_SHIFT ; //taille fifo entrée : 110

    sc_out< sc_uint<32> >  FFOUT_EXE_RES ;
    sc_out< sc_uint<32> >  FFOUT_MEM_DATA;
    sc_out< sc_uint<4> >   FFOUT_DEST;
    sc_out< sc_uint<2> >   FFOUT_MEM_SIZE ;
    sc_out< bool >   FFOUT_WB, FFOUT_MEM_LOAD, FFOUT_MEM_STORE, FFOUT_MEM_SIGN_EXTEND ; //taille fifo sortie : 74

    sc_signal< sc_uint<32> >  FFIN_EXE_RES ;

    sc_signal< sc_bv<74> >    FF_DIN;
    sc_signal< sc_bv<74> >    FF_DOUT;

    sc_signal< sc_uint<32> > ALU_IN_OP2;
    sc_signal< sc_uint<32> > ALU_OUT;
    sc_signal< sc_uint<32> > SHIFTER_OUT;
    sc_signal< sc_uint<5> > SHIFT_VAL;    

    sc_in_clk CLK;
    sc_in< bool > RESET;

    alu alu_inst;
    shifter shifter_inst;
    fifo_74b fifo_inst;
    
    void entry();
    void preprocess_op();
    void select_exec_res();
    void fifo_concat();
    void fifo_unconcat();

    SC_CTOR(exec) :
        alu_inst("alu"), 
        shifter_inst("shifter"),
        fifo_inst("exe2mem")
    {
        alu_inst.OP1(OP1);
        alu_inst.OP2(ALU_IN_OP2);
        alu_inst.CMD(CMD);
        alu_inst.CIN(NEG_OP2);
        alu_inst.RES(ALU_OUT);

        shifter_inst.DIN(OP1);
        shifter_inst.SHIFT_VAL(SHIFT_VAL);
        shifter_inst.CMD(CMD);
        shifter_inst.DOUT(SHIFTER_OUT);
        

        SC_METHOD(preprocess_op);
        sensitive << OP2 << NEG_OP2;
        SC_METHOD(select_exec_res);
        sensitive << ALU_OUT << SHIFTER_OUT << SELECT_SHIFT;
        SC_METHOD(fifo_concat);
        sensitive << MEM_DATA << DEST << MEM_SIZE << MEM_LOAD << MEM_SIGN_EXTEND << MEM_STORE << WB << FFIN_EXE_RES;
    }
};

