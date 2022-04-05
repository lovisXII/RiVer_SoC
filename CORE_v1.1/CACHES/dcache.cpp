#include "dcache.h"

void dcache::adresse_parcer()
{
  sc_uint<32> DT_A_SM = DATA_ADR_SM.read();
  address_tag.write(DT_A_SM.range(31,9));
  address_index.write(DT_A_SM.range(8,2));
  address_offset.write(DT_A_SM.range(1,0));
}

void dcache::miss_detection()
{
  // COMPARE HIT WAY0
  if(address_tag == w0_TAG[address_index.read()])
  {
    way0_hit = w0_LINE_VALIDATE[address_index.read()];
        
    if(LOAD_SM.read() && way0_hit)
    {
      DATA_C.write(w0_word[address_index.read()][address_offset.read()]);
    }
    if(STORE_SM.read())
    {
      w0_word[address_index.read()][address_offset.read()] = DATA_SM;      
      w0_LINE_VALIDATE[address_index.read()] = true;
    }
  }
  else
    way0_hit = false;

  // COMPARE HIT WAY1
  if(address_tag == w1_TAG[address_index.read()])
  {
    way1_hit = w0_LINE_VALIDATE[address_index.read()];

    if(LOAD_SM.read() && way1_hit)
    {
      DATA_C.write(w1_word[address_index.read()][address_offset.read()]);
    }
    if(STORE_SM.read())
    {
      w1_word[address_index.read()][address_offset.read()] = DATA_SM; 
      w1_LINE_VALIDATE[address_index.read()] = true;     
    }
  }
  else
    way1_hit = false;
}

void dcache::transition()
{
  typedef enum // MAE STATES
  {
    IDLE,
    WAIT_BUFF_READ,
    WAIT_MEM,
    WAIT_BUFF_WRITE,
    ERROR
  } states_fsm;

  states_fsm fsm_current_state = IDLE;
    
  bool dta_v;
  bool write_b;
  while(1)
  {
    dta_v = false;
    write_b = false;
    switch(fsm_current_state)
    {
      case IDLE:
        if(LOAD_SM.read())
        {
          if(!way0_hit && !way1_hit && full)
          {
            fsm_current_state = WAIT_BUFF_READ;
            STALL_C.write(true);
          }
          else if(!way0_hit && !way1_hit && !full)
          {
            fsm_current_state = WAIT_MEM;
            STALL_C.write(true);

            dta_v = true;
            write_b = true;
          }
          else if(way0_hit || way1_hit)
          {
            STALL_C.write(false);
          }
        }
        else if(STORE_SM.read())
        {
          if(full)
          {
            fsm_current_state = WAIT_BUFF_WRITE;
            STALL_C.write(true);
          }
          else
          {
            STALL_C.write(false);
            
            dta_v = true;
            write_b = true;
          }
        } 
        break;
      case WAIT_BUFF_READ:
        if(!full)
        {
          dta_v = true;
          write_b = true;

          fsm_current_state = WAIT_MEM;
        }
        break;
        case WAIT_MEM:
        if(SLAVE_ACK_P.read())
        {
          if(LRU_bit_check[address_index.read()])
          {
            w0_word[address_index.read()][address_offset.read()] = DT.read();
            w0_TAG[address_index.read()] = address_tag.read();
            w0_LINE_VALIDATE[address_index.read()] = true;
          }
          else
          {
            w1_word[address_index.read()][address_offset.read()] = DT.read();
            w1_TAG[address_index.read()] = address_tag.read();
            w1_LINE_VALIDATE[address_index.read()] = true;
          }
          LRU_bit_check[address_index.read()] = !LRU_bit_check[address_index.read()];
          fsm_current_state = IDLE;
        }
        break;
        case WAIT_BUFF_WRITE:
        if(!full)
        {
          dta_v = true;
          write_b = true;

          fsm_current_state = IDLE;
        }
        break;
        case ERROR:
          std::cout << "WTF" 
			<< std::endl;
        break;
    }
    dta_valid.write(dta_v);
    write_buff.write(write_b);

    //debug
    switch(fsm_current_state)
    {
      case IDLE:
        fsm_state.write(0);
      break;
      case WAIT_BUFF_READ:
        fsm_state.write(1);
      break;
      case WAIT_MEM:
        fsm_state.write(2);
      break;
      case WAIT_BUFF_WRITE:
        fsm_state.write(3);
      break;
      case ERROR:
        fsm_state.write(4);
      break;
    }
    wait();
  }
}

void dcache::trace(sc_trace_file* tf)
{
  sc_trace(tf, CLK, GET_NAME(CLK));
  sc_trace(tf, RESET_N, GET_NAME(RESET_N));

  // interfaz processeur
  sc_trace(tf, DATA_ADR_SM, GET_NAME(DATA_ADR_SM));
  sc_trace(tf, DATA_SM, GET_NAME(DATA_SM));
  sc_trace(tf, LOAD_SM, GET_NAME(LOAD_SM));
  sc_trace(tf, STORE_SM, GET_NAME(STORE_SM));
  sc_trace(tf, LOAD_SM, GET_NAME(LOAD_SM));
  sc_trace(tf, STORE_SM, GET_NAME(STORE_SM));
  sc_trace(tf, VALID_ADR_SM, GET_NAME(VALID_ADR_SM));

  sc_trace(tf, DATA_C, GET_NAME(DATA_C));
  sc_trace(tf, STALL_C, GET_NAME(STALL_C));

  // interfaz bus
  sc_trace(tf, DTA_VALID_C, GET_NAME(DTA_VALID_C));
  sc_trace(tf, READ_C, GET_NAME(READ_C)); 
  sc_trace(tf, WRITE_C, GET_NAME(WRITE_C));
  sc_trace(tf, DT, GET_NAME(DT));
  sc_trace(tf, A, GET_NAME(A));
  sc_trace(tf, SLAVE_ACK_P, GET_NAME(SLAVE_ACK_P));

  //Signals
  sc_trace(tf, way0_hit, GET_NAME(way0_hit));
  sc_trace(tf, way1_hit, GET_NAME(way1_hit));
  sc_trace(tf, full, GET_NAME(full));

  sc_trace(tf, fsm_state, GET_NAME(fsm_state));

}