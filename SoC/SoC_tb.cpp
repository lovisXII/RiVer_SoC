#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include "UTIL/colors.h"
#include "Wishbone/wb_bus.h"
#include "elfio/elfio.hpp"
#include "systemc.h"

using namespace ELFIO;

// arguments
int sc_main(int argc, char* argv[]) 
{  
    std::unordered_map<int, int> ram;
    elfio                        reader;  // creation of an elfio object
    if(argc < 2)
    {
        std::cout << "You forget the text file ../TEST/"<<std::endl;
        exit(1);
    }
    std::string                  path(argv[1]);
        
    int                          reset_adr;
    int                          start_adr;
    int                          good_adr;
    int                          exception_occur ;
    int                          bad_adr;
    int                          rvtest_code_end;
    int                          rvtest_entry_point;
    int                          begin_signature;
    int                          end_signature;

    char   test[512] = "> a.out.txt";
    std::string opt;
    std::string signature_name;
    bool   riscof;
    bool   stats;

    if (argc >= 3 && std::string(argv[2]) == "-O") {
        opt = "-O2";
    } else if (argc >= 3 && std::string(argv[2]) == "--riscof") {
        signature_name = std::string(argv[3]);
        riscof         = true;
    } else if(argc >= 3 && std::string(argv[2]) == "--stats")
    {
        stats          = true;
    }

    char temp_text[512];
    if (path.substr(path.find_last_of(".") + 1) == "s") {  // checking if the argument is a assembly file
        char temp[512];

        sprintf(temp,
                "riscv32-unknown-elf-gcc -nostdlib -march=rv32im -T ../SW/app.ld %s %s",
                opt.c_str(),
                path.c_str());  // writting "riscv32-unknown-elf-gcc -nostdlib
                                // path" in temp
        system((char*)temp);    // send the command in temp to the terminal
        path = "a.out";         // give the output
    }
    if (path.substr(path.find_last_of(".") + 1) == "c") {  // do the same but for .c file
        char temp[512];
        sprintf(temp, "riscv32-unknown-elf-gcc -nostdlib -march=rv32im -T ../SW/app.ld %s %s", opt.c_str(), path.c_str());
        system((char*)temp);
        path = "a.out";
    }
    if (!reader.load(path)) {  // verify if the path is correctly loadkernelle "
        std::cout << "Can't find or process ELF file " << argv[1] << std::endl;
        return -3;
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

    sc_signal<sc_uint<32>> PC_RESET;
    sc_signal<sc_uint<32>> PC_VALUE;
    sc_clock               CLK("clk", 1, SC_NS);
    sc_signal<bool>        RESET;

    sc_trace_file* tf;
    tf = sc_create_vcd_trace_file("tf");

    wb_bus bus_inst("wb_bus");

    bus_inst.CLK(CLK);
    bus_inst.RESET_N(RESET);
    bus_inst.PC_RESET(PC_RESET);
    bus_inst.PC_VALUE(PC_VALUE);

    bus_inst.trace(tf);
    bus_inst.init_mem(&ram);

    cout << "Reseting...";

    RESET.write(false);  // reset
    PC_RESET.write(reset_adr);
    sc_start(3, SC_NS);  // wait for 1 cycle
    RESET.write(true);   // end of reset
    cerr << "done." << endl;

    int countdown;
    int cycles = 0;

    while(1)
    {
        if (countdown) countdown--;
        cycles++;

        if(cycles > 1000)
            break;

        unsigned int pc_adr = PC_VALUE.read();
        
        if (signature_name == "" && pc_adr == bad_adr) {
            cout << FRED("Error ! ") << "Found bad at adr 0x" << std::hex << pc_adr << endl;
            sc_start(3, SC_NS);
            exit(1);
        } else if (signature_name == "" && pc_adr == good_adr) {            
            cout << FGRN("Success ! ") << "Found good at adr 0x" << std::hex << pc_adr << endl;
            sc_start(3, SC_NS);
            exit(0);
        } 
        else if(signature_name == "" && pc_adr == exception_occur){
            cout << FYEL("Error ! ") << "Found exception_occur at adr 0x" << std::hex << pc_adr << endl;
            sc_start(3, SC_NS);
            exit(2);
        }
        else if (countdown == 0 && (pc_adr == rvtest_code_end || (signature_name != "" && cycles > 2000000))) {
            cerr << "inside if : " << endl ; 
            countdown = 20;
            cout << "coutndown value : " << countdown << endl ;
        }
        if (countdown == 1) {
            cout << "Test ended at " << std::hex << pc_adr << endl;
            sc_start(3, SC_NS);
            ofstream signature;
            signature.open(signature_name, ios::out | ios::trunc);
            cout << "signature_name :" << signature_name << endl ;
            cout << "begin_signature :" << begin_signature << endl ;
            cout << "end_signature :" << end_signature << endl ;
            
            int j = 1;
            for (int i = begin_signature; i < end_signature; i += 4){
                j++;
            }
            for (int i = begin_signature; i < end_signature; i += 4) {
                signature << std::setfill('0') << std::setw(8) << hex << ram[i] << endl;
            }
            exit(0);
        }

        sc_start(500, SC_PS);
    }
    return 0;   
}