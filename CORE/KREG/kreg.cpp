#include "kreg.h"
#include<systemc.h>




void kreg::writing_csr(){
    if(RESET_N.read()){
       int address ;

        //############################## Unprivileged Address
        //##############################

       // reset of Unprivileged floating point csr :

       for(address = 0x001; address =< 0x003; address++)
       {
           CSR_adress_SK[address] = 0 ;
       }

       // reset of Unprivileged Counters/Timers

       for(address = 0xc00; address =< 0xc9f; address ++)
       {
           CSR_adress_SK[address] = 0 ;
       }

        //############################## Supervisor Level
        //##############################
       
       // reset of Supervisor Trap Setup

       for(address = 0xc00; address =< 0xc9f; address ++)
       {
           CSR_adress_SK[address] = 0 ;
       }


       // reset of Supervisor Configuration

       for(address = 0xc00; address =< 0xc9f; address ++)
       {
           CSR_adress_SK[address] = 0 ;
       }


       // reset of Supervisor Trap Handling

       for(address = 0xc00; address =< 0xc9f; address ++)
       {
           CSR_adress_SK[address] = 0 ;
       }


       // reset of Supervisor Protection and Translation


       for(address = 0xc00; address =< 0xc9f; address ++)
       {
           CSR_adress_SK[address] = 0 ;
       }


       // reset of Debug/Trace Registers

       for(address = 0xc00; address =< 0xc9f; address ++)
       {
           CSR_adress_SK[address] = 0 ;
       }


       // reset of Hypervisor Trap Setup

       for(address = 0xc00; address =< 0xc9f; address ++)
       {
           CSR_adress_SK[address] = 0 ;
       }


       // reset of Hypervisor Trap Handling

       for(address = 0xc00; address =< 0xc9f; address ++)
       {
           CSR_adress_SK[address] = 0 ;
       }


       // reset of Hypervisor Configuration

       for(address = 0xc00; address =< 0xc9f; address ++)
       {
           CSR_adress_SK[address] = 0 ;
       }


       // reset of Hypervisor Protection and Translation

       for(address = 0xc00; address =< 0xc9f; address ++)
       {
           CSR_adress_SK[address] = 0 ;
       }


       // reset of Debug/Trace Registers

       for(address = 0xc00; address =< 0xc9f; address ++)
       {
           CSR_adress_SK[address] = 0 ;
       }


       // reset of Hypervisor Counter/Timer Virtualization Registers

       for(address = 0xc00; address =< 0xc9f; address ++)
       {
           CSR_adress_SK[address] = 0 ;
       }


       // reset of Virtual Supervisor Registers

       for(address = 0xc00; address =< 0xc9f; address ++)
       {
           CSR_adress_SK[address] = 0 ;
       }


       // reset of Machine Information Registers

       for(address = 0xc00; address =< 0xc9f; address ++)
       {
           CSR_adress_SK[address] = 0 ;
       }


       // reset of Machine Trap Setup

       for(address = 0xc00; address =< 0xc9f; address ++)
       {
           CSR_adress_SK[address] = 0 ;
       }


       // reset of Machine Trap Handling

       for(address = 0xc00; address =< 0xc9f; address ++)
       {
           CSR_adress_SK[address] = 0 ;
       }


       // reset of Machine Configuration

       for(address = 0xc00; address =< 0xc9f; address ++)
       {
           CSR_adress_SK[address] = 0 ;
       }


       // reset of Machine Memory Protection

       for(address = 0xc00; address =< 0xc9f; address ++)
       {
           CSR_adress_SK[address] = 0 ;
       }


       // reset of Machine Counter/Timers

       for(address = 0xc00; address =< 0xc9f; address ++)
       {
           CSR_adress_SK[address] = 0 ;
       }


       // reset of Machine Counter Setup

       for(address = 0xc00; address =< 0xc9f; address ++)
       {
           CSR_adress_SK[address] = 0 ;
       }


       // reset of Debug/Trace Registers (shared with Debug Mode)

       for(address = 0xc00; address =< 0xc9f; address ++)
       {
           CSR_adress_SK[address] = 0 ;
       }


       // reset of Debug Mode Registers

       for(address = 0xc00; address =< 0xc9f; address ++)
       {
           CSR_adress_SK[address] = 0 ;
       }    

    }
    else{

    }
}

