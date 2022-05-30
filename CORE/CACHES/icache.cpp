#include "icache.h"


void icache::parse_adr()
{
    sc_uint<32> DT_A_SI = ADR_SI.read();
    address_tag.write(DT_A_SI.range(31,12));
    address_index.write(DT_A_SI.range(11,4));
    address_offset.write(DT_A_SI.range(3,0));
}

void icache::miss_detection()
{
    // COMPARE HIT WAY0
    if(address_tag == tag[address_index.read()])
    {   
        hit = data_validate[address_index.read()];        
    }
    else
        hit = false;

}

void icache::transition()
{
    typedef enum // MAE STATES
    {
        IDLE,
        WAIT_MEM,
        UPDT,
        ERROR
    } states_fsm;

    states_fsm fsm_current_state = IDLE;

    int cpt = 0;
    bool dta_valid;
    
    bool adr_valid;

    bool debug = true;
    while(1)
    {
        dta_valid = false;

        switch (fsm_current_state)
        {
            case IDLE:
                if(ADR_VALID_SI.read() & RESET_N.read())
                {
                    if(hit)
                    {
                        IC_INST_SI.write(data[address_index.read()][(address_offset.read()/4)]);
                        IC_STALL_SI.write(false);
                    }
                    else
                    {
                        fsm_current_state = WAIT_MEM;
                        A.write(ADR_SI.read() & 0xFFFFFFF0);
                        dta_valid = true;
                        IC_STALL_SI.write(true);
                        cpt = 0;
                        
                    }
                }
                else
                {
                    IC_STALL_SI.write(false);
                }
                break;
            case WAIT_MEM:
                if(SLAVE_ACK_SP.read())
                {
                    fsm_current_state = UPDT;

                    data[address_index.read()][cpt++] = DT.read();
                    tag[address_index.read()] = address_tag.read();
                    data_validate[address_index.read()] = true;

                }
            break;
            case UPDT:
                if(!SLAVE_ACK_SP.read())
                {
                    fsm_current_state = IDLE;

                    IC_INST_SI.write(data[address_index.read()][(address_offset.read()/4)]);

                    IC_STALL_SI.write(false);
                }
                else
                {
                    data[address_index.read()][cpt++] = DT.read();
                }
            break;
            default:
            break;
        }

        switch(fsm_current_state)
        {
          case IDLE:
            fsm_state.write(0);
          break;
          case WAIT_MEM:
            fsm_state.write(1);
          break;
          case UPDT:
            fsm_state.write(2);
          break;
          case ERROR:
            fsm_state.write(3);
          break;
        }

        DTA_VALID.write(dta_valid);
        wait();
    }
}

void icache::trace(sc_trace_file* tf)
{
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET_N, GET_NAME(RESET_N));

    sc_trace(tf, ADR_SI, GET_NAME(ADR_SI)); 
    sc_trace(tf, ADR_VALID_SI, GET_NAME(ADR_VALID_SI)); 

    sc_trace(tf, IC_INST_SI, GET_NAME(IC_INST_SI));
    sc_trace(tf, IC_STALL_SI, GET_NAME(IC_STALL_SI));

    sc_trace(tf, DT, GET_NAME(DT));
    sc_trace(tf, A, GET_NAME(A));
    sc_trace(tf, DTA_VALID, GET_NAME(DTA_VALID));

    sc_trace(tf, SLAVE_ACK_SP, GET_NAME(SLAVE_ACK_SP));

    sc_trace(tf, hit, GET_NAME(hit));

    sc_trace(tf, address_tag, GET_NAME(address_tag));
    sc_trace(tf, address_index, GET_NAME(address_index));
    sc_trace(tf, address_offset, GET_NAME(address_offset));

    sc_trace(tf, fsm_state, GET_NAME(fsm_state));

    for (int i = 0; i < 256; i++) {
        std::string icname = "ICACHE_";
        for(int j = 0; j < 4; j++)
        {
            icname = "D_I_" + std::to_string(i) + "_O_" + std::to_string(j);
            sc_trace(tf, data[i][j], signal_get_name(data[i][j].name(), icname.c_str()));
        }
        icname = "ICACHE_TAG_" + std::to_string(i);
        sc_trace(tf, tag[i], signal_get_name(tag[i].name(), icname.c_str()));
        icname = "ICACHE_VALID_" + std::to_string(i);
        sc_trace(tf, data_validate[i], signal_get_name(data_validate[i].name(), icname.c_str()));
    }
}