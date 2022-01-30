#include <systemc.h>
#include "reg.h"

int sc_main(int argc, char* argv[])
{
    // Reading Port :

    sc_signal < sc_uint<6> >        radr1 ;
    sc_signal < sc_uint<6> >        radr2 ;
    
    sc_signal <bool>               radr1_valid ;
    sc_signal <bool>               radr2_valid ;

    sc_signal < sc_uint<32> >      radr1_data ;
    sc_signal < sc_uint<32> >      radr2_data ;

    // Writing Port :

    sc_signal < sc_uint<6> >        wadr1 ;
    sc_signal <bool>                wadr1_valid ;
    sc_signal < sc_uint<32> >       wadr1_data ;

    //PC Gestion :

    sc_signal < sc_uint<32> >      read_pc ;
    sc_signal < bool >              inc_pc_valid ;
    sc_signal < bool >             read_pc_valid ;

    //Global Interface :

    sc_clock clk("clk",1,SC_NS) ;
    sc_signal <bool> reset_n ;

    //Instance declaration :

    reg reg_inst("reg_inst") ;

    //Port Map :
    // Reading Port :

    reg_inst.RADR1(radr1) ;
    reg_inst.RADR2(radr2) ;
    
    reg_inst.RADR1_VALID(radr1_valid) ;
    reg_inst.RADR2_VALID(radr2_valid) ;

    reg_inst.RADR1_DATA(radr1_data) ;
    reg_inst.RADR2_DATA(radr2_data) ;

    // Writing Port :

    reg_inst.WADR1(wadr1) ;
    reg_inst.WADR1_VALID(wadr1_valid) ;
    reg_inst.WADR1_DATA(wadr1_data) ;

    //PC Gestion :

    reg_inst.READ_PC(read_pc) ;
    reg_inst.INC_PC_VALID(inc_pc_valid) ;
    reg_inst.READ_PC_VALID(read_pc_valid) ;

    //Global Interface :

    reg_inst.CLK(clk) ;
    reg_inst.RESET_N(reset_n) ;
    
    reset_n.write(true) ; // reset 
    sc_start(3,SC_NS) ; // wait for 1 cycle
    reset_n.write(false) ; // end of reset
    
    for(int i = 0 ; i < 10 ; i++)
    {
        int radr1_ = rand() % 7;
        int radr2_ = rand() % 7;
        int wadr1_ = rand() % 7;
        int ward1_valid_ = rand() % 2 ;
        int wadr1_data_ = rand() ;
        int inc_pc_valid_ = rand() % 2 ;
        sc_start(1,SC_NS) ;
        radr1 = radr1_ ;
        radr2 = radr2_ ;
        wadr1_ = wadr1_ ;
        ward1_valid_ = wadr1_valid ;
        wadr1_data_ = (sc_uint<32>) (wadr1_data) ;
        inc_pc_valid_ = inc_pc_valid ;
        cout << "radr1_ : " << radr1_ << endl ;
        cout << "radr2_ : " << radr2_ << endl ;
        cout << "wadr1_ : " << wadr1_ << endl ;
        cout << "ward1_valid_ : " << ward1_valid_ << endl ;
        cout << "wadr1_data_ : " << wadr1_data_ << endl ;
        cout << "inc_pc_valid_ : " << inc_pc_valid_ << endl ;
        cout << "r0 : " << reg_inst.REG0.read() <<endl ;
        cout << "r1 : " << reg_inst.REG1.read() <<endl ;
        cout << "r2 : " << reg_inst.REG2.read() <<endl ;
        cout << "r3 : " << reg_inst.REG3.read() <<endl ;
        cout << "r4 : " << reg_inst.REG4.read() <<endl ;
        cout << "r5 : " << reg_inst.REG5.read() <<endl ;

    }
    return 0 ;
}