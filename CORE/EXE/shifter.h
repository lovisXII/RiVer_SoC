#pragma once

#include <systemc.h>
#include <iostream>
#include <string>



SC_MODULE(shifter)
{
    sc_in< sc_uint<32> >  DIN ; //input
    sc_in< sc_uint<5> >  SHIFT_VAL ; //shift value
    sc_in< sc_uint<2> > CMD ; //command
    sc_out< sc_uint<32> > DOUT ; //output
    sc_signal< bool > SLL, SRL, SRA ; //command, only one must be true
    sc_signal< sc_uint<32> > SLL_DOUT, SR_DOUT; //temporary output for each component
    
    void decode_cmd();
    void shifter_sll() ; //component to shift left the input
    void shifter_sr() ; //component to shift right the input, arithmetic or logic
    void shifter_agregate() ; //component to choose which shift to use

    SC_CTOR(shifter)
    {
        SC_METHOD(decode_cmd);
        sensitive << CMD;
        SC_METHOD(shifter_sll);
        sensitive << DIN << SHIFT_VAL ;
        SC_METHOD(shifter_sr);
        sensitive << DIN << SHIFT_VAL << SRA ;
        SC_METHOD(shifter_agregate);
        sensitive << SLL_DOUT << SR_DOUT << SLL << SRL << SRA ;
    }
};

