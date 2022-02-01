#include "reg.h"
#include <systemc.h>




//bool REG_VALID[33] ; //bit de validité des registres

void reg::reading_adresses()
{
    if(RADR1_VALID.read())
    {
     RADR1_DATA.write(REG[RADR1.read()]) ; //on charge la donnée dans REG[index] dans le port de lectrue 1
    }
    else
    {
        RADR1_DATA.write(0) ;
    }
    if(RADR2_VALID.read())
    {
        RADR2_DATA.write(REG[RADR2.read()]) ; //on charge la donnée dans REG[index] dans le port de lectrue 2
    } 
    else
    {
        RADR2_DATA.write(0) ;
    }
        

}

void reg::writing_adresse()
{
    //Starting RESET :

    RADR1_VALID.write(true) ;
    RADR2_VALID.write(true) ;
    
    for(int i = 0 ; i < 33 ; i++)
    {
        REG[i].write(0) ;
    }
    for(int i = 0 ; i < 33 ; i++)
    {
        REG_VALID[i] = true ;
    }
    wait(3) ;

    while(1)
    {
        if(WADR1.read() == 0) // si on cherche à écrire dans le registre 0 on ne fait rien
        {

        }
        else
        {
            if(WADR1_VALID.read())
            {
            REG[WADR1.read()].write(WADR1_DATA) ;
            REG_VALID[WADR1.read()].write(1) ; // on rend valide le port d'écriture
            } 
        }

        // Invalidation du registre destination
        
        if(!INVAL_DEST.read())
        {
         REG_VALID[ADR_DEST.read()].write(0) ; // Invalidation du port lié au registre destination 
        }

        // Invalidation de la data lue par adr1 et adr2 

        if(ADR_DEST.read() == RADR1.read())
        {
            RADR1_VALID.write(REG_VALID[ADR_DEST.read()]) ;
        }
        if(ADR_DEST.read() == RADR2.read())
        {
            RADR2_VALID.write(REG_VALID[ADR_DEST.read()]) ;
        }
        READ_PC_VALID.write(REG_VALID[32]) ; // la validité de PC est la validité du registre correspondant soit r32

        wait(1) ;
    }
}

