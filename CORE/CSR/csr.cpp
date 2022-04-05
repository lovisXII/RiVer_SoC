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
        sc_uint<32> mstatus = csr_rc[3].read();
        switch (WADR_CSR_SM.read()) {
            case 0xF11: break;                                       // mvendorid
            case 0xF12: break;                                       // marchid
            case 0xF13: break;                                       // mimpid
            case 0x301: break;                                       // misa
            case 0x304: csr_rc[5].write(CSR_DATA_WRITE_SM); break;   // mie
            case 0x305: csr_rc[6].write(CSR_DATA_WRITE_SM); break;   // mtvec
            case 0x310: break;                                       // mstatush
            case 0x341: csr_rc[8].write(CSR_DATA_WRITE_SM); break;   // mepc
            case 0x342: csr_rc[9].write(CSR_DATA_WRITE_SM); break;   // mcause
            case 0x343: csr_rc[10].write(CSR_DATA_WRITE_SM); break;  // mtval
            case 0x344: csr_rc[11].write(CSR_DATA_WRITE_SM); break;  // mip
            case 0x300:
                mstatus[3]  = CSR_DATA_WRITE_SM[3];
                mstatus[7]  = CSR_DATA_WRITE_SM[7];
                mstatus[11] = CSR_DATA_WRITE_SM[11];
                mstatus[12] = CSR_DATA_WRITE_SM[12];
                break;  // mstatus
            default: DATA_READ_CSR_SC.write(0); break;
        }
        wait(1);
    }
}

void csr::reading_csr() {
    switch (RADR_CSR_SD.read()) {
        case 0xF11: csr_rc[0].write(CSR_DATA_WRITE_SM); break;
        case 0xF12: csr_rc[1].write(CSR_DATA_WRITE_SM); break;
        case 0xF13: csr_rc[2].write(CSR_DATA_WRITE_SM); break;
        case 0x300: csr_rc[3].write(CSR_DATA_WRITE_SM); break;
        case 0x301: csr_rc[4].write(CSR_DATA_WRITE_SM); break;
        case 0x304: csr_rc[5].write(CSR_DATA_WRITE_SM); break;
        case 0x305: csr_rc[6].write(CSR_DATA_WRITE_SM); break;
        case 0x310: csr_rc[7].write(CSR_DATA_WRITE_SM); break;
        case 0x341: csr_rc[8].write(CSR_DATA_WRITE_SM); break;
        case 0x342: csr_rc[9].write(CSR_DATA_WRITE_SM); break;
        case 0x343: csr_rc[10].write(CSR_DATA_WRITE_SM); break;
        case 0x344: csr_rc[11].write(CSR_DATA_WRITE_SM); break;
        default: DATA_READ_CSR_SC.write(0); break;
    }
}