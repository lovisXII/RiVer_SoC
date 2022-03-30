#include "buffercache.h"
#include <systemc.h>


void buffercache::write()
{
    if(buff0_VALIDATE)
    {
        buff1_DATA.write(DATA_C);
        buff1_DATA_ADR.write(ADR_C);
        buff1_STORE.write(STORE_C);
        buff1_VALIDATE.write(true);

        buffer_LRU.write(false);
    }
    else
    {
        buff0_DATA.write(DATA_C);
        buff0_DATA_ADR.write(ADR_C);
        buff0_STORE.write(STORE_C);
        buff0_VALIDATE.write(true);

        buffer_LRU.write(true);
    }
}

void buffercache::read()
{
    if(buff0_VALIDATE && buffer_LRU)
    {
        DATA_MP.write(buff0_DATA);
        ADR_MP.write(buff0_DATA_ADR);
        STORE_MP.write(buff0_STORE);

        buff0_VALIDATE.write(false);
    }
    else if(buff1_VALIDATE && !buffer_LRU)
    {
        DATA_MP.write(buff1_DATA);
        ADR_MP.write(buff1_DATA_ADR);
        STORE_MP.write(buff1_STORE);

        buff1_VALIDATE.write(false);
    }
}

void buffercache::bufferfull()
{
    FULL.write(buff1_VALIDATE & buff0_VALIDATE);
    EMPTY.write(!buff1_VALIDATE & !buff0_VALIDATE);
}