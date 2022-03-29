#pragma once
#include <systemc.h>
#include "debug_util.h"
template <int T>
SC_MODULE(fifo)
{
    sc_in< sc_bv<T> > DIN ;
    sc_in_clk CLK ;
    sc_in<bool> PUSH, POP ;
    sc_in<bool> RESET_N ;
    
    sc_out<bool> FULL, EMPTY ;
    sc_out< sc_bv<T> > DOUT ;

    sc_signal<bool> fifo_v ;
    
    void function() ;
    void flags_update() ;
    void trace(sc_trace_file* tf);
    SC_CTOR(fifo)
    {
        SC_CTHREAD(function,fifo::CLK.pos()) ;
        SC_METHOD(flags_update);
        sensitive << PUSH << POP << fifo_v << DOUT;

        reset_signal_is(RESET_N, false) ;
    }
};
template <int T>
void fifo<T>::flags_update() {
    bool push = PUSH.read() ;
    bool pop = POP.read() ;
    if( fifo_v ) // if the data in the fifo is valide
    {
        if(pop)
        {
            FULL.write(0) ;
            EMPTY.write(0) ;
        }
        else
        {
            FULL.write(1) ;
            EMPTY.write(0) ;
        }
    }
    else // case where data inside the fifo is not valid
    {

        FULL.write(0) ;
        EMPTY.write(1) ;
    }
}
template <int T>
void fifo<T>::function()
{
    fifo_v.write(false) ;
    DOUT.write(0) ;

    wait(3) ;

    //when fifo_v is valide, it means that we want the data and we can pop it
    //when it's unvalaible it means we don't care about it and we can push
    
    while(1)
    {
        bool push = PUSH.read() ;
        bool pop = POP.read() ;
        if(fifo_v && !push  && pop ) // when data is valid and pop is able we sent data
        {
            fifo_v.write(0) ;
        }
        else if(push && (pop || !fifo_v) )
        {
            DOUT.write(DIN.read()) ;
            fifo_v.write(1) ; // stay valid
        }
        wait(1) ;
    } 
}
template <int T>
void fifo<T>::trace(sc_trace_file* tf) {
        sc_trace(tf, CLK, GET_NAME(CLK));
        sc_trace(tf, RESET_N, GET_NAME(RESET_N));
        sc_trace(tf, DIN, GET_NAME(DIN));
        sc_trace(tf, PUSH, GET_NAME(PUSH));
        sc_trace(tf, POP, GET_NAME(POP));
        sc_trace(tf, FULL, GET_NAME(FULL));
        sc_trace(tf, EMPTY, GET_NAME(EMPTY));
        sc_trace(tf, DOUT, GET_NAME(DOUT));
        sc_trace(tf, fifo_v, GET_NAME(fifo_v));
}

