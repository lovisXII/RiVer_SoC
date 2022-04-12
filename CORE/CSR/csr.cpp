#include "csr.h"
#include <systemc.h>

void csr::writing_csr() {
    for (int rid = 0; rid < N_CSR; rid++) {
        csr_rc[rid].write(0);
    }
    csr_rc[3].write(0x00000000);  // mstatus
    csr_rc[4].write(0x40100100);  // misa

    wait(3);

    while (1) {
        if (CSR_WENABLE_RM.read()) {
            sc_uint<32> mstatus = csr_rc[3].read();
            switch (CSR_WADR_SM.read()) {
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
                case 0x300:
                    mstatus[3]  = CSR_WDATA_SM[3];
                    mstatus[7]  = CSR_WDATA_SM[7];
                    mstatus[11] = CSR_WDATA_SM[11];
                    mstatus[12] = CSR_WDATA_SM[12];
                    break;  // mstatus
                default: CSR_RDATA_SC.write(0); break;
            }
        }
        if(EXCEPTION_RM.read()){
            csr_rc[3] = MSTATUS_WDATA_RM.read() ;
            csr_rc[11]= MIP_WDATA_RM.read() ;
            csr_rc[8]= MEPC_WDATA_RM.read() ;
            csr_rc[9]= MCAUSE_WDATA_RM.read() ;
        }

        wait(1);
    }
}

void csr::reading_csr() {
    switch (CSR_RADR_SD.read()) {
        case 0xF11: csr_rc[0].write(CSR_WDATA_SM); break;
        case 0xF12: csr_rc[1].write(CSR_WDATA_SM); break;
        case 0xF13: csr_rc[2].write(CSR_WDATA_SM); break;
        case 0x300: csr_rc[3].write(CSR_WDATA_SM); break;
        case 0x301: csr_rc[4].write(CSR_WDATA_SM); break;
        case 0x304: csr_rc[5].write(CSR_WDATA_SM); break;
        case 0x305: csr_rc[6].write(CSR_WDATA_SM); break;
        case 0x310: csr_rc[7].write(CSR_WDATA_SM); break;
        case 0x341: csr_rc[8].write(CSR_WDATA_SM); break;
        case 0x342: csr_rc[9].write(CSR_WDATA_SM); break;
        case 0x343: csr_rc[10].write(CSR_WDATA_SM); break;
        case 0x344: csr_rc[11].write(CSR_WDATA_SM); break;
        default: CSR_RDATA_SC.write(0); break;
    if(EXCEPTION_RM.read()){
        MTVEC_VALUE_RC.write(csr_rc[6]) ;
        MIP_VALUE_RC.write(csr_rc[11]) ;
    }
    }
}