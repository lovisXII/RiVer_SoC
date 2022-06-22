#pragma once
#include <systemc.h>
#include "debug_util.h"

/* The idea behind this buffer is the following :
    - It has 2 Internals pointer telling which case to read_s1 and to write
    - The READ_S1 and READ_S2 interface will allow to clean validity from
    a read_s1 data. Indeed this buffer will do the interface between IFETCH
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

#define depth 4

template <int T>

SC_MODULE(buffer) {

    // Data interface

    sc_in<sc_bv<T>>     DATA_IN_S_S1; // data write for S1 SS2 way
    sc_in<sc_bv<T>>     DATA_IN_S_S2; // data write for S2 SS2 way

    sc_out<sc_bv<T>>    DATA_OUT_S_S1; // data read_s1 for S1 SS2 way
    sc_out<sc_bv<T>>    DATA_OUT_S_S2; // data read_s1 for S2 SS2 way

    // Flags Interface

    sc_in<bool>         WRITE_S;
    sc_in<bool>         READ_S_S1; 
    sc_in<bool>         READ_S_S2;
    sc_in<bool>         FLUSH_S;

    sc_out<bool>        FULL_S;
    sc_out<bool>        EMPTY_S;

    // General Interface

    sc_in_clk           CLK;
    sc_in<bool>         RESET_N;

    // Internals signals

    sc_signal<sc_bv<32>>        inside_data_s[depth] ; 
    sc_signal<sc_bv<depth>>     buffer_valid;
    sc_signal<int>              write_ptr ;
    sc_signal<int>              read_ptr_s1 ;
    sc_signal<int>              read_ptr_s2 ;
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
        sensitive   << read_ptr_s1
                    << read_ptr_s2
                    <<  RESET_N;
        for(int i = 0 ; i < depth ; i++){
            sensitive << inside_data_s[i] ;
        }
        SC_METHOD(flags_update);
        sensitive   << RESET_N  
                    << WRITE_S
                    << READ_S_S1
                    << read_ptr_s1 
                    << write_ptr
                    << buffer_valid
                    << CLK.pos();
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
        sc_bv<depth> test_full1 = 0b1111 ;
        sc_bv<depth> test_full2 = 0b1110 ;
        sc_bv<depth> test_full3 = 0b1101 ;
        sc_bv<depth> test_full4 = 0b1011 ;
        sc_bv<depth> test_full5 = 0b0111 ;

        if(((buffer_valid.read() == test_full1) || (buffer_valid.read() == test_full2)
            || (buffer_valid.read() == test_full3) || (buffer_valid.read() == test_full4)
            || (buffer_valid.read() == test_full5))
          && (!READ_S_S1.read() ||READ_S_S2.read()))
        {
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
void buffer<T>::write() {

    for(int i = 0 ; i < depth ; i++){
        inside_data_s[i] = 0;
    }
    write_ptr          = depth-1;
    read_ptr_s1        = depth-1;
    read_ptr_s2        = depth-2;
    buffer_valid    = 0 ;

    wait(3);
    while (1) {
        
        bool write              = WRITE_S.read();
        bool read_s1            = READ_S_S1.read();
        bool read_s2            = READ_S_S2.read() ;

        int write_ptr_var       = write_ptr;
        int read_ptr_var_s1     = read_ptr_s1;
        int read_ptr_var_s2     = read_ptr_s2;
        
        sc_bv<depth>  buffer_valid_var = buffer_valid ;
        
        sc_bv<depth>  mask_s1   = 0;
        sc_bv<depth>  mask_s2   = 0;

        

        cerr << "############################################## " << endl ;
        cerr << sc_time_stamp() << " write " << write << endl ;
        cerr << sc_time_stamp() << " read_s1 " << write << endl ;
        cerr << sc_time_stamp() << " read_s2 " << read_s2 << endl ;
        cerr << endl ;
        cerr << sc_time_stamp() << " write_ptr_var " << write_ptr_var << endl ;
        cerr << sc_time_stamp() << " read_ptr_var_s1 " << read_ptr_var_s1 << endl ;
        cerr << sc_time_stamp() << " read_ptr_var_s2 " << read_ptr_var_s2 << endl ;
        cerr << endl ;
        cerr << sc_time_stamp() << " empty_s " << empty_s << endl ;
        cerr << sc_time_stamp() << " full_s " << full_s << endl ;
        cerr << endl ;
        if(!read_s1 && !read_s2 && write && !full_s) // only write
        {
            cerr << sc_time_stamp() << " first case " << endl ;
            inside_data_s[write_ptr_var]    = DATA_IN_S_S1 ;
            inside_data_s[write_ptr_var-1]  = DATA_IN_S_S2 ;

            mask_s1 = 0b1 << depth - (write_ptr_var + 1);
            mask_s2 = 0b1 << depth - (write_ptr_var );
            
            write_ptr_var -=2 ;
        }
        else if(read_s1 && !read_s2 && !write && !empty_s) // read only one inst
        {
            cerr << sc_time_stamp() << " 2 case " << endl ;

            mask_s1 = 0b1 << depth - (read_ptr_var_s1 + 1);
            mask_s2 = 0b1 << depth - (read_ptr_var_s2 + 1);

            read_ptr_var_s1 --;
            read_ptr_var_s2 --;
        }
        else if (read_s1 && !read_s2 && write) // read only 1 inst and write
        {
            cerr << sc_time_stamp() << " 3 case " << endl ;
            if(!empty_s)
            {
                mask_s1 = 0b1 << depth - (read_ptr_var_s1 + 1);
                mask_s2 = 0b1 << depth - (read_ptr_var_s2 + 1);

                read_ptr_var_s1 --;
                read_ptr_var_s2 --;
            }
            if(!full_s){

                inside_data_s[write_ptr_var]    = DATA_IN_S_S1 ;
                inside_data_s[write_ptr_var-1]  = DATA_IN_S_S2 ;

                mask_s1 = 0b1 << depth - (write_ptr_var + 1);
                mask_s2 = 0b1 << depth - (write_ptr_var );

                write_ptr_var -=2 ;
            }
        }
        else if (read_s1 && read_s2 && !write && !empty_s){
            
                cerr << sc_time_stamp() << " 4 case " << endl ;
                mask_s1 = 0b1 << depth - (read_ptr_var_s1 + 1);
                mask_s2 = 0b1 << depth - (read_ptr_var_s2 + 1);

                read_ptr_var_s1 -=  2 ;
                read_ptr_var_s2 -=  2 ;
        }
        else if(read_s1 && read_s2 && write){
            cerr << sc_time_stamp() << " 5 case " << endl ;
            if(!empty_s)
            {
                mask_s1 = 0b1 << depth - (read_ptr_var_s1 + 1);
                mask_s2 = 0b1 << depth - (read_ptr_var_s2 + 1);

                read_ptr_var_s1 -=  2 ;
                read_ptr_var_s2 -=  2 ;
            }
            if(!full_s){

                inside_data_s[write_ptr_var]    = DATA_IN_S_S1 ;
                inside_data_s[write_ptr_var-1]  = DATA_IN_S_S2 ;

                mask_s1 = 0b1 << depth - (write_ptr_var + 1);
                mask_s2 = 0b1 << depth - (write_ptr_var );

                write_ptr_var -=2;
            }
        }
        if(write_ptr_var == -1){
            write_ptr_var = depth-1 ;
        }
        if(read_ptr_var_s1 == -1){
            read_ptr_var_s1 = depth-1 ;    
        } 
        if(read_ptr_var_s2 == -2 ){
            read_ptr_var_s2 = depth-2 ;    
        } 
        else if(read_ptr_var_s2 == -1){
            read_ptr_var_s2 = depth - 1;
        }

        buffer_valid_var   ^= mask_s1 ;
        buffer_valid_var   ^= mask_s2 ;

        buffer_valid = buffer_valid_var ;
        
        read_ptr_s1         = read_ptr_var_s1 ;
        read_ptr_s2         = read_ptr_var_s2 ;
        write_ptr           = write_ptr_var;
        
        cerr << "############################################## " << endl ;
        wait(1);
    }
}

template <int T>
void buffer<T>::read()
{
    // This will allow to always have the data on the output
    int read_ptr_var_s1     = read_ptr_s1;
    int read_ptr_var_s2     = read_ptr_s2;

    if(RESET_N.read())
    {
        DATA_OUT_S_S1 = inside_data_s[read_ptr_var_s1] ;
        DATA_OUT_S_S2 = inside_data_s[read_ptr_var_s2] ;
    }
    else{
        DATA_OUT_S_S1 = 0;
        DATA_OUT_S_S2 = 0;
    }
}
template <int T>
void buffer<T>::trace(sc_trace_file* tf) {
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET_N, GET_NAME(RESET_N));
    sc_trace(tf, DATA_IN_S_S1, GET_NAME(DATA_IN_S_S1));
    sc_trace(tf, DATA_IN_S_S2, GET_NAME(DATA_IN_S_S2));
    sc_trace(tf, WRITE_S, GET_NAME(WRITE_S));
    sc_trace(tf, READ_S_S1, GET_NAME(READ_S_S1));
    sc_trace(tf, READ_S_S2, GET_NAME(READ_S_S2));
    sc_trace(tf, FULL_S, GET_NAME(FULL_S));
    sc_trace(tf, empty_s, GET_NAME(empty_s));
    sc_trace(tf, full_s, GET_NAME(full_s));
    sc_trace(tf, EMPTY_S, GET_NAME(EMPTY_S));
    sc_trace(tf, DATA_OUT_S_S1, GET_NAME(DATA_OUT_S_S1));
    sc_trace(tf, DATA_OUT_S_S2, GET_NAME(DATA_OUT_S_S2));
    sc_trace(tf, write_ptr, GET_NAME(write_ptr));
    sc_trace(tf, read_ptr_s1, GET_NAME(read_ptr_s1));
    sc_trace(tf, read_ptr_s2, GET_NAME(read_ptr_s2));
    sc_trace(tf, buffer_valid, GET_NAME(buffer_valid));
    for(int i = 0; i < depth ; i++){
        std::string regname = "inside_data_s_";
        regname += std::to_string(i);
        sc_trace(tf, inside_data_s[i], signal_get_name(inside_data_s[i].name(), regname.c_str()));
    }
}
