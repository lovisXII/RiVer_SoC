#include "alu.h"

void alu::trace(sc_trace_file* tf) {
    sc_trace(tf, OP1_SE, GET_NAME(OP1_SE));
    sc_trace(tf, OP2_SE, GET_NAME(OP2_SE));
    sc_trace(tf, CIN_SE, GET_NAME(CIN_SE));
    sc_trace(tf, CMD_SE, GET_NAME(CMD_SE));
    sc_trace(tf, RES_SE, GET_NAME(RES_SE));
}