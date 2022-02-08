#pragma once
#include <systemc.h>

SC_MODULE(reg)
{
    // Reading Port :

    sc_in < sc_uint<6> >        RADR1 ;
    sc_in < sc_uint<6> >        RADR2 ;
    
    sc_out <bool>               RADR1_VALID ; // tells if the register read is valid or no
    sc_out <bool>               RADR2_VALID ;

    sc_out < sc_uint<32> >      RADR1_DATA ; //data output read from register
    sc_out < sc_uint<32> >      RADR2_DATA ;

    // Writing Port :

    sc_in < sc_uint<6> >        WADR1 ;
    sc_in <bool>                WADR1_VALID ;
    sc_in < sc_uint<32> >       WADR1_DATA ;

    // Inval Port :

    sc_in < sc_uint<6> >        INVAL_ADR ;
    sc_in <bool>                INVAL_ENABLE ;

    //PC Gestion :

    sc_out < sc_uint<32> >      READ_PC ;
    sc_in < bool >              INC_PC_VALID ;
    sc_out < bool >             READ_PC_VALID ;

    //Global Interface :

    sc_in_clk CLK ;
    sc_in <bool> RESET_N ;

    //Registres :

    sc_signal < bool >        REG_VALID[33] ; //bit de validité des registres
    sc_signal < sc_uint<32> > REG[33] ;
    

    void reading_adresses() ;
    void writing_adresse() ;

    SC_CTOR(reg)
    {
        SC_METHOD  (reading_adresses) ;
        sensitive << RADR1 << RADR2;
        for (int i = 0; i < 33; i++) sensitive << REG[i];
        for (int i = 0; i < 33; i++) sensitive << REG_VALID[i];
        SC_CTHREAD  (writing_adresse,reg::CLK.pos()) ;
        reset_signal_is(RESET_N,false ) ;
        
    }
};