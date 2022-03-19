#include "dcache.h"


//a function to print the bits of any value
template<typename T>
void print_bits(T ptr) {
    size_t size = sizeof(T);
    unsigned char *b = (unsigned char*) &ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            cout << (int) byte;
        }
    }
}


int sc_main(int argc, char** argv)
{
    sc_signal<bool> CK;
    sc_signal<bool> RESET;

//   interfaz processeur
    sc_signal<sc_uint<32>> DATA_ADDRESS_M;
    sc_signal<sc_uint<32>> DATA_M;
    sc_signal<bool> LOAD_M;
    sc_signal<bool> STORE_M;
    sc_signal<bool> VALID_ADDRESS_M;

    sc_signal<sc_uint<32>> DATA_C;
    sc_signal<bool> STALL;       
    sc_signal<bool> MISS_C;             // 0 : HIT, 1 : MISS
    sc_signal<bool> VALID_DATA_C;
//   interfaz bus
    sc_signal<sc_uint<3>> SLAVE_ACK;     //  RDY: 00   WAIT: 01   RETRACT: 10
    sc_signal< bool > PI_GRANT;

    sc_signal<sc_uint<32>> DT;
    sc_signal<sc_uint<32>> A;
    
    sc_signal<bool> PI_REQ;

    dcache unit_dcache("dcache");    
    //base
    unit_dcache.CK(CK);
    unit_dcache.RESET(RESET);

    //processor interface
    //IN
    unit_dcache.DATA_ADDRESS_M(DATA_ADDRESS_M);
    unit_dcache.DATA_M(DATA_M);
    unit_dcache.LOAD_M(LOAD_M);
    unit_dcache.STORE_M(STORE_M);
    unit_dcache.VALID_ADDRESS_M(VALID_ADDRESS_M);
    //OUT
    unit_dcache.DATA_C(DATA_C);
    unit_dcache.STALL(STALL);
    unit_dcache.MISS_C(MISS_C);
    unit_dcache.VALID_DATA_C(VALID_DATA_C);

    //bus interface
    //IN
    unit_dcache.SLAVE_ACK(SLAVE_ACK);
    unit_dcache.PI_GRANT(PI_GRANT);
    //INTOUT
    unit_dcache.DT(DT);
    unit_dcache.A(A);
    //OUT
    unit_dcache.PI_REQ(PI_REQ);

    //INIT
    sc_start(1, SC_NS);
    
    sc_start(1, SC_NS);
    printf("good end\n");
    return 0;
}