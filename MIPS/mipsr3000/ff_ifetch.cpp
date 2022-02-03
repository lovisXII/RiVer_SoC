#include "ff_ifetch.h"


void ff_ifetch::processClock()
{
  IR_RI.write(MUX_IR_SI.read());
  INSTRED_RI.write(MUX_INSTRED_SI.read());
  BDSLOT_RI.write(MUX_BDSLOT_SI.read());
  PC_RI.write(MUX_PC_SI.read());
  SR_RI.write(MUX_SR_SI.read());
}
