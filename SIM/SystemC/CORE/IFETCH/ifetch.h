#pragma once
#include <systemc.h>
#include <iostream>
#include "../../UTIL/debug_util.h"
#include "../../UTIL/fifo.h"
#include "../config.h"

enum  // PREDICTION STATE
{     // 1,2,4,8 -> one-hot
    strongly_taken     = 1,
    weakly_taken       = 2,
    weakly_not_taken   = 4,
    strongly_not_taken = 8
};

SC_MODULE(ifetch) {
    // Icache Interface :

    sc_in<sc_bv<32>> INST_SIC;
    sc_in<bool>      STALL_SIC;

    sc_out<sc_uint<32>> ADR_SI;  // @ which we search a data block from memory
    sc_out<bool>        ADR_VALID_SI;

    // dec2if interface :

    sc_in<bool>  DEC2IF_EMPTY_SI;
    sc_out<bool> DEC2IF_POP_SI;

    sc_in<sc_uint<32>> PC_RD;  // PC coming to fetch an instruction
    sc_in<bool>        PRED_FAILED_RD;
    sc_in<bool>        PRED_SUCCESS_RD;
    sc_in<bool>        INSTR_IS_BRANCH_RD;
    sc_in<sc_uint<32>> BRANCH_INST_ADR_RD;
    sc_in<sc_uint<32>> BRANCH_TARGET_ADR_RD;

    sc_in<sc_uint<32>> PRED_ADR_SD;
    sc_in<bool>        PRED_TAKEN_SD;

    // RAS : return-address stack
    sc_in<bool> PUSH_ADR_RAS_RD;
    sc_in<bool> POP_ADR_RAS_RD;

    sc_in<sc_uint<32>> RETURN_ADR_RD;

    sc_in<bool> RET_INST_RD;

    // if2dec interface

    sc_in<bool> IF2DEC_FLUSH_SD;  // allow to flush if2dec in case of a branch
    sc_in<bool> IF2DEC_POP_SD;

    sc_out<bool>        IF2DEC_EMPTY_SI;
    sc_out<sc_bv<32>>   INSTR_RI;      // instruction sent to if2dec
    sc_out<sc_uint<32>> PC_IF2DEC_RI;  // pc sent to if2dec

    sc_out<bool> EXCEPTION_RI;  // tells if an instruction have been made in IFETCH

    sc_out<sc_uint<32>> PRED_ADR_RI;
    sc_out<bool>        PRED_TAKEN_RI;

    // Interruption :

    sc_in<bool>        INTERRUPTION_SE;
    sc_in<sc_uint<2>>  CURRENT_MODE_SM;
    sc_in<bool>        MRET_SM;
    sc_in<sc_uint<32>> RETURN_ADRESS_SM;

    // Global Interface :

    sc_in<bool> EXCEPTION_SM;
    sc_in_clk   CLK;
    sc_in<bool> RESET;

    // FIFO
    fifo<if2dec_size> fifo_inst;

    // Internals signals :

    sc_signal<bool>               IF2DEC_PUSH_SI;
    sc_signal<bool>               IF2DEC_FULL_SI;
    sc_signal<sc_bv<if2dec_size>> if2dec_in_si;
    sc_signal<sc_bv<if2dec_size>> instr_ri;  // instruction sent to if2dec

    // branch prediction register
    sc_signal<sc_uint<32>>                   BRANCH_ADR_REG[predictor_register_size];
    sc_signal<sc_uint<32>>                   PREDICTED_ADR_REG[predictor_register_size];
    sc_signal<sc_uint<4>>                    PRED_STATE_REG[predictor_register_size];
    sc_signal<bool>                          VALID_PRED_REG[predictor_register_size];
    sc_signal<sc_uint<size_of_pred_pointer>> pred_write_pointer_si;

    sc_signal<sc_uint<32>> pred_branch_next_adr_si;
    sc_signal<bool>        pred_branch_taken_si;

    sc_signal<sc_uint<4>> next_state_pred_si;

    // function return prediction register
    sc_signal<sc_uint<32>>                         RET_ADR_RI[ret_predictor_register_size];
    sc_signal<bool>                                VALID_RET_REG[ret_predictor_register_size];
    sc_signal<sc_uint<ret_predictor_pointer_size>> ret_write_pointer_si;

    sc_signal<sc_uint<32>>             RET_STACK_RI[ret_stack_size];
    sc_signal<sc_uint<ret_stack_size>> ret_stack_pointer_si;

    sc_signal<sc_uint<32>> pred_ret_next_adr_si;
    sc_signal<bool>        pred_ret_taken_si;

    sc_signal<sc_uint<32>> PRED_NEXT_ADR_SI;
    sc_signal<bool>        PRED_ADR_TAKEN_SI;

    void fetch_method();

    void write_pred_reg();
    void read_pred_reg();

    void calc_prob_pred();
    void apply_prob();

    void write_pred_ret_reg();

    void ret_stack();

    void next_pred_adr();

    void trace(sc_trace_file * tf);
    void exception();

    SC_CTOR(ifetch) : fifo_inst("if2dec") {
        fifo_inst.DIN_S(if2dec_in_si);
        fifo_inst.DOUT_R(instr_ri);
        fifo_inst.EMPTY_S(IF2DEC_EMPTY_SI);
        fifo_inst.FULL_S(IF2DEC_FULL_SI);
        fifo_inst.PUSH_S(IF2DEC_PUSH_SI);
        fifo_inst.POP_S(IF2DEC_POP_SD);
        fifo_inst.CLK(CLK);
        fifo_inst.RESET_N(RESET);

        SC_METHOD(fetch_method);
        sensitive << INST_SIC << DEC2IF_EMPTY_SI << IF2DEC_FULL_SI << PC_RD << IF2DEC_FLUSH_SD << STALL_SIC << RESET
                  << EXCEPTION_SM << MRET_SM << RETURN_ADRESS_SM << PRED_ADR_RI << PRED_TAKEN_RI << PRED_SUCCESS_RD
                  << PRED_FAILED_RD << PRED_ADR_TAKEN_SI << PRED_NEXT_ADR_SI;
        SC_METHOD(exception);
        sensitive << RESET << EXCEPTION_SM;

        SC_METHOD(write_pred_reg);
        sensitive << CLK.neg();

        SC_METHOD(read_pred_reg);
        sensitive << PC_RD << EXCEPTION_SM << DEC2IF_EMPTY_SI;

        SC_METHOD(calc_prob_pred);
        sensitive << PRED_FAILED_RD << PRED_SUCCESS_RD;

        // ret prediction

        SC_METHOD(write_pred_ret_reg);
        sensitive << CLK.neg();

        SC_METHOD(ret_stack);
        sensitive << CLK.neg() << RESET;

        SC_METHOD(next_pred_adr);
        sensitive << pred_branch_taken_si << pred_ret_taken_si << pred_ret_next_adr_si << pred_branch_next_adr_si;
    }
};
