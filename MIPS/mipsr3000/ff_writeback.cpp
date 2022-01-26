#include "ff_writeback.h"


void ff_writeback::processClock()
{
  LO_RW.write(MUX_LO_SW.read());
  HI_RW.write(MUX_HI_SW.read());
}
