#include "systemc.h"
#include "core.h"
#include <unordered_map>
#include "elfio/elfio.hpp"
#include <string>
using namespace std; 
using namespace ELFIO;
//arguments 
int sc_main(int argc, char* argv[]) {
    unordered_map<int, int> ram;
    elfio reader;
    string path(argv[1]);
    if (find(path.begin(), path.end(), ".s") != path.end()) {
        char temp[512];
        sprintf(temp, "riscv32-unknown-elf-as -%s", path);
        system((char *)temp);
        path = "a.out";
    }
    if (find(path.begin(), path.end(), ".c") != path.end()) {
        char temp[512];
        sprintf(temp, "riscv32-unknown-elf-gcc -%s", path);
        system((char *)temp);
        path = "a.out";
    }
    if ( !reader.load( path ) ) {        
        std::cout << "Can't find or process ELF file " << argv[1] << std::endl; 
        return 2; 
    }
    int n_seg = reader.segments.size();
    cout << "Loading ELF file..." << endl;
    for (int i = 0; i < n_seg; i++) {
        const segment* seg;
        cout << "Segment " << i << " at address " << seg->get_virtual_address() << endl;
        int adr = seg->get_virtual_address();
        int size = seg->get_memory_size();
        int* data = (int*) seg->get_data();
        for (int j = 0; j < size; j+=4) {
            ram.insert(adr + j, data[j/4]);
        }
    }

    core core_inst("core_inst");

    sc_trace_file *tf;
    tf=sc_create_vcd_trace_file("tf");


    sc_signal< sc_uint<32> >        mem_adr ;
    sc_signal< sc_uint<32> >        mem_data ;
    sc_signal< bool>                mem_adr_valid,
                                    mem_store,
                                    mem_load ; 

    sc_signal< sc_uint<32> >        mem_result ;
    sc_signal< bool>                mem_stall ;

    //Icache interface
    sc_signal< sc_uint<32> >        if_adr ; 
    sc_signal< bool >               if_adr_valid ; 

    sc_signal< sc_uint<32> >        ic_inst ;
    sc_signal< bool >               ic_stall ;
    sc_clock                        clk("clk",1,SC_NS);  
    sc_signal<bool>                 reset;

    core_inst.MCACHE_MEM_ADR(mem_adr);
    core_inst.MCACHE_MEM_DATA(mem_data);
    core_inst.MCACHE_MEM_ADR_VALID(mem_adr_valid);
    core_inst.MCACHE_MEM_STORE(mem_store);
    core_inst.MCACHE_MEM_LOAD(mem_load);
    core_inst.MCACHE_MEM_RESULT(mem_result);
    core_inst.MCACHE_MEM_STALL(mem_stall);

    core_inst.IF_ADR(if_adr);
    core_inst.IF_ADR_VALID(if_adr_valid);
    core_inst.IC_INST(ic_inst);
    core_inst.IC_STALL(ic_stall);
    core_inst.CLK(clk);
    core_inst.RESET(reset);

}
