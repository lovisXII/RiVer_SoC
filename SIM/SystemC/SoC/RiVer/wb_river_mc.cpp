#include "wb_river_mc.h"

void wb_river_mc::new_state()
{
    if(!RESET_N)
        current_state = R_IDLE;
    else
        current_state = future_state;
}
void wb_river_mc::state_transition()
{
    switch(current_state.read())
    {
        case R_IDLE:
            if(DTA_VALID_DC | DTA_VALID_IC)
                future_state = R_REQ;
            else
                future_state = R_IDLE;
        break;
        case R_REQ:
            if(GRANT_I & DTA_VALID_DC & READ_DC)
                future_state = R_DC_READ;
            else if(GRANT_I & DTA_VALID_DC & WRITE_DC)
                future_state = R_DC_WRITE;
            else if(GRANT_I & DTA_VALID_IC & !DTA_VALID_DC)
                future_state = R_IC;
            else
                future_state = R_REQ;
        break;
        case R_DC_READ:
            if(ACK_I)
                future_state = R_DC_END_BURST;
            else
                future_state = R_DC_READ;
        break;
        case R_DC_END_BURST:
            if(!ACK_I & DTA_VALID_IC)
                future_state = R_IC;
            else if(!ACK_I & !DTA_VALID_IC)
                future_state = R_IDLE;
            else
                future_state = R_DC_END_BURST;
        break;
        case R_DC_WRITE:
            if(ACK_I & DTA_VALID_IC)
                future_state = R_IC;
            else if(ACK_I & !DTA_VALID_IC)
                future_state = R_IDLE;
            else
                future_state = R_DC_WRITE;
        break;
        case R_IC:
            if(ACK_I)
                future_state = R_IC_END_BURST;
            else
                future_state = R_IC;
        break;
        case R_IC_END_BURST:
            if(!ACK_I)
                future_state = R_IDLE;
            else if(ACK_I)
                future_state = R_IC_END_BURST;
        break;
    }
}
void wb_river_mc::mae_output()
{
    switch(current_state.read())
    {
        case R_IDLE:
            CYC_O = false;
            STB_O = false;
            ACK_IC = false;
            ACK_DC = false;
            STALL_SW = false;
        break;
        case R_REQ:
            CYC_O = true;
            STB_O = false;
            ACK_IC = false;
            ACK_DC = false;
            STALL_SW = true;
        break;
        case R_DC_READ:
            CYC_O = true;
            STB_O = true;
            
            ADR_O = A_DC;
            SEL_O = SIZE_SEL_DC;
            WE_O = false;
            ACK_IC = false;
            ACK_DC = ACK_I;
            STALL_SW = true;
            DT_RM = DAT_I;
        break;
        case R_DC_END_BURST:
            CYC_O = true;
            STB_O = false;
            ACK_IC = false;
            ACK_DC = ACK_I;
            STALL_SW = true;
            DT_RM = DAT_I;
        break;
        case R_DC_WRITE:
            CYC_O = true;
            STB_O = true;

            DAT_O = DT_DC;
            ADR_O = A_DC;
            SEL_O = SIZE_SEL_DC;
            WE_O = true;
            ACK_IC = false;
            ACK_DC = ACK_I;
            STALL_SW = true;
        break;
        case R_IC:
            CYC_O = true;
            STB_O = true;

            ADR_O = A_IC;
            SEL_O = 0;

            DT_IC = DAT_I;
            ACK_IC = ACK_I;
            ACK_DC = false;
            STALL_SW = true;
            WE_O = false;
        break;
        case R_IC_END_BURST:
            CYC_O = true;
            STB_O = false; 
            DT_IC = DAT_I;
            ACK_IC = ACK_I;
            ACK_DC = false;
            STALL_SW = true;
        break;
    }
}

void wb_river_mc::trace(sc_trace_file* tf)
{
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET_N, GET_NAME(RESET_N));

    sc_trace(tf, DAT_I, GET_NAME(DAT_I));
    sc_trace(tf, ACK_I, GET_NAME(ACK_I));
    sc_trace(tf, DAT_O, GET_NAME(DAT_O));
    sc_trace(tf, ADR_O, GET_NAME(ADR_O));
    sc_trace(tf, CYC_O, GET_NAME(CYC_O));
    sc_trace(tf, SEL_O, GET_NAME(SEL_O));
    sc_trace(tf, STB_O, GET_NAME(DT_IC));
    sc_trace(tf, ACK_IC, GET_NAME(ACK_IC));

    sc_trace(tf, DTA_VALID_DC, GET_NAME(DTA_VALID_DC));
    sc_trace(tf, READ_DC, GET_NAME(READ_DC));
    sc_trace(tf, WRITE_DC, GET_NAME(WRITE_DC));
    sc_trace(tf, SIZE_SEL_DC, GET_NAME(SIZE_SEL_DC));
    sc_trace(tf, DT_DC, GET_NAME(DT_DC));
    sc_trace(tf, A_DC, GET_NAME(A_DC));
    sc_trace(tf, DT_RM, GET_NAME(DT_RM));
    sc_trace(tf, ACK_DC, GET_NAME(ACK_DC));
    sc_trace(tf, STALL_SW, GET_NAME(STALL_SW));
    sc_trace(tf, A_IC, GET_NAME(A_IC));
    sc_trace(tf, GRANT_I, GET_NAME(GRANT_I));
    
    sc_trace(tf, current_state, GET_NAME(current_state));
    sc_trace(tf, future_state, GET_NAME(future_state));
}