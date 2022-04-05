#include <systemc.h>
#include <string>
#include "reg.h"

using namespace std;

int sc_main(int argc, char* argv[]) {
    cerr << "start of main" << endl;
    sc_trace_file* tf;
    tf = sc_create_vcd_trace_file("tf");
    // Reading Port :

    sc_signal<sc_uint<6>> radr1;
    sc_signal<sc_uint<6>> radr2;

    sc_signal<bool> radr1_valid;
    sc_signal<bool> radr2_valid;

    sc_signal<sc_uint<32>> radr1_data;
    sc_signal<sc_uint<32>> radr2_data;

    // Writing Port :

    sc_signal<sc_uint<6>>  wadr1;
    sc_signal<bool>        wadr1_valid;
    sc_signal<sc_uint<32>> wadr1_data;

    // Inval Port :

    sc_signal<sc_uint<6>> inval_adr;
    sc_signal<bool>       inval_enable;

    // PC Gestion :

    sc_signal<sc_uint<32>> read_pc;
    sc_signal<bool>        read_pc_valid;

    // Global Interface :

    sc_clock        clk("clk", 1, SC_NS);
    sc_signal<bool> reset_n;

    // Instance declaration :

    reg reg_inst("reg_inst");

    // Port Map :
    // Reading Port :

    reg_inst.RADR1_SD(radr1);
    reg_inst.RADR2_SD(radr2);

    reg_inst.RDATA1_SR(radr1_data);
    reg_inst.RDATA2_SR(radr2_data);
    reg_inst.WADR_SW(wadr1);
    reg_inst.WENABLE_SW(wadr1_valid);
    reg_inst.WDATA_SW(wadr1_data);

    // PC Gestion :

    reg_inst.READ_PC_SR(read_pc);

    // Global Interface :

    reg_inst.CLK(clk);
    reg_inst.RESET_N(reset_n);

    // vcd :

    reg_inst.trace(tf);

    // sc_trace(tf,reg_inst.RADR1,"RADR1") ;
    // sc_trace(tf,reg_inst.RADR2,"RADR2") ;
    // sc_trace(tf,reg_inst.RADR1_VALID,"RADR1_VALID") ;
    // sc_trace(tf,reg_inst.RADR2_VALID,"RADR2_VALID") ;
    // sc_trace(tf,reg_inst.RADR1_DATA,"RADR1_DATA") ;
    // sc_trace(tf,reg_inst.RADR2_DATA,"RADR2_DATA") ;
    // sc_trace(tf,reg_inst.WADR1,"WADR1") ;
    // sc_trace(tf,reg_inst.WADR1_VALID,"WADR1_VALID") ;
    // sc_trace(tf,reg_inst.WADR1_DATA,"WADR1_DATA") ;
    // sc_trace(tf,reg_inst.INVAL_ADR,"INVAL_ADR") ;
    // sc_trace(tf,reg_inst.INVAL_ENABLE,"INVAL_ENABLE") ;

    // sc_trace(tf,reg_inst.READ_PC,"READ_PC") ;
    // sc_trace(tf,reg_inst.INC_PC_VALID,"INC_PC_VALID") ;
    // sc_trace(tf,reg_inst.READ_PC_VALID,"READ_PC_VALID") ;
    // sc_trace(tf,reg_inst.CLK,"CLK") ;

    // sc_trace(tf,reg_inst.RESET_N,"RESET_N") ;

    // for(int i = 0 ; i < 33 ; i++)
    // {
    //     string s ;
    //     s = "reg_inst.REG" + to_string(i)    ;
    //     sc_trace(tf,reg_inst.REG[i],s) ;
    // }
    // cerr << "starting tracing SC_VALID Signals" << endl ;
    // for(int i = 0 ; i < 33 ; i++)
    // {
    //     string s ;
    //     s="REG_VALID" + to_string(i) ;
    //     sc_trace(tf,reg_inst.REG_VALID[i],s) ;
    // }

    reset_n.write(false);  // reset
    sc_start(3, SC_NS);    // wait for 1 cycle
    reset_n.write(true);   // end of reset
    cerr << "end of reset" << endl;

    int  registers[33] = {0};
    bool valid[33];
    for (int i = 0; i < 33; i++) {
        valid[i] = true;
    }
    for (int i = 0; i < 1000; i++) {
        int radr1_        = rand() % 33;
        int radr2_        = rand() % 33;
        int wadr1_        = rand() % 33;
        int wadr1_valid_  = rand() % 2;
        int wadr1_data_   = rand();
        int inc_pc_valid_ = rand() % 2;
        int inval_enable_ = rand() % 2;
        int inval_adr_    = rand() % 33;

        radr1.write(radr1_);
        radr2.write(radr2_);
        wadr1.write((sc_uint<6>)wadr1_);
        wadr1_valid.write(wadr1_valid_);
        wadr1_data.write((sc_uint<32>)(wadr1_data_));
        inval_adr.write(inval_adr_);
        inval_enable.write(inval_enable_);

        sc_start(1, SC_NS);

        if (wadr1_valid_) {
            registers[wadr1_] = wadr1_data_;
            registers[0]      = 0;
            valid[wadr1_]     = true;
        }

        if (inval_enable_) {
            valid[inval_adr_] = false;
            valid[0]          = true;
        }
        if (registers[radr1_] != (int)radr1_data.read()) {
            cerr << "Error : register mismatch in register " << radr1_ << "from port 1, expected " << registers[radr1_] << " got " << (int)radr1_data.read()
                 << endl;
            exit(1);
        }
        if (registers[radr2_] != (int)radr2_data.read()) {
            cerr << "Error : register mismatch in register " << radr2_ << " from port 2, expected " << registers[radr2_] << " got " << (int)radr2_data.read()
                 << endl;
            exit(1);
        }
        if (registers[32] != (int)read_pc.read()) {
            cerr << "Error : register mismatch in register pc, expected " << registers[32] << " got " << (int)read_pc.read() << endl;
            exit(1);
        }
        if (valid[radr1_] != (int)radr1_valid.read()) {
            cerr << "Error : register validity mismatch in register " << radr1_ << " from port 1, expected " << valid[radr1_] << " got "
                 << (int)radr1_valid.read() << endl;
            exit(1);
        }
        if (valid[radr2_] != (int)radr2_valid.read()) {
            cerr << "Error : register validity mismatch in register " << radr2_ << " from port 2, expected " << valid[radr2_] << " got "
                 << (int)radr2_valid.read() << endl;
            exit(1);
        }
        // cout << "--------------------------------" << endl ;
        // cout << "radr1_ : " << radr1_ << endl ;
        // cout << "radr2_ : " << radr2_ << endl ;
        // cout << "wadr1_ : " << wadr1_ << endl ;
        // cout << "ward1_valid_ : " << ward1_valid_ << endl ;
        // cout << "wadr1_data_ : " << wadr1_data_ << endl ;
        // cout << "inc_pc_valid_ : " << inc_pc_valid_ << endl ;
        // cout << "Data read 1 :" << radr1_data.read() << endl ;
        // cout << "Data read 2 : " << radr2_data.read() << endl ;
        // cout << "ADR_DEST :" << adr_dest_ << endl ;
        // cout << "inval_dest : " << inval_dest_ << endl ;
        // for(int i = 0 ; i < 33 ; i++)
        // {
        //     string s ;
        //     s = "r"+ to_string(i) ;
        //     cout << s << " ,value : "<< reg_inst.REG[i].read() << endl ;
        // }
    }
    cout << "All tests passed sucessfully" << endl;
    sc_close_vcd_trace_file(tf);
    return 0;
}
