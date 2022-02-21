#include <systemc.h>
#include "dec.h"

using namespace std;
int tab2[33] ; 

void print_reg(sc_signal<sc_uint<32>>* tab)
{
    cout << sc_time_stamp() << endl ;  
    for(int i = 0 ; i < 33 ; i++)
    {
        // cout << "REG_VALID[" << i << "] = " << tab[i].read() << endl ; 
    }
}

void affectation_validity(sc_signal<bool>* tab, int* tab2)
/*This function will be used to reset the validity of a bit
It receives a table (REG_VALID) and it checks the validity of each bits, if one bit is = to 0
Then tab2 is incremented, tab2 is a counter for each validity bit 
A unvalid bit is keep for 3 cycle.
At the end of those 3 cycle it has to be reset to valid 
*/
{
    for(int i = 0; i < 33 ; i++) // we check if there is unvalaible bits
    {
        if(tab[i] == 0) // if yes we increment the counter
        {
            tab2[i]= tab2[i]+1;
        }
        if(tab2[i] == 3)
        {
            tab2[i] = 0 ;
            tab[i] = 1 ;
        }
    
    }  
    // cout << sc_time_stamp() << endl ;  
    // for(int i = 0 ; i < 33 ; i++)
    // {
    //     cout << "REG_VALID[" << i << "] = " << tab[i].read() << endl ; 
    // }
}


