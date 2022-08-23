#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include "UTIL/colors.h"
#include "core.h"
#include "elfio/elfio.hpp"
#include "systemc.h"

using namespace std;
using namespace ELFIO;

//#define ICACHE_ON
//#define DCACHE_ON
//#define DEBUG_MAX_ITERATIONS

#ifdef ICACHE_ON

#include "CACHES/icache.h"

// ICACHE I/O INTERFACE MAE STATES
enum IC_FSM {
    IC_IDLE,
    IC_SEND_INSTRUCTION_1,
    IC_CLK_SIMU0,
    IC_SEND_INSTRUCTION_2,
    IC_CLK_SIMU1,
    IC_SEND_INSTRUCTION_3,
    IC_END_BURST
};

#endif

#ifdef DCACHE_ON

#include "CACHES/dcache.h"

// DCACHE ANSWER MAE STATES
enum DC_FSM {
    DC_IDLE,
    DC_CLK_SIMU0,
    DC_SEND_DTA_1,
    DC_CLK_SIMU1,
    DC_SEND_DTA_2,
    DC_CLK_SIMU2,
    DC_SEND_DTA_3,
    DC_END_BURST
};

#endif

// arguments
int sc_main(int argc, char* argv[]) {
    unordered_map<int, int> ram;
    elfio                   reader;  // creation of an elfio object
    string                  path(argv[1]);
    int                     reset_adr;
    int                     start_adr;
    int                     good_adr;
    int                     exception_occur;
    int                     bad_adr;
    int                     rvtest_code_end;
    int                     rvtest_entry_point;
    int                     begin_signature;
    int                     end_signature;

    char   test[512] = "> a.out.txt.s";
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
                "riscv32-unknown-elf-gcc -nostdlib -march=rv32im -T ../SW/app.ld %s %s",
                opt.c_str(),
                path.c_str());  // writting "riscv32-unknown-elf-gcc -nostdlib
                                // path" in temp
        system((char*)temp);    // send the command in temp to the terminal
        path = "a.out";         // give the output
    }
    if (path.substr(path.find_last_of(".") + 1) == "c") {  // do the same but for .c file
        char temp[512];
        sprintf(temp, "riscv32-unknown-elf-gcc -nostdlib -march=rv32im -T ../SW/app.ld  %s %s"
        , opt.c_str()
        , path.c_str());
        
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
                    reset_adr = value - 4;  // minus 4 to acount for init inc_pc
                    cout << "Found reset at adr "<< std::hex << reset_adr << endl;
                }
                if (name == "_start") {
                    cout << "Found start at adr " << std::hex << start_adr << endl;
                    start_adr = value - 4;  // minus 4 to acount for init inc_pc
                }
                if (name == "_bad") {
                    bad_adr = value;
                    cout << "Found bad at adr " << std::hex << bad_adr << endl;
                }
                if (name == "_good") {
                    good_adr = value;
                    cout << "Found good at adr " << std::hex << good_adr << endl;
                }
                if (name == "_exception_occur") {
                    exception_occur = value;
                    cout << "Found exception_occur at adr " << std::hex << exception_occur << endl;
                }
                if (name == "rvtest_code_end") {
                    rvtest_code_end = value;
                    cout << "Found rvtest_code_end at adr " << std::hex << rvtest_code_end << endl;
                }
                if (name == "rvtest_entry_point") {
                    reset_adr = value - 4;
                    cout << "Found rvtest_entry_point at adr " << std::hex << reset_adr << endl;
                }
                if (name == "begin_signature") {
                    begin_signature = value;
                    cout << "Found begin_signature at adr " << std::hex << begin_signature << endl;
                }
                if (name == "end_signature") {
                    end_signature = value;
                    cout << "Found end_signature at adr " << std::hex << end_signature << endl;
                }
            }
        }
    }

    // Components instanciation

    core core_inst("core_inst");

#ifdef ICACHE_ON
    icache icache_inst("icache");
#endif

#ifdef DCACHE_ON
    dcache dcache_inst("dcache");
