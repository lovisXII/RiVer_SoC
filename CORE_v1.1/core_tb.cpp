#include "systemc.h"

// CORE 5 stages
#include "core.h"

// CACHES
#include "CACHES/dcache.h"
//#include "CACHES/icache.h"

#include <string>
#include <unordered_map>
#include "UTIL/colors.h"
#include "elfio/elfio.hpp"

using namespace std;
using namespace ELFIO;
// arguments
int sc_main(int argc, char* argv[]) {
    unordered_map<int, int> ram;
    elfio                   reader;  // creation of an elfio object
    string                  path(argv[1]);
    int                     start_adr;
    int                     good_adr;
    int                     bad_adr;
    char                    test[512] = "> a.out.txt";

    if (path.substr(path.find_last_of(".") + 1) == "s") {  // checking if the argument is a assembly file
        char temp[512];
        char temp_text[512];

        sprintf(temp,
                "riscv32-unknown-elf-gcc -nostdlib %s",
                path.c_str());  // writting "riscv32-unknown-elf-gcc -nostdlib path" in temp
        system((char*)temp);    // send the command in temp to the terminal
        path = "a.out";         // give the output

        sprintf(temp_text, "riscv32-unknown-elf-objdump -d %s", path.c_str());
        strcat(temp_text, test);
        system((char*)temp_text);
    }
    if (path.substr(path.find_last_of(".") + 1) == "c") {  // do the same but for .c file
        char temp[512];
        char temp_text[512];
        sprintf(temp, "riscv32-unknown-elf-gcc -nostdlib %s", path.c_str());
        system((char*)temp);
        path = "a.out";

        sprintf(temp_text, "riscv32-unknown-elf-objdump -d %s", path.c_str());
        strcat(temp_text, test);
        system((char*)temp_text);
    }
    if (!reader.load(path)) {  // verify if the path is correctly load
        std::cout << "Can't find or process ELF file " << argv[1] << std::endl;
        return 2;
    }
    cout << "Loading ELF file..." << endl;

    /*
    An Elf binary file consist of segments and sections. Each sections has its own responsability, some contains
    executable code, others programs data...etc. We need to find out the sections of ELF file. The code below find out
    theses sections.
    */

    int n_sec = reader.sections.size();  // get the total amount of sectionss
    for (int i = 0; i < n_sec; i++) {
        section* sec = reader.sections[i];
        cout << "Section " << sec->get_name() << " at address 0x" << std::hex << sec->get_address() << endl;
        int  adr  = sec->get_address();
        int  size = sec->get_size();
        int* data = (int*)sec->get_data();
        if (adr) {
            cout << "Loading data";
            for (int j = 0; j < size; j += 4) {
                cout << ".";
                ram[adr + j] = data[j / 4];  // put every adress segment in the ram
            }
            cout << endl;
        }

        if (sec->get_type() == SHT_SYMTAB) {
            cout << "Reading symbols table..." << endl;
            const symbol_section_accessor symbols(reader, sec);
            for (unsigned int j = 0; j < symbols.get_symbols_num(); ++j) {
                std::string   name;
                Elf64_Addr    value;
                Elf_Xword     size;
                unsigned char bind;
                unsigned char type;
                Elf_Half      section_index;
                unsigned char other;

                symbols.get_symbol(j, name, value, size, bind, type, section_index, other);
                if (name == "_start") {
                    cout << "Found start" << endl;
                    start_adr = value - 4;  // minus 4 to acount for init inc_pc
                }
                if (name == "_bad") {
                    cout << "Found bad" << endl;
                    bad_adr = value;
                }
                if (name == "_good") {
                    cout << "Found good" << endl;
                    good_adr = value;
                }
            }
        }
    }

    core   core_inst("core_inst");
    dcache dcache_inst("dcache");

    sc_trace_file* tf;
    tf = sc_create_vcd_trace_file("tf");

    // Dcache interface
    sc_signal<sc_uint<32>> MEM_ADR;
    sc_signal<sc_uint<32>> MEM_DATA;
    sc_signal<bool>        MEM_ADR_VALID, MEM_STORE, MEM_LOAD;

    sc_signal<sc_uint<32>> MEM_RESULT;
    sc_signal<bool>        MEM_STALL;

    // Icache interface
    sc_signal<sc_uint<32>> IF_ADR;
    sc_signal<bool>        IF_ADR_VALID;

    sc_signal<sc_bv<32>> IC_INST;
    sc_signal<bool>      IC_STALL;

    sc_signal<sc_uint<32>> PC_RESET;
    sc_signal<sc_uint<32>> PC_VALUE;
    sc_clock               CLK("clk", 1, SC_NS);
    sc_signal<bool>        RESET;

    // MP interface
    sc_signal<bool>        LOAD_MP, STORE_MP;
    sc_signal<bool>        DTA_VALID_MP;
    sc_signal<sc_uint<32>> DT_MP;
    sc_signal<sc_uint<32>> A_MP;
    sc_signal<bool>        ACK_MP;

    // CORE map
    core_inst.MCACHE_MEM_ADR(MEM_ADR);
    core_inst.MCACHE_MEM_DATA(MEM_DATA);
    core_inst.MCACHE_MEM_ADR_VALID(MEM_ADR_VALID);
    core_inst.MCACHE_MEM_STORE(MEM_STORE);
    core_inst.MCACHE_MEM_LOAD(MEM_LOAD);
    core_inst.MCACHE_MEM_RESULT(MEM_RESULT);
    core_inst.MCACHE_MEM_STALL(MEM_STALL);

    core_inst.IF_ADR(IF_ADR);
    core_inst.IF_ADR_VALID(IF_ADR_VALID);
    core_inst.IC_INST(IC_INST);
    core_inst.IC_STALL(IC_STALL);
    core_inst.CLK(CLK);
    core_inst.RESET(RESET);

    core_inst.DEBUG_PC_READ(PC_VALUE);
    core_inst.DEBUG_PC_RESET(PC_RESET);
    core_inst.trace(tf);

    // Dcache map
    dcache_inst.CLK(CLK);
    dcache_inst.RESET_N(RESET);
    dcache_inst.trace(tf);
    // processor side
    dcache_inst.DATA_ADR_SM(MEM_ADR);
    dcache_inst.DATA_SM(MEM_DATA);
    dcache_inst.LOAD_SM(MEM_LOAD);
    dcache_inst.STORE_SM(MEM_STORE);
    dcache_inst.VALID_ADR_SM(MEM_ADR_VALID);
    dcache_inst.DATA_C(MEM_RESULT);
    dcache_inst.STALL_C(MEM_STALL);
    // MP side
    dcache_inst.DTA_VALID_C(DTA_VALID_MP);
    dcache_inst.READ_C(LOAD_MP);
    dcache_inst.WRITE_C(STORE_MP);
    dcache_inst.DT(DT_MP);
    dcache_inst.A(A_MP);
    dcache_inst.SLAVE_ACK_P(ACK_MP);

    cout << "Reseting...";

    RESET.write(false);  // reset
    PC_RESET.write(start_adr);
    sc_start(3, SC_NS);  // wait for 1 cycle
    RESET.write(true);   // end of reset
    cerr << "done." << endl;

    int cpt = 0;
    while (cpt < 100) {
        cpt++;
        // icache simulation
        int  if_adr       = IF_ADR.read();
        bool if_afr_valid = IF_ADR_VALID.read();
        int  if_result;

        int pc_adr = PC_VALUE.read();
        if (pc_adr == bad_adr) {
            cout << FRED("Error ! ") << "Found bad at adr 0x" << std::hex << pc_adr << endl;
            sc_start(3, SC_NS);
            exit(1);
        } else if (pc_adr == good_adr) {
            cout << FGRN("Success ! ") << "Found good at adr 0x" << std::hex << pc_adr << endl;
            sc_start(3, SC_NS);
            exit(0);
        }

        if_result = ram[if_adr];

        cout << "new IFC adr : " << std::hex << if_adr << "    data on : " << if_result << std::endl;
        IC_INST.write(if_result);
        IC_STALL.write(false);

        // dcache simulation
        bool dta_valid = DTA_VALID_MP.read();
        if (dta_valid) {
            bool read    = LOAD_MP.read();
            bool write   = STORE_MP.read();
            int  mem_adr = A_MP.read();
            if (read) {
                DT_MP.write(ram[mem_adr]);
                ACK_MP.write(true);
            } else
                ACK_MP.write(false);

            if (write) {
                int data     = MEM_DATA.read();
                ram[mem_adr] = data;
            }
        }
        sc_start(500, SC_PS);
    }
    return 0;
}
