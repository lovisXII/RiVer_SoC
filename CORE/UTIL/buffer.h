#pragma once
#include <systemc.h>
#include "debug_util.h"

#define depth 4

template <int T>

SC_MODULE(buffer) {
    sc_in<sc_bv<T>>     DATA_IN_S;
    sc_in_clk           CLK;
    sc_in<bool>         WRITE_S;
    sc_in<bool>         READ_S;
    sc_in<bool>         RESET_N;

    sc_out<bool>        FULL_S;
    sc_out<bool>        EMPTY_S;
    sc_out<sc_bv<T>>    DATA_OUT_S;
    sc_in<bool>         FLUSH_S;

    sc_signal<sc_bv<32>>        inside_data_s[depth] ; 
    sc_signal<sc_bv<depth>>     buffer_valid;
    sc_signal<int>              write_ptr ;
    sc_signal<int>              read_ptr ;
    sc_signal<bool>             full_s;
    sc_signal<bool>             empty_s;
    
    void function();
    void flags_update();
    void trace(sc_trace_file * tf);
    
    SC_CTOR(buffer) 
    {
        SC_CTHREAD(function, buffer::CLK.pos());
        SC_METHOD(flags_update);
        sensitive   << RESET_N  
                    << WRITE_S
                    << READ_S
                    << read_ptr 
                    << write_ptr
                    << buffer_valid;
        reset_signal_is(RESET_N, false);
    }
};

template <int T>
void buffer<T>::flags_update() {
    if(RESET_N.read() == 0){
        empty_s = 1 ;
        full_s = 0;
    }
    else{
        sc_bv<depth> test_full = 0b1111 ;
        if((buffer_valid.read() == test_full)  && !READ_S.read()){
            full_s = 1;
        }
        else{
            full_s = 0;
        }
        sc_bv<depth> test_empty = 0b0000 ;
        if((buffer_valid.read() == test_empty)){
            empty_s = 1;
        }
        else{
            empty_s = 0;
        }
    }
    FULL_S = full_s ;
    EMPTY_S = empty_s ;
}

template <int T>
void buffer<T>::function() {

    for(int i = 0 ; i < depth ; i++){
        inside_data_s[i] = 0;
    }
    DATA_OUT_S      = 0;
    write_ptr       = depth-1;
    read_ptr        = depth-1;
    buffer_valid    = 0 ;

    wait(3);
    while (1) {
        bool write          = WRITE_S.read();
        bool read           = READ_S.read();
        int write_ptr_var   = write_ptr;
        int read_ptr_var    = read_ptr;
        sc_bv<depth>  buffer_valid_var = buffer_valid ;
        sc_bv<depth>  mask = 0;

        if(!read && write && !full_s){
            inside_data_s[write_ptr_var] = DATA_IN_S ;
            mask = 0b1 << depth - (write_ptr_var + 1);
            write_ptr_var --;
        }
        else if(!write && read && !empty_s){
            DATA_OUT_S = inside_data_s[read_ptr_var] ;
            mask = 0b1 << depth - (read_ptr_var + 1);
            read_ptr_var --;
        }
        else if(read && write){
            if(!empty_s)
            {
                DATA_OUT_S = inside_data_s[read_ptr_var] ;
                mask = 0b1 << depth - (read_ptr_var + 1);
                read_ptr_var --;
            }
            if(!full_s){
                inside_data_s[write_ptr_var] = DATA_IN_S ;
                mask = 0b1 << depth - (write_ptr_var + 1);
                write_ptr_var --;
            }
        }
        if(write_ptr_var == -1){
            write_ptr_var = depth-1 ;
        }
        if(read_ptr_var == -1){
            read_ptr_var = depth-1 ;    
        } 
        buffer_valid_var   ^= mask ;
        buffer_valid = buffer_valid_var ;
        read_ptr        = read_ptr_var ;
        write_ptr       = write_ptr_var;
        wait(1);
    }
}
template <int T>
void buffer<T>::trace(sc_trace_file* tf) {
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET_N, GET_NAME(RESET_N));
    sc_trace(tf, DATA_IN_S, GET_NAME(DATA_IN_S));
    sc_trace(tf, WRITE_S, GET_NAME(WRITE_S));
    sc_trace(tf, READ_S, GET_NAME(READ_S));
    sc_trace(tf, FULL_S, GET_NAME(FULL_S));
    sc_trace(tf, empty_s, GET_NAME(empty_s));
    sc_trace(tf, full_s, GET_NAME(full_s));
    sc_trace(tf, EMPTY_S, GET_NAME(EMPTY_S));
    sc_trace(tf, DATA_OUT_S, GET_NAME(DATA_OUT_S));
    sc_trace(tf, write_ptr, GET_NAME(write_ptr));
    sc_trace(tf, read_ptr, GET_NAME(read_ptr));
    sc_trace(tf, buffer_valid, GET_NAME(buffer_valid));
    for(int i = 0; i < depth ; i++){
        std::string regname = "inside_data_s_";
        regname += std::to_string(i);
        sc_trace(tf, inside_data_s[i], signal_get_name(inside_data_s[i].name(), regname.c_str()));
    }
}