#endif

    sc_trace_file* tf;
    tf = sc_create_vcd_trace_file("tf");

    sc_signal<sc_uint<32>> MEM_ADR_S1;
    sc_signal<sc_uint<32>> MEM_DATA_S1;
    sc_signal<sc_uint<32>> MEM_ADR_S2;
    sc_signal<sc_uint<32>> MEM_DATA_S2;
    sc_signal<bool>        MEM_ADR_VALID_S1, MEM_STORE_S1, MEM_LOAD_S1;
    sc_signal<bool>        MEM_ADR_VALID_S2, MEM_STORE_S2, MEM_LOAD_S2;
    sc_signal<sc_uint<2>>  MEM_SIZE_SM_S1;
    sc_signal<sc_uint<2>>  MEM_SIZE_SM_S2;

    sc_signal<sc_uint<32>> MEM_RESULT_S1;
    sc_signal<sc_uint<32>> MEM_RESULT_S2;
    sc_signal<bool>        MEM_STALL;

#ifdef DCACHE_ON
    // MP interface DCACHE
    sc_signal<bool>        DCACHE_LOAD, DCACHE_STORE;
    sc_signal<bool>        DCACHE_DTA_VALID;
    sc_signal<sc_uint<32>> DCACHE_DT;
    sc_signal<sc_uint<32>> DCACHE_A;
    sc_signal<sc_uint<2>>  MEM_SIZE_SC;

    sc_signal<sc_uint<32>> MP_DT;
    sc_signal<sc_uint<32>> MP_A;
    sc_signal<bool>        MP_ACK_DCACHE;
#endif

    // Icache interface
    sc_signal<sc_uint<32>> IF_ADR_S1;
    sc_signal<bool>        IF_ADR_VALID_S1;

    sc_signal<sc_uint<32>> IF_ADR_S2;
    sc_signal<bool>        IF_ADR_VALID_S2;

    sc_signal<sc_bv<32>> IC_INST_S1;
    sc_signal<sc_bv<32>> IC_INST_S2;
    sc_signal<bool>      IC_STALL;

#ifdef ICACHE_ON
    // MP interface ICACHE
    sc_signal<bool>        MP_ACK_ICACHE;
    sc_signal<sc_uint<32>> ICACHE_DT;
    sc_signal<sc_uint<32>> ICACHE_A;
    sc_signal<bool>        ICACHE_DTA_VALID;
#endif

    sc_signal<sc_uint<32>> PC_RESET;
    sc_signal<sc_uint<32>> PC_VALUE;
    sc_clock               CLK("clk", 1, SC_NS);
    sc_signal<bool>        RESET;

    core_inst.MCACHE_ADR_SM_S1(MEM_ADR_S1);
    core_inst.MCACHE_DATA_SM_S1(MEM_DATA_S1);
    core_inst.MCACHE_ADR_VALID_SM_S1(MEM_ADR_VALID_S1);
    core_inst.MCACHE_STORE_SM_S1(MEM_STORE_S1);
    core_inst.MCACHE_LOAD_SM_S1(MEM_LOAD_S1);
    core_inst.MCACHE_RESULT_SM_S1(MEM_RESULT_S1);
    core_inst.MCACHE_STALL_SM_S1(MEM_STALL);
    core_inst.MEM_SIZE_SM_S1(MEM_SIZE_SM_S1);

    core_inst.MCACHE_ADR_SM_S2(MEM_ADR_S2);
    core_inst.MCACHE_DATA_SM_S2(MEM_DATA_S2);
    core_inst.MCACHE_ADR_VALID_SM_S2(MEM_ADR_VALID_S2);
    core_inst.MCACHE_STORE_SM_S2(MEM_STORE_S2);
    core_inst.MCACHE_LOAD_SM_S2(MEM_LOAD_S2);
    core_inst.MCACHE_RESULT_SM_S2(MEM_RESULT_S2);
    core_inst.MCACHE_STALL_SM_S2(MEM_STALL);
    core_inst.MEM_SIZE_SM_S2(MEM_SIZE_SM_S2);

    core_inst.ADR_SI_S1(IF_ADR_S1);
    core_inst.ADR_SI_S2(IF_ADR_S2);
    core_inst.ADR_VALID_SI_S1(IF_ADR_VALID_S1);
    core_inst.ADR_VALID_SI_S2(IF_ADR_VALID_S2);
    core_inst.IC_INST_SI_S1(IC_INST_S1);
    core_inst.IC_INST_SI_S2(IC_INST_S2);
    core_inst.IC_STALL_SI(IC_STALL);
    core_inst.CLK(CLK);
    core_inst.RESET(RESET);

    core_inst.DEBUG_PC_READ(PC_VALUE);
    core_inst.PC_INIT(PC_RESET);
    core_inst.trace(tf);

