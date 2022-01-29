#include "exec.h"


#include "shifter.h"
#include <cstdlib>
#include <string>
#define NB_TEST 10
using namespace std;

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
    cout << endl;
}

int sc_main(int argc, char* argv[])
{
    cout << "Testing component \"Exec\"..." << endl;

    exec unit_exec("exec") ;

    //sc_trace_file* file = sc_create_vcd_trace_file("trace");
    

    //declare signals 
    sc_signal< sc_uint<32> >  op1;
    sc_signal< sc_uint<32> >  op2;
    sc_signal< sc_uint<32> >  mem_data;
    sc_signal< sc_uint<4> >   dest;
    sc_signal< sc_uint<2> >   cmd ;
    sc_signal< sc_uint<2> >   mem_size ;
    sc_signal< bool >   neg_op1, wb, mem_load, mem_store, mem_sign_extend, select_shift ; //taille fifo entrée : 110
    sc_signal< bool >   exe2mem_pop, dec2exe_empty;

    sc_signal< sc_uint<32> >  ffout_exe_res ;
    sc_signal< sc_uint<32> >  ffout_mem_data;
    sc_signal< sc_uint<4> >   ffout_dest;
    sc_signal< sc_uint<2> >   ffout_mem_size ;
    sc_signal< bool >   ffout_wb, ffout_mem_load, ffout_mem_store, ffout_mem_sign_extend ; //taille fifo sortie : 74
    sc_signal< bool >   exe2mem_empty, dec2exe_pop;

    sc_signal< bool > reset;
    sc_clock clk("clk",1,SC_NS);    
    
    //Port Map :
    
    unit_exec.OP1(op1);
    unit_exec.OP2(op2);
    unit_exec.MEM_DATA(mem_data);
    unit_exec.DEST(dest);
    unit_exec.CMD(cmd);
    unit_exec.MEM_SIZE(mem_size);
    unit_exec.NEG_OP1(neg_op1);
    unit_exec.WB(wb);
    unit_exec.MEM_LOAD(mem_load);
    unit_exec.MEM_STORE(mem_store);
    unit_exec.MEM_SIGN_EXTEND(mem_sign_extend);
    unit_exec.SELECT_SHIFT(select_shift); //taille fifo entrée : 110
    unit_exec.EXE2MEM_POP(exe2mem_pop);
    unit_exec.DEC2EXE_EMPTY(dec2exe_empty);

    unit_exec.FFOUT_EXE_RES(ffout_exe_res);
    unit_exec.FFOUT_MEM_DATA(ffout_mem_data);
    unit_exec.FFOUT_DEST(ffout_dest);
    unit_exec.FFOUT_MEM_SIZE(ffout_mem_size);
    unit_exec.FFOUT_WB(ffout_wb);
    unit_exec.FFOUT_MEM_LOAD(ffout_mem_load);
    unit_exec.FFOUT_MEM_STORE(ffout_mem_store);
    unit_exec.FFOUT_MEM_SIGN_EXTEND(ffout_mem_sign_extend); //taille fifo sortie : 74
    unit_exec.EXE2MEM_EMPTY(exe2mem_empty);
    unit_exec.DEC2EXE_POP(dec2exe_pop);

    unit_exec.CLK(clk);
    unit_exec.RESET(reset);

    
    //Reset Process :

    reset.write(true);
    sc_start(1, SC_NS);
    sc_start(1, SC_PS);
    reset.write(false);
    sc_start(2, SC_NS);
    cout << "reset is done " << endl ;
    int i;
    bool should_be_full = false;
    
    //test fifo
    
    for (i = 0; i < NB_TEST; i++) {
        cout << "." << flush;
        
        int op1_ = rand();
        int mem_data_ = rand();
        int dest_ = rand() % 16;
        int mem_size_ = rand() % 4;
        bool wb_ = rand() % 2;
        bool mem_load_ = rand() % 2;
        bool mem_sign_extend_ = rand() % 2;
        bool mem_store_ = rand() % 2;
        bool exe2mem_pop_ = rand() % 2;
        bool dec2exe_empty_ = rand() % 2;
        op1 = op1_;
        op2 = 0;
        select_shift = 0;
        cmd = 0;
        mem_data = mem_data_;
        dest = dest_;
        mem_size = mem_size_; 
        wb = wb_; 
        mem_load = mem_load_; 
        mem_sign_extend = mem_sign_extend_;
        mem_store = mem_store_;
        exe2mem_pop = exe2mem_pop_;
        dec2exe_empty = dec2exe_empty_;

        sc_start(0, SC_NS);
        sc_start(0, SC_NS);
        sc_start(0, SC_NS);
        sc_start(0, SC_NS);

        cout << "------------------------------------------" << endl ;
        cout << "Interface avec decode : " << endl ;
        cout << "dec2exe_empty "<< DEC2EXE_EMPTY.read() << endl ;
        cout << "Calcul interne à EXE :" << endl ;
        


       /*  cout << "------------------------------------------"<<endl ;
        cout << "op1 :" << unit_exec.OP1.read() << endl ;

        cout << "EXE2MEM_PUSH : " ;
        print_bits((bool)unit_exec.EXE2MEM_PUSH.read());
        cout << "EXE2MEM_FULL : " ;        
        print_bits((bool)unit_exec.EXE2MEM_FULL.read());
        cout << "DEC2EXE_EMPTY : " ;
        print_bits((bool)unit_exec.DEC2EXE_EMPTY.read());
        cout << "FF_DIN : " ;
        print_bits((int) (sc_uint<32>) (sc_bv_base) unit_exec.FF_DIN.read().range(31, 0));
        cout << "FF_DOUT : " ;
        sc_start(1, SC_NS);
        print_bits((int) (sc_uint<32>) (sc_bv_base) unit_exec.FF_DOUT.read().range(31, 0));

        
        cerr << "avant if" << endl ;
        
        if (i > 0) {
            should_be_full = should_be_full && !exe2mem_pop_;
            if (!should_be_full) {
                if (
                    ffout_exe_res.read() != op1_ ||
                    ffout_mem_data.read() != mem_data_ ||
                    ffout_dest.read() != dest_ ||
                    ffout_mem_size.read() != mem_size_ ||
                    ffout_wb.read() != wb_ ||
                    ffout_mem_load.read() != mem_load_ ||
                    ffout_mem_sign_extend.read() != mem_sign_extend_ ||
                    ffout_mem_store.read() != mem_store_
                ) {
                    cout << "1ere position : ce que l'on a \n2eme position : ce que l'on devrait avoir" << endl ;
                    cout << "error : mismatch in fifo" << endl;
                    cout << "ffout_exe_res" << endl;
                    print_bits((int) ffout_exe_res.read());
                    print_bits(op1_);
                    cout << "ffout_mem_data" << endl;
                    print_bits((int) ffout_mem_data.read());
                    print_bits(mem_data_);
                    cout << "ffout_dest" << endl;
                    print_bits((int) ffout_dest.read());
                    print_bits(dest_);
                    cout << "ffout_mem_size" << endl;
                    print_bits((int) ffout_mem_size.read());
                    print_bits(mem_size_);
                    cout << "ffout_wb" << endl;
                    print_bits((int) ffout_wb.read());
                    print_bits(wb_);
                    cout << "ffout_mem_load" << endl;
                    print_bits((int) ffout_mem_load.read());
                    print_bits(mem_load_);
                    cout << "ffout_mem_sign_extend" << endl;
                    print_bits((int) ffout_mem_sign_extend.read());
                    print_bits(mem_sign_extend_);
                    cout << "ffout_mem_store" << endl;
                    print_bits((int) ffout_mem_store.read());
                    print_bits(mem_store_);
                }
            }
        }
        should_be_full = !dec2exe_empty_;
    } */
    //cout << i << " tests successfully ran on FIFO" << endl;

    
    return 0 ;
}