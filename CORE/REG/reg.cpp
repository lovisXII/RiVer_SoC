#include "reg.h"
#include <systemc.h>




//bool REG_VALID[33] ; //bit de validité des registres

void reg::reading_adresses()
{
    RADR1_DATA.write(REG[RADR1.read()]) ; //on charge la donnée dans REG[index] dans le port de lectrue 1
    RADR2_DATA.write(REG[RADR2.read()]) ; //on charge la donnée dans REG[index] dans le port de lectrue 2
    READ_PC.write(REG[32]);
        
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
    
    for(int i = 0 ; i < 33 ; i++)
    {
        REG[i].write(0) ; // all register are initialized to 0
    }
    for(int i = 0 ; i < 33 ; i++)
    {
        REG_VALID[i] = true ;// Valid bits from every register are setup to true during reset
    }
    while(1)
    {
        if(WADR1.read() != 0) // si on cherche à écrire dans le registre 0 on ne fait rien
        {
            if(WADR1_VALID.read()) // if the register written is valid
            {
            REG[WADR1.read()].write(WADR1_DATA) ; // we write the data into the register from the written adress
            REG_VALID[WADR1.read()].write(1) ;    // Register written is written as valid 
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