#ifdef DCACHE_ON

    // Dcache map
    dcache_inst.CLK(CLK);
    dcache_inst.RESET_N(RESET);
    dcache_inst.trace(tf);
    // processor side
    dcache_inst.DATA_ADR_SM(MEM_ADR_S1);
    dcache_inst.DATA_SM(MEM_DATA_S1);
    dcache_inst.LOAD_SM(MEM_LOAD_S1);
    dcache_inst.STORE_SM(MEM_STORE_S1);
    dcache_inst.MEM_SIZE_SM_S1(MEM_SIZE_SM_S1);
    dcache_inst.MEM_SIZE_SC(MEM_SIZE_SC);
    dcache_inst.VALID_ADR_SM(MEM_ADR_VALID_S1);
    dcache_inst.DATA_SC(MEM_RESULT);
    dcache_inst.STALL_SC(MEM_STALL);
    // MP side
    dcache_inst.DTA_VALID_SC(DCACHE_DTA_VALID);
    dcache_inst.READ_SC(DCACHE_LOAD);
    dcache_inst.WRITE_SC(DCACHE_STORE);
    dcache_inst.DT_SC(DCACHE_DT);
    dcache_inst.A_SC(DCACHE_A);
    dcache_inst.DT_SP(MP_DT);
    dcache_inst.A_SP(MP_A);
    dcache_inst.SLAVE_ACK_SP(MP_ACK_DCACHE);

    DC_FSM DC_fsm_current_state = DC_IDLE;

    bool dcache_dta_valid;
    bool read, write;
#endif

#ifdef ICACHE_ON

    // ICache map
    icache_inst.CLK(CLK);
    icache_inst.RESET_N(RESET);
    icache_inst.trace(tf);
    // processor side
    icache_inst.ADR_SI_S1(IF_ADR_S1);
    icache_inst.ADR_VALID_SI_S1(IF_ADR_VALID_S1);
    icache_inst.IC_INST_SI_S1(IC_INST_S1);
    icache_inst.IC_STALL_SI(IC_STALL);
    // MP side
    icache_inst.DT(ICACHE_DT);
    icache_inst.A(ICACHE_A);
    icache_inst.DTA_VALID(ICACHE_DTA_VALID);
    icache_inst.SLAVE_ACK_SP(MP_ACK_ICACHE);

    // init MAE state
    IC_FSM IC_fsm_current_state = IC_IDLE;

    bool icache_dta_valid;
#endif

    cout << "Reseting...";

    RESET.write(false);  // reset
    
    //PC_RESET.write(reset_adr);
    PC_RESET.write(reset_adr);
    
    sc_start(3, SC_NS);  // wait for 1 cycle
    RESET.write(true);   // end of reset
    cerr << "done." << endl;

    int cycles = 0;
    int countdown;

    int if_adr_s1;
    int if_adr_s2;
    int if_result_s1;
    int if_result_s2;
    int mem_adr_s1;
    int mem_adr_s2;
    int mem_size_s1;
    int mem_size_s2;

