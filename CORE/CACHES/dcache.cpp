#include "dcache.h"

void dcache::miss_detection() {
    sc_uint<32> DT_A_M = DATA_ADDRESS_M.read();

    address_tag    = DT_A_M.range(31, 9);
    address_index  = DT_A_M.range(8, 2);
    address_offset = DT_A_M.range(1, 0);

    data  = DATA_M;
    store = STORE_M;
    load  = LOAD_M;

    VALID_DATA_C = false;
    // COMPARE HIT WAY0
    if (address_tag == w0_TAG[address_index.read()]) {
        way0_hit = true;

        if (load) {
            DATA_C.write(w0_word[address_index.read()][address_offset.read()]);
            VALID_DATA_C = true;
        }
        if (store) {
            w0_word[address_index.read()][address_offset.read()] = DATA_M;
        }
    } else
        way0_hit = false;

    // COMPARE HIT WAY1
    if (address_tag == w1_TAG[address_index.read()]) {
        way1_hit = true;

        if (load) {
            DATA_C.write(w1_word[address_index.read()][address_offset.read()]);
            VALID_DATA_C = true;
        }
        if (store) {
            w1_word[address_index.read()][address_offset.read()] = DATA_M;
        }
    } else
        way1_hit = false;

    MISS_C.write(way0_hit || way1_hit);
}
void dcache::transition_clk() {
    if (RESET.read()) {
        fsm_current_state = IDLE;

        for (int i = 0; i < 128; i++) {
            // LRU
            LRU_bit_check[i] = false;
            // PRESENCE
            w0_LINE_VALIDATE[i] = false;
            w1_LINE_VALIDATE[i] = false;
        }
        buff0_VALIDATE = false;
        buff1_VALIDATE = false;
    } else {
        fsm_current_state = fsm_future_state.read();
        full              = buff0_VALIDATE && buff1_VALIDATE;
    }
}
void dcache::transition() {
    switch (fsm_current_state) {
        case IDLE:
            if (load && !(way0_hit && way1_hit) && full) {
                fsm_future_state = WAIT_BUFF_READ;
            } else if (load && !(way0_hit && way1_hit) && !full) {
                fsm_future_state = WAIT_MEM;
            }
            if (store) {
                if (full)
                    fsm_future_state = WAIT_BUFF_WRITE;
                else {
                    if (buff0_VALIDATE) {
                        buff1_DATA         = data;
                        buff1_DATA_ADDRESS = DATA_ADDRESS_M.read();
                        buff1_LOAD         = load;
                        buff1_STORE        = store;
                        buff1_VALIDATE     = true;
                    } else {
                        buff0_DATA         = data;
                        buff0_DATA_ADDRESS = DATA_ADDRESS_M.read();
                        buff0_LOAD         = load;
                        buff0_STORE        = store;
                        buff0_VALIDATE     = true;
                    }
                }
            }

            break;
        case WAIT_BUFF_READ:
            if (!full) {
                if (buff0_VALIDATE) {
                    buff1_DATA         = data;
                    buff1_DATA_ADDRESS = DATA_ADDRESS_M.read();
                    buff1_LOAD         = load;
                    buff1_STORE        = store;
                    buff1_VALIDATE     = true;
                } else {
                    buff0_DATA         = data;
                    buff0_DATA_ADDRESS = DATA_ADDRESS_M.read();
                    buff0_LOAD         = load;
                    buff0_STORE        = store;
                    buff0_VALIDATE     = true;
                }
                fsm_future_state = WAIT_MEM;
            }
            break;
        case WAIT_MEM:
            if (SLAVE_ACK.read()) {
                if (LRU_bit_check[address_index.read()]) {
                    w0_word[address_index.read()][address_offset.read()] =
                        DT.read();
                    w0_TAG[address_index.read()]           = address_tag.read();
                    w0_LINE_VALIDATE[address_index.read()] = true;
                } else {
                    w1_word[address_index.read()][address_offset.read()] =
                        DT.read();
                    w1_TAG[address_index.read()]           = address_tag.read();
                    w1_LINE_VALIDATE[address_index.read()] = true;
                }
                LRU_bit_check[address_index.read()] =
                    !LRU_bit_check[address_index.read()];
                fsm_future_state = IDLE;
            }
            break;
        case WAIT_BUFF_WRITE:
            if (!full) {
                if (buff0_VALIDATE) {
                    buff1_DATA         = data;
                    buff1_DATA_ADDRESS = DATA_ADDRESS_M;
                    buff1_LOAD         = load;
                    buff1_STORE        = store;
                    buff1_VALIDATE     = true;
                } else {
                    buff0_DATA         = data;
                    buff0_DATA_ADDRESS = DATA_ADDRESS_M;
                    buff0_LOAD         = load;
                    buff0_STORE        = store;
                    buff0_VALIDATE     = true;
                }
                fsm_future_state = IDLE;
            }
            break;
    }
}

void dcache::trace(sc_trace_file* tf) {
    sc_trace(tf, CK, GET_NAME(CK));
    sc_trace(tf, RESET, GET_NAME(RESET));

    // interfaz processeur
    sc_trace(tf, DATA_ADDRESS_M, GET_NAME(DATA_ADDRESS_M));
    sc_trace(tf, DATA_M, GET_NAME(DATA_M));
    sc_trace(tf, LOAD_M, GET_NAME(LOAD_M));
    sc_trace(tf, STORE_M, GET_NAME(STORE_M));
    sc_trace(tf, VALID_ADDRESS_M, GET_NAME(VALID_ADDRESS_M));

    sc_trace(tf, DATA_C, GET_NAME(DATA_C));
    sc_trace(tf, STALL, GET_NAME(STALL));
    sc_trace(tf, MISS_C, GET_NAME(MISS_C));
    sc_trace(tf, VALID_DATA_C, GET_NAME(VALID_DATA_C));

    // interfaz bus
    sc_trace(tf, DTA_VALID, GET_NAME(DTA_VALID));
    sc_trace(tf, READ, GET_NAME(READ));
    sc_trace(tf, WRITE, GET_NAME(WRITE));
    sc_trace(tf, DT, GET_NAME(DT));
    sc_trace(tf, A, GET_NAME(A));
    sc_trace(tf, SLAVE_ACK, GET_NAME(SLAVE_ACK));
}