int sc_main(int argc, char* argv[])
{

    sc_trace_file *tf ;
    tf=sc_create_vcd_trace_file("tf");
    sc_time t1(10,SC_NS) ;

    decod dec("decode") ;

    //Interface with REG :

    sc_signal     < sc_uint<32> >       radr1_data ; 
    sc_signal     < sc_uint<32> >       radr2_data ;
    sc_signal     < bool >              radr1_valid ; // tells if the data read is valid or no
    sc_signal     < bool >              radr2_valid ; // same but for rt

    sc_signal    < sc_uint<6> >       radr1 ; // adress of rs
    sc_signal    < sc_uint<6> >       radr2 ; // adress of rt

    sc_signal    < sc_uint<6> >       adr_dest ; // rd

    sc_signal     < sc_uint<32> >      read_pc ; // value of r32 which is pc
    sc_signal    < bool >             inc_pc ; // tells to reg if he does pc+4 or no
    sc_signal    < bool >             read_pc_valid ; // say if pc is valid or no

    //Interface with EXE :

    sc_signal   < sc_uint<32> >      dec2exe_op1 ; // value of op1
    sc_signal   < sc_uint<32> >      dec2exe_op2 ; // value of op2
    sc_signal   < sc_uint<2> >       dec2exe_cmd ; // value of the command sent to exe
    sc_signal   < bool >             dec2exe_neg_op1 ; // say if we take the opposite of the op1 to do a substraction for example
    sc_signal   < bool >             dec2exe_wb ; // say if we plan to wbk the value of rd or no
    
    sc_signal   < sc_uint<32> >      mem_data ; // data sent to mem for storage
    sc_signal   < bool >             mem_load ; // say to mem if we do a load
    sc_signal   < bool >             mem_store ; // say to mem if we do a store
    sc_signal   < bool >             mem_sign_extend ; 
    sc_signal   < sc_uint<2> >       mem_size ; // tells to mem if we do an acces in word, hw or byte
    sc_signal   < bool >             select_shift ; //taille fifo entrée : 110
    
    // Interface with DEC2IF : 

    sc_signal    < bool >              dec2if_pop ;
    sc_signal    < bool >              dec2if_empty ;
    sc_signal   < sc_uint<32> >        dec2if_pc ;


    sc_signal   < sc_uint<32> >        if_ir ;
    sc_signal   < bool >               if2dec_empty ;
    sc_signal   < bool >               if2dec_pop ;


    //Interface with IF2DEC :

    sc_signal   < bool >              dec2exe_pop ;
    sc_signal    < bool >             dec2exe_empty ;
    sc_signal    < sc_bv<110> >       dec2exe_out ;


    //General Interface :
    sc_clock                          clk ;
    sc_signal  <bool>                 reset_n ;
    

    //Signals for tb :

    sc_signal<sc_uint<32>> REG[33] ;
    sc_signal<bool> REG_VALID[33] ;




    //Port MAP :

    dec.RADR1_DATA(radr1_data) ; 
    dec.RADR2_DATA(radr2_data) ;
    dec.RADR1_VALID(radr1_valid) ; 
    dec.RADR2_VALID(radr2_valid) ; 

    dec.RADR1(radr1) ; 
    dec.RADR2(radr2) ; 

    dec.ADR_DEST(adr_dest) ; 

    dec.READ_PC(read_pc) ; 
    dec.INC_PC(inc_pc) ; 
    dec.READ_PC_VALID(read_pc_valid) ; 

    

    dec.DEC2EXE_OP1(dec2exe_op1) ; 
    dec.DEC2EXE_OP2(dec2exe_op2) ; 
    dec.DEC2EXE_CMD(dec2exe_cmd) ; 
    dec.DEC2EXE_NEG_OP1(dec2exe_neg_op1) ; 
    dec.DEC2EXE_WB(dec2exe_wb) ; 
    
    dec.MEM_DATA(mem_data) ; 
    dec.MEM_LOAD(mem_load) ; 
    dec.MEM_STORE(mem_store) ; 
    dec.MEM_SIGN_EXTEND(mem_sign_extend) ; 
    dec.MEM_SIZE(mem_size) ; 
    dec.SELECT_SHIFT(select_shift) ; 
    
    

    dec.DEC2IF_POP(dec2if_pop) ;
    dec.DEC2IF_EMPTY(dec2if_empty) ;
    dec.DEC2IF_PC(dec2if_pc) ;

    dec.IF_IR(if_ir) ;
    dec.IF2DEC_EMPTY(if2dec_empty) ;
    dec.IF2DEC_POP(if2dec_pop) ;

    dec.DEC2EXE_POP(dec2exe_pop) ;
    dec.DEC2EXE_EMPTY(dec2exe_empty) ;
    dec.DEC2EXE_OUT(dec2exe_out) ;
    
    dec.CLK(clk) ;
    dec.RESET_N(reset_n) ;
    dec.trace(tf) ;

    //Reset :

    reset_n.write(false) ;
    sc_start(1,SC_NS) ;
    sc_start(1, SC_NS);
    reset_n.write(true) ;
    sc_start(1,SC_NS) ;

    sc_start(1,SC_NS) ;
    //End of reset 
   
    //Bank registre simulation :



    //Resting bank register :
    
    for(int i = 0 ; i < 32 ; i++)
    {
        REG[i].write(0) ;
        REG_VALID[i].write(1) ;
        tab2[i] = 0 ;
    }
    REG[32].write(0) ;
    REG_VALID[32].write(1) ;
    tab2[32] = 0 ;


//--------------------------------------------2eme Test :--------------------------------------------------------
        sc_start(1,SC_NS) ; // 6 ns
        
        if_ir.write(0b00000010000100001001000010100011) ; 
        //Testing dec2if condition :

        radr1_data.write(rand()) ;
        radr2_data.write(rand()) ;



        radr1_valid.write(REG_VALID[radr1.read()]) ;
        radr2_valid.write(REG_VALID[radr2.read()]) ;
 
        //Setting the destination register as unvalaible :

        read_pc.write(REG[32].read()) ;
        read_pc_valid.write(1) ;
        REG[32].write(dec.DEC2IF_PC.read()) ;

        if2dec_empty.write(1) ;

        dec2if_pop.write(1) ;
        dec2exe_pop.write(1) ;


        if(dec.adr_dest.read() != 0)
        {
            REG_VALID[dec.adr_dest.read()] = 0 ;
        }

        print_reg(REG) ;
        affectation_validity(REG_VALID,tab2) ;
        sc_start(1,SC_NS) ;
//--------------------------------------------2eme Test :--------------------------------------------------------
        sc_start(1,SC_NS) ; // 6 ns
        
        if_ir.write(0b00000010000100001001000010100011) ; 
        //Testing dec2if condition :

        radr1_data.write(rand()) ;
        radr2_data.write(rand()) ;



        radr1_valid.write(REG_VALID[radr1.read()]) ;
        radr2_valid.write(REG_VALID[radr2.read()]) ;
 
        //Setting the destination register as unvalaible :

        read_pc.write(REG[32].read()) ;
        read_pc_valid.write(1) ;
        REG[32].write(dec.DEC2IF_PC.read()) ;


        dec2if_pop.write(1) ;
        dec2exe_pop.write(1) ;


        if(dec.adr_dest.read() != 0)
        {
            REG_VALID[dec.adr_dest.read()] = 0 ;
        }

        print_reg(REG) ;
        affectation_validity(REG_VALID,tab2) ;
        sc_start(1,SC_NS) ;
        sc_close_vcd_trace_file(tf) ;
    
    
    return 0; 
}

