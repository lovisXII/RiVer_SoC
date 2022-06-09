#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <cstdlib>
#include <unordered_map>
#include "../ELFIO/elfio/elfio.hpp"

using namespace std; 
using namespace ELFIO;

unordered_map<int, int> ram;


extern int ghdl_main(int argc, char const* argv[]);
//argc nombre d'arguments du terminal
//argv nom des arguments
int main(int argc, char const* argv[]) {
    elfio                   reader;  // creation of an elfio object
    string                  path(argv[1]);
    int                     reset_adr;
    int                     start_adr;
    int                     good_adr;
    int                     exception_occur ;
    int                     bad_adr;
    int                     rvtest_code_end;
    int                     rvtest_entry_point;
    int                     begin_signature;
    int                     end_signature;

    char   test[512] = "> a.out.txt";
    string opt;
    string signature_name;
    bool   riscof;
    if (argc >= 3 && std::string(argv[2]) == "-O") {
        opt = "-O2";
    } else if (argc >= 3 && std::string(argv[2]) == "--riscof") {
        signature_name = string(argv[3]);
        riscof         = true;
    };

    char temp_text[512];
    if (path.substr(path.find_last_of(".") + 1) == "s") {  // checking if the argument is a assembly file
        char temp[512];

        sprintf(temp,
                "riscv32-unknown-elf-gcc -nostdlib -march=rv32im -T seg.ld %s %s",
                opt.c_str(),
                path.c_str());  // writting "riscv32-unknown-elf-gcc -nostdlib
                                // path" in temp
        system((char*)temp);    // send the command in temp to the terminal
        path = "a.out";         // give the output
    }
    if (path.substr(path.find_last_of(".") + 1) == "c") {  // do the same but for .c file
        char temp[512];
        sprintf(temp, "riscv32-unknown-elf-gcc -nostdlib -march=rv32im -T seg.ld %s %s", opt.c_str(), path.c_str());
        system((char*)temp);
        path = "a.out";
    }
    if (!reader.load(path)) {  // verify if the path is correctly loadkernelle "
        std::cout << "Can't find or process ELF file " << argv[1] << std::endl;
        return 2;
    }
    sprintf(temp_text, "riscv32-unknown-elf-objdump -D %s", path.c_str());
    strcat(temp_text, test);
    system((char*)temp_text);
    cout << "Loading ELF file..." << endl;

    /*
    An Elf binary file consist of segments and sections. Each sections has its
    own responsability, some contains executable code, others programs
    data...etc.
    We need to find out the sections of ELF file. The code below find out theses
    sections.
    */
    // section* text_sec = reader.sections.add( ".section .kernel" );
    // text_sec->set_type( SHT_PROGBITS );
    // text_sec->set_flags( SHF_ALLOC | SHF_EXECINSTR );
    // text_sec->set_address( 0x80000000 );

    int n_sec = reader.sections.size();  // get the total amount of sections

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
                if (name == "_reset") {
                    cout << "Found reset" << endl;
                    reset_adr = value - 4;  // minus 4 to acount for init inc_pc
                }
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
                if (name == "_exception_occur") {
                    cout << "Found exception_occur" << endl;
                    exception_occur = value;
                }
                if (name == "rvtest_code_end") {
                    cout << "Found rvtest_code_end" << endl;
                    rvtest_code_end = value;
                }
                if (name == "rvtest_entry_point") {
                    cout << "Found rvtest_entry_point" << endl;
                    reset_adr = value - 4;
                }
                if (name == "begin_signature") {
                    cout << "Found begin_signature" << endl;
                    begin_signature = value;
                }
                if (name == "end_signature") {
                    cout << "Found end_signature" << endl;
                    end_signature = value;
                }
            }
        }
    }
    ghdl_main(argc - 1, argv);
}

int get_inst(int a) {
    a = a >> 2;
    return ram[a];
}

int get_mem(int a) {
    a = a >> 2; 
    return ram[a];    
}

int write_mem(int a, int data) {
    a = a >> 2;
    ram[a] = data; 
    return 0;
}