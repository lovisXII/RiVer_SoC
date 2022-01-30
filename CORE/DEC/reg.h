#pragma once
#include <systemc.h>

SC_MODULE(reg)
{
    // Reading Port :

    sc_in < sc_uint<6> >        RADR1 ;
    sc_in < sc_uint<6> >        RADR2 ;
    
    sc_out <bool>               RADR1_VALID ;
    sc_out <bool>               RADR2_VALID ;

    sc_out < sc_uint<32> >      RADR1_DATA ;
    sc_out < sc_uint<32> >      RADR2_DATA ;

    // Writing Port :

    sc_in < sc_uint<6> >        WADR1 ;
    sc_in <bool>                WADR1_VALID ;
    sc_in < sc_uint<32> >       WADR1_DATA ;

    //PC Gestion :

    sc_out < sc_uint<32> >      READ_PC ;
    sc_in < bool >              INC_PC_VALID ;
    sc_out < bool >             READ_PC_VALID ;

    //Global Interface :

    sc_in_clk CLK ;
    sc_in <bool> RESET_N ;

    //Registres :

    sc_signal < sc_uint<32> > REG0 ;
    sc_signal < sc_uint<32> > REG1 ;
    sc_signal < sc_uint<32> > REG2 ;
    sc_signal < sc_uint<32> > REG3 ;
    sc_signal < sc_uint<32> > REG4 ;
    sc_signal < sc_uint<32> > REG5 ;
    sc_signal < sc_uint<32> > REG6 ;
    sc_signal < sc_uint<32> > REG7 ;
    sc_signal < sc_uint<32> > REG8 ;
    sc_signal < sc_uint<32> > REG9 ;
    sc_signal < sc_uint<32> > REG10 ;
    sc_signal < sc_uint<32> > REG11 ;
    sc_signal < sc_uint<32> > REG12 ;
    sc_signal < sc_uint<32> > REG13 ;
    sc_signal < sc_uint<32> > REG14 ;
    sc_signal < sc_uint<32> > REG15 ;
    sc_signal < sc_uint<32> > REG16 ;
    sc_signal < sc_uint<32> > REG17 ;
    sc_signal < sc_uint<32> > REG18 ;
    sc_signal < sc_uint<32> > REG19 ;
    sc_signal < sc_uint<32> > REG20 ;
    sc_signal < sc_uint<32> > REG21 ;
    sc_signal < sc_uint<32> > REG22 ;
    sc_signal < sc_uint<32> > REG23 ;
    sc_signal < sc_uint<32> > REG24 ;
    sc_signal < sc_uint<32> > REG25 ;
    sc_signal < sc_uint<32> > REG26 ;
    sc_signal < sc_uint<32> > REG27 ;
    sc_signal < sc_uint<32> > REG28 ;
    sc_signal < sc_uint<32> > REG29 ;
    sc_signal < sc_uint<32> > REG30 ;
    sc_signal < sc_uint<32> > REG31 ;
    sc_signal < sc_uint<32> > REG32 ;


    void reading_adresses() ;
    void writing_adresse() ;

    SC_CTOR(reg)
    {
        SC_CTHREAD  (reading_adresses,reg::CLK.pos()) ;
        SC_CTHREAD  (writing_adresse,reg::CLK.pos()) ;
        reset_signal_is(RESET_N,true) ;
        
    }
};