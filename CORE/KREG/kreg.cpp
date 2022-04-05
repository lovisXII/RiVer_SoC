#include "kreg.h"
#include <systemc.h>

void kreg::writing_csr() {
    if (RESET_N.read()) {
        int adress;

        for (adress = 0; adress < 8191; adress++) {
            CSR_adress_SK[adress].write(0);
        }
        CSR_adress_SK[0x301].write(0x40100100);  // misa
        CSR_adress_SK[0x300].write(0x0);         // mstatus
        CSR_adress_SK[0x305].write(0x0);         // mtvec
        CSR_adress_SK[0x305].write(0x0);         // mie
        CSR_adress_SK[0x305].write(0x0);         // mip

        else {
        }
    }
