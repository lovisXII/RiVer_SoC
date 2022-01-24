#include <systemc.h>

SC_MODULE(alu)
{
    sc_in< sc_uint<32> > op1,op2 ;
    sc_in< sc_uint<1> > cin ;
    sc_in< sc_uint<2> > cmd ;
    sc_out< sc_uint<32> > res ;
    sc_out< sc_uint<1> > cout, z, n ;
    
    void operation()
    {
        switch (cmd.read)
        {
        case "00" :
             res.write(op1.read() + op2.read()) ;
            break;
        
        case "01" :
             res.write(op1.read() & op2.read()) ;
            break;
        
        case "10" :
             res.write(op1.read() ^ op2.read()) ;
            break;
        
        case "11" :
             res.write(op1.read() | op2.read()) ;
            break;
        
        default:
            break;
        }
    }

    SC_CTOR(alu)
    {
        SC_METHODE(operation);
        sensitive << op1 << op2 << cin ;
    }
};