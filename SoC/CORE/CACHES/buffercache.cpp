#include "buffercache.h"
#include <systemc.h>


void buffercache::fifo()
{
    //WRITE_OBUFF ON BUFFER
    if(!CLK.read())
    {
        if(WRITE_OBUFF.read())
        {
            if(!buff0_VALIDATE && !buff1_VALIDATE)
            {
                if(buffer_choice.read())
                {
                    buff1_DATA.write(DATA_C.read());
                    buff1_DATA_ADR.write(ADR_C.read());
                    buff1_STORE.write(STORE_C.read());
                    buff1_LOAD.write(LOAD_C.read());
                    buff1_VALIDATE.write(STORE_C.read() || LOAD_C.read());
                    buff1_SIZE.write(SIZE_C.read());
                }
                else
                {
                    buff0_DATA.write(DATA_C.read());
                    buff0_DATA_ADR.write(ADR_C.read());
                    buff0_STORE.write(STORE_C.read());
                    buff0_LOAD.write(LOAD_C.read());
                    buff0_VALIDATE.write(STORE_C.read() || LOAD_C.read());
                    buff0_SIZE.write(SIZE_C.read());
                }
            }
            else if(!buff0_VALIDATE)
            {
                buff0_DATA.write(DATA_C.read());
                buff0_DATA_ADR.write(ADR_C.read());
                buff0_STORE.write(STORE_C.read());
                buff0_LOAD.write(LOAD_C.read());
                buff0_VALIDATE.write(STORE_C.read() || LOAD_C.read());
                buff0_SIZE.write(SIZE_C.read());
            }
            else if(!buff1_VALIDATE)
            {
                buff1_DATA.write(DATA_C.read());
                buff1_DATA_ADR.write(ADR_C.read());
                buff1_STORE.write(STORE_C.read());
                buff1_LOAD.write(LOAD_C.read());
                buff1_VALIDATE.write(STORE_C.read() || LOAD_C.read());
                buff1_SIZE.write(SIZE_C.read());
            }
        }
    }
    else
    {
        if(ACK.read())
        {
            if(busreq_we)
            {
                if(buffer_choice.read())
                    buff1_VALIDATE = false;
                else
                    buff0_VALIDATE = false;
            }
            else
                wait_for_ack_falling_edge = true;
        }
        else if(!ACK.read() && wait_for_ack_falling_edge)
        {
            wait_for_ack_falling_edge = false;
            if(buffer_choice.read())
                    buff1_VALIDATE = false;
                else
                    buff0_VALIDATE = false;
        }
    }
}
void buffercache::write_output()
{
    if(buffer_choice.read() && buff1_VALIDATE.read())
    {
        DATA_MP.write(buff1_DATA.read());
        ADR_MP.write(buff1_DATA_ADR.read());
        STORE_MP.write(buff1_STORE.read());
        LOAD_MP.write(buff1_LOAD.read());
        SIZE_MP.write(buff1_SIZE.read());

        busreq_we = buff1_STORE;
    }
    else if(!buffer_choice.read() && buff0_VALIDATE.read())
    {
        DATA_MP.write(buff0_DATA.read());
        ADR_MP.write(buff0_DATA_ADR.read());
        STORE_MP.write(buff0_STORE.read());
        LOAD_MP.write(buff0_LOAD.read());
        SIZE_MP.write(buff0_SIZE.read());

        busreq_we = buff0_STORE;
    }
    else
    {
        STORE_MP.write(false);
        LOAD_MP.write(false);
    }
}
void buffercache::bufferfull()
{
    FULL.write(buff1_VALIDATE & buff0_VALIDATE);
    EMPTY.write(!buff1_VALIDATE & !buff0_VALIDATE);
}
void buffercache::choice_buff()
{
    //READ ON BUFFER
    bool bc = buffer_choice.read(); 
    if(!RESET_N.read())
    {
        bc = 0;
    }
    else
    {
        if(ACK.read() && busreq_we)
        {
            bc = !buffer_choice.read();
        }
        else if(!ACK.read() && wait_for_ack_falling_edge)
        {
            bc = !buffer_choice.read();
        }
    }
    buffer_choice = bc;
}
void buffercache::trace(sc_trace_file* tf)
{
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET_N, GET_NAME(RESET_N));

    //INPUT
    sc_trace(tf, WRITE_OBUFF, GET_NAME(WRITE_OBUFF));
    sc_trace(tf, ACK, GET_NAME(ACK));
    sc_trace(tf, DATA_C, GET_NAME(DATA_C));
    sc_trace(tf, ADR_C, GET_NAME(ADR_C));
    sc_trace(tf, STORE_C, GET_NAME(STORE_C));
    sc_trace(tf, LOAD_C, GET_NAME(STORE_C));

    //OUTPUT
    sc_trace(tf, FULL, GET_NAME(FULL));
    sc_trace(tf, EMPTY, GET_NAME(EMPTY));
    sc_trace(tf, DATA_MP, GET_NAME(DATA_MP));
    sc_trace(tf, ADR_MP, GET_NAME(ADR_MP));
    sc_trace(tf, STORE_MP, GET_NAME(STORE_MP));
    sc_trace(tf, LOAD_MP, GET_NAME(LOAD_MP));

    //signals

    sc_trace(tf, buff0_DATA, GET_NAME(buff0_DATA));
    sc_trace(tf, buff0_DATA_ADR, GET_NAME(buff0_DATA_ADR));
    sc_trace(tf, buff0_STORE, GET_NAME(buff0_STORE));
    sc_trace(tf, buff0_LOAD, GET_NAME(buff0_LOAD));
    sc_trace(tf, buff0_VALIDATE, GET_NAME(buff0_VALIDATE));
    sc_trace(tf, buff0_SIZE, GET_NAME(buff0_SIZE));

    sc_trace(tf, buff1_DATA, GET_NAME(buff1_DATA));
    sc_trace(tf, buff1_DATA_ADR, GET_NAME(buff1_DATA_ADR));
    sc_trace(tf, buff1_STORE, GET_NAME(buff1_STORE));
    sc_trace(tf, buff1_LOAD, GET_NAME(buff1_LOAD));
    sc_trace(tf, buff1_VALIDATE, GET_NAME(buff1_VALIDATE));
    sc_trace(tf, buff1_SIZE, GET_NAME(buff1_SIZE));

    sc_trace(tf, buffer_choice, GET_NAME(buffer_choice));

    sc_trace(tf, busreq_we, GET_NAME(busreq_we));
    sc_trace(tf, wait_for_ack_falling_edge, GET_NAME(wait_for_ack_falling_edge));

}