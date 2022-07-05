#pragma once
#include <systemc.h>
#include "debug_util.h"

/* The idea behind this buffer is the following :
    - It has 2 Internals pointer telling which case to read and to write
    - The READ_S1 and READ_S2 interface will allow to clean validity from
    a read data. Indeed this buffer will do the interface between IFETCH
    and DECOD. But DECOD needs to check data dependencies before realy
    extracting a data. 
    This is why we use 2 signals to do so.
 5 cases can occur :
 - DECOD can READ_S1 only
 - DECOD can READ_S1 and READ_S2 only
 - IFETCH can WRITE only
 - IFETCH can write and DECOD can READ_S1 only
 - IFETCH can write and DECOD can READS_1 and READ_S2
*/


template <int size_data, int depth>

SC_MODULE(buffer) {

    // Data interface

    sc_in<sc_bv<size_data>>     DIN_S; // data write for S1 SS2 way

    sc_out<sc_bv<size_data>>    DOUT_R; // data read for S1 SS2 way

    // Flags Interface

    sc_in<bool>         PUSH_S;
    sc_in<bool>         POP_S; 
    sc_in<bool>         FLUSH_S;
    sc_out<bool>        FULL_S;
    sc_out<bool>        EMPTY_S;

    // General Interface

    sc_in_clk           CLK;
    sc_in<bool>         RESET_N;

    // Internals signals

    sc_signal<sc_bv<size_data>>        inside_data_s[depth] ; 
    sc_signal<sc_bv<depth>>     buffer_valid;
    sc_signal<int>              write_ptr ;
    sc_signal<int>              read_ptr ;
    sc_signal<bool>             full_s;
    sc_signal<bool>             empty_s;
    
    void write();
    void read() ;
    void flags_update();
    void trace(sc_trace_file * tf);
    
    SC_CTOR(buffer) 
    {
        SC_CTHREAD(write, buffer::CLK.pos());
        SC_METHOD(read)
        sensitive   << read_ptr
                    <<  RESET_N;
        for(int i = 0 ; i < depth ; i++){
            sensitive << inside_data_s[i] ;
        }
        SC_METHOD(flags_update);
        sensitive   << RESET_N 
                    << POP_S
                    << buffer_valid 
                    << empty_s
                    << full_s;
        reset_signal_is(RESET_N, false);
    }
};

template <int size_data, int depth>
void buffer<size_data, depth>::flags_update() {
    if(RESET_N.read() == 0){
        empty_s = 1 ;
        full_s = 0;
    }
    else{
        unsigned int test_full = 0xFFFFFFFF ;
        test_full >>= (32-depth);
        if((buffer_valid.read() == test_full) && !POP_S.read() )
        {
            full_s = 1;
        }
        else{
            full_s = 0;
        }

        sc_bv<depth> test_empty = 0 ;
        if(buffer_valid.read() == test_empty){
            empty_s = 1;
        }
        else{
            empty_s = 0;
        }
    }
    FULL_S = full_s ;
    EMPTY_S = empty_s ;
}

template <int size_data, int depth>
void buffer<size_data, depth>::write() {

    for(int i = 0 ; i < depth ; i++){
        inside_data_s[i] = 0;
    }
    write_ptr       = depth-1;
    read_ptr        = depth-1;
    buffer_valid    = 0 ;

    wait(3);
    while (1) {
        
        bool write           = PUSH_S.read();
        bool read            = POP_S.read();

        int write_ptr_var       = write_ptr;
        int read_ptr_var_s1     = read_ptr;
        
        sc_bv<depth>  buffer_valid_var = buffer_valid ;
        
        sc_bv<depth>  mask_s1   = 0;

        
        if(!FLUSH_S.read())
        {
            if(!read && write && !full_s)
            {
                inside_data_s[write_ptr_var]    = DIN_S ;

                buffer_valid_var[write_ptr_var] = 1 ;
                write_ptr_var -- ;
            }
            else if(read && !write && !empty_s) // read only one inst
            {
                buffer_valid_var[read_ptr_var_s1] = 0 ;
                read_ptr_var_s1 --;
            }
            else if (read && write) // read only 1 inst and write
            {
                if(!empty_s)
                {
                    buffer_valid_var[read_ptr_var_s1] = 0 ;
                    read_ptr_var_s1 --;
                }
                if(!full_s){

                    inside_data_s[write_ptr_var]    = DIN_S ;
                    buffer_valid_var[write_ptr_var] = 1 ;
                    write_ptr_var -- ;
                }
                cerr << sc_time_stamp() << " mask " << std::hex << mask_s1 << endl ;
            }

            if(write_ptr_var == -1){
                write_ptr_var = depth-1 ;
            }
            if(read_ptr_var_s1 == -1){
                read_ptr_var_s1 = depth-1 ;    
            } 
            buffer_valid = buffer_valid_var ;
            
            read_ptr         = read_ptr_var_s1 ;
            write_ptr           = write_ptr_var;
        }
        else{
            read_ptr = 0 ;
            write_ptr = 0 ;
            buffer_valid = 0;
        }

        
        wait(1);
    }
}

template <int size_data, int depth>
void buffer<size_data, depth>::read()
{
    // This will allow to always have the data on the output
    int read_ptr_var_s1     = read_ptr;

    DOUT_R = inside_data_s[read_ptr_var_s1] ;
  
}
template <int size_data, int depth>
void buffer<size_data, depth>::trace(sc_trace_file* tf) {
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET_N, GET_NAME(RESET_N));
    sc_trace(tf, DIN_S, GET_NAME(DIN_S));
    sc_trace(tf, PUSH_S, GET_NAME(PUSH_S));
    sc_trace(tf, POP_S, GET_NAME(POP_S));
    sc_trace(tf, FULL_S, GET_NAME(FULL_S));
    sc_trace(tf, empty_s, GET_NAME(empty_s));
    sc_trace(tf, full_s, GET_NAME(full_s));
    sc_trace(tf, EMPTY_S, GET_NAME(EMPTY_S));
    sc_trace(tf, DOUT_R, GET_NAME(DOUT_R));
    sc_trace(tf, write_ptr, GET_NAME(write_ptr));
    sc_trace(tf, read_ptr, GET_NAME(read_ptr));
    sc_trace(tf, buffer_valid, GET_NAME(buffer_valid));
    for(int i = 0; i < depth ; i++){
        std::string regname = "inside_data_s_";
        regname += std::to_string(i);
        sc_trace(tf, inside_data_s[i], signal_get_name(inside_data_s[i].name(), regname.c_str()));
    }
}
