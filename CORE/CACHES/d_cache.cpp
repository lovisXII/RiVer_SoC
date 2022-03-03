#include "d_cache.h"


void d_cache::init()
{
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
    way0.PRESENCE[i] = false;
    way1.PRESENCE[i] = false;
  }
}

void d_cache::transition()
{
  
}
