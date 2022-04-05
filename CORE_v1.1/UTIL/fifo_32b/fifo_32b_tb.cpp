#include <systemc.h>
#include "fifo_32b.h"

using namespace std ;

int sc_main(int argc, char* argv[])
{
    //testbench signals :
    sc_signal<sc_uint<32>> din ;
    sc_clock clk("clk",1,SC_NS) ;
    sc_signal<bool> push, pop ;
    sc_signal<bool> reset_n ;

    sc_signal<bool> full, empty ;
    sc_signal<sc_uint<32>> dout ;

    //entity declaration :
    fifo_32b fifo_32b0("fifo_32b0") ;

    //Port map :
    fifo_32b0.DIN(din) ;
    fifo_32b0.CLK(clk) ;
    fifo_32b0.PUSH(push) ;
    fifo_32b0.POP(pop) ;
    fifo_32b0.RESET_N(reset_n) ;
    fifo_32b0.FULL(full) ;
    fifo_32b0.EMPTY(empty) ;
    fifo_32b0.DOUT(dout) ;

    

    reset_n.write(false) ; // reset 
    sc_start(1,SC_NS) ; // wait for 1 cycle
    reset_n.write(true) ; // end of reset
    for(int i =0 ; i < 15 ; i++)
    {
        bool rand1 ;
        bool rand2 ;
        if(rand() % 2 == 1)
        {
            rand1 = true ;
        } 
        else
        {
            rand1 = false ;
        }
        if(rand() % 2 == 1) rand2 = true ;
        else rand2 =false ;
        din.write( rand()) ;
        push.write(rand1) ;
        pop.write( rand2)  ;
        sc_start(1,SC_NS) ;
        cout  << "---------------------"<<endl ;
        cout  << "din : " 
			<< din.read() 
			<< endl ;   
        cout  << "push : " 
			<< push.read()<<" "<<rand1 
			<< endl ;   
        cout  << "pop : " 
			<< pop.read()<<" "<<rand2 
			<< endl ;   
        cout  << "full : " 
			<< full.read() 
			<< endl ;
        cout  << "empty : " 
			<< empty.read() 
			<< endl ;
        cout  << "dout : " 
			<< dout.read() 
			<< endl ;
        cout  << "fifo_v : "
			<< fifo_32b0.fifo_v.read() 
			<< endl ;

    }
    return 0 ;

}