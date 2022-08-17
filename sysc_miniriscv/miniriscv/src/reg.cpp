#include "reg.h"

void reg::read() {
    RDATA1_SR = REG_RR[RADR1_SD.read()];  // on charge la donnée dans REG[index] dans
                                          // le port de lecture 1
    RDATA2_SR = REG_RR[RADR2_SD.read()];  // on charge la donnée dans REG[index] dans
                                          // le port de lecture 2
    READ_PC_SR = REG_RR[32];
}

void reg::write() {
    // Starting RESET :

    for (int i = 0; i < 32; i++) {
        REG_RR[i].write(0);  // all register are initialized to 0
    }
    REG_RR[32].write(START_ADRESS);
    wait(1);
    while (1) {
        if (WRITE_PC_ENABLE_SD || (WADR_SW.read() == 32 && WENABLE_SW)) {
            REG_RR[32] = WRITE_PC_SD;  // we write the data into the
                                       // register from the written adress
        }
        if (WADR_SW.read() != 0 && WADR_SW.read() != 32 && WENABLE_SW) {
            REG_RR[WADR_SW.read()].write(WDATA_SW);  // we write the data into the register
                                                     // from the written adress
        }
        wait(1);
    }
}

void reg::trace(sc_trace_file* tf) {
    sc_trace(tf, RADR1_SD, GET_NAME(RADR1_SD));
    sc_trace(tf, RADR2_SD, GET_NAME(RADR2_SD));
    sc_trace(tf, RDATA1_SR, GET_NAME(RDATA1_SR));
    sc_trace(tf, RDATA2_SR, GET_NAME(RDATA2_SR));
    sc_trace(tf, WADR_SW, GET_NAME(WADR_SW));
    sc_trace(tf, WENABLE_SW, GET_NAME(WENABLE_SW));
    sc_trace(tf, WDATA_SW, GET_NAME(WDATA_SW));
    sc_trace(tf, READ_PC_SR, GET_NAME(READ_PC_SR));
    sc_trace(tf, WRITE_PC_SD, GET_NAME(WRITE_PC_SD));
    sc_trace(tf, WRITE_PC_ENABLE_SD, GET_NAME(WRITE_PC_ENABLE_SD));
    sc_trace(tf, PC_RR, GET_NAME(PC_RR));

    for (int i = 0; i < 32; i++) {
        std::string regname = "REG_";
        regname += std::to_string(i);
        sc_trace(tf, REG_RR[i], signal_get_name(REG_RR[i].name(), regname.c_str()));
    }
}