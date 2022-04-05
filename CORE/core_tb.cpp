#include <string>
#include <unordered_map>
#include "UTIL/colors.h"
#include "core.h"
#include "elfio/elfio.hpp"
#include "systemc.h"

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
    string                  opt;
    if (argc >= 2 && argv[2] == "O") {
        opt = "-O2";
    } else {
        opt = "";
    }

    if (path.substr(path.find_last_of(".") + 1) ==
        "s") {  // checking if the argument is a assembly file
        char temp[512];
        char temp_text[512];

        sprintf(temp,
                "riscv32-unknown-elf-gcc -nostdlib %s %s",
                opt.c_str(),
                path.c_str());  // writting "riscv32-unknown-elf-gcc -nostdlib
                                // path" in temp
        system((char*)temp);    // send the command in temp to the terminal
        path = "a.out";         // give the output

        sprintf(temp_text, "riscv32-unknown-elf-objdump -d %s", path.c_str());
        strcat(temp_text, test);
        system((char*)temp_text);
    }
    if (path.substr(path.find_last_of(".") + 1) ==
        "c") {  // do the same but for .c file
        char temp[512];
        char temp_text[512];
        sprintf(temp,
                "riscv32-unknown-elf-gcc -nostdlib %s %s",
                opt.c_str(),
                path.c_str());
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
    An Elf binary file consist of segments and sections. Each sections has its
    own responsability, some contains executable code, others programs
    data...etc.
    We need to find out the sections of ELF file. The code below find out theses
    sections.
    */

    int n_sec = reader.sections.size();  // get the total amount of sectionss
    for (int i = 0; i < n_sec; i++) {
        section* sec = reader.sections[i];
        cout << "Section " << sec->get_name() << " at address 0x" << std::hex
             << sec->get_address() << endl;
        int  adr  = sec->get_address();
        int  size = sec->get_size();
        int* data = (int*)sec->get_data();
        if (adr) {
            cout << "Loading data";
            for (int j = 0; j < size; j += 4) {
                cout << ".";
                ram[adr + j] =
                    data[j / 4];  // put every adress segment in the ram
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

                symbols.get_symbol(
                    j, name, value, size, bind, type, section_index, other);
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

    core core_inst("core_inst");

    sc_trace_file* tf;
    tf = sc_create_vcd_trace_file("tf");

    sc_signal<sc_uint<32> > MEM_ADR;
    sc_signal<sc_uint<32> > MEM_DATA;
    sc_signal<bool>         MEM_ADR_VALID, MEM_STORE, MEM_LOAD;

    sc_signal<sc_uint<32> > MEM_RESULT;
    sc_signal<bool>         MEM_STALL;

    // Icache interface
    sc_signal<sc_uint<32> > IF_ADR;
    sc_signal<bool>         IF_ADR_VALID;

    sc_signal<sc_bv<32> > IC_INST;
    sc_signal<bool>       IC_STALL;

    sc_signal<sc_uint<32> > PC_RESET;
    sc_signal<sc_uint<32> > PC_VALUE;
    sc_clock                CLK("clk", 1, SC_NS);
    sc_signal<bool>         RESET;

    core_inst.MCACHE_ADR_SM(MEM_ADR);
    core_inst.MCACHE_DATA_SM(MEM_DATA);
    core_inst.MCACHE_ADR_VALID_SM(MEM_ADR_VALID);
    core_inst.MCACHE_STORE_SM(MEM_STORE);
    core_inst.MCACHE_LOAD_SM(MEM_LOAD);
    core_inst.MCACHE_RESULT_SM(MEM_RESULT);
    core_inst.MCACHE_STALL_SM(MEM_STALL);

    core_inst.ADR_SI(IF_ADR);
    core_inst.ADR_VALID_SI(IF_ADR_VALID);
    core_inst.IC_INST_SI(IC_INST);
    core_inst.IC_STALL_SI(IC_STALL);
    core_inst.CLK(CLK);
    core_inst.RESET(RESET);

    core_inst.DEBUG_PC_READ(PC_VALUE);
    core_inst.PC_INIT(PC_RESET);
    core_inst.trace(tf);

    cout << "Reseting...";

    RESET.write(false);  // reset
    PC_RESET.write(start_adr);
    sc_start(3, SC_NS);  // wait for 1 cycle
    RESET.write(true);   // end of reset
    cerr << "done." << endl;

    while (1) {
        int  mem_adr       = MEM_ADR.read();
        bool mem_adr_valid = MEM_ADR_VALID.read();
        int  mem_data      = MEM_DATA.read();
        bool mem_store     = MEM_STORE.read();
        bool mem_load      = MEM_LOAD.read();
        int  mem_result;

        int  if_adr       = IF_ADR.read();
        bool if_afr_valid = IF_ADR_VALID.read();
        int  if_result;

        int pc_adr = PC_VALUE.read();
        if (pc_adr == bad_adr) {
            cout << FRED("Error ! ") << "Found bad at adr 0x" << std::hex
                 << pc_adr << endl;
            sc_start(3, SC_NS);
            exit(1);
        } else if (pc_adr == good_adr) {
            cout << FGRN("Success ! ") << "Found good at adr 0x" << std::hex
                 << pc_adr << endl;
            sc_start(3, SC_NS);
            exit(0);
        }

        if (mem_store && mem_adr_valid) { ram[mem_adr] = mem_data; }
        mem_result = ram[mem_adr];
        if_result  = ram[if_adr];
        MEM_RESULT.write(mem_result);
        MEM_STALL.write(false);
        IC_INST.write(if_result);
        IC_STALL.write(false);

        sc_start(500, SC_PS);
    }
    return 0;
}
