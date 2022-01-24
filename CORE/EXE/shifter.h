#include <systemc.h>
#include <iostream>
#include <string>



SC_MODULE(shifter)
{
    sc_in< sc_uint<32> >  DIN ;
    sc_in< sc_uint<5> >  SHIFT_VAL ;
    sc_in< bool > CIN, SLL, SRL, SRA ;
    sc_out< sc_uint<32> > DOUT ;
    sc_out< bool > COUT ;

    sc_signal< sc_uint<32> > SLL_DOUT, SR_DOUT;
    sc_signal< bool > SLL_COUT, SR_COUT;
    
    void shifter_sll() ;
    void shifter_sr() ;
    void shifter_agregate() ;

    SC_CTOR(shifter)
    {
        SC_METHOD(shifter_sll);
        sensitive << DIN << CIN << SHIFT_VAL ;
        SC_METHOD(shifter_sr);
        sensitive << DIN << CIN << SHIFT_VAL << SRA ;
        SC_METHOD(shifter_agregate);
        sensitive << SLL_DOUT << SR_DOUT << SLL_COUT << SR_COUT << SLL << SRL << SRA ;
    }
};

