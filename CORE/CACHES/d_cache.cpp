#include "d_cache.h"


void d_cache::transition()
{
  if(RESET)
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
    //way0.PRESENCE[i] = false;
    //way1.PRESENCE[i] = false;
    }
  }
  
  switch(fsm_current_state)
  {
    case IDLE:
      if(VALID_ADDRESS_M)/*&& @ is readable or writable*/
      {
        fsm_current_state = COMPARE_TAG;

      //init
        way0_hit = false;
        way1_hit = false;
        
        address_tag = DATA_ADDRESS.range(31,9);
        address_index = DATA_ADDRESS.range(8,2);
        address_offset = DATA_ADDRESS.range(1,0);
      }
      break;
    case COMPARE_TAG:
      for(int i = 0; i < WAY_SIZE; ++i)
      {
        if(address_tag == way0.TAGS[i])
        {
          way0_hit = true;
          DATA_C = way0.DATA[i];
          break;
        }
        if(address_tag == way1.TAGS[i])
        {
          way1_hit = true;
          DATA_C = way1.DATA[i];
          break;
        }
      }
      
      if(!LOAD_M)
      {
        VALID_DATA_C = false;
      }
      
      if(way0_hit || way1_hit)
      {
        fsm_current_state = IDLE;
      }
      else // MISS
      {
        VALID_DATA_C = false;
        if(/*clean block*/)
        {
          fsm_current_state = ALLOCATE;
        }
        else if(/*dirty block*/)
        {
          fsm_current_state = WRITE_BACK;
        }
      }
      break;
    case ALLOCATE:
      if(/*memory ready*/)
      {
        fsm_current_state = COMPARE_TAG;
      }
      break;
    case WRITE_BACK:
      if(/*memory ready*/)
      {
        fsm_current_state = ALLOCATE;
      }
      break;
  }
}
