#pragma once
#include <systemc.h>
#include <iostream>
#include "../UTIL/debug_util.h"
#include "../UTIL/fifo.h"
#include "../UTIL/buffer.h"

#define nop_encoding 0x0000013
#define if2dec_size  64 //(pc+inst)
#define if2dec_depth 2
SC_MODULE(ifetch) {
    // FIFO
    
    buffer<if2dec_size,if2dec_depth> buffer_inst_1 ;
    buffer<if2dec_size, if2dec_depth> buffer_inst_2 ;

    // Icache Interface :

    sc_in<sc_bv<32>>        IC_INST_SI_S1;  //Instruction sent by the cache/memory
    sc_in<sc_bv<32>>        IC_INST_SI_S2;  //Instruction sent by the cache/memory
    sc_in<bool>             IC_STALL_SI;

    sc_out<sc_uint<32>>     ADR_SI_S1;  // @ which we search a data block from memory
    sc_out<sc_uint<32>>     ADR_SI_S2;  // @ which we search a data block from memory
    sc_out<bool>            ADR_VALID_SI_S1;
    sc_out<bool>            ADR_VALID_SI_S2;

    // dec2if interface :

    sc_in<bool>             DEC2IF_EMPTY_SI;
    sc_out<bool>            DEC2IF_POP_SI;

    // if2dec interface

    sc_in<bool>             IF2DEC_FLUSH_SD;   // allow to flush if2dec in case of a branch
    
    sc_in<bool>             IF2DEC_POP_SD_S1;
    sc_in<bool>             IF2DEC_POP_SD_S2;

    sc_in<sc_bv<32>>        PC_DEC2IF_RD_S1;          // PC coming to fetch an instruction
    sc_in<sc_bv<32>>        PC_DEC2IF_RD_S2;          // PC coming to fetch an instruction

    sc_out<bool>            IF2DEC_EMPTY_SI_S1;
    sc_out<bool>            IF2DEC_EMPTY_SI_S2;

    sc_out<sc_bv<32>>       INSTR_RI_S1;      // instruction sent to if2dec
    sc_out<sc_bv<32>>       INSTR_RI_S2;      // instruction sent to if2dec
    sc_out<sc_uint<32>>     PC_IF2DEC_RI_S1;  // pc sent to if2dec
    sc_out<sc_uint<32>>     PC_IF2DEC_RI_S2;  // pc sent to if2dec

    sc_out<bool>        EXCEPTION_RI;  // tells if an instruction have been made in IFETCH

    // Interruption :

    sc_in<bool>         INTERRUPTION_SE_S1;
    sc_in<sc_uint<2>>   CURRENT_MODE_SM_S1 ;
    sc_in<bool>         MRET_SM_S1 ;
    sc_in<sc_uint<32>>  RETURN_ADRESS_SM_S1 ;

    // Global Interface :

    sc_in<bool> EXCEPTION_SM_S1;
    sc_in_clk   CLK;
    sc_in<bool>   RESET;


    // Internals signals :

    sc_signal<bool>                 if2dec_push_si_s1;
    sc_signal<bool>                 if2dec_full_si_s1;
    sc_signal<bool>                 if2dec_push_si_s2;
    sc_signal<bool>                 if2dec_full_si_s2;
    sc_signal<sc_bv<if2dec_size>>   if2dec_in_si_s1;
    sc_signal<sc_bv<if2dec_size>>   instr_ri_s1;  // instruction sent to if2dec
   
    sc_signal<sc_bv<if2dec_size>>   if2dec_in_si_s2;
    sc_signal<sc_bv<if2dec_size>>   instr_ri_s2;  // instruction sent to if2dec

    
    void fetch_method();
    void trace(sc_trace_file * tf);
    void exception();
    
    SC_CTOR(ifetch) : buffer_inst_1("if2dec_s1"), buffer_inst_2("if2dec_s2") {

        // Buffer 1 port map :

        buffer_inst_1.DIN_S(if2dec_in_si_s1);
        buffer_inst_1.DOUT_R(instr_ri_s1);
        buffer_inst_1.EMPTY_S(IF2DEC_EMPTY_SI_S1);
        buffer_inst_1.FULL_S(if2dec_full_si_s1);
        buffer_inst_1.PUSH_S(if2dec_push_si_s1);
        buffer_inst_1.POP_S(IF2DEC_POP_SD_S1);
        buffer_inst_1.CLK(CLK);
        buffer_inst_1.RESET_N(RESET);

        // Buffer 2 port map :

        buffer_inst_2.DIN_S(if2dec_in_si_s2);
        buffer_inst_2.DOUT_R(instr_ri_s2);
        buffer_inst_2.EMPTY_S(IF2DEC_EMPTY_SI_S2);
        buffer_inst_2.FULL_S(if2dec_full_si_s2);
        buffer_inst_2.PUSH_S(if2dec_push_si_s2);
        buffer_inst_2.POP_S(IF2DEC_POP_SD_S2);
        buffer_inst_2.CLK(CLK);
        buffer_inst_2.RESET_N(RESET); 

        SC_METHOD(fetch_method);
        sensitive 
        << IC_INST_SI_S1 
        << IC_INST_SI_S2 
        << instr_ri_s1
        << instr_ri_s2
        << DEC2IF_EMPTY_SI 
        << if2dec_full_si_s1 
        << if2dec_full_si_s2 
        << PC_DEC2IF_RD_S1  
        << PC_DEC2IF_RD_S2 
        << IF2DEC_FLUSH_SD 
        << IF2DEC_FLUSH_SD 
        << IC_STALL_SI 
        << RESET
        << EXCEPTION_SM_S1 
        << MRET_SM_S1 
        << RETURN_ADRESS_SM_S1;
        SC_METHOD(exception)
        sensitive << RESET << EXCEPTION_SM_S1 ;
    }
};
