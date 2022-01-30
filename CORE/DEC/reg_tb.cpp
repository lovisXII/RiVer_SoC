#include <systemc.h>
#include "reg.h"

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
    

    // vcd :



   
    sc_trace(tf,reg_inst.RADR1,"RADR1") ;
    sc_trace(tf,reg_inst.RADR2,"RADR2") ;
    sc_trace(tf,reg_inst.RADR1_VALID,"RADR1_VALID") ;
    sc_trace(tf,reg_inst.RADR2_VALID,"RADR2_VALID") ;
    sc_trace(tf,reg_inst.RADR1_DATA,"RADR1_DATA") ;
    sc_trace(tf,reg_inst.RADR2_DATA,"RADR2_DATA") ;
    sc_trace(tf,reg_inst.WADR1,"WADR1") ;
    sc_trace(tf,reg_inst.WADR1_DATA,"WADR1_DATA") ;
    sc_trace(tf,reg_inst.READ_PC,"READ_PC") ;
    sc_trace(tf,reg_inst.INC_PC_VALID,"INC_PC_VALID") ;
    sc_trace(tf,reg_inst.READ_PC_VALID,"READ_PC_VALID") ;
    sc_trace(tf,reg_inst.CLK,"CLK") ;
    
    sc_trace(tf,reg_inst.RESET_N,"RESET_N") ;
    sc_trace(tf,reg_inst.REG0,"reg_inst.REG0") ;
    sc_trace(tf,reg_inst.REG1,"reg_inst.REG1") ;
    sc_trace(tf,reg_inst.REG2,"reg_inst.REG2") ;
    sc_trace(tf,reg_inst.REG3,"reg_inst.REG3") ;
    sc_trace(tf,reg_inst.REG4,"reg_inst.REG4") ;
    sc_trace(tf,reg_inst.REG5,"reg_inst.REG5") ;
    sc_trace(tf,reg_inst.REG6,"reg_inst.REG6") ;
    sc_trace(tf,reg_inst.REG7,"reg_inst.REG7") ;
    sc_trace(tf,reg_inst.REG8,"reg_inst.REG8") ;
    sc_trace(tf,reg_inst.REG9,"reg_inst.REG9") ;
    sc_trace(tf,reg_inst.REG10,"reg_inst.REG10") ;
    sc_trace(tf,reg_inst.REG11,"reg_inst.REG11") ;
    sc_trace(tf,reg_inst.REG12,"reg_inst.REG12") ;
    sc_trace(tf,reg_inst.REG13,"reg_inst.REG13") ;
    sc_trace(tf,reg_inst.REG14,"reg_inst.REG14") ;
    sc_trace(tf,reg_inst.REG15,"reg_inst.REG15") ;
    sc_trace(tf,reg_inst.REG16,"reg_inst.REG16") ;
    sc_trace(tf,reg_inst.REG17,"reg_inst.REG17") ;
    sc_trace(tf,reg_inst.REG18,"reg_inst.REG18") ;
    sc_trace(tf,reg_inst.REG19,"reg_inst.REG19") ;
    sc_trace(tf,reg_inst.REG20,"reg_inst.REG20") ;
    sc_trace(tf,reg_inst.REG21,"reg_inst.REG21") ;
    sc_trace(tf,reg_inst.REG22,"reg_inst.REG22") ;
    sc_trace(tf,reg_inst.REG23,"reg_inst.REG23") ;
    sc_trace(tf,reg_inst.REG24,"reg_inst.REG24") ;
    sc_trace(tf,reg_inst.REG25,"reg_inst.REG25") ;
    sc_trace(tf,reg_inst.REG26,"reg_inst.REG26") ;
    sc_trace(tf,reg_inst.REG27,"reg_inst.REG27") ;
    sc_trace(tf,reg_inst.REG28,"reg_inst.REG28") ;
    sc_trace(tf,reg_inst.REG29,"reg_inst.REG29") ;
    sc_trace(tf,reg_inst.REG30,"reg_inst.REG30") ;
    sc_trace(tf,reg_inst.REG31,"reg_inst.REG31") ;
    sc_trace(tf,reg_inst.REG32,"reg_inst.REG32") ;


    reset_n.write(true) ; // reset 
    sc_start(3,SC_NS) ; // wait for 1 cycle
    reset_n.write(false) ; // end of reset
    cerr << "test :" << endl ;
    for(int i = 0 ; i < 10 ; i++)
    {
        int radr1_ = rand() % 7;
        int radr2_ = rand() % 7;
        int wadr1_ = rand() % 7;
        int ward1_valid_ = 1 ;
        int wadr1_data_ = rand() ;
        int inc_pc_valid_ = rand() % 2 ;
        

        radr1.write(radr1_) ;
        radr2.write(radr2_) ;
        wadr1.write((sc_uint<6>) wadr1_) ;
        wadr1_valid.write(ward1_valid_ );
        wadr1_data.write((sc_uint<32>) (wadr1_data_)) ;
        inc_pc_valid.write(inc_pc_valid_) ;
        
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
        cout << "r0 : " << reg_inst.REG0.read() <<endl ;
        cout << "r1 : " << reg_inst.REG1.read() <<endl ;
        cout << "r2 : " << reg_inst.REG2.read() <<endl ;
        cout << "r3 : " << reg_inst.REG3.read() <<endl ;
        cout << "r4 : " << reg_inst.REG4.read() <<endl ;
        cout << "r5 : " << reg_inst.REG5.read() <<endl ;
        cout << "r6 : " << reg_inst.REG6.read() <<endl ;
        cout << "r7 : " << reg_inst.REG7.read() <<endl ;
        cout << "r8 : " << reg_inst.REG8.read() <<endl ;
        cout << "r9 : " << reg_inst.REG9.read() <<endl ;
        cout << "r10 : " << reg_inst.REG10.read() <<endl ;
        cout << "r11 : " << reg_inst.REG11.read() <<endl ;
        cout << "r12 : " << reg_inst.REG12.read() <<endl ;
        cout << "r13 : " << reg_inst.REG13.read() <<endl ;
        cout << "r14 : " << reg_inst.REG14.read() <<endl ;
        cout << "r15 : " << reg_inst.REG15.read() <<endl ;
        cout << "r16 : " << reg_inst.REG16.read() <<endl ;
        cout << "r17 : " << reg_inst.REG17.read() <<endl ;
        cout << "r18 : " << reg_inst.REG18.read() <<endl ;
        cout << "r19 : " << reg_inst.REG19.read() <<endl ;
        cout << "r20 : " << reg_inst.REG20.read() <<endl ;
        cout << "r21 : " << reg_inst.REG21.read() <<endl ;
        cout << "r22 : " << reg_inst.REG22.read() <<endl ;
        cout << "r23 : " << reg_inst.REG23.read() <<endl ;
        cout << "r24 : " << reg_inst.REG24.read() <<endl ;
        cout << "r25 : " << reg_inst.REG25.read() <<endl ;
        cout << "r26 : " << reg_inst.REG26.read() <<endl ;
        cout << "r27 : " << reg_inst.REG27.read() <<endl ;
        cout << "r28 : " << reg_inst.REG28.read() <<endl ;
        cout << "r29 : " << reg_inst.REG29.read() <<endl ;
        cout << "r30 : " << reg_inst.REG30.read() <<endl ;
        cout << "r31 : " << reg_inst.REG31.read() <<endl ;
        cout << "r32 : " << reg_inst.REG32.read() <<endl ;

    }
    sc_close_vcd_trace_file(tf);
    return 0 ;
}