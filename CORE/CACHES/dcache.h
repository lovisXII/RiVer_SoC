#ifndef D_CACHE
#define D_CACHE

#include <systemc.h>
#include "../UTIL/debug_util.h"

// cache N-way associatif, write through et buffet
// taille du cache 1024
// buffer de taille 2

// 2-way => 1024/2 => 512 / 4mots => 128 lignes
// index => 7 bits
// offset => 4 mots => 2 bits
// tag => 23 bits

// communication direct avec la MP (pas de bus ni BCU)

#define WAY_SIZE 128

// ACKNOWLEDGE
#define A_READY   0x00
#define A_WAIT    0x01
#define A_RETRACT 0x10

enum STATE  // MAE STATES
{ IDLE,
  WAIT_BUFF_READ,
  WAIT_MEM,
  WAIT_BUFF_WRITE };

SC_MODULE(dcache) {
    sc_in<bool> CK;
    sc_in<bool> RESET;

    // interface processeur
    sc_in<sc_uint<32>> DATA_ADDRESS_M;
    sc_in<sc_uint<32>> DATA_M;
    sc_in<bool>        LOAD_M;
    sc_in<bool>        STORE_M;
    sc_in<bool>        VALID_ADDRESS_M;

    sc_out<sc_uint<32>> DATA_C;
    sc_out<bool>        STALL;
    sc_out<bool>        MISS_C;  // 0 : HIT, 1 : MISS
    sc_out<bool>        VALID_DATA_C;
    // interface MP
    sc_out<bool>          DTA_VALID;  // data or/and adresse valid
    sc_out<bool>          READ, WRITE;
    sc_inout<sc_uint<32>> DT;
    sc_inout<sc_uint<32>> A;
    sc_in<bool>           SLAVE_ACK;  // slave answer (slave dt valid)

    // signals
    sc_signal<sc_uint<23>> address_tag;
    sc_signal<sc_uint<7>>  address_index;
    sc_signal<sc_uint<2>>  address_offset;
    sc_signal<sc_uint<32>> data;
    sc_signal<bool>        store, load;

    sc_signal<bool> way0_hit;
    sc_signal<bool> way1_hit;

    sc_signal<sc_uint<32>> selected_data;

    sc_signal<bool> full;
    sc_signal<bool> current_LRU;  // false: 0, true: 1

    // WAYS 128 lines
    bool LRU_bit_check[128];  // bit to compare least recently used

    // WAY 0
    sc_signal<sc_uint<32>> w0_word[128][4];
    sc_signal<sc_uint<23>> w0_TAG[128];
    sc_signal<bool>        w0_LINE_VALIDATE[128];

    // WAY 1
    sc_signal<sc_uint<32>> w1_word[128][4];
    sc_signal<sc_uint<23>> w1_TAG[128];
    sc_signal<bool>        w1_LINE_VALIDATE[128];

    // buffers
    // buff0
    sc_signal<sc_uint<32>> buff0_DATA;
    sc_signal<sc_uint<32>> buff0_DATA_ADDRESS;
    sc_signal<bool>        buff0_LOAD;
    sc_signal<bool>        buff0_STORE;
    sc_signal<bool>        buff0_VALIDATE;  // data valid on buffer

    // buff1
    sc_signal<sc_uint<32>> buff1_DATA;
    sc_signal<sc_uint<32>> buff1_DATA_ADDRESS;
    sc_signal<bool>        buff1_LOAD;
    sc_signal<bool>        buff1_STORE;
    sc_signal<bool>        buff1_VALIDATE;  // data valid on buffer

    // fsm : finite state machine
    sc_signal<STATE> fsm_current_state;
    sc_signal<STATE> fsm_future_state;

    void miss_detection();
    void transition();
    void transition_clk();

    void trace(sc_trace_file*);

    SC_CTOR(dcache) {
        SC_METHOD(miss_detection);
        sensitive 
			<< VALID_ADDRESS_M.pos();

        SC_METHOD(transition_clk);
        sensitive 
			<< CK.pos();

        SC_METHOD(transition);
        sensitive 
			<< CK.neg();

        reset_signal_is(RESET, true);
    }
};

#endif
