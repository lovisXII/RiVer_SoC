#include "ff_decode.h"

void ff_decode::processClock()
{
  IR_RD.write(MUX_IR_SD.read());
  I_TYPE_RD.write(MUX_I_TYPE_SD.read());
  OPCOD_RD.write(MUX_OPCOD_SD.read());
  BDSLOT_RD.write(MUX_BDSLOT_SD.read());
  SWAP_RD.write(MUX_SWAP_SD.read());
  RS_RD.write(MUX_RS_SD.read());
  RT_RD.write(MUX_RT_SD.read());
  RD_RD.write(MUX_RD_SD.read());
  COP0D_RD.write(MUX_COP0D_SD.read());
  ILLGINS_RD.write(MUX_ILLGINS_SD.read());
  C0UNUSE_RD.write(MUX_C0UNUSE_SD.read());
  NEXTPC_RD.write(MUX_NEXTPC_SD.read());
  PC_RD.write(MUX_PC_SD.read());
  SR_RD.write(MUX_SR_SD.read());
  SOPER_RD.write(MUX_SOPER_SD.read());
  TOPER_RD.write(MUX_TOPER_SD.read());
  IOPER_RD.write(MUX_IOPER_SD.read());
}
