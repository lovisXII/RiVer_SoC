#pragma once

#include <systemc.h>
#include <iostream>
#include <string>
#include "debug_util.h"

SC_MODULE(shifter) {
    sc_in<sc_uint<32>> DIN_SE;        // input
    sc_in<sc_uint<5>>  SHIFT_VAL_SE;  // shift value
    sc_in<sc_uint<2>>  CMD_SE;        // command
    /*
    Command value :
        - 0 : Shift Left Logical (sll)
        - 1 : Shift Right Logical (srl)
        - 2 : Shift Right Arithmetic (sra)
    */

    sc_out<sc_uint<32>> DOUT_SE;  // output

    /*
     Vous pouvez ajouter des signaux internes ici.
     Si vous les faites, n'oubliez pas de les ajouter à la fonction trace
     dans shifter.cpp, pour qu'ils apparaissent dans GTKWAVE
    */

    void trace(sc_trace_file * tf);

    SC_CTOR(shifter) {}
};
