#include "csr.h"
#include <systemc.h>

sc_uint<32> test = 0;

void csr::writing_csr() {
    for (int rid = 0; rid < N_CSR; rid++) {
        csr_rc[rid].write(0);
    }
    csr_rc[3].write(0x00000000);  // mstatus
    csr_rc[4].write(0x40100100);  // misa

    wait(3);

    while (1) {
        if (CSR_ENABLE_BEFORE_FIFO_SM_S1.read()) {
            sc_uint<32> csr_wadr_sm = CSR_WADR_SM_S1.read();
            switch (csr_wadr_sm) {
                case 0xF11: break;                                  // mvendorid
                case 0xF12: break;                                  // marchid
                case 0xF13: break;                                  // mimpid
                case 0x301: break;                                  // misa
                case 0x304: csr_rc[5].write(CSR_WDATA_SM_S1); break;   // mie
                case 0x305: csr_rc[6].write(CSR_WDATA_SM_S1); break;   // mtvec
                case 0x310: break;                                  // mstatush
                case 0x341: csr_rc[8].write(CSR_WDATA_SM_S1); break;   // mepc
                case 0x342: csr_rc[9].write(CSR_WDATA_SM_S1); break;   // mcause
                case 0x343: csr_rc[10].write(CSR_WDATA_SM_S1); break;  // mtval
                case 0x344: csr_rc[11].write(CSR_WDATA_SM_S1); break;  // mip
                case 0x300: csr_rc[3].write(CSR_WDATA_SM_S1); break;   // mstatus
                case 0x340: csr_rc[12].write(CSR_WDATA_SM_S1); break;  // mstatus
                default: break;
            }
        }

        if (EXCEPTION_SM_S1.read()) {
            csr_rc[3]  = MSTATUS_WDATA_RM_S1.read();
            csr_rc[11] = MIP_WDATA_RM_S1.read();
            csr_rc[8]  = MEPC_WDATA_RM_S1.read();
            csr_rc[9]  = MCAUSE_WDATA_SM_S1.read();
            csr_rc[10] = MTVAL_WDATA_SM_S1;
        }
        wait(1);
    }
}

void csr::reading_csr() {
    switch (CSR_RADR_SD_S1.read()) {
        case 0xF11: CSR_RDATA_SC_S1.write(csr_rc[0]); break;
        case 0xF12: CSR_RDATA_SC_S1.write(csr_rc[1]); break;
        case 0xF13: CSR_RDATA_SC_S1.write(csr_rc[2]); break;
        case 0x300: CSR_RDATA_SC_S1.write(csr_rc[3]); break;
        case 0x301: CSR_RDATA_SC_S1.write(csr_rc[4]); break;
        case 0x304: CSR_RDATA_SC_S1.write(csr_rc[5]); break;
        case 0x305: CSR_RDATA_SC_S1.write(csr_rc[6]); break;
        case 0x310: CSR_RDATA_SC_S1.write(csr_rc[7]); break;
        case 0x341: CSR_RDATA_SC_S1.write(csr_rc[8]); break;
        case 0x342: CSR_RDATA_SC_S1.write(csr_rc[9]); break;
        case 0x343: CSR_RDATA_SC_S1.write(csr_rc[10]); break;
        case 0x344: CSR_RDATA_SC_S1.write(csr_rc[11]); break;
        case 0x340: CSR_RDATA_SC_S1.write(csr_rc[12]); break;
        default: CSR_RDATA_SC_S1.write(0); break;
    }
    MEPC_SC.write(csr_rc[8]);
    MSTATUS_RC.write(csr_rc[3]);
    MTVEC_VALUE_RC.write(csr_rc[6]);
    MIP_VALUE_RC.write(csr_rc[11]);
    MCAUSE_SC.write(csr_rc[9]);
}

void csr::trace(sc_trace_file* tf) {
    sc_trace(tf, CSR_WADR_SM_S1, GET_NAME(CSR_WADR_SM_S1));
    sc_trace(tf, CSR_WDATA_SM_S1, GET_NAME(CSR_WDATA_SM_S1));
    sc_trace(tf, CSR_ENABLE_BEFORE_FIFO_SM_S1, GET_NAME(CSR_ENABLE_BEFORE_FIFO_SM_S1));

    sc_trace(tf, EXCEPTION_SM_S1, GET_NAME(EXCEPTION_SM_S1));
    sc_trace(tf, MSTATUS_WDATA_RM_S1, GET_NAME(MSTATUS_WDATA_RM_S1));
    sc_trace(tf, MIP_WDATA_RM_S1, GET_NAME(MIP_WDATA_RM_S1));
    sc_trace(tf, MEPC_WDATA_RM_S1, GET_NAME(MEPC_WDATA_RM_S1));
    sc_trace(tf, MCAUSE_WDATA_SM_S1, GET_NAME(MCAUSE_WDATA_SM_S1));
    sc_trace(tf, MTVEC_VALUE_RC, GET_NAME(MTVEC_VALUE_RC));
    sc_trace(tf, MIP_VALUE_RC, GET_NAME(MIP_VALUE_RC));

    // Output :

    sc_trace(tf, CSR_RADR_SD_S1, GET_NAME(CSR_RADR_SD_S1));
    sc_trace(tf, CSR_RDATA_SC_S1, GET_NAME(CSR_RDATA_SC_S1));

    // General Interface :
    sc_trace(tf, csr_rc[0], signal_get_name(csr_rc[0].name(), "mvendorid"));
    sc_trace(tf, csr_rc[1], signal_get_name(csr_rc[1].name(), "marchid"));
    sc_trace(tf, csr_rc[2], signal_get_name(csr_rc[2].name(), "mimpid"));
    sc_trace(tf, csr_rc[3], signal_get_name(csr_rc[3].name(), "mstatus"));
    sc_trace(tf, csr_rc[4], signal_get_name(csr_rc[4].name(), "misa"));
    sc_trace(tf, csr_rc[5], signal_get_name(csr_rc[5].name(), "mie"));
    sc_trace(tf, csr_rc[6], signal_get_name(csr_rc[6].name(), "mtvec"));
    sc_trace(tf, csr_rc[7], signal_get_name(csr_rc[7].name(), "mstatush"));
    sc_trace(tf, csr_rc[8], signal_get_name(csr_rc[8].name(), "mepc"));
    sc_trace(tf, csr_rc[9], signal_get_name(csr_rc[9].name(), "mcause"));
    sc_trace(tf, csr_rc[10], signal_get_name(csr_rc[10].name(), "mtval"));
    sc_trace(tf, csr_rc[11], signal_get_name(csr_rc[11].name(), "mip"));
    sc_trace(tf, csr_rc[12], signal_get_name(csr_rc[12].name(), "mscratch"));

    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET_N, GET_NAME(RESET_N));
}