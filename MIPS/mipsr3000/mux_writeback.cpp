#include "mux_writeback.h"


void mux_writeback::processMUX_LO_SW()
{
  if (WLO_SW.read()==1)
    MUX_LO_SW.write(DATA_RM.read());
  else
    MUX_LO_SW.write(LO_RW.read());
}

void mux_writeback::processMUX_HI_SW()
{
  if (WHI_SW.read()==1)
    MUX_HI_SW.write(DATA_RM.read());
  else
    MUX_HI_SW.write(HI_RW.read());
}
