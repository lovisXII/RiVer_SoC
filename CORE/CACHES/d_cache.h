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

#define WAY_SIZE 128
struct WAY_128
{
    sc_uint<32> DATA[128];
    sc_uint<23> TAGS[128];
    bool LRU[128];          //least recently used
  //bool VALID[128];  usefull on write trough?
};

struct BUFFER
{
    sc_uint<32> DATA[32];
    sc_uint<32> DATA_ADDRESS[32];
    sc_uint READ;
    sc_uint WRITE;
    sc_uint VALIDATE;  // buffer validation
};
SC_MODULE(d_cache)
{
  sc_in<bool> CK;
  sc_in<bool> RESET;

// interfaz processeur
  sc_in<sc_uint<32>> DATA_ADDRESS_M;
  sc_in<sc_uint<32>> DATA_M;
  sc_in<bool> LOAD_M;
  sc_in<bool> STORE_M;
  sc_in<bool> VALID_ADDRESS_M;

  sc_out<sc_uint<32>> DATA_C;
  sc_out<bool> HIT_C; // 1 : HIT, 0 : MISS
  sc_out<bool> VALID_DATA_C;
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

  sc_signal<bool> way0_hit;
  sc_signal<bool> way1_hit;
  
  sc_signal<sc_uint<32>> selected_data;
  
// data structures
  WAY_128 way0;
  WAY_128 way1;

  BUFFER buff0;
  BUFFER buff1;
  enum
  {
    IDLE,
    COMPARE_TAG,
    ALLOCATE,
    WRITE_BACK,
  };
//fsm : finite state machine
  sc_register<int> fsm_current_state;

  void transition();
  
  SC_CTOR(d_cache)
  {
    SC_METHOD(transition);
    sensitive << CK.pos();
  }
}

#endif
