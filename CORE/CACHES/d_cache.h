#ifndef D_CACHE
#define D_CACHE

#include <systemc.h>

//cache N-way associatif, write through et buffet
// taille du cache 1024
// 2-way => 1024/2 => 512 / 4mots => 128 => 7bits
// buffer de taille 2
// 4 mots => offset 2
// tag: 23

// index  = ligne    <= size est one hot
// offset = column

struct WAY_128
{
    sc_uint<32> DATA[128];
    sc_uint<23> TAGS[128];
    bool LRU[128];
    bool PRESENCE[128];
};
SC_MODULE(d_cache)
{
  sc_in<bool> CK;
  sc_in<bool> RESET;

// interfaz processeur
  sc_in<sc_uint<32>> DATA_ADDRESS;

  sc_out<bool> HIT;

// interfaz bus
  sc_in<bool> PI_DATA_REQUEST;
  sc_in<sc_uint<3>> PI_ACKNOWLEDGE;

  sc_inout<sc_uint<32>> PI_ADDRESS;
  sc_inout<sc_uint<32>> PI_DATA;
  sc_inout<sc_uint<4>> PI_OPCODE;
  sc_inout<bool> PI_READ;
  sc_inout<bool> PI_LOCK;
  
  sc_out<bool> PI_GRANT;
  
//signals
  sc_signal<sc_uint<23>> address_tag;
  sc_signal<sc_uint<7>> address_index;
  sc_signal<sc_uint<2>> address_offset;

  sc_signal<sc_uint<32>> selected_data;
  
// data structures
  WAY_128 way0;
  WAY_128 way1;

  void init();
  
  SC_CTOR(d_cache)
  {
    SC_METHOD(init);
  }
}

#endif
