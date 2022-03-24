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
   
        
    /*
    The register 1 or 2 validity is based on the destination register. So the validity of radr1 and radr2 is the valid bit from rd
    */

    R1_VALID_SD.write(REG_VALID_RD[RADR1_SD.read()]) ;

    R2_VALID_SD.write(REG_VALID_RD[RADR2_SD.read()]) ;

    RDEST_VALID_SD.write(REG_VALID_RD[RDEST_SD.read()]) ;

    READ_PC_VALID_SD.write(REG_VALID_RD[32]) ; // la validité de PC est la validité du registre correspondant soit r32


}

void reg::writing_adresse()
{
    //Starting RESET :
    
    for(int i = 0 ; i < 32 ; i++)
    {
        REG_RD[i].write(0) ; // all register are initialized to 0
    }
    REG_RD[32].write(PC_INIT.read());
    for(int i = 0 ; i < 33 ; i++)
    {
        REG_VALID_RD[i] = true ;// Valid bits from every register are setup to true during reset
    }
    while(1)
    {

        if (WRITE_PC_ENABLE_SD.read()) {
            REG_RD[32].write(WRITE_PC_SD) ; // we write the data into the register from the written adress
            REG_VALID_RD[32].write(1) ;    // Register written is written as valid
        }
        if (not(WADR_SD.read() == 32 && WRITE_PC_ENABLE_SD.read())) {
            if(WADR_SD.read() != 0) // si on cherche à écrire dans le registre 0 on ne fait rien
            {
                if(WENABLE_SD.read()) // if the register written is valid
                {
                REG_RD[WADR_SD.read()].write(WDATA_SD) ; // we write the data into the register from the written adress
                REG_VALID_RD[WADR_SD.read()].write(1) ;    // Register written is written as valid 
                } 
            }
        }
        // Invalidation du registre destination
        /*
        While the register written is unvalid, we set up his valid bit to 0.
        Indeed when the destination register will be decod in DEC, it ll be unvalid until it is write back in WBK.
        So if the wadr1_valid is to 0 it means we dont plan to write back the register right now.
        More over we have to verify that this register is not r0, cause r0 is the constant 0 so it's can't be unvalid.
        */
        if(INVAL_ENABLE_SD.read() && INVAL_ADR_SD.read() != 0) 
        {
         REG_VALID_RD[INVAL_ADR_SD.read()].write(0) ; // Invalidation du port lié au registre destination 
        }
 
        // Invalidation de la data lue par adr1 et adr2 


        wait(1) ;
    }
}

void reg::trace(sc_trace_file* tf) {
        sc_trace(tf, RADR1_SD, GET_NAME(RADR1_SD));
        sc_trace(tf, RADR2_SD, GET_NAME(RADR2_SD));
        sc_trace(tf, R1_VALID_SD, GET_NAME(R1_VALID_SD));
        sc_trace(tf, R2_VALID_SD, GET_NAME(R2_VALID_SD));
        sc_trace(tf, RDEST_VALID_SD, GET_NAME(RDEST_VALID_SD));
        sc_trace(tf, RDEST_SD, GET_NAME(RDEST_SD));
        sc_trace(tf, RDATA1_SD, GET_NAME(RDATA1_SD)); 
        sc_trace(tf, RDATA2_SD, GET_NAME(RDATA2_SD));
        sc_trace(tf, WADR_SD, GET_NAME(WADR_SD));
        sc_trace(tf, WENABLE_SD, GET_NAME(WENABLE_SD));
        sc_trace(tf, WDATA_SD, GET_NAME(WDATA_SD));
        sc_trace(tf, INVAL_ADR_SD, GET_NAME(INVAL_ADR_SD));
        sc_trace(tf, INVAL_ENABLE_SD, GET_NAME(INVAL_ENABLE_SD));
        sc_trace(tf, READ_PC_SD, GET_NAME(READ_PC_SD));
        sc_trace(tf, READ_PC_VALID_SD, GET_NAME(READ_PC_VALID_SD));
        sc_trace(tf,WRITE_PC_SD,GET_NAME(WRITE_PC_SD));
        sc_trace(tf,WRITE_PC_ENABLE_SD,GET_NAME(WRITE_PC_ENABLE_SD)); 
        
        for (int i = 0; i < 33; i++) {
            std::string regname = "REG_";
            regname += std::to_string(i);
            sc_trace(tf, REG_RD[i], signal_get_name(REG_RD[i].name(), regname.c_str()));
        }
        for (int i = 0; i < 33; i++) {
            std::string regname = "REG_VALID_";
            regname += std::to_string(i);
            sc_trace(tf, REG_VALID_RD[i], signal_get_name(REG_VALID_RD[i].name(), regname.c_str()));
        }
        sc_trace(tf, CLK, GET_NAME(CLK));
        sc_trace(tf, RESET_N, GET_NAME(RESET_N));
}