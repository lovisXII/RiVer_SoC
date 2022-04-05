#include <systemc.h>
#include "fifo_110b.h"

using namespace std ;

int sc_main(int argc, char* argv[])
{
    //testbench signals :
    sc_signal<sc_bv<110>> din ;
    sc_clock clk("clk",1,SC_NS) ;
    sc_signal<bool> push, pop ;
    sc_signal<bool> reset_n ;

    sc_signal<bool> full, empty ;
    sc_signal<sc_bv<110>> dout ;

    //entity declaration :
    fifo_110b fifo_110b0("fifo_110b0") ;

    //Port map :
    fifo_110b0.DIN(din) ;
    fifo_110b0.CLK(clk) ;
    fifo_110b0.PUSH(push) ;
    fifo_110b0.POP(pop) ;
    fifo_110b0.RESET_N(reset_n) ;
    fifo_110b0.FULL(full) ;
    fifo_110b0.EMPTY(empty) ;
    fifo_110b0.DOUT(dout) ;

    

    reset_n.write(false) ; // reset 
    sc_start(3,SC_NS) ; // wait for 1 cycle
    reset_n.write(true) ; // end of reset
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
        cerr   << "fifo_v : "<< fifo_110b0.fifo_v.read() << endl ;

    }
    return 0 ;

}