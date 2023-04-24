#include <fstream>
#include <iomanip>
#include <iostream>
#include "colors.h"
#include "cpu.h"
#include "elflib.h"
#include "systemc.h"
#include <unordered_map>

/*
--------------Main file handling simulation---------------
Arguments :
    - A riscv-elf executable to load in memory
    - (optional) --riscof [signature file], for use within the riscof test suite
*/

using namespace std;
int sc_main(int argc, char** argv) {
    bool                         riscof;
    string                       signature_name;
    string                       executable_name    = argv[1];
    uint32_t                     rvtest_entry_point = 0;
    uint32_t                     good_adr;
    uint32_t                     bad_adr;
    uint32_t                     begin_signature;
    uint32_t                     end_signature;
    uint32_t                     rvtest_code_end;
    std::unordered_map<int, int> ram;
    // Receiving arguments
    if (argc >= 4 && strcmp(argv[2], "--riscof") == 0) {
        signature_name = argv[3];
        riscof         = true;
    } else
        riscof = false;

    // Reading elf file, parsing it and getting sections and segments

    FILE_READ* structure = (FILE_READ*)malloc(sizeof(FILE_READ));
    structure            = Read_Elf32(executable_name.c_str());

    printf("Number of Instruction : %d\n", (structure->size) / 4);

    good_adr = mem_goodadr();
    bad_adr  = mem_badadr();

    int i = 0;
    int j = 0;

    int* instruction = (int*)malloc(((structure->size) / 4) * sizeof(int));
    int* adresses    = (int*)malloc(((structure->size) / 4) * sizeof(int));
    // Sections loading
    Elf32_Obj* pObj = structure->pObj_struct;

    for (int i = 0; i < pObj->Head.e_shnum; i++) {
        for (int j = 0; j < (pObj->size[i]); j += 4) {
            ram[pObj->Section_Hdr[i]->sh_addr + j] = mem_lw(pObj->Section_Hdr[i]->sh_addr + j);
        }
    }

    if (Elf32_SymAdr(pObj, &begin_signature, "begin_signature") == 0)
        fprintf(stderr, "Found begin_signature at : 0x%8x\n", begin_signature);

    if (Elf32_SymAdr(pObj, &rvtest_code_end, "rvtest_code_end") == 0)
        fprintf(stderr, "Found rvtest_code_end at : 0x%8x\n", rvtest_code_end);

    if (Elf32_SymAdr(pObj, &rvtest_entry_point, "rvtest_entry_point") == 0)
        fprintf(stderr, "Found rvtest_entry_point at : 0x%8x\n", rvtest_entry_point);

    if (Elf32_SymAdr(pObj, &end_signature, "end_signature") == 0)
        fprintf(stderr, "Found end_signature at : 0x%8x\n", end_signature);

    cpu            cpu_inst("cpu");
    sc_trace_file* tf;
    tf = sc_create_vcd_trace_file("tf");

    sc_signal<sc_uint<32>> MEM_ADR;
    sc_signal<sc_uint<32>> MEM_DATA;
    sc_signal<bool>        MEM_ADR_VALID, MEM_STORE, MEM_LOAD;
    sc_signal<sc_uint<2>>  MEM_SIZE_SM;

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

    cpu_inst.MCACHE_ADR_SM(MEM_ADR);
    cpu_inst.MCACHE_DATA_SM(MEM_DATA);
    cpu_inst.MCACHE_ADR_VALID_SM(MEM_ADR_VALID);
    cpu_inst.MCACHE_STORE_SM(MEM_STORE);
    cpu_inst.MCACHE_LOAD_SM(MEM_LOAD);
    cpu_inst.MCACHE_RESULT_SM(MEM_RESULT);
    cpu_inst.MCACHE_STALL_SM(MEM_STALL);
    cpu_inst.MEM_SIZE_SM(MEM_SIZE_SM);

    cpu_inst.ADR_SI(IF_ADR);
    cpu_inst.ADR_VALID_SI(IF_ADR_VALID);
    cpu_inst.IC_INST_SI(IC_INST);
    cpu_inst.IC_STALL_SI(IC_STALL);
    cpu_inst.READ_PC_SR(PC_VALUE);
    cpu_inst.CLK(CLK);
    cpu_inst.RESET(RESET);

    cpu_inst.trace(tf);

    cout << "Reseting...";

    RESET.write(false);  // reset
    sc_start(3, SC_NS);  // wait for 1 cycle
    RESET.write(true);   // end of reset
    cerr << "done." << endl;

    // STATS

    int NB_CYCLES = 0;
    //

    int cycles = 0;
    int countdown;

    int if_adr;
    int if_result;
    int mem_adr;
    int mem_size;

    while (1) {
        if (countdown) countdown--;
        cycles++;

        mem_adr  = MEM_ADR.read() & 0XfffffffC;  // removing the least 2 significant bits
        mem_size = MEM_SIZE_SM.read();
        bool         mem_adr_valid = MEM_ADR_VALID.read();
        unsigned int mem_data      = MEM_DATA.read();
        bool         mem_store     = MEM_STORE.read();
        bool         mem_load      = MEM_LOAD.read();
        unsigned int mem_result;
        if_adr            = IF_ADR.read();
        bool if_afr_valid = IF_ADR_VALID.read();

        unsigned int pc_adr = PC_VALUE.read();
        NB_CYCLES           = sc_time_stamp().to_double() / 1000;

        if (signature_name == "" && pc_adr == bad_adr) {
            cout << FRED("Error ! ") << "Found bad at adr 0x" << std::hex << pc_adr << endl;
            sc_start(3, SC_NS);
            exit(1);
        } else if (signature_name == "" && pc_adr == good_adr) {
            cout << FGRN("Success ! ") << "Found good at adr 0x" << std::hex << pc_adr << endl;
            sc_start(3, SC_NS);
            exit(0);
        } else if (countdown == 0 && signature_name != "" &&
                   (pc_adr == rvtest_code_end || cycles > 2000000)) {
            cerr << "inside if : " << endl;
            countdown = 20;
            cout << "coutndown value : " << countdown << endl;
        } else if (countdown == 1) {
            cout << "Test ended at " << std::hex << pc_adr << endl;
            sc_start(3, SC_NS);
            ofstream signature;
            signature.open(signature_name, ios::out | ios::trunc);
            cout << "signature_name :" << signature_name << endl;
            cout << "begin_signature :" << begin_signature << endl;
            cout << "end_signature :" << end_signature << endl;

            int j = 1;
            for (int i = begin_signature; i < end_signature; i += 4) {
                j++;
            }
            for (int i = begin_signature; i < end_signature; i += 4) {
                signature << setfill('0') << setw(8) << hex << ram[i] << endl;
            }
            exit(0);
        }
        if (mem_store && mem_adr_valid) {
            int          temporary_value       = ram[mem_adr];
            unsigned int temporary_store_value = mem_data;
            if (mem_size == 2) {  // access in byte
                // doing a mask on the least 2 significant bits
                int mask_adr = MEM_ADR.read() & 0x00000003;
                // The switch will allow to keep only the bits we want to store
                switch (mask_adr) {
                    case 0:
                        temporary_store_value = temporary_store_value & 0x000000FF;
                        temporary_value       = 0xFFFFFF00 & temporary_value;
                        ram[mem_adr]          = temporary_value | temporary_store_value;
                        break;
                    case 1:
                        temporary_store_value = temporary_store_value & 0x0000FF00;
                        temporary_value       = 0xFFFF00FF & temporary_value;
                        ram[mem_adr]          = temporary_value | temporary_store_value;
                        break;
                    case 2:
                        temporary_store_value = temporary_store_value & 0x00FF0000;
                        temporary_value       = 0xFF00FFFF & temporary_value;
                        ram[mem_adr]          = temporary_value | temporary_store_value;
                        break;
                    case 3:
                        temporary_store_value = temporary_store_value & 0xFF000000;
                        temporary_value       = 0x00FFFFFF & temporary_value;
                        ram[mem_adr]          = temporary_value | temporary_store_value;
                        break;
                    default: break;
                }
            } else if (mem_size == 1) {  // access in half word
                int mask_adr = MEM_ADR.read() & 0x00000003;
                switch (mask_adr) {
                    case 0:
                        temporary_store_value = temporary_store_value & 0x0000FFFF;
                        temporary_value       = 0xFFFF0000 & temporary_value;
                        ram[mem_adr]          = temporary_value | temporary_store_value;
                        break;
                    case 2:
                        temporary_store_value = temporary_store_value & 0xFFFF0000;
                        temporary_value       = 0x00000FFFF & temporary_value;
                        ram[mem_adr]          = temporary_value | temporary_store_value;
                        break;
                    default: break;
                }
            } else  // access in word
            {
                ram[mem_adr] = mem_data;
            }
        }
        mem_result = ram[mem_adr];
        MEM_RESULT.write(mem_result);
        MEM_STALL.write(false);
        if_result = ram[if_adr];
        IC_INST.write(if_result);
        IC_STALL.write(false);
        sc_start(500, SC_PS);
    }
    return 0;
}