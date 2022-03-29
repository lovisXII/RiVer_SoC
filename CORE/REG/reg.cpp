#include "reg.h"
#include <systemc.h>




//bool REG_VALID[33] ; //bit de validité des registres

void reg::reading_adresses()
{
    RDATA1_SD.write(REG_RD[RADR1_SD.read()]) ; //on charge la donnée dans REG[index] dans le port de lecture 1
    RDATA2_SD.write(REG_RD[RADR2_SD.read()]) ; //on charge la donnée dans REG[index] dans le port de lecture 2
    if (RESET_N.read()) {
        READ_PC_SD.write(REG_RD[32]);
    }
    else {
        READ_PC_SD.write(PC_INIT.read());
    }

}

void reg::writing_adresse()
{
    //Starting RESET :
    
    for(int i = 0 ; i < 32 ; i++)
    {
        REG_RD[i].write(0) ; // all register are initialized to 0
    }
    REG_RD[32].write(PC_INIT.read());
    while(1)
    {

        if (WRITE_PC_ENABLE_SD.read()) {
            REG_RD[32].write(WRITE_PC_SD) ; // we write the data into the register from the written adress
        }
        if (not(WADR_SD.read() == 32 && WRITE_PC_ENABLE_SD.read())) {
            if(WADR_SD.read() != 0) // si on cherche à écrire dans le registre 0 on ne fait rien
            {
                if(WENABLE_SD.read()) // if the register written is valid
                {
                REG_RD[WADR_SD.read()].write(WDATA_SD) ; // we write the data into the register from the written adress
                } 
            }
        }

        wait(1) ;
    }
}

void reg::trace(sc_trace_file* tf) {
        sc_trace(tf, RADR1_SD, GET_NAME(RADR1_SD));
        sc_trace(tf, RADR2_SD, GET_NAME(RADR2_SD));
        sc_trace(tf, RDATA1_SD, GET_NAME(RDATA1_SD)); 
        sc_trace(tf, RDATA2_SD, GET_NAME(RDATA2_SD));
        sc_trace(tf, WADR_SD, GET_NAME(WADR_SD));
        sc_trace(tf, WENABLE_SD, GET_NAME(WENABLE_SD));
        sc_trace(tf, WDATA_SD, GET_NAME(WDATA_SD));
        sc_trace(tf, READ_PC_SD, GET_NAME(READ_PC_SD));
        sc_trace(tf,WRITE_PC_SD,GET_NAME(WRITE_PC_SD));
        sc_trace(tf,WRITE_PC_ENABLE_SD,GET_NAME(WRITE_PC_ENABLE_SD)); 
        
        for (int i = 0; i < 33; i++) {
            std::string regname = "REG_";
            regname += std::to_string(i);
            sc_trace(tf, REG_RD[i], signal_get_name(REG_RD[i].name(), regname.c_str()));
        }
        sc_trace(tf, CLK, GET_NAME(CLK));
        sc_trace(tf, RESET_N, GET_NAME(RESET_N));
}