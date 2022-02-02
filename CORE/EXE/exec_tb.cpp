#include "exec.h"


#include "shifter.h"
#include <cstdlib>
#include <string>
#define NB_TEST 1000
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
    sc_trace_file *tf;
    tf=sc_create_vcd_trace_file("tf");
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
    sc_signal< bool >   neg_op1, wb, mem_sign_extend, select_shift ; //taille fifo entrée : 110
    sc_signal< sc_uint<3> > mem_load, mem_store;
    sc_signal< bool >   exe2mem_pop, dec2exe_empty;

    sc_signal< sc_uint<32> >  ffout_exe_res ;
    sc_signal< sc_uint<32> >  ffout_mem_data;
    sc_signal< sc_uint<4> >   ffout_dest;
    sc_signal< sc_uint<2> >   ffout_mem_size ;
    sc_signal< bool >   ffout_wb, ffout_mem_sign_extend ; //taille fifo sortie : 74
    sc_signal< sc_uint<3> >     ffout_mem_load, ffout_mem_store ; 
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

    // vcd :
    sc_trace(tf,unit_exec.OP1,"OP1");
    sc_trace(tf,unit_exec.OP2,"OP2") ;
    sc_trace(tf,unit_exec.MEM_DATA,"MEM_DATA");
    sc_trace(tf,unit_exec.DEST,"DEST");
    sc_trace(tf,unit_exec.CMD,"CMD");
    sc_trace(tf,unit_exec.MEM_SIZE,"MEM_SIZE");
    sc_trace(tf,unit_exec.NEG_OP1,"NEG_OP1");
    sc_trace(tf,unit_exec.WB,"WB");
    sc_trace(tf,unit_exec.MEM_LOAD,"MEM_LOAD");
    sc_trace(tf,unit_exec.MEM_STORE,"MEM_STORE");
    sc_trace(tf,unit_exec.MEM_SIGN_EXTEND,"MEM_SIGN_EXTEND");
    sc_trace(tf,unit_exec.SELECT_SHIFT,"SELECT_SHIFT") ;//taille fifo entrée : 110
    sc_trace(tf,unit_exec.EXE2MEM_POP,"EXE2MEM_POP");
    sc_trace(tf,unit_exec.DEC2EXE_EMPTY,"DEC2EXE_EMPTY");

    sc_trace(tf,unit_exec.FFOUT_EXE_RES,"FFOUT_EXE_RES");
    sc_trace(tf,unit_exec.FFOUT_MEM_DATA,"FFOUT_MEM_DATA");
    sc_trace(tf,unit_exec.FFOUT_DEST,"FFOUT_DEST");
    sc_trace(tf,unit_exec.FFOUT_MEM_SIZE,"FFOUT_MEM_SIZE");
    sc_trace(tf,unit_exec.FFOUT_WB,"FFOUT_WB");
    sc_trace(tf,unit_exec.FFOUT_MEM_LOAD,"FFOUT_MEM_LOAD");
    sc_trace(tf,unit_exec.FFOUT_MEM_STORE,"FFOUT_MEM_STORE");
    sc_trace(tf,unit_exec.FFOUT_MEM_SIGN_EXTEND,"FFOUT_MEM_SIGN_EXTEND") ;//taille fifo sortie : 74
    sc_trace(tf,unit_exec.EXE2MEM_EMPTY,"EXE2MEM_EMPTY");
    sc_trace(tf,unit_exec.DEC2EXE_POP,"DEC2EXE_POP");

    sc_trace(tf,unit_exec.CLK,"CLK");
    sc_trace(tf,unit_exec.RESET,"RESET");
    sc_trace(tf,unit_exec.shifter_inst.SHIFT_VAL,"shift_val") ;
    sc_trace(tf,unit_exec.FFIN_EXE_RES,"exe_res");
