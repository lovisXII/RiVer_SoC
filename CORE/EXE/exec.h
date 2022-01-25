#include <systemc.h>
#include <iostream>
#include <string>



SC_MODULE(exec)
{
    sc_in< sc_uint<32> >  OP1 ;
    sc_in< sc_uint<32> >  OP2 ;
    sc_in< sc_uint<32> >  MEM_DATA;
    sc_in< sc_uint<4> >   DEST;
    sc_in< sc_uint<2> >   CMD ;
    sc_in< sc_uint<2> >   MEM_SIZE ;
    sc_in< bool >   NEG_OP2, WB, MEM_LOAD, MEM_STORE, MEM_SIGN_EXTEND, SELECT_SHIFT ; //taille fifo entrée : 110

    sc_out< sc_uint<32> >  EXE_RES ;
    sc_out< sc_uint<32> >  MEM_DATA;
    sc_out< sc_uint<4> >   DEST;
    sc_out< sc_uint<2> >   MEM_SIZE ;
    sc_out< bool >   WB, MEM_LOAD, MEM_STORE, MEM_SIGN_EXTEND ; //taille fifo sortie : 74

    sc_signal< sc_uint<32> >  FF_EXE_RES ;
    sc_signal< sc_uint<32> >  FF_MEM_DATA;
    sc_signal< sc_uint<4> >   FF_DEST;
    sc_signal< sc_uint<2> >   FF_MEM_SIZE ;
    sc_signal< bool >   FF_WB, FF_MEM_LOAD, FF_MEM_STORE, FF_MEM_SIGN_EXTEND ;

    sc_signal< sc_uint<32> > ALU_IN_OP2;
    sc_signal< sc_uint<32> > ALU_OUT;
    sc_signal< sc_uint<32> > SHIFTER_OUT;
    sc_signal< sc_uint<5> > SHIFT_VAL;
    
    sc_in_clk CLK;
    sc_in< bool > RESET;

    alu alu_inst;
    shifter shifter_inst;
    
    void entry();
    void comp_op2();
    void select_exec_res();
    void mem_passthrough();

    SC_CTOR(exec) :
        alu_inst("alu"), 
        shifter_inst("shifter")
    {
        alu_inst.OP1(OP1);
        alu_inst.OP2(ALU_IN_OP2);
        alu_inst.CMD(CMD);
        alu_inst.CIN(NEG_OP2);
        alu_inst.RES(ALU_OUT);

        shifter_inst.DIN(OP1);
        shifter_inst.SHIFT_VAL(SHIFT_VAL);
        shifter_inst.CMD(CMD);
        shifter_inst.CIN(NEG_OP2);
        shifter_inst.RES(ALU_OUT);
        
        SC_METHOD(comp_op2);
        sensitive << OP2 << NEG_OP2;
        SC_METHOD(select_exec_res);
        sensitive << ALU_OUT << SHIFTER_OUT << SELECT_SHIFT;
        SC_METHOD(map_alu);
        sensitive << OP1 << ALU_IN_OP2 << CMD << NEG_OP2;
        SC_METHOD(map_shifter);
        sensitive << OP1 << OP2 << CMD << SHIFT_VALUE;
        SC_CTHREAD(entry, CLK.pos());
        reset_signal_is(RESET, true);
    }
};

