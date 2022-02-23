#include "fifo_32b.h"


void fifo_32b::flags_update() {
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

        if(push)
        {
            FULL.write(0) ;
            EMPTY.write(0) ;
        }
        else
        {
            FULL.write(0) ;
            EMPTY.write(1) ;
        }
    }

    DOUT.write(data_inside) ;
}

void fifo_32b::function()
{
    fifo_v.write(false) ;
    data_inside.write(0) ;

    wait(3) ;

    //when fifo_v is valide, it means that we want the data and we can pop it
    //when it's unvalaible it means we don't care about it and we can push
    
    while(1)
    {
        bool push = PUSH.read() ;
        bool pop = POP.read() ;
        if( fifo_v ) // if the data in the fifo is valide
        {
            if(!push  && pop ) // when data is valid and pop is able we sent data
            {
                fifo_v.write(0) ;
            }
            else if(push && pop )
            {
                data_inside.write(DIN.read()) ;
                fifo_v.write(1) ; // stay valid
            }
        }
        else // case where data inside the fifo is not valid
        {
            if(push)
            {
                data_inside.write(DIN.read()) ;                
                fifo_v.write(1) ;
            }
        }

        wait(1) ;
    } 
}

void fifo_32b::trace(sc_trace_file* tf) {
        sc_trace(tf, CLK, GET_NAME(CLK));
        sc_trace(tf, RESET_N, GET_NAME(RESET_N));
        sc_trace(tf, DIN, GET_NAME(DIN));
        sc_trace(tf, PUSH, GET_NAME(PUSH));
        sc_trace(tf, POP, GET_NAME(POP));
        sc_trace(tf, FULL, GET_NAME(FULL));
        sc_trace(tf, EMPTY, GET_NAME(EMPTY));
        sc_trace(tf, DOUT, GET_NAME(DOUT));
        sc_trace(tf, fifo_v, GET_NAME(fifo_v));
        sc_trace(tf, data_inside, GET_NAME(data_inside));
}