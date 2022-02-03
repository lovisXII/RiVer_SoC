#include "ff_execute.h"

void ff_execute::processClock()
{
  IR_RE.write(MUX_IR_SE.read());
  I_TYPE_RE.write(MUX_I_TYPE_SE.read());
  OPCOD_RE.write(MUX_OPCOD_SE.read());
  BDSLOT_RE.write(MUX_BDSLOT_SE.read());
  RD_RE.write(MUX_RD_SE.read());
  COP0D_RE.write(MUX_COP0D_SE.read());
  OVF_RE.write(MUX_OVF_SE.read());
  IAMALGN_RE.write(MUX_IAMALGN_SE.read());
  IASVIOL_RE.write(MUX_IASVIOL_SE.read());
  IABUSER_RE.write(MUX_IABUSER_SE.read());
  BREAK_RE.write(MUX_BREAK_SE.read());
  SYSCALL_RE.write(MUX_SYSCALL_SE.read());
  ILLGINS_RE.write(MUX_ILLGINS_SE.read());
  C0UNUSE_RE.write(MUX_C0UNUSE_SE.read());
  SWAP_RE.write(MUX_SWAP_SE.read());
  PC_RE.write(MUX_PC_SE.read());
  SR_RE.write(MUX_SR_SE.read());
  NEXTPC_RE.write(MUX_NEXTPC_SE.read());
  RES_RE.write(MUX_RES_SE.read());
  WDATA_RE.write(MUX_WDATA_SE.read());
  COPYCAP_RE.write(MUX_COPYCAP_SE.read());
  REDOPC_RE.write(MUX_REDOPC_SE.read());
}
