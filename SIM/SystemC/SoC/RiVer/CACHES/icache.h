#ifndef I_CACHE
#define I_CACHE

#include <systemc.h>
#include "../../../UTIL/debug_util.h"

SC_MODULE(icache)
{
    // interface global
    sc_in_clk CLK;
    sc_in<bool> RESET_N;

    // interface RiVer
    sc_in<sc_uint<32> > ADR_SI; 
    sc_in<bool> ADR_VALID_SI; 

    sc_out<sc_bv<32> > INST_SIC;
    sc_out<bool> STALL_SIC;


    //interface wrapper
    sc_in<sc_uint<32>> DT_SW;
    sc_out<sc_uint<32>> A_SIC;
    sc_out<bool> DTA_VALID_SIC;

    sc_in<bool> ACK_SW;
    sc_in<bool> STALL_SW;

    //signals
    sc_signal<sc_bv<32>> data[256][4];
    sc_signal<sc_uint<20>> tag[256];
    sc_signal<bool> data_validate[256];

    sc_signal<sc_uint<20>> address_tag;
    sc_signal<sc_uint<8>> address_index;
    sc_signal<sc_uint<4>> address_offset;

    sc_signal<sc_uint<20>> current_address_tag;
    sc_signal<sc_uint<8>> current_address_index;

    sc_signal<bool> hit;

    sc_signal<sc_uint<3>> fsm_state;

    void trace(sc_trace_file*);

    void parse_adr();
    void miss_detection();
    void transition();

    SC_CTOR(icache)
    {
        SC_METHOD(parse_adr);
        sensitive << ADR_SI; 

        SC_METHOD(miss_detection);
        sensitive   << address_tag 
                    << address_index 
                    << address_offset 
                    << STALL_SIC;
        for(int i=0; i < 256 ;i++){
            sensitive << data[i][0]
                        << data[i][1]
                        << data[i][2]
                        << data[i][3]
                        << data_validate[i];
        }

        SC_THREAD(transition);
        sensitive << CLK.neg();

        reset_signal_is(RESET_N, false);
    }
};

#endif