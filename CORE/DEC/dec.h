#include <systemc.h>


SC_MODULE(decod)
{
    
    //Interface with REG :

    sc_in   < sc_uint<32> >       RADR1_DATA ; 
    sc_in   < sc_uint<32> >       RADR2_DATA ;
    sc_in   < bool >              RADR1_VALID ; // tells if the data read is valid or no
    sc_in   < bool >              RADR2_VALID ; // same but for rt

    sc_out  < sc_uint<6> >       RADR1 ; // adress of rs
    sc_out  < sc_uint<6> >       RADR2 ; // adress of rt

    sc_out  < sc_uint<6> >       ADR_DEST ; // rd

    sc_in   < sc_uint<32> >      READ_PC ; // value of r32 which is pc
    sc_out  < bool >             INC_PC ; // tells to reg if he does pc+4 or no
    sc_in   < bool >             READ_PC_VALID ; // say if pc is valid or no

    //Interface with EXE :

    sc_out  < sc_uint<32> >      DEC2EXE_OP1 ; // value of op1
    sc_out  < sc_uint<32> >      DEC2EXE_OP2 ; // value of op2
    sc_out  < sc_uint<4> >       DEC2EXE_CMD ; // value of the command sent to exe
    sc_out  < bool >             DEC2EXE_NEG_OP1 ; // say if we take the opposite of the op1 to do a substraction for example
    sc_out  < bool >             DEC2EXE_WB ; // say if we plan to wbk the value of rd or no
    sc_out  < bool >             MEM_LOAD ; // say to mem if we do a load
    sc_out  < bool >             MEM_STORE ; // say to mem if we do a store
    sc_out  < bool >             MEM_SIGN_EXTEND ;
    sc_out  < bool >             SELECT_SHIFT ; //taille fifo entrée : 110
    
    // Interface with DEC2IF : 

    sc_in   < bool >              DEC2IF_EMPTY ;
    sc_in   < bool >              DEC2IF_FULL ;
    sc_out  < bool >              DEC2IF_PUSH ;
    sc_out  < sc_uint<32> >       DEC2IF_PC ;

    //Interface with IF2DEC :

    sc_out  < bool >              IF2DEC_POP ;
    sc_in   < sc_uint<32> >       IF_IR ;
    sc_in   < bool >              IF2DEC_EMPTY ;
    sc_in   < bool >              IF2DEC_FULL ;

    //General Interface :
    sc_in_clk CLK ;
    sc_in RESET_N ;

    void pushingToIf() ;
    void popFromIf() ; 
    void decoding() ;

    SC_CTOR(decode)
    {
        reset_signal_is(RESET_N,false) ;
    }
}