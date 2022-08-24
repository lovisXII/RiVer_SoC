#include <systemc.h>
#include "wb_bus.h"


int sc_main(int argc, char* argv[])
{
    sc_trace_file* tf;
    tf = sc_create_vcd_trace_file("tf");

    //river cache interface
    sc_signal<sc_uint<32>>     RC_DT_I;
    sc_signal<sc_uint<32>>     RC_ADR;
    sc_signal<bool>            RC_DT_VALID;
    sc_signal<bool>            RC_WRITE;
    sc_signal<bool>            RC_READ;
    sc_signal<sc_uint<4>>      RC_SEL;
    sc_signal<sc_uint<32>>     RC_DT_O;
    sc_signal<bool>            HANGING_RQST;
    //ram interface (RAM) 
    sc_signal<bool>            RAM_DTA_RDY;
    sc_signal<sc_uint<32>>     RAM_DT_I;

    sc_signal<sc_uint<32>>     RAM_DT_O;
    sc_signal<sc_uint<32>>     RAM_ADR;
    sc_signal<bool>            RAM_WE;
    sc_signal<sc_uint<4>>      RAM_SEL;

    // Global interface
    sc_clock clk("clk", 1, SC_NS);
    sc_signal<bool> reset_n;

    // Instance declaration
    wb_bus bus_inst("bus_inst");


    bus_inst.CLK(clk);
    bus_inst.RESET_N(reset_n);

    // river cache interface
    bus_inst.RC_DT_I(RC_DT_I);
    bus_inst.RC_ADR(RC_ADR);
    bus_inst.RC_DT_VALID(RC_DT_VALID);
    bus_inst.RC_WRITE(RC_WRITE);
    bus_inst.RC_READ(RC_READ);
    bus_inst.RC_SEL(RC_SEL);
    bus_inst.RC_DT_O(RC_DT_O);
    bus_inst.HANGING_RQST(HANGING_RQST);
    
    //ram interface (RAM) 
    bus_inst.RAM_DTA_RDY_I(RAM_DTA_RDY);
    bus_inst.RAM_DT_I(RAM_DT_I);
    bus_inst.RAM_DT_O(RAM_DT_O);
    bus_inst.RAM_ADR_O(RAM_ADR);
    bus_inst.RAM_WE_O(RAM_WE);
    bus_inst.RAM_SEL_O(RAM_SEL);

    bus_inst.trace(tf);

    reset_n.write(false);
    sc_start(3, SC_NS);
    reset_n.write(true);
    cerr << "end of reset" << endl;
    
    cerr << "start single read test"<<endl;
    //single read cycle
    RC_DT_I.write(0xFAAFFAAF);
    RC_ADR.write(0xBBBBCCCC);
    RC_DT_VALID.write(1);
    RC_WRITE.write(0);
    RC_READ.write(1);
    RC_SEL.write(0xF);
    sc_start(1, SC_NS);
    sc_start(1, SC_NS);
    sc_start(1, SC_NS);
    if(HANGING_RQST.read())
    {
        RC_DT_VALID.write(0);
        RC_READ.write(0);
    }
    sc_start(1, SC_NS);
    sc_start(1, SC_NS);
    sc_start(1, SC_NS);
    sc_start(1, SC_NS);
    sc_start(1, SC_NS);
    RAM_DTA_RDY = true;
    RAM_DT_I.write(0x000000FF);
    sc_start(1, SC_NS);
    RAM_DT_I.write(0x0000FF00);
    sc_start(1, SC_NS);
    RAM_DT_I.write(0x00FF0000);
    sc_start(1, SC_NS);
    RAM_DT_I.write(0xFF000000);
    sc_start(1, SC_NS);
    RAM_DTA_RDY = false;
    sc_start(1, SC_NS);
    sc_start(1, SC_NS);
    sc_start(1, SC_NS);
    //end of single read cycle
    cerr << "test ended"<<endl;
    //single write cycle
    cerr << "start single write test"<<endl;
    sc_start(1, SC_NS);
    RC_DT_I.write(0xAAAABBBB);
    RC_ADR.write(0xEEEEEEEE);
    RC_DT_VALID.write(1);
    RC_WRITE.write(1);
    RC_READ.write(0);
    RC_SEL.write(0xF);
    sc_start(1, SC_NS);
    sc_start(1, SC_NS);
    sc_start(1, SC_NS);
    sc_start(1, SC_NS);
    cerr << "test ended"<<endl;
    sc_start(10, SC_NS);
    sc_close_vcd_trace_file(tf);
    return 0;
}