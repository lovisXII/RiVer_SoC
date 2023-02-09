#pragma once
#include <systemc.h>
#include "debug_util.h"

/*
 * This is a generic fifo using template.
*/
template <int T>
SC_MODULE(fifo) {
    sc_in<sc_bv<T>> DIN_S;
    sc_in_clk       CLK;
    sc_in<bool>     PUSH_S, POP_S;
    sc_in<bool>     RESET_N;

    sc_out<bool>     FULL_S, EMPTY_S;
    sc_out<sc_bv<T>> DOUT_R;

    sc_signal<bool> fifo_v; // tells if data inside the fifo is valid or not

    void function();
    void flags_update();
    void trace(sc_trace_file * tf);
    SC_CTOR(fifo) {
        SC_CTHREAD(function, fifo::CLK.pos());
        SC_METHOD(flags_update);
        sensitive << PUSH_S << POP_S << fifo_v << DOUT_R;

        reset_signal_is(RESET_N, false);
    }
};

/**
 * full when :
    * data inside is valid and no pop  
 * empty when :
    * data inside invalid
*/
template <int T>
void fifo<T>::flags_update() {
    bool push = PUSH_S.read();
    bool pop  = POP_S.read();
    if (fifo_v)  
    {
        if (pop) {
            FULL_S.write(0);
            EMPTY_S.write(0);
        } else {
            FULL_S.write(1);
            EMPTY_S.write(0);
        }
    } else  
    {
        FULL_S.write(0);
        EMPTY_S.write(1);
    }
}
template <int T>
void fifo<T>::function() {

    // RESET :

    fifo_v.write(false);
    DOUT_R.write(0);

    wait(3);

    // END OF RESET

    while (1) {
        bool push = PUSH_S.read();
        bool pop  = POP_S.read();
        if (fifo_v && !push && pop)  
        {
            fifo_v.write(0);
        } else if (push && (pop || !fifo_v)) {
            DOUT_R.write(DIN_S.read());
            fifo_v.write(1);  
        }
        wait(1);
    }
}
template <int T>
void fifo<T>::trace(sc_trace_file* tf) {
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET_N, GET_NAME(RESET_N));
    sc_trace(tf, DIN_S, GET_NAME(DIN_S));
    sc_trace(tf, PUSH_S, GET_NAME(PUSH_S));
    sc_trace(tf, POP_S, GET_NAME(POP_S));
    sc_trace(tf, FULL_S, GET_NAME(FULL_S));
    sc_trace(tf, EMPTY_S, GET_NAME(EMPTY_S));
    sc_trace(tf, DOUT_R, GET_NAME(DOUT_R));
    sc_trace(tf, fifo_v, GET_NAME(fifo_v));
}
