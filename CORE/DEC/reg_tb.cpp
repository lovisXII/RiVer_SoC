#include <systemc.h>
#include "reg.h"
#include <string> 

using namespace std ;

int sc_main(int argc, char* argv[])
{
    sc_trace_file *tf;
    tf=sc_create_vcd_trace_file("tf");
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

    // Invalidation de Rd :

    sc_signal < sc_uint<6> >        adr_dest ;
    sc_signal < bool >              inval_dest ;

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
    reg_inst.WADR1(wadr1) ;
    reg_inst.WADR1_VALID(wadr1_valid) ;
    reg_inst.WADR1_DATA(wadr1_data) ;

    reg_inst.ADR_DEST(adr_dest) ;
    reg_inst.INVAL_DEST(inval_dest) ;
    //PC Gestion :

    reg_inst.READ_PC(read_pc) ;
    reg_inst.INC_PC_VALID(inc_pc_valid) ;
    reg_inst.READ_PC_VALID(read_pc_valid) ;

    //Global Interface :

    reg_inst.CLK(clk) ;
    reg_inst.RESET_N(reset_n) ;
    

    // vcd :



   
    sc_trace(tf,reg_inst.RADR1,"RADR1") ;
    sc_trace(tf,reg_inst.RADR2,"RADR2") ;
    sc_trace(tf,reg_inst.RADR1_VALID,"RADR1_VALID") ;
    sc_trace(tf,reg_inst.RADR2_VALID,"RADR2_VALID") ;
    sc_trace(tf,reg_inst.RADR1_DATA,"RADR1_DATA") ;
    sc_trace(tf,reg_inst.RADR2_DATA,"RADR2_DATA") ;
    sc_trace(tf,reg_inst.WADR1,"WADR1") ;
    sc_trace(tf,reg_inst.WADR1_DATA,"WADR1_DATA") ;

    sc_trace(tf,reg_inst.ADR_DEST,"adr_dest") ;
    sc_trace(tf,reg_inst.INVAL_DEST,"INVAL_DEST") ;

    sc_trace(tf,reg_inst.READ_PC,"READ_PC") ;
    sc_trace(tf,reg_inst.INC_PC_VALID,"INC_PC_VALID") ;
    sc_trace(tf,reg_inst.READ_PC_VALID,"READ_PC_VALID") ;
    sc_trace(tf,reg_inst.CLK,"CLK") ;
    
    sc_trace(tf,reg_inst.RESET_N,"RESET_N") ;

    for(int i = 0 ; i < 33 ; i++)
    {
        string s ;
        s = "reg_inst.REG" + to_string(i)    ;
        sc_trace(tf,reg_inst.REG[i],s) ;
    }

    reset_n.write(true) ; // reset 
    sc_start(3,SC_NS) ; // wait for 1 cycle
    reset_n.write(false) ; // end of reset
    cerr << "test :" << endl ;
    for(int i = 0 ; i < 1000 ; i++)
    {
        int radr1_ = rand() % 7;
        int radr2_ = rand() % 7 ;
        int wadr1_ = rand() % 7;
        int ward1_valid_ = 1 ;
        int wadr1_data_ = rand() ;
        int inc_pc_valid_ = rand() % 2 ;
        int adr_dest_ = rand() % 7;
        int inval_dest_ = rand() % 2;

        radr1.write(radr1_) ;
        radr2.write(radr2_) ;
        wadr1.write((sc_uint<6>) wadr1_) ;
        wadr1_valid.write(ward1_valid_ );
        wadr1_data.write((sc_uint<32>) (wadr1_data_)) ;
        inc_pc_valid.write(inc_pc_valid_) ;
        adr_dest.write(adr_dest_) ;
        inval_dest.write(inval_dest_) ;

        sc_start(1,SC_NS) ;
        cout << "--------------------------------" << endl ;
        cout << "radr1_ : " << radr1_ << endl ;
        cout << "radr2_ : " << radr2_ << endl ;
        cout << "wadr1_ : " << wadr1_ << endl ;
        cout << "ward1_valid_ : " << ward1_valid_ << endl ;
        cout << "wadr1_data_ : " << wadr1_data_ << endl ;
        cout << "inc_pc_valid_ : " << inc_pc_valid_ << endl ;
        cout << "Data read 1 :" << radr1_data.read() << endl ;
        cout << "Data read 2 : " << radr2_data.read() << endl ;
        cout << "ADR_DEST :" << adr_dest_ << endl ;
        cout << "inval_dest : " << inval_dest_ << endl ;  
        for(int i = 0 ; i < 33 ; i++)
        {
            string s ;
            s = "r"+ to_string(i) ;
           cout << s << " ,value : "<< reg_inst.REG[i].read() << endl ;
        }       

    }
    sc_close_vcd_trace_file(tf);
    return 0 ;
}