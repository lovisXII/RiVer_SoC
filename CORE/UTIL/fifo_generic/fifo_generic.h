#pragma once
#include <systemc.h>


template <int T>
SC_MODULE(fifo_generic)
{
    sc_in< sc_bv<T> > DIN ;
    sc_in_clk CLK ;
    sc_in<bool> PUSH, POP ;
    sc_in<bool> RESET_N ;
    
    sc_out<bool> FULL, EMPTY ;
    sc_out< sc_bv<T> > DOUT ;

    sc_signal<bool> fifo_v ;
    
    void reg_maj() ;
    void compute_flags() ;

    SC_CTOR(fifo_generic)
    {
        SC_METHOD(compute_flags);
        sensitive << POP << fifo_v;
        SC_CTHREAD(reg_maj, CLK.pos()) ;
        reset_signal_is(RESET_N, true) ;
    }
};

template <int T>
void fifo_generic<T>::compute_flags() {
    FULL.write(fifo_v.read() && !POP.read());
    EMPTY.write(!fifo_v.read());
}


template <int T>
void fifo_generic<T>::reg_maj() {
    fifo_v.write(false);
    DOUT.write(0);

    wait(3);

    while (1) {
        bool pop = POP.read();
        bool push = PUSH.read();
        sc_uint<T> din = DIN.read();
        if ((pop && push) || (push && !pop && !fifo_v.read())) {
            DOUT.write(din);
            fifo_v.write(true);
        }
        else if (pop) {
            fifo_v.write(false);
        }
    }
}

