#include "kreg.h"
#include<systemc.h>




void kreg::writing_csr(){
    if(RESET_N.read()){
        //must reset :
        /*
        - misa -> cf 
        - mvendorid
        - marchid & mimpid
        - mstatus
        - mtvec
        - mie & mip
        - mycle
        - minstret
        - mepc
        - mcause
        - mtval -> 0
        */

    }
    else{

    }
}

