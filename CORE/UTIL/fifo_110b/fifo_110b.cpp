#include "fifo_110b.h"

void fifo_110b::flags_update() {
    bool push = PUSH.read() ;
    bool pop = POP.read() ;
    if( fifo_v ) // if the data in the fifo is valide
    {
        
        if(!push && !pop)
        {
            //do nothing
            FULL.write(1) ;
            EMPTY.write(0) ;
        }
        else if(!push  && pop ) // when data is valid and pop is able we sent data
        {
            FULL.write(0) ;
            EMPTY.write(1) ;
        }
        else if(push  && !pop )
        {
            FULL.write(1) ;
            EMPTY.write(0) ;
        }
        else if(push && pop )
        {
            FULL.write(1) ;
            EMPTY.write(0) ;
        }
    }
    else // case where data inside the fifo is not valid
    {

        if(!push && !pop )
        {
            FULL.write(0) ;
            EMPTY.write(1) ;
            //do nothing
        }
        else if(!push && pop) // when data is valid and pop is able we sent data
        {
            FULL.write(0) ;
            EMPTY.write(1) ;
            //do nothing cause data is not valid
        }
        else if(push && !pop)
        {
            FULL.write(1) ;
            EMPTY.write(0) ;
        }
        else if(push && pop)
        {
            FULL.write(1) ;
            EMPTY.write(0) ;
        }
    }
}

void fifo_110b::function()
{
    fifo_v.write(false) ;
    data_inside.write(0) ;
    FULL.write(0) ;
    EMPTY.write(1) ;

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

                DOUT.write(data_inside) ;
                fifo_v.write(0) ;
            }
            else if(push  && !pop )
            {
                DOUT.write(data_inside) ;
            }
            else if(push && pop )
            {
                DOUT.write(data_inside) ;
                data_inside.write(DIN.read()) ;
                fifo_v.write(1) ; // stay valid
            }
        }
        else // case where data inside the fifo is not valid
        {

            if(push && !pop)
            {
                data_inside.write(DIN.read()) ;
                DOUT.write(DIN.read()) ;
                
                fifo_v.write(1) ;
            }
            else if(push && pop)
            {
                data_inside.write(DIN.read()) ; // we just push
                DOUT.write(DIN.read()) ;

                fifo_v.write(1) ; // became valid
            }
        }

        wait(1) ;
    } 
}

void fifo_110b::trace(sc_trace_file* tf) {
        sc_trace(tf, CLK, GET_NAME(CLK));
        sc_trace(tf, RESET_N, GET_NAME(RESET_N));
        sc_trace(tf, DIN, GET_NAME(DIN));
        sc_trace(tf, PUSH, GET_NAME(PUSH));
        sc_trace(tf, POP, GET_NAME(POP));
        sc_trace(tf, DIN, GET_NAME(FULL));
        sc_trace(tf, DIN, GET_NAME(EMPTY));
        sc_trace(tf, DIN, GET_NAME(DOUT));
        sc_trace(tf, DIN, GET_NAME(fifo_v));
        sc_trace(tf, DIN, GET_NAME(data_inside));
}