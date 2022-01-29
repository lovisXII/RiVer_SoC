#include "fifo_74b.h"

void fifo_74b::function()
{
    fifo_v.write(false) ;
    data_inside.write(0) ;
    FULL.write(0) ;
    EMPTY.write(1) ;

    wait(3) ;
    cerr << "###############################################" << endl ;
    cerr << "end of fifo reset" << endl ;
    cerr << "###############################################" << endl ;
 
    //when fifo_v is valide, it means that we want the data and we can pop it
    //when it's unvalaible it means we don't care about it and we can push
    
    while(1)
    {
        bool push = PUSH.read() ;
        bool pop = POP.read() ;
        if( fifo_v ) // if the data in the fifo is valide
        {
            
            if(!push && !pop)
            {
                //do nothing
                FULL.write(1) ;
                EMPTY.write(0) ;
                DOUT.write(data_inside) ; // si on a une valeur dans la fifo la sortie est toujours la valeur interne
            }
            else if(!push  && pop ) // when data is valid and pop is able we sent data
            {
                FULL.write(0) ;
                EMPTY.write(1) ;

                DOUT.write(data_inside) ;
                fifo_v.write(0) ;
            }
            else if(push  && !pop )
            {
                //la nouvelle donnée ne peut pas etre push 
                FULL.write(1) ;
                EMPTY.write(0) ;
                DOUT.write(data_inside) ;
            }
            else if(push && pop )
            {
                FULL.write(1) ;
                EMPTY.write(0) ;
                DOUT.write(data_inside) ;
                data_inside.write(DIN.read()) ;
                fifo_v.write(1) ; // stay valid
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

                data_inside.write(DIN.read()) ;
                DOUT.write(DIN.read()) ;
                
                fifo_v.write(1) ;
            }
            else if(push && pop)
            {
                FULL.write(1) ;
                EMPTY.write(0) ;
                
                
                data_inside.write(DIN.read()) ; // we just push
                DOUT.write(DIN.read()) ;
                fifo_v.write(1) ; // became valid
            }
        }

        wait(1) ;
    } 
}