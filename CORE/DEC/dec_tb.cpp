#include <systemc.h>
#include "dec.h"

int sc_main(int argc, char* argv[])
{

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
    sc_signal   < sc_uint<4> >       dec2exe_cmd ; // value of the command sent to exe
    sc_signal   < bool >             dec2exe_neg_op1 ; // say if we take the opposite of the op1 to do a substraction for example
    sc_signal   < bool >             dec2exe_wb ; // say if we plan to wbk the value of rd or no
    
    sc_signal   < sc_uint<32> >      mem_data ; // data sent to mem for storage
    sc_signal   < sc_uint<3> >       mem_load ; // say to mem if we do a load
    sc_signal   < sc_uint<3> >       mem_store ; // say to mem if we do a store
    sc_signal   < bool >             mem_sign_extend ; 
    sc_signal   < sc_uint<2> >       mem_size ; // tells to mem if we do an acces in word, hw or byte
    sc_signal   < bool >             select_shift ; //taille fifo entrée : 110
    
    // Interface with DEC2IF : 

    sc_signal    < bool >              dec2if_empty ;
    sc_signal    < bool >              dec2if_full ;
    sc_signal   < bool >              dec2if_push ;
    sc_signal   < sc_uint<32> >       dec2if_pc ;

    //Interface with IF2DEC :

    sc_signal   < bool >              if2dec_pop ;
    sc_signal    < sc_uint<32> >       if_ir ;
    sc_signal    < bool >              if2dec_empty ;
    sc_signal    < bool >              if2dec_full ;

    //General Interface :
    sc_clock                            clk ;
    sc_signal  <bool>                 reset_n ;
    




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
    
    

    dec.DEC2IF_EMPTY(dec2if_empty) ;
    dec.DEC2IF_FULL(dec2if_full) ;
    dec.DEC2IF_PUSH(dec2if_push) ;
    dec.DEC2IF_PC(dec2if_pc) ;

    

    dec.IF2DEC_POP(if2dec_pop) ;
    dec.IF_IR(if_ir) ;
    dec.IF2DEC_EMPTY(if2dec_empty) ;
    dec.IF2DEC_FULL(if2dec_full) ;

    
    dec.CLK(clk) ;
    dec.RESET_N(reset_n) ;


    //Reset :

    reset_n.write(false) ;
    sc_start(1,SC_NS) ;
    reset_n.write(true) ;

    //End of reset 

    // tests :

    
    //Bank registre simulation :

    int REG[33] ;

    //Resting bank register :
    
    for(int i = 0 ; i < 33 ; i++)
    {
        REG[i] = 0 ;
    }

    if_ir.write(0b11111100000001100110111110010011) ; 
    //0000000 00001 00001 000 00011 0110011
    //correspond at add r3 r1 r1
    //111111000000 01100 110 11111 0010011
    
    radr1_data.write(REG[radr1.read()]) ;
    radr2_data.write(REG[radr2.read()]) ;

    read_pc.write(REG[33]) ; 
    inc_pc.write(0) ;
    read_pc_valid.write(1) ; 
    dec2if_empty .write(1) ;
    dec2if_full.write(0) ;
    sc_start(1,SC_NS) ;
    cout << "if_ir :" << dec.IF_IR.read() << endl ;
    cout << dec.IF_IR.read().range(6,0) << endl ;
    cout << "r_type_inst :" << dec.r_type_inst.read() << endl ;
    cout << "dec2exe_op1 : " << dec.DEC2EXE_OP1.read() << endl ;
    cout << "dec2exe_op2 : " << dec.DEC2EXE_OP2.read() << endl ;
    
    cout << "ori :" << dec.ori_i.read() << endl ;
    cout << "xori :" << dec.xori_i.read() << endl ;
    cout << "dec_exe_dest :" << dec.ADR_DEST.read() << endl ;
    cout << "RADR1 : " << dec.RADR1.read() << endl ;
    cout << "RADR2 : " << dec.RADR2.read() << endl ;
    cout << "DEC2EXE_CMD : " << dec.DEC2EXE_CMD.read() << endl ;
    cout << "DEC2EXE_NEG_OP1 : " << dec.DEC2EXE_NEG_OP1.read() << endl ;
    cout << "SELECT_SHIFT : " << dec.SELECT_SHIFT.read() << endl ;
    
    return 0; 
}