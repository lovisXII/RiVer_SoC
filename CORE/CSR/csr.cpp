#include "csr.h"
#include <systemc.h>

sc_uint<32> test = 0 ;

void csr::writing_csr() {
    for (int rid = 0; rid < N_CSR; rid++) {
        csr_rc[rid].write(0);
    }
    csr_rc[3].write(0x00000000);  // mstatus
    csr_rc[4].write(0x40100100);  // misa
    csr_rc[6].write(adress_mtvec);  // mtvec

    wait(3);

    while (1) {
        if (CSR_ENABLE_BEFORE_FIFO_SM.read()) {
            sc_uint<32> csr_wadr_sm = CSR_WADR_SM.read();
            switch (csr_wadr_sm) {
                case 0xF11: break;                                  // mvendorid
                case 0xF12: break;                                  // marchid
                case 0xF13: break;                                  // mimpid
                case 0x301: break;                                  // misa
                case 0x304: csr_rc[5].write(CSR_WDATA_SM); break;   // mie
                case 0x305: csr_rc[6].write(CSR_WDATA_SM); break;   // mtvec
                case 0x310: break;                                  // mstatush
                case 0x341: csr_rc[8].write(CSR_WDATA_SM); break;   // mepc
                case 0x342: csr_rc[9].write(CSR_WDATA_SM); break;   // mcause
                case 0x343: csr_rc[10].write(CSR_WDATA_SM); break;  // mtval
                case 0x344: csr_rc[11].write(CSR_WDATA_SM); break;  // mip
                case 0x300: csr_rc[3].write(CSR_WDATA_SM); break;   // mstatus
                default: break;
            }
        }

        if (EXCEPTION_RM.read()) {
            csr_rc[3]  = MSTATUS_WDATA_RM.read();
            csr_rc[11] = MIP_WDATA_RM.read();
            csr_rc[8]  = MEPC_WDATA_RM.read();
            csr_rc[9]  = MCAUSE_WDATA_RM.read();
        }
        wait(1);
    }
}

void csr::reading_csr() {
    if (EXCEPTION_RM.read() == 0) {
        switch (CSR_RADR_SD.read()) {
            case 0xF11: CSR_RDATA_SC.write(csr_rc[0]); break;
            case 0xF12: CSR_RDATA_SC.write(csr_rc[1]); break;
            case 0xF13: CSR_RDATA_SC.write(csr_rc[2]); break;
            case 0x300: CSR_RDATA_SC.write(csr_rc[3]); break;
            case 0x301: CSR_RDATA_SC.write(csr_rc[4]); break;
            case 0x304: CSR_RDATA_SC.write(csr_rc[5]); break;
            case 0x305: CSR_RDATA_SC.write(csr_rc[6]); break;
            case 0x310: CSR_RDATA_SC.write(csr_rc[7]); break;
            case 0x341: CSR_RDATA_SC.write(csr_rc[8]); break;
            case 0x342: CSR_RDATA_SC.write(csr_rc[9]); break;
            case 0x343: CSR_RDATA_SC.write(csr_rc[10]); break;
            case 0x344: CSR_RDATA_SC.write(csr_rc[11]); break;
            default: CSR_RDATA_SC.write(0); break;
        }
    }
    MTVEC_VALUE_RC.write(csr_rc[6]);
    if (EXCEPTION_RM.read()) {
        MIP_VALUE_RC.write(csr_rc[11]);
    }
}

void csr::trace(sc_trace_file* tf) {
    sc_trace(tf, CSR_WADR_SM, GET_NAME(CSR_WADR_SM));
    sc_trace(tf, CSR_WDATA_SM, GET_NAME(CSR_WDATA_SM));
    sc_trace(tf, CSR_ENABLE_BEFORE_FIFO_SM, GET_NAME(CSR_ENABLE_BEFORE_FIFO_SM));

    sc_trace(tf, EXCEPTION_RM, GET_NAME(EXCEPTION_RM));
    sc_trace(tf, MSTATUS_WDATA_RM, GET_NAME(MSTATUS_WDATA_RM));
    sc_trace(tf, MIP_WDATA_RM, GET_NAME(MIP_WDATA_RM));
    sc_trace(tf, MEPC_WDATA_RM, GET_NAME(MEPC_WDATA_RM));
    sc_trace(tf, MCAUSE_WDATA_RM, GET_NAME(MCAUSE_WDATA_RM));
    sc_trace(tf, MTVEC_VALUE_RC, GET_NAME(MTVEC_VALUE_RC));
    sc_trace(tf, MIP_VALUE_RC, GET_NAME(MIP_VALUE_RC));

    // Output :

    sc_trace(tf, CSR_RADR_SD, GET_NAME(CSR_RADR_SD));
    sc_trace(tf, CSR_RDATA_SC, GET_NAME(CSR_RDATA_SC));

    // General Interface :

    for (int i = 0; i < N_CSR; i++) {
        std::string regname = "CSR_";
        regname += std::to_string(i);
        sc_trace(tf, csr_rc[i], signal_get_name(csr_rc[i].name(), regname.c_str()));
    }
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET_N, GET_NAME(RESET_N));
}