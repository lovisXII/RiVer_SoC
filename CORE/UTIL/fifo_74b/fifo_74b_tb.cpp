#include <systemc.h>
#include "fifo_74b.h"

using namespace std ;

int sc_main(int argc, char* argv[])
{
    //testbench signals :
    sc_signal< sc_bv<74> > din ;
    sc_clock clk("clk",1,SC_NS) ;
    sc_signal<bool> push, pop ;
    sc_signal<bool> reset_n ;

    sc_signal <bool> full, empty ;
    sc_signal< sc_bv<74> > dout ;

    //entity declaration :
    fifo_74b fifo_74b0("fifo_74b0") ;

    //Port map :
    fifo_74b0.DIN(din) ;
    fifo_74b0.CLK(clk) ;
    fifo_74b0.PUSH(push) ;
    fifo_74b0.POP(pop) ;
    fifo_74b0.RESET_N(reset_n) ;
    fifo_74b0.FULL(full) ;
    fifo_74b0.EMPTY(empty) ;
    fifo_74b0.DOUT(dout) ;

    

    reset_n.write(true) ; // reset 
    sc_start(3,SC_NS) ; // wait for 1 cycle
    reset_n.write(false) ; // end of reset
    for(int i =0 ; i < 15 ; i++)
    {
        bool rand1 ;
        bool rand2 ;
        int din_value = rand() ; 

        if(rand() % 2 == 1) rand1 = true ; 
        else rand1 = false ;
        if(rand() % 2 == 1) rand2 = true ;
        else rand2 =false ;

        din.write( din_value) ;
        push.write(rand1) ;
        pop.write( rand2)  ;
        
        sc_start(1,SC_NS) ;
        cerr   << "---------------------"<<endl ;
        if(push.read() == 1 && empty.read() == 1)//si la fifo est vide et qu'on push
        {
            if(dout.read() != din.read())
            {
                cerr << "probleme transmission data" << endl ;

            }
        }
        cerr   << "din : " << din.read() << endl ;   
        cerr   << "push : " << push.read()<<" "<<rand1 << endl ;   
        cerr   << "pop : " << pop.read()<<" "<<rand2 << endl ;   
        cerr   << "full : " << full.read() << endl ;
        cerr   << "empty : " << empty.read() << endl ;
        cerr   << "dout : " << dout.read() << endl ;
        cerr   << "fifo_v : "<< fifo_74b0.fifo_v.read() << endl ;

    }
    return 0 ;

}