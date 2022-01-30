#include "reg.h"
#include <systemc.h>




    bool REG0_VALID ;
    bool REG1_VALID ;
    bool REG2_VALID ;
    bool REG3_VALID ;
    bool REG4_VALID ;
    bool REG5_VALID ;
    bool REG6_VALID ;
    bool REG7_VALID ;
    bool REG8_VALID ;
    bool REG9_VALID ;
    bool REG10_VALID ;
    bool REG11_VALID ;
    bool REG12_VALID ;
    bool REG13_VALID ;
    bool REG14_VALID ;
    bool REG15_VALID ;
    bool REG16_VALID ;
    bool REG17_VALID ;
    bool REG18_VALID ;
    bool REG19_VALID ;
    bool REG20_VALID ;
    bool REG21_VALID ;
    bool REG22_VALID ;
    bool REG23_VALID ;
    bool REG24_VALID ;
    bool REG25_VALID ;
    bool REG26_VALID ;
    bool REG27_VALID ;
    bool REG28_VALID ;
    bool REG29_VALID ;
    bool REG30_VALID ;
    bool REG31_VALID ;
    bool REG32_VALID ;

void reg::reading_adresses()
{

    wait(3) ;

    cerr << "reading process :" << endl ;
    while(1)
    {

//----------------------------------------Reading Port 2 :-----------------------------------

        if(RADR1.read() == 0 && RADR1_VALID == 1)
        {
            // r0 is the constant registrer equal to 0, can't be modify
        }
        if(RADR1.read() == 1 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG1.read()) ;
            REG1_VALID = 0 ;
        }
        if(RADR1.read() == 2 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG2.read()) ;
            REG2_VALID = 0 ;
        }
        if(RADR1.read() == 3 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG3.read()) ;
            REG3_VALID = 0 ;
        }
        if(RADR1.read() == 4 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG4.read()) ;
            REG4_VALID = 0 ;
        }
        if(RADR1.read() == 5 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG5.read()) ;
            REG5_VALID = 0 ;
        }
        if(RADR1.read() == 6 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG6.read()) ;
            REG6_VALID = 0 ;
        }
        if(RADR1.read() == 7 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG7.read()) ;
            REG7_VALID = 0 ;
        }
        if(RADR1.read() == 8 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG8.read()) ;
            REG8_VALID = 0 ;
        }
        if(RADR1.read() == 9 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG9.read()) ;
            REG9_VALID = 0 ;
        }
        if(RADR1.read() == 10 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG10.read()) ;
            REG10_VALID = 0 ;
        }
        if(RADR1.read() == 11 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG11.read()) ;
            REG11_VALID = 0 ;
        }
        if(RADR1.read() == 12 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG12.read()) ;
            REG12_VALID = 0 ;
        }
        if(RADR1.read() == 13 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG13.read()) ;
            REG13_VALID = 0 ;
        }
        if(RADR1.read() == 14 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG14.read()) ;
            REG14_VALID = 0 ;
        }
        if(RADR1.read() == 15 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG15.read()) ;
            REG15_VALID = 0 ;
        }
        if(RADR1.read() == 16 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG16.read()) ;
            REG16_VALID = 0 ;
        }
        if(RADR1.read() == 17 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG17.read()) ;
            REG17_VALID = 0 ;
        }
        if(RADR1.read() == 18 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG18.read()) ;
            REG18_VALID = 0 ;
        }
        if(RADR1.read() == 19 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG19.read()) ;
            REG19_VALID = 0 ;
        }
        if(RADR1.read() == 20 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG20.read()) ;
            REG20_VALID = 0 ;
        }
        if(RADR1.read() == 21 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG21.read()) ;
            REG21_VALID = 0 ;
        }
        if(RADR1.read() == 22 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG22.read()) ;
            REG22_VALID = 0 ;
        }
        if(RADR1.read() == 23 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG23.read()) ;
            REG23_VALID = 0 ;
        }
        if(RADR1.read() == 24 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG24.read()) ;
            REG24_VALID = 0 ;
        }
        if(RADR1.read() == 25 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG25.read()) ;
            REG25_VALID = 0 ;
        }
        if(RADR1.read() == 26 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG26.read()) ;
            REG26_VALID = 0 ;
        }
        if(RADR1.read() == 27 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG27.read()) ;
            REG27_VALID = 0 ;
        }
        if(RADR1.read() == 28 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG28.read()) ;
            REG28_VALID = 0 ;
        }
        if(RADR1.read() == 29 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG29.read()) ;
            REG29_VALID = 0 ;
        }
        if(RADR1.read() == 30 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG30.read()) ;
            REG30_VALID = 0 ;
        }
        if(RADR1.read() == 31 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG31.read()) ;
            REG31_VALID = 0 ;
        }
        if(RADR1.read() == 32 && RADR1_VALID == 1)
        {
            RADR1_DATA.write(REG32.read()) ;
            REG32_VALID = 0 ;
        }

