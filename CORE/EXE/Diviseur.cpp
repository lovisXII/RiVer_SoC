#include "Diviseur.h"

void Diviseur::preprocess()
{
    if(!busy_se)
    {
        remainder_re = (sc_biguint<64>)OP1_SE.read();

        sc_bv<64> div = 0;
        div.range(63, 32) = OP2_SE.read(); 
        divisor_re = div;

        quotient_re = 0;
    }
    else
    {
        divisor_re = divisor_se;
        quotient_re = quotient_se;
        remainder_re = remainder_se;
    }
}
void Diviseur::control()
{
    current_state = next_state;
    
    switch(current_state.read())
    {
        case 0:
            busy_se = SELECT_TYPE_OPERATIONS_RD.read() == 0b1000;
            if(busy_se)
            {
                cout << "starting  divisor = "<< divisor_re.read() 
                     << "   quotient = " << quotient_re.read()
                     << "   remainder = " << remainder_re.read() << endl;
                divisor_se = divisor_re.read() >> 1;

                sc_uint<64> quo = quotient_re.read(); 
                quo <<= 1;
                quo[0] = 0;
                quotient_se = quo;

                remainder_se = remainder_re;

                next_state = 1;
                cout << "  new div go to next state" << endl << endl;;
            }
            else
            {
                cout << "  nothing new -> loop state" << endl << endl;;
                next_state = 0;
                done_se = false;
            }
            shift_cpt_se = 0;
            cout<<"  cu: "<<current_state<<"  ne: "<<next_state<<endl;
        break;
        case 1:
            if(shift_cpt_se.read() < 32)
            {   
                cout << "  divisor = "<< divisor_re.read() 
                 << "   quotient = " << quotient_re.read()
                 << "   remainder = " << remainder_re.read() << endl;
                if(divisor_se.read() > remainder_se.read())
                {
                    cout << "    # divisor > remainder" << endl << endl;;
                    divisor_se = divisor_re.read() >> 1;

                    sc_uint<64> quo = quotient_re.read(); 
                    quo <<= 1;
                    quo[0] = 0;
                    quotient_se = quo;

                    next_state = 1;
                }
                else
                {
                    cout << "    # divisor <= remainder" << endl << endl;;
                    remainder_se = remainder_re.read() - divisor_re.read();
                    
                    sc_uint<64> quo = quotient_re.read(); 
                    quo <<= 1;
                    quo[0] = 1;
                    quotient_se = quo;

                    divisor_se = divisor_re.read() >> 1;

                    next_state = 1;
                }

                shift_cpt_se = shift_cpt_se.read() + 1;
            }
            else
            {
                cout << "> END of division" << endl << endl;
                busy_se = false;
                done_se = true;

                next_state = 0;
                cout << "next: "<<next_state<<endl;
                if(EXE_CMD_RD.read() == 0 || EXE_CMD_RD.read() == 3)
                {
                    sc_bv<64> rem = remainder_re.read();
                    DIV_RES_SE.write((sc_bv_base)rem.range(31, 0));
                }
                else
                {
                    DIV_RES_SE.write(quotient_re.read());
                }
            }
            cout << "       shifting => "<<shift_cpt_se.read()<<"  cu: "<<current_state<<"  ne: "<<next_state<<endl;
        break;
    }

    BUSY_SE.write(busy_se);
    DONE_SE.write(done_se);
}