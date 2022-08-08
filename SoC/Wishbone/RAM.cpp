#include "RAM.h"


void RAM::init_mem(std::unordered_map<int,int>* tab)
{
    RAM_REGISTERS = tab;
}
void RAM::reponse()
{
    if(VALID_I)
    {
        if(WE_I.read())
        {
            int temporary_value = (*RAM_REGISTERS)[ADR_I.read()];
            unsigned int temporary_store_value = DAT_I.read();
            if(MEM_SIZE.read() == 2)//access in byte
            {
                int mask_adr = ADR_I.read() & 0x00000003 ;
                switch (mask_adr)
                {
                case 0 :
                    temporary_store_value = temporary_store_value & 0x000000FF ;
                    temporary_value = 0xFFFFFF00 & temporary_value ;
                    (*RAM_REGISTERS)[ADR_I.read()] = temporary_value | temporary_store_value ;
                    break ;
                case 1 :
                    temporary_store_value = temporary_store_value & 0x0000FF00 ;
                    temporary_value = 0xFFFF00FF & temporary_value ;
                    (*RAM_REGISTERS)[ADR_I.read()] = temporary_value | temporary_store_value ;
                    break ;
                case 2 :
                    temporary_store_value = temporary_store_value & 0x00FF0000 ;
                    temporary_value = 0xFF00FFFF & temporary_value ;
                    (*RAM_REGISTERS)[ADR_I.read()] = temporary_value | temporary_store_value ;
                    break ;
                case 3 :      
                    temporary_store_value = temporary_store_value & 0xFF000000 ;
                    temporary_value = 0x00FFFFFF & temporary_value ;
                    (*RAM_REGISTERS)[ADR_I.read()] = temporary_value | temporary_store_value ;
                    break ;
                default:
                    break;
                }
            }
            else if(MEM_SIZE.read() == 1){//access in half word
                int mask_adr = ADR_I.read() & 0x00000003 ;
                switch (mask_adr)
                {
                case 0 :
                    temporary_store_value = temporary_store_value & 0x0000FFFF ;
                    temporary_value = 0xFFFF0000 & temporary_value ;
                    (*RAM_REGISTERS)[ADR_I.read()] = temporary_value | temporary_store_value ;
                    break ;
                case 2 :          
                    temporary_store_value = temporary_store_value & 0xFFFF0000 ;
                    temporary_value = 0x00000FFFF & temporary_value ;
                    (*RAM_REGISTERS)[ADR_I.read()] = temporary_value | temporary_store_value ;
                    break ;      
                default:
                    break;
                }
            }
            else//access in word
            {
                (*RAM_REGISTERS)[ADR_I.read()] = temporary_store_value;
            }
        }
        else
        {
            DAT_O = (*RAM_REGISTERS)[ADR_I.read()];
        }
    }
}
void RAM::trace(sc_trace_file* tf)
{
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET_N, GET_NAME(RESET_N));
    sc_trace(tf, ADR_I, GET_NAME(ADR_I));
    sc_trace(tf, DAT_I, GET_NAME(DAT_I));
    sc_trace(tf, VALID_I, GET_NAME(VALID_I));
    sc_trace(tf, WE_I, GET_NAME(WE_I));
    sc_trace(tf, MEM_SIZE, GET_NAME(MEM_SIZE));
    sc_trace(tf, DAT_O, GET_NAME(DAT_O));
    sc_trace(tf, current_state, GET_NAME(current_state));
    sc_trace(tf, future_state, GET_NAME(future_state));
}