//    sc_trace(tf,unit_exec.shifter_inst.CMD,"cmd shifter");
    sc_trace(tf,unit_exec.fifo_inst.DIN,"exe2mem_din");
    sc_trace(tf,unit_exec.fifo_inst.DOUT,"exe2mem_fout");
    sc_trace(tf,unit_exec.fifo_inst.PUSH,"exe2mem_push");
    sc_trace(tf,unit_exec.fifo_inst.POP,"exe2mem_pop");

    //Reset Process :

    reset.write(false);
    sc_start(1, SC_NS);
    sc_start(1, SC_PS);
    reset.write(true);
    sc_start(2, SC_NS);
    cout << "reset is done " << endl ;
    int i;
    bool should_be_full = false;
    
    //test fifo
    bool test_passed = true ;
    for (i = 0; i < NB_TEST; i++) {
        int op1_ = rand();
        int op2_ = rand() ;
        int cmd_ = rand() %4 ;
        int select_shift_ = rand() % 2 ;
        int mem_data_ = rand();
        int dest_ = rand() % 16;
        int mem_size_ = rand() % 4;
        bool wb_ = rand() % 2;
        int mem_load_ = rand() % 4;
        bool mem_sign_extend_ = rand() % 2;
        int mem_store_ = rand() % 4;
        bool exe2mem_pop_ = rand() % 2;
        bool dec2exe_empty_ = rand() % 2;
        op1 = op1_;
        op2 = op2_;
        select_shift = select_shift_;
        cmd = cmd_;
        mem_data = mem_data_;
        dest = dest_;
        mem_size = mem_size_; 
        wb = wb_; 
        mem_load = mem_load_; 
        mem_sign_extend = mem_sign_extend_;
        mem_store = mem_store_;
        exe2mem_pop = exe2mem_pop_;
        dec2exe_empty = dec2exe_empty_;

        sc_start(1, SC_NS);

        if(cmd_ == 0 && select_shift_ == 0)
        {
            if(unit_exec.FFIN_EXE_RES.read() != (sc_uint<32>) ((op1_ + op2_)))
            {
                cerr << "error sur +" <<endl ;
                test_passed = false ;
            }
        }
        if(cmd_ == 1 && select_shift_ == 0)
        {
            if(unit_exec.FFIN_EXE_RES.read() != (sc_uint<32>) ((op1_ && op2_)))
            {
                cerr << "error sur &&" <<endl ;
                test_passed = false ;
            }
        }
        if(cmd_ == 2 && select_shift_ == 0)
        {
            if(unit_exec.FFIN_EXE_RES.read() != (sc_uint<32>) ((op1_ | op2_)))
            {
                cerr << "error sur |" <<endl ;
                test_passed = false ;
            }            
        }
        if(cmd_ == 3 && select_shift_ == 0)
        {
            if(unit_exec.FFIN_EXE_RES.read() != (sc_uint<32>) ((op1_ ^ op2_)))
            {
                cerr << "error sur ^" <<endl ;
                test_passed = false ;
            }
        }
        if(select_shift_ && unit_exec.CMD.read() == 0)
        {
            if(unit_exec.FFIN_EXE_RES.read() != (sc_uint<32>) ((op1_ << unit_exec.SHIFT_VAL.read())))
            {
                cerr << "error sur sll" <<endl ;
                test_passed = false ;
            }
        }
        if(select_shift_ && unit_exec.CMD.read() == 1)
        {
            if(unit_exec.FFIN_EXE_RES.read() != (sc_uint<32>) ((((unsigned int)op1_) >> unit_exec.SHIFT_VAL.read())))
            {
                cerr << "error sra" <<endl ;
                test_passed = false ;
            }
        }
        if(select_shift_ && unit_exec.CMD.read() == 2)
        {

            if(unit_exec.FFIN_EXE_RES.read() != (sc_uint<32>) ((op1_ >> unit_exec.SHIFT_VAL.read())))
            {
                cerr << "error srl" <<endl ;
                test_passed = false ;
            } 
        }    

    }
    if(test_passed) cout << "All test passed successfully !" << endl ;
    sc_close_vcd_trace_file(tf);
    return 0 ;
}