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
void d_cache::write_buffer()
{
  if(!buff0.VALIDATE)
  {
    buff0.DATA = DATA_M;
    buff0.DATA_ADDRESS = DATA_ADDRESS_M;
    buff0.LOAD = LOAD_M;
    buff0.STORE = STORE_M;
    buff0.VALIDATE = true;

    current_LRU = false;
  }
  else if(!buff1.VALIDATE)
  {
    buff1.DATA = DATA_M;
    buff1.DATA_ADDRESS = DATA_ADDRESS_M;
    buff1.LOAD = LOAD_M;
    buff1.STORE = STORE_M;
    buff1.VALIDATE = true;

    current_LRU = true;
  }
  else
    BUFFER_FULL.write(true);
}
void d_cache::transition()
{
  switch(fsm_current_state)
  {
    case IDLE:
      if(buff0.VALIDATE || buff1.VALIDATE)/*&& @ is readable or writable*/
      {
        fsm_current_state = COMPARE_TAG;
        
      //init
        if(current_LRU)
        {
          address_tag = buff1.DATA_ADDRESS.range(31,9);
          address_index = buff1.DATA_ADDRESS.range(8,2);
          address_offset = buff1.DATA_ADDRESS.range(1,0);

          data = buff1.DATA;
          store = buff1.STORE;
          load = buff1.LOAD;
        }
        else
        {
          address_tag = buff0.DATA_ADDRESS.range(31,9);
          address_index = buff0.DATA_ADDRESS.range(8,2);
          address_offset = buff0.DATA_ADDRESS.range(1,0);
          
          data = buff0.DATA;
          store = buff0.STORE;
          load = buff0.LOAD;
        }
      }
      break;
    case COMPARE_TAG:
      if(address_tag == way0.TAGS[address_index])
      {
        way0_hit = true;
        DATA_C = way0.DATA[address_index];
      }
      else
        way0_hit = false;
      
      if(address_tag == way1.TAGS[address_index])
      {
        way1_hit = true;
        DATA_C = way1.DATA[address_index];
      }
      else
        way1_hit = false;

      
      if(way0_hit)
      {
        HIT_C.write(true);
        fsm_current_state = IDLE;
        
        if(load)
        {
          DATA_C.write(way0.DATA[address_index]);
          VALID_DATA_C = true;
        }
        if(store)
        {
          way0.DATA[address_index] = data;
          // write to memory
        }
      }
      else (way1_hit)
      {
        HIT_C.write(true)
        fsm_current_state = IDLE;
        
        if(load)
        {
          DATA_C.write(way1.DATA[address_index]);
          VALID_DATA_C = true;
        }
        if(store)
        {
          way1.DATA[address_index] = data;
        // write to memory
        }
      }
      else // MISS
      {
        HIT_C.write(false);
        VALID_DATA_C.write(false);
        
        fsm_current_state = ALLOCATE;
      }
      break;
    case ALLOCATE:
      if(PI_ACKNOWLOEDGE == 0b011) //READY
      {
        fsm_current_state = COMPARE_TAG;
      }
      else
      {
      //
      }
      break;
  }
}