//----------------------------------------Reading Port 2 :-----------------------------------

        cerr << "reading port 2 :" << endl ; 

        if(RADR2.read() == 0 && RADR2_VALID == 1)
        {
            // r0 is the constant registrer equal to 0, can't be modify
        }
        if(RADR2.read() == 1 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG1.read()) ;
            REG1_VALID = 0 ;
        }
        if(RADR2.read() == 2 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG2.read()) ;
            REG2_VALID = 0 ;
        }
        if(RADR2.read() == 3 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG3.read()) ;
            REG3_VALID = 0 ;
        }
        if(RADR2.read() == 4 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG4.read()) ;
            REG4_VALID = 0 ;
        }
        if(RADR2.read() == 5 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG5.read()) ;
            REG5_VALID = 0 ;
        }
        if(RADR2.read() == 6 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG6.read()) ;
            REG6_VALID = 0 ;
        }
        if(RADR2.read() == 7 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG7.read()) ;
            REG7_VALID = 0 ;
        }
        if(RADR2.read() == 8 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG8.read()) ;
            REG8_VALID = 0 ;
        }
        if(RADR2.read() == 9 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG9.read()) ;
            REG9_VALID = 0 ;
        }
        if(RADR2.read() == 10 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG10.read()) ;
            REG10_VALID = 0 ;
        }
        if(RADR2.read() == 11 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG11.read()) ;
            REG11_VALID = 0 ;
        }
        if(RADR2.read() == 12 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG12.read()) ;
            REG12_VALID = 0 ;
        }
        if(RADR2.read() == 13 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG13.read()) ;
            REG13_VALID = 0 ;
        }
        if(RADR2.read() == 14 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG14.read()) ;
            REG14_VALID = 0 ;
        }
        if(RADR2.read() == 15 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG15.read()) ;
            REG15_VALID = 0 ;
        }
        if(RADR2.read() == 16 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG16.read()) ;
            REG16_VALID = 0 ;
        }
        if(RADR2.read() == 17 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG17.read()) ;
            REG17_VALID = 0 ;
        }
        if(RADR2.read() == 18 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG18.read()) ;
            REG18_VALID = 0 ;
        }
        if(RADR2.read() == 19 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG19.read()) ;
            REG19_VALID = 0 ;
        }
        if(RADR2.read() == 20 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG20.read()) ;
            REG20_VALID = 0 ;
        }
        if(RADR2.read() == 21 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG21.read()) ;
            REG21_VALID = 0 ;
        }
        if(RADR2.read() == 22 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG22.read()) ;
            REG22_VALID = 0 ;
        }
        if(RADR2.read() == 23 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG23.read()) ;
            REG23_VALID = 0 ;
        }
        if(RADR2.read() == 24 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG24.read()) ;
            REG24_VALID = 0 ;
        }
        if(RADR2.read() == 25 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG25.read()) ;
            REG25_VALID = 0 ;
        }
        if(RADR2.read() == 26 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG26.read()) ;
            REG26_VALID = 0 ;
        }
        if(RADR2.read() == 27 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG27.read()) ;
            REG27_VALID = 0 ;
        }
        if(RADR2.read() == 28 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG28.read()) ;
            REG28_VALID = 0 ;
        }
        if(RADR2.read() == 29 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG29.read()) ;
            REG29_VALID = 0 ;
        }
        if(RADR2.read() == 30 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG30.read()) ;
            REG30_VALID = 0 ;
        }
        if(RADR2.read() == 31 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG31.read()) ;
            REG31_VALID = 0 ;
        }
        if(RADR2.read() == 32 && RADR2_VALID == 1)
        {
            RADR2_DATA.write(REG32.read()) ;
            REG32_VALID = 0 ;
        }
        
        
    READ_PC_VALID.write(REG32_VALID) ;
    wait(1) ;
    }

}

