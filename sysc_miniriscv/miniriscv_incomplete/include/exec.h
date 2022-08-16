#pragma once
#include <systemc.h>
#include <iostream>
#include <string>
#include "alu.h"
#include "constants.h"
#include "debug_util.h"
#include "fifo.h"
#include "shifter.h"
SC_MODULE(exec) {
    /*****************************************************
                    Interface with DEC
    ******************************************************/

    sc_in<sc_uint<32>> OP1_RD;
    sc_in<sc_uint<32>> OP2_RD;

    // Address of the source operands, to be able to bypass the right registers
    sc_in<sc_uint<6>> RADR1_RD;
    sc_in<sc_uint<6>> RADR2_RD;

    // In some cases bypasses need to be blocked
    sc_in<bool> BLOCK_BP_RD;

    // Destination register and whether to write back to it
    sc_in<sc_uint<6>> DEST_RD;
    sc_in<bool>       WB_RD;

    // negates the second operator, to do substractions
    sc_in<bool> NEG_OP2_RD;
    /* operation types :
       - 0 : alu
       - 1 : shifter
       - 2 : slt
    */
    sc_in<sc_uint<3>> OPTYPE_RD;
    /* Command sent to EXE.
    In case of a shift :
        - 0 : Shift Left Logical (sll)
        - 1 : Shift Right Logical (srl)
        - 2 : Shift Right Arithmetic (sra)

    In case of an ALU operation :
        - 0 : add
        - 1 : and
        - 2 : or
        - 3 : xor
    In case of an SLT operation
        - 0 : slt
        - 1 : sltu
    */
    sc_in<sc_uint<2>> CMD_RD;

    // Data sent to MEM through EXE
    sc_in<sc_uint<32>> MEM_DATA_RD;
    sc_in<sc_uint<2>>  MEM_SIZE_RD;
    sc_in<bool>        MEM_SIGN_EXTEND_RD;
    sc_in<bool>        MEM_LOAD_RD;
    sc_in<bool>        MEM_STORE_RD;

    /*****************************************************
             Interface with EXE2MEM
    ******************************************************/

    sc_in<bool>  EXE2MEM_POP_SM;  // The POP signal coming from MEM
    sc_out<bool> EXE2MEM_EMPTY_SE;

    /*****************************************************
                Interface with DEC2EXE
    ******************************************************/

    sc_in<bool>  DEC2EXE_EMPTY_SD;
    sc_out<bool> DEC2EXE_POP_SE;

    /*****************************************************
                Interface with MEM
    ******************************************************/

    sc_out<sc_uint<32>> EXE_RES_RE;  // exe result

    sc_out<sc_uint<6>> DEST_RE;
    sc_out<bool>       WB_RE;

    sc_out<sc_uint<32>> MEM_DATA_RE;
    sc_out<sc_uint<2>>  MEM_SIZE_RE;
    sc_out<bool>        MEM_SIGN_EXTEND_RE;
    sc_out<bool>        MEM_LOAD_RE;
    sc_out<bool>        MEM_STORE_RE;

    /*****************************************************
                    Bypasses
    ******************************************************/
    sc_in<sc_uint<6>>  BP_DEST_RM;
    sc_in<sc_uint<32>> BP_MEM_RES_RM;
    sc_in<bool>        BP_MEM2WBK_EMPTY_SM;

    // General Interface :

    sc_in_clk   CLK;
    sc_in<bool> RESET;

    // Internals signals :

    sc_signal<sc_uint<32>> exe_res_se;

    sc_signal<sc_bv<EXE2MEM_SIZE>> exe2mem_din_se;
    sc_signal<sc_bv<EXE2MEM_SIZE>> exe2mem_dout_se;

    sc_signal<sc_uint<32>> op1_se;
    sc_signal<sc_uint<32>> op2_se;
    sc_signal<sc_uint<32>> alu_in_op2_se;
    sc_signal<sc_uint<32>> alu_out_se;
    sc_signal<sc_uint<32>> shifter_out_se;
    sc_signal<sc_uint<32>> mem_data_se;
    sc_signal<sc_uint<5>>  shift_val_se;

    sc_signal<bool> exe2mem_push_se, exe2mem_full_se;

    sc_signal<bool> blocked_se;

    sc_signal<bool> wb_re;
    sc_signal<bool> mem_load_re;
    sc_signal<bool> mem_store_re;

    sc_signal<bool> invalid_operand_se;

    // Instance used :

    alu                alu_inst;
    shifter            shifter_inst;
    fifo<EXE2MEM_SIZE> fifo_inst;

    void preprocess_op();    // send op2 or ~op2 in ALU_IN_OP2
    void select_exec_res();  // setup FFIN_EXE_RES as ALU_OUT or SHIFTER_OUT
    void fifo_concat();      // setup result  in fifo exe2mem
    void fifo_unconcat();    // unconcatenet result from the fifo
    void manage_fifo();      // allow the push/pop of fifo exe2mem

    void bypasses();  // allow the push/pop of fifo exe2mem

    void trace(sc_trace_file * tf);
    SC_CTOR(exec) : alu_inst("alu"), shifter_inst("shifter"), fifo_inst("exe2mem") {
        // ALU port map :

        alu_inst.OP1_SE(op1_se);
        alu_inst.OP2_SE(alu_in_op2_se);
        alu_inst.CMD_SE(CMD_RD);
        alu_inst.CIN_SE(NEG_OP2_RD);
        alu_inst.RES_SE(alu_out_se);

        // Shifter port map :

        shifter_inst.DIN_SE(op1_se);
        shifter_inst.SHIFT_VAL_SE(shift_val_se);
        shifter_inst.CMD_SE(CMD_RD);
        shifter_inst.DOUT_SE(shifter_out_se);

        // fifo port map

        fifo_inst.DATAIN_S(exe2mem_din_se);
        fifo_inst.DATAOUT_R(exe2mem_dout_se);
        fifo_inst.EMPTY_S(EXE2MEM_EMPTY_SE);
        fifo_inst.FULL_S(exe2mem_full_se);
        fifo_inst.PUSH_S(exe2mem_push_se);
        fifo_inst.POP_S(EXE2MEM_POP_SM);
        fifo_inst.CLK(CLK);
        fifo_inst.RESET(RESET);

        SC_METHOD(preprocess_op);
        sensitive << op1_se << NEG_OP2_RD << op2_se;
        SC_METHOD(select_exec_res);
        sensitive << alu_out_se << shifter_out_se << OPTYPE_RD << MEM_LOAD_RD << MEM_STORE_RD
                  << RESET << CMD_RD << op1_se << op2_se;
        SC_METHOD(fifo_concat);
        sensitive << mem_data_se << DEST_RD << MEM_SIZE_RD << MEM_LOAD_RD << MEM_SIGN_EXTEND_RD
                  << MEM_STORE_RD << WB_RD << exe_res_se << mem_load_re << mem_store_re << wb_re;
        SC_METHOD(fifo_unconcat);
        sensitive << exe2mem_dout_se;
        SC_METHOD(manage_fifo);
        sensitive << exe2mem_full_se << DEC2EXE_EMPTY_SD << blocked_se << invalid_operand_se;
        SC_METHOD(bypasses);
        sensitive << BP_DEST_RM << BP_MEM_RES_RM << DEST_RE << EXE_RES_RE << RADR1_RD << BLOCK_BP_RD
                  << DEST_RE << MEM_LOAD_RE << RADR2_RD << OP1_RD << OP2_RD << MEM_DATA_RD
                  << MEM_STORE_RD << BP_MEM2WBK_EMPTY_SM << EXE2MEM_EMPTY_SE;
    }
};
