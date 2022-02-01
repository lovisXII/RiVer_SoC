#include <systemc.h>

SC_MODULE(decod)
{
    sc_in < sc_uint<32> > IF_IR ;
    
    //Interface with REG :

    sc_in < sc_uint<32> > ADR1_DATA;
    sc_in < sc_uint<32> > ADR2_DATA;

    sc_out < sc_uint<6> > RADR1 ;
    sc_out < sc_uint<6> > RADR2 ;

    sc_out < sc_uint<6> > ADR_DEST ;
    void state_machine() ;
    void decoding() ;
    SC_CTOR(decode)
    {

    }
}