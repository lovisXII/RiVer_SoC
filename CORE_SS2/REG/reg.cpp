#include "reg.h"
#include <systemc.h>

// bool REG_VALID[33] ; //bit de validité des registres

void reg::reading_adresses() {
    RDATA1_SR_S1 = REG_RR[RADR1_SD_S1.read()];  // on charge la donnée dans REG[index] dans
                                                // le port de lecture 1
    RDATA2_SR_S1 = REG_RR[RADR2_SD_S1.read()];  // on charge la donnée dans REG[index] dans
                                                // le port de lecture 2

    RDATA1_SR_S2 = REG_RR[RADR1_SD_S2.read()];

    RDATA2_SR_S2 = REG_RR[RADR2_SD_S2.read()];

    if (RESET_N.read()) {
        READ_PC_SR.write(REG_RR[32]);
    } else {
        READ_PC_SR.write(PC_INIT.read());
    }
}

void reg::writing_adresse() {
    // Starting RESET :

    for (int i = 0; i < 32; i++) {
        REG_RR[i].write(0);  // all register are initialized to 0
    }
    REG_RR[32].write(PC_INIT.read());
    wait(1);
    while (1) {
        if (WRITE_PC_ENABLE_SD.read()) {
            REG_RR[32].write(WRITE_PC_SD);  // we write the data into the
                                            // register from the written adress
        }
        if (not(WADR_SW_S1.read() == 32 && WRITE_PC_ENABLE_SD.read())) {
            if (WADR_SW_S1.read() != 0)  // si on cherche à écrire dans le registre 0 on ne fait rien
            {
                if (WENABLE_SW_S1.read())  // if the register written is valid
                {
                    REG_RR[WADR_SW_S1.read()].write(WDATA_SW_S1);  // we write the data into the register from
                                                             // the written adress
                }
            }
        }
        if (not(WADR_SW_S2.read() == 32 && WRITE_PC_ENABLE_SD.read())) {
            if (WADR_SW_S2.read() != 0)  // si on cherche à écrire dans le registre 0 on ne fait rien
            {
                if (WENABLE_SW_S2.read())  // if the register written is valid
                {
                    REG_RR[WADR_SW_S2.read()].write(WDATA_SW_S2);  // we write the data into the register from
                                                             // the written adress
                }
            }
        }
        wait(1);
    }
}

void reg::trace(sc_trace_file* tf) {
    sc_trace(tf, RADR1_SD_S1, GET_NAME(RADR1_SD_S1));
    sc_trace(tf, RADR2_SD_S1, GET_NAME(RADR2_SD_S1));
    sc_trace(tf, RDATA1_SR_S1, GET_NAME(RDATA1_SR_S1));
    sc_trace(tf, RDATA2_SR_S1, GET_NAME(RDATA2_SR_S1));
    sc_trace(tf, RADR1_SD_S2, GET_NAME(RADR1_SD_S2));
    sc_trace(tf, RADR2_SD_S2, GET_NAME(RADR2_SD_S2));
    sc_trace(tf, RDATA1_SR_S2, GET_NAME(RDATA1_SR_S2));
    sc_trace(tf, RDATA2_SR_S2, GET_NAME(RDATA2_SR_S2));
    sc_trace(tf, WADR_SW_S1, GET_NAME(WADR_SW_S1));
    sc_trace(tf, WENABLE_SW_S1, GET_NAME(WENABLE_SW_S1));
    sc_trace(tf, WDATA_SW_S1, GET_NAME(WDATA_SW_S1));
    sc_trace(tf, READ_PC_SR, GET_NAME(READ_PC_SR));
    sc_trace(tf, WRITE_PC_SD, GET_NAME(WRITE_PC_SD));
    sc_trace(tf, WRITE_PC_ENABLE_SD, GET_NAME(WRITE_PC_ENABLE_SD));

    for (int i = 0; i < 33; i++) {
        std::string regname = "REG_";
        regname += std::to_string(i);
        sc_trace(tf, REG_RR[i], signal_get_name(REG_RR[i].name(), regname.c_str()));
    }
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET_N, GET_NAME(RESET_N));
}