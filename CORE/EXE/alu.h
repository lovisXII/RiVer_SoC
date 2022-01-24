#include <systemc.h>
#include <iostream>
#include <string>



SC_MODULE(alu)
{
    sc_in< sc_uint<32> >  op1,op2 ;
    sc_in< bool > cin ;
    sc_in< sc_uint<2> > cmd ;
    sc_out< sc_uint<32> > res ;
    sc_out< bool > cout, z, n ;
    
    void operation() ;

    SC_CTOR(alu)
    {
        SC_METHOD(operation);
        sensitive << op1 << op2 << cin ;
    }
};

