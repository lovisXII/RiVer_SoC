#pragma once
#include <systemc.h>
#include <unordered_map>
#include "../UTIL/debug_util.h"

#define RAM_SIZE 6000 //0xFFFF FFFF -> max size


SC_MODULE(RAM)
{
    sc_in_clk           CLK;
    sc_in<bool>         RESET_N;

    sc_in<sc_uint<32>>  ADR_I;
    sc_in<sc_uint<32>>  DAT_I;
    sc_in<bool>         VALID_I;
    sc_in<bool>         WE_I;
    sc_in<sc_uint<2>>   MEM_SIZE;

    sc_out<sc_uint<32>> DAT_O;

    //signals
    sc_signal<sc_uint<4>> current_state;
    sc_signal<sc_uint<4>> future_state;

    std::unordered_map<int,int>* RAM_REGISTERS;

    void init_mem(std::unordered_map<int,int>*);

    void reponse();

    void trace(sc_trace_file*);

    SC_CTOR(RAM)
    {
        SC_METHOD(reponse);
        sensitive << CLK.neg();
    }
};