#ifdef DEBUG_MAX_ITERATIONS
    int cpt = 0;
    while (cpt < 200) {
        cpt++;
#else
    while (1) {
#endif
        if (countdown) countdown--;
        cycles++;
#ifdef DCACHE_ON
        dcache_dta_valid = DCACHE_DTA_VALID.read();

        read  = DCACHE_LOAD.read();
        write = DCACHE_STORE.read();

        switch (DC_fsm_current_state) {
            case DC_IDLE:
                if (dcache_dta_valid) {
                    if (read) {
                        mem_adr = DCACHE_A.read() & 0xFFFFFFF0;

                        std::cout << "start reading  adr: " << mem_adr << std::endl;

                        MP_DT.write(ram[mem_adr]);
                        MP_A.write(mem_adr);
                        MP_ACK_DCACHE.write(true);
                        DC_fsm_current_state = DC_CLK_SIMU0;
                    } else
                        MP_ACK_DCACHE.write(false);

                    if (write) {
                        mem_adr      = DCACHE_A.read();
                        int data     = DCACHE_DT.read();
                        ram[mem_adr] = data;

                        std::cout << "store " << ram[mem_adr] << " at " << mem_adr << std::endl;
                    }
                } else
                    MP_ACK_DCACHE.write(false);
                break;
            case DC_CLK_SIMU0: DC_fsm_current_state = DC_SEND_DTA_1; break;
            case DC_SEND_DTA_1:
                MP_DT.write(ram[mem_adr + 4]);
                MP_A.write(mem_adr + 4);
                MP_ACK_DCACHE.write(true);
                DC_fsm_current_state = DC_CLK_SIMU1;
                break;
            case DC_CLK_SIMU1: DC_fsm_current_state = DC_SEND_DTA_2; break;
            case DC_SEND_DTA_2:
                MP_DT.write(ram[mem_adr + 8]);
                MP_A.write(mem_adr + 8);
                MP_ACK_DCACHE.write(true);
                DC_fsm_current_state = DC_CLK_SIMU2;
                break;
            case DC_CLK_SIMU2: DC_fsm_current_state = DC_SEND_DTA_3; break;
            case DC_SEND_DTA_3:
                MP_DT.write(ram[mem_adr + 12]);
                MP_A.write(mem_adr + 12);
                MP_ACK_DCACHE.write(true);
                DC_fsm_current_state = DC_END_BURST;
                break;
            case DC_END_BURST:
                DC_fsm_current_state = DC_IDLE;
                MP_ACK_DCACHE.write(false);
                break;
        }
#else
        mem_adr_s1                    = MEM_ADR_S1.read() & 0XfffffffC;  // removing the least 2 significant bits
        mem_size_s1                   = MEM_SIZE_SM_S1.read();

        mem_adr_s2                    = MEM_ADR_S2.read() & 0XfffffffC;  // removing the least 2 significant bits
        mem_size_s2                   = MEM_SIZE_SM_S2.read();

        bool         mem_adr_valid_s1 = MEM_ADR_VALID_S1.read();
        unsigned int mem_data_s1      = MEM_DATA_S1.read();
        bool         mem_store_s1     = MEM_STORE_S1.read();
        bool         mem_load_s1      = MEM_LOAD_S1.read();
        
        bool         mem_adr_valid_s2 = MEM_ADR_VALID_S2.read();
        unsigned int mem_data_s2      = MEM_DATA_S2.read();
        bool         mem_store_s2     = MEM_STORE_S2.read();
        bool         mem_load_s2      = MEM_LOAD_S2.read();
        
        unsigned int mem_result_s1;
        unsigned int mem_result_s2;
#endif

#ifdef ICACHE_ON
        switch (IC_fsm_current_state) {
            case IC_IDLE:
                icache_dta_valid = ICACHE_DTA_VALID.read();
                if (icache_dta_valid) {
                    if_adr_s1 = ICACHE_A.read();

                    ICACHE_DT.write(ram[if_adr_s1]);
                    // std::cout << "inst 0: " << ram[if_adr_s1] << std::endl;
                    ICACHE_A.write(if_adr_s1);
                    MP_ACK_ICACHE.write(true);
                    IC_fsm_current_state = IC_SEND_INSTRUCTION_1;
                } else {
                    MP_ACK_ICACHE.write(false);
                }
                break;
            case IC_SEND_INSTRUCTION_1:
                ICACHE_DT.write(ram[if_adr_s1 + 4]);
                ICACHE_A.write(if_adr_s1 + 4);
                MP_ACK_ICACHE.write(true);
                IC_fsm_current_state = IC_CLK_SIMU0;

                // std::cout << "inst 1: " << ram[if_adr_s1+4] << std::endl;
                break;
            case IC_CLK_SIMU0: IC_fsm_current_state = IC_SEND_INSTRUCTION_2; break;
            case IC_SEND_INSTRUCTION_2:
                ICACHE_DT.write(ram[if_adr_s1 + 8]);
                ICACHE_A.write(if_adr_s1 + 8);
                MP_ACK_ICACHE.write(true);
                IC_fsm_current_state = IC_CLK_SIMU1;

                // std::cout << "inst 2: " << ram[if_adr_s1+8] << std::endl;
                break;
            case IC_CLK_SIMU1: IC_fsm_current_state = IC_SEND_INSTRUCTION_3; break;
            case IC_SEND_INSTRUCTION_3:
                ICACHE_DT.write(ram[if_adr_s1 + 12]);
                ICACHE_A.write(if_adr_s1 + 12);
                MP_ACK_ICACHE.write(true);
                IC_fsm_current_state = IC_END_BURST;

                // std::cout << "inst 3: " << ram[if_adr_s1 + 12] << std::endl;
                break;
            case IC_END_BURST:
                IC_fsm_current_state = IC_IDLE;
                MP_ACK_ICACHE.write(false);
                break;
        }
#else
        if_adr_s1         = IF_ADR_S1.read();
        if_adr_s2         = IF_ADR_S2.read();
        bool if_afr_valid = IF_ADR_VALID_S1.read();
#endif

        unsigned int pc_adr = PC_VALUE.read();
        if (signature_name == "" && pc_adr == (bad_adr + 4)) {
            cout << FRED("Error ! ") << "Found bad at adr 0x" << std::hex << pc_adr << endl;
            sc_start(3, SC_NS);
            exit(1);
        } else if (signature_name == "" && pc_adr == (good_adr + 4)) {
            cout << FGRN("Success ! ") << "Found good at adr 0x" << std::hex << pc_adr << endl;
            sc_start(3, SC_NS);
            exit(0);
        } else if (signature_name == "" && pc_adr == (exception_occur + 4)) {
            cout << FYEL("Error ! ") << "Found exception_occur at adr 0x" << std::hex << pc_adr << endl;
            sc_start(3, SC_NS);
            exit(1);
        } else if (countdown == 0 && (pc_adr == (rvtest_code_end +4) || (signature_name != "" && cycles > 20000))) {
            cerr << "inside if : " << endl;
            countdown = 20;
            cout << "coutndown value : " << countdown << endl;
        }
        if (countdown == 1) {
            cout << "Test ended at " << std::hex << pc_adr << endl;
            sc_start(3, SC_NS);
            ofstream signature;
            signature.open(signature_name, ios::out | ios::trunc);
            cout << "signature_name :" << signature_name << endl;
            cout << "begin_signature :" << begin_signature << endl;
            cout << "end_signature :" << end_signature << endl;

            for (int i = begin_signature; i < end_signature; i += 4) {
                // 10002210 + 5 * 4 do shit : 10002210 +14 = 10002224
                cout << "adress is :" << i << " " << setfill('0') << setw(8) << hex << ram[i] << endl;
            }
            for (int i = begin_signature; i < end_signature; i += 4) {
                signature << setfill('0') << setw(8) << hex << ram[i] << endl;
            }
            exit(0);
        }

        // unsigned int rounded_mem_adr = mem_adr - (mem_adr % 4);
        // unsigned int offset          = 8 * (mem_adr % 4);
        // unsigned int mask;
        // if (mem_size == 2)
        //     mask = 0xFF;
        // else if (mem_size == 1)
        //     mask = 0xFFFF;
        // else
        //     mask = 0xFFFFFFFF;
        // mask <<= offset;
        // if (mem_store && mem_adr_valid) {
        //     ram[rounded_mem_adr] &= ~mask;
        //     ram[rounded_mem_adr] |= (mask & (mem_data << offset));
        // }
        // mem_result = (ram[rounded_mem_adr] & mask) >> offset;

#ifndef DCACHE_ON

        // S1 GESTION :

        if (mem_store_s1 && mem_adr_valid_s1) {
            int          temporary_value       = ram[mem_adr_s1];
            unsigned int temporary_store_value = mem_data_s1;
            if (mem_size_s1 == 2) {  // access in byte
                // doing a mask on the least 2 significant bits
                int mask_adr = MEM_ADR_S1.read() & 0x00000003;
                // The switch will allow to keep only the bits we want to store
                switch (mask_adr) {
                    case 0:
                        temporary_store_value = temporary_store_value & 0x000000FF;
                        temporary_value       = 0xFFFFFF00 & temporary_value;
                        ram[mem_adr_s1]          = temporary_value | temporary_store_value;
                        break;
                    case 1:
                        temporary_store_value = temporary_store_value & 0x0000FF00;
                        temporary_value       = 0xFFFF00FF & temporary_value;
                        ram[mem_adr_s1]          = temporary_value | temporary_store_value;
                        break;
                    case 2:
                        cout << "byte access 3d case at adr : " << mem_adr_s1 << endl;
                        cout << "value to be store " << std::hex << temporary_store_value << endl;
                        temporary_store_value = temporary_store_value & 0x00FF0000;
                        temporary_value       = 0xFF00FFFF & temporary_value;
                        ram[mem_adr_s1]          = temporary_value | temporary_store_value;
                        break;
                    case 3:
                        temporary_store_value = temporary_store_value & 0xFF000000;
                        temporary_value       = 0x00FFFFFF & temporary_value;
                        ram[mem_adr_s1]          = temporary_value | temporary_store_value;
                        cout << ram[mem_adr_s1] << endl;
                        break;
                    default: break;
                }
            } else if (mem_size_s1 == 1) {  // access in half word
                int mask_adr = MEM_ADR_S1.read() & 0x00000003;
                switch (mask_adr) {
                    case 0:
                        temporary_store_value = temporary_store_value & 0x0000FFFF;
                        temporary_value       = 0xFFFF0000 & temporary_value;
                        ram[mem_adr_s1]          = temporary_value | temporary_store_value;
                        break;
                    case 2:
                        temporary_store_value = temporary_store_value & 0xFFFF0000;
                        temporary_value       = 0x00000FFFF & temporary_value;
                        ram[mem_adr_s1]          = temporary_value | temporary_store_value;
                        break;
                    default: break;
                }
            } else  // access in word
            {
                cout << sc_time_stamp() << " adr  : " << mem_adr_s1 << " data  : " << mem_data_s1 << endl;
                ram[mem_adr_s1] = mem_data_s1;
            }
        }
        mem_result_s1 = ram[mem_adr_s1];
        MEM_RESULT_S1.write(mem_result_s1);
        MEM_STALL.write(false);
        // S2 Gestion :

        if (mem_store_s2 && mem_adr_valid_s2) {
            int          temporary_value       = ram[mem_adr_s2];
            unsigned int temporary_store_value = mem_data_s2;
            if (mem_size_s2 == 2) {  // access in byte
                // doing a mask on the least 2 significant bits
                int mask_adr = MEM_ADR_S2.read() & 0x00000003;
                // The switch will allow to keep only the bits we want to store
                switch (mask_adr) {
                    case 0:
                        temporary_store_value = temporary_store_value & 0x000000FF;
                        temporary_value       = 0xFFFFFF00 & temporary_value;
                        ram[mem_adr_s2]          = temporary_value | temporary_store_value;
                        break;
                    case 1:
                        temporary_store_value = temporary_store_value & 0x0000FF00;
                        temporary_value       = 0xFFFF00FF & temporary_value;
                        ram[mem_adr_s2]          = temporary_value | temporary_store_value;
                        break;
                    case 2:
                        cout << "byte access 3d case at adr : " << mem_adr_s2 << endl;
                        cout << "value to be store " << std::hex << temporary_store_value << endl;
                        temporary_store_value = temporary_store_value & 0x00FF0000;
                        temporary_value       = 0xFF00FFFF & temporary_value;
                        ram[mem_adr_s2]          = temporary_value | temporary_store_value;
                        break;
                    case 3:
                        temporary_store_value = temporary_store_value & 0xFF000000;
                        temporary_value       = 0x00FFFFFF & temporary_value;
                        ram[mem_adr_s2]          = temporary_value | temporary_store_value;
                        cout << ram[mem_adr_s2] << endl;
                        break;
                    default: break;
                }
            } else if (mem_size_s2 == 1) {  // access in half word
                int mask_adr = MEM_ADR_S2.read() & 0x00000003;
                switch (mask_adr) {
                    case 0:
                        temporary_store_value = temporary_store_value & 0x0000FFFF;
                        temporary_value       = 0xFFFF0000 & temporary_value;
                        ram[mem_adr_s2]          = temporary_value | temporary_store_value;
                        break;
                    case 2:
                        temporary_store_value = temporary_store_value & 0xFFFF0000;
                        temporary_value       = 0x00000FFFF & temporary_value;
                        ram[mem_adr_s2]          = temporary_value | temporary_store_value;
                        break;
                    default: break;
                }
            } else  // access in word
            {
                ram[mem_adr_s2] = mem_data_s2;
            }
        }
        mem_result_s2 = ram[mem_adr_s2];
        MEM_RESULT_S2.write(mem_result_s2);


#endif

#ifndef ICACHE_ON
        if_result_s1 = ram[if_adr_s1];
        if_result_s2 = ram[if_adr_s2];
        IC_INST_S1.write(if_result_s1);
        IC_INST_S2.write(if_result_s2);
        IC_STALL.write(false);
#endif

        sc_start(500, SC_PS);
    }
    return 0;
}