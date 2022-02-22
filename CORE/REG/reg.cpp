#include "reg.h"
#include <systemc.h>




//bool REG_VALID[33] ; //bit de validité des registres

void reg::reading_adresses()
{
    RADR1_DATA.write(REG[RADR1.read()]) ; //on charge la donnée dans REG[index] dans le port de lectrue 1
    RADR2_DATA.write(REG[RADR2.read()]) ; //on charge la donnée dans REG[index] dans le port de lectrue 2
    if (RESET_N.read()) {
        READ_PC.write(REG[32]);
    }
    else {
        READ_PC.write(DEBUG_PC_RESET.read());
    }
   
        
    /*
    The register 1 or 2 validity is based on the destination register. So the validity of radr1 and radr2 is the valid bit from rd
    */

    RADR1_VALID.write(REG_VALID[RADR1.read()]) ;

    RADR2_VALID.write(REG_VALID[RADR2.read()]) ;

    READ_PC_VALID.write(REG_VALID[32]) ; // la validité de PC est la validité du registre correspondant soit r32


}

void reg::writing_adresse()
{
    //Starting RESET :
    
    for(int i = 0 ; i < 32 ; i++)
    {
        REG[i].write(0) ; // all register are initialized to 0
    }
    REG[32].write(DEBUG_PC_RESET.read());
    for(int i = 0 ; i < 33 ; i++)
    {
        REG_VALID[i] = true ;// Valid bits from every register are setup to true during reset
    }
    while(1)
    {

        if (WRITE_PC_ENABLE.read()) {
            REG[32].write(WRITE_PC) ; // we write the data into the register from the written adress
            REG_VALID[32].write(1) ;    // Register written is written as valid
        }
        if (not(WADR1.read() == 32 && WRITE_PC_ENABLE.read())) {
            if(WADR1.read() != 0) // si on cherche à écrire dans le registre 0 on ne fait rien
            {
                if(WADR1_VALID.read()) // if the register written is valid
                {
                REG[WADR1.read()].write(WADR1_DATA) ; // we write the data into the register from the written adress
                REG_VALID[WADR1.read()].write(1) ;    // Register written is written as valid 
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
        if(INVAL_ENABLE.read() && INVAL_ADR.read() != 0) 
        {
         REG_VALID[INVAL_ADR.read()].write(0) ; // Invalidation du port lié au registre destination 
        }
 
        // Invalidation de la data lue par adr1 et adr2 


        wait(1) ;
    }
}

void reg::trace(sc_trace_file* tf) {
        sc_trace(tf, RADR1, GET_NAME(RADR1));
        sc_trace(tf, RADR2, GET_NAME(RADR2));
        sc_trace(tf, RADR1_VALID, GET_NAME(RADR1_VALID));
        sc_trace(tf, RADR2_VALID, GET_NAME(RADR2_VALID));
        sc_trace(tf, RADR1_DATA, GET_NAME(RADR1_DATA)); 
        sc_trace(tf, RADR2_DATA, GET_NAME(RADR2_DATA));
        sc_trace(tf, WADR1, GET_NAME(WADR1));
        sc_trace(tf, WADR1_VALID, GET_NAME(WADR1_VALID));
        sc_trace(tf, WADR1_DATA, GET_NAME(WADR1_DATA));
        sc_trace(tf, INVAL_ADR, GET_NAME(INVAL_ADR));
        sc_trace(tf, INVAL_ENABLE, GET_NAME(INVAL_ENABLE));
        sc_trace(tf, READ_PC, GET_NAME(READ_PC));
        sc_trace(tf, READ_PC_VALID, GET_NAME(READ_PC_VALID));
        sc_trace(tf,WRITE_PC,GET_NAME(WRITE_PC));
        sc_trace(tf,WRITE_PC_ENABLE,GET_NAME(WRITE_PC_ENABLE)); 
        
        for (int i = 0; i < 33; i++) {
            std::string regname = "REG_";
            regname += std::to_string(i);
            sc_trace(tf, REG[i], signal_get_name(REG[i].name(), regname.c_str()));
        }
        for (int i = 0; i < 33; i++) {
            std::string regname = "REG_VALID_";
            regname += std::to_string(i);
            sc_trace(tf, REG_VALID[i], signal_get_name(REG_VALID[i].name(), regname.c_str()));
        }
        sc_trace(tf, CLK, GET_NAME(CLK));
        sc_trace(tf, RESET_N, GET_NAME(RESET_N));
}