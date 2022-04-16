#include <fstream>
#include <iostream>
#include "dcache.h"
#include "systemc.h"

using namespace std;

// un bloc est de 4mots - 512 blocs.
#define RAM_NB_BLOCK 512
#define RAM_NB_WORDS 4

// a function to print the bits of any value
template <typename T>
void print_bits(T ptr) {
    size_t         size = sizeof(T);
    unsigned char* b    = (unsigned char*)&ptr;
    unsigned char  byte;
    int            i, j;

    for (i = size - 1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            cout << (int)byte;
        }
    }
}
bool check_line_size(string line) {
    int cpt = 0;
    for (size_t i = 1; i < line.length(); i++) {
        if (line[i] == ' ') cpt++;
    }
    return cpt >= RAM_NB_WORDS;
}
int stringtoint(string integer) {
    int num = 0;
    for (int i = 0; i < integer.size() - 1; i++) {
        num += (integer[i] - 48) * pow(10, integer.size() - 1 - i);
    }
    return (num + integer[integer.size() - 1] - 48);
}
int sc_main(int argc, char** argv) {
    if (argc != 2) {
        printf("Error : number of argument are incorrect, send only the ram path");
        return 2;
    }
    // MP(RAM) DEFINITION
    int RAM[RAM_NB_BLOCK][RAM_NB_WORDS];
    // Attention div par 4 pour acceder a une adresse.

    ifstream ram_file(argv[1]);
    string   line;
    size_t   start, end;
    int      RAM_block = 0;
    int      RAM_block_offset;
    int      block_word;
    if (ram_file.is_open()) {
        while (getline(ram_file, line)) {
            if (RAM_block == RAM_NB_BLOCK || check_line_size(line)) {
                printf("ERROR : RAM file sizes didnt respect RAM Architecture\n");
                printf("\t\tNB BLOCK : %d\n", RAM_NB_BLOCK);
                printf("\t\tNB WORDS PER BLOCK : %d\n", RAM_NB_WORDS);
                return 1;
            }
            RAM_block_offset = 0;
            start            = 0;
            for (int i = 0; i < 4; ++i) {
                end                              = line.find_first_of(' ', start);
                block_word                       = stringtoint(line.substr(start, end - start));
                RAM[RAM_block][RAM_block_offset] = block_word;
                RAM_block_offset++;
                start = end + 1;
            }
            // std::cout <<RAM_block<<" : "
            << RAM[RAM_block][0] << " " <<
            // RAM[RAM_block][1]
            //          << " "
            << RAM[RAM_block][2] << " "
            << RAM[RAM_block][3]
            //
            << std::endl;
            RAM_block++;
        }
        ram_file.close();
    } else {
        printf("Error : wrong ram path");
        return 3;
    }

    // SIGNALS DEFINITION
    sc_signal<bool> CK;
    sc_signal<bool> RESET;

    //   interface processeur
    sc_signal<sc_uint<32>> DATA_ADDRESS_M;
    sc_signal<sc_uint<32>> DATA_M;
    sc_signal<bool>        LOAD_M;
    sc_signal<bool>        STORE_M;
    sc_signal<bool>        VALID_ADDRESS_M;

    sc_signal<sc_uint<32>> DATA_C;
    sc_signal<bool>        STALL;
    sc_signal<bool>        MISS_C;  // 0 : HIT, 1 : MISS
    sc_signal<bool>        VALID_DATA_C;
    //   interface bus
    sc_signal<bool>        DTA_VALID;  // data or/and adresse valid
    sc_signal<bool>        READ, WRITE;
    sc_signal<sc_uint<32>> DT;
    sc_signal<sc_uint<32>> A;
    sc_signal<bool>        SLAVE_ACK;

    dcache         unit_dcache("dcache");
    sc_trace_file* tf;
    tf = sc_create_vcd_trace_file("tf");
    // base
    unit_dcache.CK(CK);
    unit_dcache.RESET(RESET);

    // processor interface
    // IN
    unit_dcache.DATA_ADDRESS_M(DATA_ADDRESS_M);
    unit_dcache.DATA_M(DATA_M);
    unit_dcache.LOAD_M(LOAD_M);
    unit_dcache.STORE_M(STORE_M);
    unit_dcache.VALID_ADDRESS_M(VALID_ADDRESS_M);
    // OUT
    unit_dcache.DATA_C(DATA_C);
    unit_dcache.STALL(STALL);
    unit_dcache.MISS_C(MISS_C);
    unit_dcache.VALID_DATA_C(VALID_DATA_C);

    // bus interface
    // OUT
    unit_dcache.DTA_VALID(DTA_VALID);  // data or/and adresse valid
    unit_dcache.READ(READ);
    unit_dcache.WRITE(WRITE);
    // INTOUT
    unit_dcache.DT(DT);
    unit_dcache.A(A);
    unit_dcache.SLAVE_ACK(SLAVE_ACK);
    // trace
    unit_dcache.trace(tf);

    // RESET
    cout << "Reseting...." << endl;
    RESET.write(true);
    sc_start(3, SC_NS);
    RESET.write(false);
    // INIT
    bool dta_valid, read, write;
    int  dt, a;

    int bloc, offset;
    int cpt = 0;
    while (cpt < 10000000) {
        dta_valid = DTA_VALID.read();
        read      = LOAD_M.read();
        write     = STORE_M.read();

        dt = DT.read();
        a  = A.read();

        if (dta_valid) {
            bloc   = a >> 2;
            offset = a & 1 + (a & 2) * 2;
            if (read) { dt = RAM[bloc][offset]; }
            if (write) { RAM[bloc][offset] = dt; }
        }
        DT.write(dt);
        A.write(a);
        sc_start(500, SC_PS);
        cpt++;
    }
    sc_close_vcd_trace_file(tf);
    printf("good end\n");
    return 0;
}