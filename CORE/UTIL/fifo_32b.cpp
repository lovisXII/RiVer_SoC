#include "fifo_32b.h"

void fifo_32b::function()
{
    fifo_v.write(false) ;
    data_inside.write(0) ;
    wait(3) ;
    bool push = PUSH.read() ;
    bool pop = POP.read() ;
    //when fifo_v is valide, it means that we want the data and we can pop it
    //when it's unvalaible it means we don't care about it and we can push
    
    cout << "avant boucle" << endl ;
    while(1)
    {
        cout << "apres boucle " <<endl ;
        if( fifo_v ) // if the data in the fifo is valide
        {
            if(!push && !pop)
            {
                //do nothing
            }
            else if(!push  && pop ) // when data is valid and pop is able we sent data
            {
                DOUT.write(data_inside) ;
                fifo_v.write(0) ;
            }
            else if(push  && !pop )
            {
                // do nothing
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
            cout << "test" ;
            cout << "oush :" << push <<endl ;
            cout << "pop " << pop << endl ;

            if(!push && !pop )
            {
                //do nothing
            }
            else if(!push && pop) // when data is valid and pop is able we sent data
            {
                //do nothing cause data is not valid
            }
            else if(push && !pop)
            {
                data_inside.write(DIN.read()) ;
                fifo_v.write(1) ;
                cout << "push = 1 et pop = 0" << endl ;
            }
            else if(push && pop)
            {
                // we don't pop cause data is not valid
                data_inside.write(DIN.read()) ; // we just push
                cout << "push = 1 et pop = 1" << endl ;
                fifo_v.write(1) ; // became valid
            }
        }
        if(fifo_v == 1)
        {
            FULL.write(1) ;
            EMPTY.write(0) ;
        }
        else 
        {
            FULL.write(0) ;
            EMPTY.write(1) ;
        }
        wait(1) ;
    } 
}