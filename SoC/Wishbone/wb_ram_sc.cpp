#include "wb_ram_sc.h"

void wb_ram_sc::new_state()
{
    if(!RESET_N)
        current_state = SLAVE_IDLE;
    else
        current_state = future_state;
}
void wb_ram_sc::state_transition()
{
    switch(current_state.read())
    {
        case SLAVE_IDLE:
            if(STB_I.read() & CYC_I.read() & ARBITER_SEL_I.read())
                future_state = SLAVE_BUS_REQUEST;
            else
                future_state = SLAVE_IDLE;
        break;
        case SLAVE_BUS_REQUEST:
            if(WE_I)
                future_state = SLAVE_IDLE;
            else
                future_state = SLAVE_MODULE_WAIT_D0;
        break;
        case SLAVE_MODULE_WAIT_D0:
            future_state = SLAVE_MODULE_WAIT_D1;
        break;
        case SLAVE_MODULE_WAIT_D1:
            future_state = SLAVE_MODULE_WAIT_D2;
        break;
        case SLAVE_MODULE_WAIT_D2:
            future_state = SLAVE_MODULE_WAIT_D3;
        break;
        case SLAVE_MODULE_WAIT_D3:
            future_state = SLAVE_IDLE;
        break;
    }
}
void wb_ram_sc::mae_output()
{
    sc_uint<32> tmp;
    switch(current_state.read())
    {
        case SLAVE_IDLE:
            ACK_O = false;
            RAM_DTA_RDY_O = false;
        break;
        case SLAVE_BUS_REQUEST:
            RAM_WE_O = WE_I;
            
            if(WE_I)
            {
                RAM_DT_O = DAT_I;
                RAM_SEL_O = SEL_I;
                RAM_ADR_O = ADR_I.read();
            }
            else
            {
                tmp = ADR_I.read() & 0xFFFFFFFC;
                ADR_REG = tmp;
                RAM_ADR_O = tmp;
            }
            RAM_DTA_RDY_O = true;
        break;
        case SLAVE_MODULE_WAIT_D0:
            ACK_O = true;
            tmp = ADR_REG.read() + 4;
            RAM_ADR_O = tmp;
            ADR_REG = tmp;
            RAM_DTA_RDY_O = true;
            DAT_O = RAM_DT_I;
        break;
        case SLAVE_MODULE_WAIT_D1:
            ACK_O = true;
            tmp = ADR_REG.read() + 4;
            RAM_ADR_O = tmp;
            ADR_REG = tmp;
            RAM_DTA_RDY_O = true;
            DAT_O = RAM_DT_I;
        break;
        case SLAVE_MODULE_WAIT_D2:
            ACK_O = true;
            tmp = ADR_REG.read() + 4;
            RAM_ADR_O = tmp;
            ADR_REG = tmp;
            RAM_DTA_RDY_O = true;
            DAT_O = RAM_DT_I;
        break;
        case SLAVE_MODULE_WAIT_D3:
            ACK_O = true;
            RAM_DTA_RDY_O = true;
            DAT_O = RAM_DT_I;
        break;
    }
    std::cout << sc_time_stamp() << "   " << ADR_REG << std::endl;
}

void wb_ram_sc::trace(sc_trace_file* tf)
{
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET_N, GET_NAME(RESET_N));

    sc_trace(tf, DAT_I, GET_NAME(DAT_I));
    sc_trace(tf, ADR_I, GET_NAME(ADR_I));
    sc_trace(tf, CYC_I, GET_NAME(CYC_I));
    sc_trace(tf, SEL_I, GET_NAME(SEL_I));
    sc_trace(tf, STB_I, GET_NAME(STB_I));
    sc_trace(tf, WE_I, GET_NAME(WE_I));
    sc_trace(tf, DAT_O, GET_NAME(DAT_O));
    sc_trace(tf, ACK_O, GET_NAME(ACK_O));
    sc_trace(tf, RAM_DTA_RDY_O, GET_NAME(RAM_DTA_RDY_O));
    sc_trace(tf, RAM_DT_I, GET_NAME(RAM_DT_I));
    sc_trace(tf, RAM_DT_O, GET_NAME(RAM_DT_O));
    sc_trace(tf, RAM_ADR_O, GET_NAME(RAM_ADR_O));
    sc_trace(tf, RAM_WE_O, GET_NAME(RAM_WE_O));
    sc_trace(tf, RAM_SEL_O, GET_NAME(RAM_SEL_O));
    sc_trace(tf, ARBITER_SEL_I, GET_NAME(ARBITER_SEL_I));
    sc_trace(tf, current_state, GET_NAME(current_state));
    sc_trace(tf, future_state, GET_NAME(future_state));
}