#include "d_cache.h"

void d_cache::reset()
{
  fsm_current_state = IDLE;
  
  for(int i = 0; i < 128; i++)
  {
  // DATA
    way0.DATA[i] = 0x0;
    way1.DATA[i] = 0x0;
  // TAGS
    way0.TAGS[i] = 0x0;
    way1.TAGS[i] = 0x0;
  // LRU
    way0.LRU[i] = false;
    way1.LRU[i] = false;
  // PRESENCE
    way0.VALID[i] = false;
    way1.VALID[i] = false;
  }
  
}
void d_cache::miss_detection()
{
  address_tag = DATA_ADDRESS_M.range(31,9);
  address_index = DATA_ADDRESS_M.range(8,2);
  address_offset = DATA_ADDRESS_M.range(1,0);

  data = DATA_M;
  store = STORE_M;
  load = LOAD_M;

  // COMPARE HIT WAY0
  if(address_tag == way0.TAGS[address_index])
  {
    way0_hit = true;
        
    if(load)
    {
      DATA_C.write(way0.DATA[address_index]);
      VALID_DATA_C = true;
    }
  }
  else
    way0_hit = false;

  // COMPARE HIT WAY1
  if(address_tag == way1.TAGS[address_index])
  {
    way1_hit = true;

    if(load)
    {
      DATA_C.write(way1.DATA[address_index]);
      VALID_DATA_C = true;
    }
  }
  else
    way1_hit = false;
  
  HIT_C.write(way0_hit | way1_hit);
}
void d_cache::transition()
{
  switch(fsm_current_state)
  {
    case IDLE:
      break;
    case READ_INST:
      break;
      case WRITE_INST:
      break;
      case WRITE_INST_UPDATE:
      break;
      case WAIT_MEM:
      break;
      case UPDATE_CACHE:
      break;
  }
}
