#pragma once
#include <systemc.h>
#include "../UTIL/debug_util.h"
SC_MODULE(reg)
{
    // Reading Port :

    sc_in<sc_uint<6>>        RADR1_SD ;
    sc_in<sc_uint<6>>        RADR2_SD ;
    
    sc_out<bool>               R1_VALID_SD ; // tells if the register read is valid or no
    sc_out<bool>               R2_VALID_SD ;
    sc_out<bool>               RDEST_VALID_SD ;
    sc_in<sc_uint<6>>        RDEST_SD ;

    sc_out<sc_uint<32>>      RDATA1_SD ; //data output read from register
    sc_out<sc_uint<32>>      RDATA2_SD ;

    // Writing Port :

    sc_in<sc_uint<6>>        WADR_SD ;
    sc_in<bool>                WENABLE_SD ;
    sc_in<sc_uint<32>>       WDATA_SD ;

    sc_in<sc_uint<32>>      WRITE_PC_SD ;
    sc_in<bool>             WRITE_PC_ENABLE_SD ;

    // Inval Port :

    sc_in<sc_uint<6>>        INVAL_ADR_SD ;
    sc_in<bool>                INVAL_ENABLE_SD ;

    //PC Gestion :

    sc_out<sc_uint<32>>      READ_PC_SD ;
    sc_out<bool>             READ_PC_VALID_SD ;

    //Global Interface :

    sc_in_clk CLK ;
    sc_in<bool> RESET_N ;
    sc_in<sc_uint<32>> PC_INIT;

    //Registres :

    sc_signal<bool>        REG_VALID_RD[33] ; //bit de validité des registres
    sc_signal<sc_uint<32>> REG_RD[33] ;
    

    void reading_adresses() ;
    void writing_adresse() ;
    void trace(sc_trace_file* tf);

    SC_CTOR(reg)
    {
        SC_METHOD  (reading_adresses) ;
        sensitive 
			<< RADR1_SD 
			<< RADR2_SD 
			<< RDEST_SD 
			<< RESET_N;
        for (int i = 0; i < 33; i++) sensitive 
			<< REG_RD[i];
        for (int i = 0; i < 33; i++) sensitive 
			<< REG_VALID_RD[i];
        SC_CTHREAD  (writing_adresse,reg::CLK.pos()) ;
        reset_signal_is(RESET_N,false ) ;
        
    }
};