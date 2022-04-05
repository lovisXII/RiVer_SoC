#pragma once
#include <systemc.h>
#include <iostream>
#include <string>
#include "alu.h"
#include "shifter.h"
#include "../UTIL/fifo.h"
#include "../UTIL/debug_util.h"
SC_MODULE(exec)
{
    // Input/Output of EXE : 

    sc_in<sc_uint<32>>    OP1_SE ;
    sc_in<sc_uint<32>>    OP2_SE ;
    sc_in<sc_uint<32>>    IN_MEM_DATA_SE;
    sc_in<sc_uint<6>>     IN_DEST_SE;
    sc_in<sc_uint<2>>     CMD_SE ;
    sc_in<sc_uint<2>>     IN_MEM_SIZE_SE ;
    sc_in<bool>           NEG_OP2_SE, IN_WB_SE, IN_MEM_SIGN_EXTEND_SE, SELECT_SHIFT_SE ; //taille fifo entrée : 110
    sc_in<bool>           IN_MEM_LOAD_SE, IN_MEM_STORE_SE ; 
    sc_in<bool>           EXE2MEM_POP_SE, DEC2EXE_EMPTY_SE;
    sc_in<bool>           SLT_SE, SLTU_SE;
    sc_in_clk               CLK;
    sc_in<bool>           RESET;
    
    //Fifo exe2mem interface :

    sc_out<sc_uint<32>>  EXE_RES_SE ;
    sc_out<sc_uint<32>>  OUT_MEM_DATA_SE;
    sc_out<sc_uint<6>>   OUT_DEST_SE;
    sc_out<sc_uint<2>>   OUT_MEM_SIZE_SE ;

    sc_out<bool>   OUT_WB_SE,  OUT_MEM_SIGN_EXTEND_SE ; //taille fifo sortie : 7
    sc_out<bool> OUT_MEM_LOAD_SE, OUT_MEM_STORE_SE ; 
    sc_out<bool>   EXE2MEM_EMPTY_SE, DEC2EXE_POP_SE;
    

    //Internals signals :
    
    sc_signal<sc_uint<32>>  exe_res_se ;

    sc_signal<sc_bv<76>>    exe2mem_din_se; // concatenation of exe_res, mem_data...etc
    sc_signal<sc_bv<76>>    exe2mem_dout_se;

    sc_signal<sc_uint<32>> alu_in_op2_se;
    sc_signal<sc_uint<32>> alu_out_se;
    sc_signal<sc_uint<32>> shifter_out_se;
    sc_signal<sc_uint<5>> shift_val_se;  
    sc_signal<bool> exe2mem_push_se, exe2mem_full_se;  

    //Instance used :

    alu         alu_inst;
    shifter     shifter_inst;
    fifo<76>    fifo_inst;
    
    void preprocess_op();   // send op2 or ~op2 in ALU_IN_OP2
    void select_exec_res(); // setup FFIN_EXE_RES as ALU_OUT or SHIFTER_OUT
    void fifo_concat();     // setup result  in fifo exe2mem
    void fifo_unconcat();   // unconcatenet result from the fifo
    void manage_fifo(); // allow the push/pop of fifo exe2mem
    void trace(sc_trace_file* tf);
    SC_CTOR(exec) :
        alu_inst("alu"), 
        shifter_inst("shifter"),
        fifo_inst("exe2mem")
    {
        //ALU port map :

        alu_inst.OP1_SE(OP1_SE);
        alu_inst.OP2_SE(alu_in_op2_se);
        alu_inst.CMD_SE(CMD_SE);
        alu_inst.CIN_SE(NEG_OP2_SE);
        alu_inst.RES_SE(alu_out_se);

        //Shifter port map :

        shifter_inst.DIN_SE(OP1_SE);
        shifter_inst.SHIFT_VAL_SE(shift_val_se);
        shifter_inst.CMD_SE(CMD_SE);
        shifter_inst.DOUT_SE(shifter_out_se);
        
        fifo_inst.DIN(exe2mem_din_se);
        fifo_inst.DOUT(exe2mem_dout_se);
        fifo_inst.EMPTY(EXE2MEM_EMPTY_SE);
        fifo_inst.FULL(exe2mem_full_se);
        fifo_inst.PUSH(exe2mem_push_se);
        fifo_inst.POP(EXE2MEM_POP_SE);
        fifo_inst.CLK(CLK);
        fifo_inst.RESET_N(RESET);

        SC_METHOD(preprocess_op);
        sensitive << OP1_SE << NEG_OP2_SE << OP2_SE;
        SC_METHOD(select_exec_res);
        sensitive << alu_out_se << shifter_out_se << SELECT_SHIFT_SE;
        SC_METHOD(fifo_concat);
        sensitive << IN_MEM_DATA_SE << IN_DEST_SE << IN_MEM_SIZE_SE << IN_MEM_LOAD_SE << IN_MEM_SIGN_EXTEND_SE << IN_MEM_STORE_SE << IN_WB_SE << exe_res_se;
        SC_METHOD(fifo_unconcat);
        sensitive << exe2mem_dout_se;
        SC_METHOD(manage_fifo);
        sensitive << exe2mem_full_se << DEC2EXE_EMPTY_SE;
    }
};

