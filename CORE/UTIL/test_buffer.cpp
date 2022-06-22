#include <systemc.h>
#include <string>
#include "buffer.h"

using namespace std;

int sc_main(int argc, char* argv[]) {

    sc_trace_file* tf;
    tf = sc_create_vcd_trace_file("tf");


    sc_signal<sc_bv<32>>    DATA_IN_S_S1;
    sc_signal<sc_bv<32>>    DATA_IN_S_S2;
    sc_clock                CLK("clk", 1, SC_NS);
    sc_signal<bool>         WRITE_S;
    sc_signal<bool>         READ_S_S1;
    sc_signal<bool>         READ_S_S2;
    sc_signal<bool>         RESET_N;
    sc_signal<bool>         FLUSH_S ;
    sc_signal<bool>         FULL_S;
    sc_signal<bool>         EMPTY_S;
    sc_signal<sc_bv<32>>    DATA_OUT_S_S1;
    sc_signal<sc_bv<32>>    DATA_OUT_S_S2;


    buffer<32> buffer_inst("buff_test") ;

    buffer_inst.DATA_IN_S_S1(DATA_IN_S_S1) ;
    buffer_inst.DATA_IN_S_S2(DATA_IN_S_S2) ;

    buffer_inst.DATA_OUT_S_S1(DATA_OUT_S_S1) ;
    buffer_inst.DATA_OUT_S_S2(DATA_OUT_S_S2) ;


    buffer_inst.WRITE_S(WRITE_S) ;
    buffer_inst.READ_S_S1(READ_S_S1) ;
    buffer_inst.READ_S_S2(READ_S_S2) ;
    buffer_inst.FLUSH_S(FLUSH_S) ;

    buffer_inst.FULL_S(FULL_S) ;
    buffer_inst.EMPTY_S(EMPTY_S) ;

    buffer_inst.CLK(CLK) ;
    buffer_inst.RESET_N(RESET_N) ;


    buffer_inst.trace(tf);


    RESET_N.write(false);  // reset
    sc_start(3, SC_NS);    // wait for 1 cycle
    RESET_N.write(true);   // end of reset
    for(int j = 0; j <30; j++){
        int data_in1 = rand() % 33 ; 
        int data_in2 = rand() % 33 ; 
        int write   = rand() % 2 ;
        int read1    = rand() % 2 ;
        int read2;
        if(read1)
            read2    = rand()%2 ;
        else
            read2 = 0;
        DATA_IN_S_S1   = data_in1 ;
        DATA_IN_S_S2   = data_in2 ;
        WRITE_S     = write ;
        READ_S_S1      = read1 ;
        READ_S_S2      = read2 ;
        sc_start(1, SC_NS);
    }
    sc_close_vcd_trace_file(tf);
    return 0;
}
