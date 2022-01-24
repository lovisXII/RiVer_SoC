#include "alu.h"

int sc_main(int argc, char* argv[])
{
    sc_signal< sc_uint<32> > op1, op2, res ;
    sc_signal< sc_uint<2> > cmd ;
    sc_signal< bool > cin, cout ;
    alu unit_alu("alu") ;
    unit_alu.op1(op1) ;
    unit_alu.op2(op2) ;
    unit_alu.cmd(cmd) ;
    unit_alu.cin(cin) ;
    unit_alu.cout(cout) ;
    unit_alu.res(res) ;
    sc_start(1, SC_NS) ;
    for(int i = 0 ; i < 1000 ; i++)
    {
        int command = rand() % 3 ;
        int operand1, operand2 ;
        operand1 = rand() % 1000 ;
        operand2 = rand() % 1000 ;
        int result ;
        int carry = rand()%1 ;
        switch (command)
        {
        case 0 :
            result = operand1 + operand2 + carry ;
            break;
        case 1 :
            result = operand1 & operand2 ;
            break;
        case 2 :
            result = operand1 | operand2 ;
            break;
        case 3 : 
            result = operand1 ^ operand2 ;
            break;
        }
        op1 = operand1 ;
        op2 = operand2 ;
        cmd = command ;
        cin = carry ;
        sc_start(1,SC_NS);
    
    }
    printf("ok ta mere\n") ;
    return 0 ;
}