void reg::writing_adresse()
{
    RADR1_VALID.write(true) ;
    RADR2_VALID.write(true) ;
    
    REG0.write(0) ;
    REG1.write(0) ;
    REG2.write(0) ;
    REG3.write(0) ;
    REG4.write(0) ;
    REG5.write(0) ;
    REG6.write(0) ;
    REG7.write(0) ;
    REG8.write(0) ;
    REG9.write(0) ;
    REG10.write(0) ;
    REG11.write(0) ;
    REG12.write(0) ;
    REG13.write(0) ;
    REG14.write(0) ;
    REG15.write(0) ;
    REG16.write(0) ;
    REG17.write(0) ;
    REG18.write(0) ;
    REG19.write(0) ;
    REG20.write(0) ;
    REG21.write(0) ;
    REG22.write(0) ;
    REG23.write(0) ;
    REG24.write(0) ;
    REG25.write(0) ;
    REG26.write(0) ;
    REG27.write(0) ;
    REG28.write(0) ;
    REG29.write(0) ;
    REG30.write(0) ;
    REG31.write(0) ;
    REG32.write(0) ;


    // REG0_VALID = true ;
    // REG1_VALID = true ;
    // REG2_VALID = true ;
    // REG3_VALID = true ;
    // REG4_VALID = true ;
    // REG5_VALID = true ;
    // REG6_VALID = true ;
    // REG7_VALID = true ;
    // REG8_VALID = true ;
    // REG9_VALID = true ;
    // REG10_VALID = true ;
    // REG11_VALID = true ;
    // REG12_VALID = true ;
    // REG13_VALID = true ;
    // REG14_VALID = true ;
    // REG15_VALID = true ;
    // REG16_VALID = true ;
    // REG17_VALID = true ;
    // REG18_VALID = true ;
    // REG19_VALID = true ;
    // REG20_VALID = true ;
    // REG21_VALID = true ;
    // REG22_VALID = true ;
    // REG23_VALID = true ;
    // REG24_VALID = true ;
    // REG25_VALID = true ;
    // REG26_VALID = true ;
    // REG27_VALID = true ;
    // REG28_VALID = true ;
    // REG29_VALID = true ;
    // REG30_VALID = true ;
    // REG31_VALID = true ;
    // REG32_VALID = true ;

    wait(3) ;
    cerr <<"#############################"<< "end reset : " << endl ;
    while(1)
    {
        if(WADR1.read() == 0 && WADR1_VALID == 1)
        {
            // r0 is the constant registrer equal to 0, can't be modify
        }
        if(WADR1.read() == 1 && WADR1_VALID == 1)
        {
            REG1.write(WADR1_DATA.read()) ;
            REG1_VALID = 1 ;
        }
        if(WADR1.read() == 2 && WADR1_VALID == 1)
        {
            REG2.write(WADR1_DATA.read()) ;
            REG2_VALID = 1 ;
        }
        if(WADR1.read() == 3 && WADR1_VALID == 1)
        {
            REG3.write(WADR1_DATA.read()) ;
            REG3_VALID = 1 ;
        }
        if(WADR1.read() == 4 && WADR1_VALID == 1)
        {
            REG4.write(WADR1_DATA.read()) ;
            REG4_VALID = 1 ;
        }
        if(WADR1.read() == 5 && WADR1_VALID == 1)
        {
            REG5.write(WADR1_DATA.read()) ;
            REG5_VALID = 1 ;
        }
        if(WADR1.read() == 6 && WADR1_VALID == 1)
        {
            REG6.write(WADR1_DATA.read()) ;
            REG6_VALID = 1 ;
            cerr << "ecrit dans 6" << endl ;
        }
        if(WADR1.read() == 7 && WADR1_VALID == 1)
        {
            REG7.write(WADR1_DATA.read()) ;
            REG7_VALID = 1 ;
        }
        if(WADR1.read() == 8 && WADR1_VALID == 1)
        {
            REG8.write(WADR1_DATA.read()) ;
            REG8_VALID = 1 ;
        }
        if(WADR1.read() == 9 && WADR1_VALID == 1)
        {
            REG9.write(WADR1_DATA.read()) ;
            REG9_VALID = 1 ;
        }
        if(WADR1.read() == 10 && WADR1_VALID == 1)
        {
            REG10.write(WADR1_DATA.read()) ;
            REG10_VALID = 1 ;
        }
        if(WADR1.read() == 11 && WADR1_VALID == 1)
        {
            REG11.write(WADR1_DATA.read()) ;
            REG11_VALID = 1 ;
        }
        if(WADR1.read() == 12 && WADR1_VALID == 1)
        {
            REG12.write(WADR1_DATA.read()) ;
            REG12_VALID = 1 ;
        }
        if(WADR1.read() == 13 && WADR1_VALID == 1)
        {
            REG13.write(WADR1_DATA.read()) ;
            REG13_VALID = 1 ;
        }
        if(WADR1.read() == 14 && WADR1_VALID == 1)
        {
            REG14.write(WADR1_DATA.read()) ;
            REG14_VALID = 1 ;
        }
        if(WADR1.read() == 15 && WADR1_VALID == 1)
        {
            REG15.write(WADR1_DATA.read()) ;
            REG15_VALID = 1 ;
        }
        if(WADR1.read() == 16 && WADR1_VALID == 1)
        {
            REG16.write(WADR1_DATA.read()) ;
            REG16_VALID = 1 ;
        }
        if(WADR1.read() == 17 && WADR1_VALID == 1)
        {
            REG17.write(WADR1_DATA.read()) ;
            REG17_VALID = 1 ;
        }
        if(WADR1.read() == 18 && WADR1_VALID == 1)
        {
            REG18.write(WADR1_DATA.read()) ;
            REG18_VALID = 1 ;
        }
        if(WADR1.read() == 19 && WADR1_VALID == 1)
        {
            REG19.write(WADR1_DATA.read()) ;
            REG19_VALID = 1 ;
        }
        if(WADR1.read() == 20 && WADR1_VALID == 1)
        {
            REG20.write(WADR1_DATA.read()) ;
            REG20_VALID = 1 ;
        }
        if(WADR1.read() == 21 && WADR1_VALID == 1)
        {
            REG21.write(WADR1_DATA.read()) ;
            REG21_VALID = 1 ;
        }
        if(WADR1.read() == 22 && WADR1_VALID == 1)
        {
            REG22.write(WADR1_DATA.read()) ;
            REG22_VALID = 1 ;
        }
        if(WADR1.read() == 23 && WADR1_VALID == 1)
        {
            REG23.write(WADR1_DATA.read()) ;
            REG23_VALID = 1 ;
        }
        if(WADR1.read() == 24 && WADR1_VALID == 1)
        {
            REG24.write(WADR1_DATA.read()) ;
            REG24_VALID = 1 ;
        }
        if(WADR1.read() == 25 && WADR1_VALID == 1)
        {
            REG25.write(WADR1_DATA.read()) ;
            REG25_VALID = 1 ;
        }
        if(WADR1.read() == 26 && WADR1_VALID == 1)
        {
            REG26.write(WADR1_DATA.read()) ;
            REG26_VALID = 1 ;
        }
        if(WADR1.read() == 27 && WADR1_VALID == 1)
        {
            REG27.write(WADR1_DATA.read()) ;
            REG27_VALID = 1 ;
        }
        if(WADR1.read() == 28 && WADR1_VALID == 1)
        {
            REG28.write(WADR1_DATA.read()) ;
            REG28_VALID = 1 ;
        }
        if(WADR1.read() == 29 && WADR1_VALID == 1)
        {
            REG29.write(WADR1_DATA.read()) ;
            REG29_VALID = 1 ;
        }
        if(WADR1.read() == 30 && WADR1_VALID == 1)
        {
            REG30.write(WADR1_DATA.read()) ;
            REG30_VALID = 1 ;
        }
        if(WADR1.read() == 31 && WADR1_VALID == 1)
        {
            REG31.write(WADR1_DATA.read()) ;
            REG31_VALID = 1 ;
        }
        if(WADR1.read() == 32 && WADR1_VALID == 1)
        {
            REG32.write(WADR1_DATA.read()) ;
            REG32_VALID = 1 ;
        }
        if(INC_PC_VALID.read() == 1)
        {
            REG32.write(REG32.read() + 4) ;
        }
        wait(1) ;
    }
}

