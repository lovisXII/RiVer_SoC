#include "ff_memory.h"


void ff_memory::processClock()
{
  IR_RM.write(MUX_IR_SM.read());
  DATARED_RM.write(MUX_DATARED_SM.read());
  I_TYPE_RM.write(MUX_I_TYPE_SM.read());
  RD_RM.write(MUX_RD_SM.read());
  OPCOD_RM.write(MUX_OPCOD_SM.read());
  DATA_RM.write(MUX_DATA_SM.read());
  BADVADR_RM.write(MUX_BADVADR_SM.read());
}
