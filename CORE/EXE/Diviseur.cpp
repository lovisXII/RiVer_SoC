#include "Diviseur.h"

void Diviseur::new_state()
{
    current_state = next_state;

    shift_cpt_re = shift_cpt_se;

    divisor_re = divisor_se;
    quotient_re = quotient_se;
    reminder_re = reminder_se;

    /*cout << "End of cycle: "
         << "   Divisor:   " << divisor_re.read()
         << "   Quotient:  " << quotient_re.read()
         << "   reminder: " << reminder_re.read() 
         << "   shift:  "<<shift_cpt_re.read()
         << "   next state: "<<current_state<< endl << endl;*/
}
void Diviseur::state_transition()
{
    switch(current_state.read())
    {
        case 0:
            if(START_SE)
            {
                next_state = 0b001;
            }
            else
            {
                next_state = 0b000;
            }
        break;
        case 1:
            if(shift_cpt_re.read() == 31)
                next_state = 0b000;
            else    
                next_state = 0b001;          
        break;
    }
}
void Diviseur::mae_output()
{
    //cout << "CURRENT STATE: "<<current_state<<endl;
    switch(current_state.read())
    {
        case 0:// init/idle
            if(START_SE)
            {
                sc_uint<32> op2 = OP2_SE.read();
                sc_uint<32> op1 = OP1_SE.read();

                sign_reg_se = op2[31] ^ op1[31];

                if(op1[31])
                    op1 = (~op1) + 1;
                
                if(op2[31])
                    op2 = (~op2) + 1;

                sc_bv<64> div = 0;
                div.range(62, 31) = op2; 
                divisor_se = div;

                quotient_se = 0;

                reminder_se = (sc_biguint<64>)op1;
                BUSY_SE.write(true);
            }
            else
            {
                BUSY_SE.write(false);
            }

            shift_cpt_se = 0;
            DONE_SE.write(shift_cpt_re.read() == 32);
        break;
        case 1: // calculating
            if(shift_cpt_re.read() <= 31)
            {
                if(divisor_re.read() > reminder_re.read())
                {
                    ////cout << "    # divisor > reminder" << endl << endl;
                    divisor_se = divisor_re.read() >> 1;

                    sc_uint<64> quo = quotient_re.read(); 
                    quo <<= 1;
                    quo[0] = 0;
                    quotient_se = quo;
                }
                else
                {
                    ////cout << "    # divisor <= reminder" << endl << endl;
                    reminder_se = reminder_re.read() - divisor_re.read();
                    
                    sc_uint<64> quo = quotient_re.read(); 
                    quo <<= 1;
                    quo[0] = 1;
                    quotient_se = quo;

                    divisor_se = divisor_re.read() >> 1;

                }
                /*cout << "  -->  divisor = "<< divisor_re.read() 
                 << "   quotient = " << quotient_re.read()
                 << "   reminder = " << reminder_re.read() << endl;*/
                shift_cpt_se = shift_cpt_re.read() + 1;
            }
        break;
    }
}
void Diviseur::RET()
{
    if(CMD_RD.read() == 3 || CMD_RD.read() == 0)
    {
        sc_uint<32> rem = (sc_bv_base)((sc_bv<64>)reminder_re.read()).range(31, 0);
        if(sign_reg_se)
            rem = (~rem)+1;

        DIVIDER_RES_OUTPUT.write(rem);
    }
    else
    {
        sc_uint<32> quo = quotient_re.read();
        if(sign_reg_se)
            quo = (~quo) + 1;

        DIVIDER_RES_OUTPUT.write(quo);
    }
}

void Diviseur::trace(sc_trace_file * tf)
{
    sc_trace(tf, OP1_SE, GET_NAME(OP1_SE));
    sc_trace(tf, OP2_SE, GET_NAME(OP2_SE));
    sc_trace(tf, DIVIDER_RES_OUTPUT, GET_NAME(DIVIDER_RES_OUTPUT));
    sc_trace(tf, BUSY_SE, GET_NAME(BUSY_SE));
    sc_trace(tf, DONE_SE, GET_NAME(DONE_SE));
    sc_trace(tf, START_SE, GET_NAME(START_SE));
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, divisor_re, GET_NAME(divisor_re));
    sc_trace(tf, quotient_re, GET_NAME(quotient_re));
    sc_trace(tf, reminder_re, GET_NAME(reminder_re));
    sc_trace(tf, divisor_se, GET_NAME(divisor_se));
    sc_trace(tf, quotient_se, GET_NAME(quotient_se));
    sc_trace(tf, reminder_se, GET_NAME(reminder_se));
    sc_trace(tf, current_state, GET_NAME(current_state));
    sc_trace(tf, next_state, GET_NAME(next_state));
    sc_trace(tf, shift_cpt_se, GET_NAME(shift_cpt_se));
    sc_trace(tf, shift_cpt_re, GET_NAME(shift_cpt_re));
    sc_trace(tf, sign_reg_se, GET_NAME(sign_reg_se));
}