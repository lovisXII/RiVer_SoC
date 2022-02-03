#include "mux_execute.h"


void mux_execute::processMUX_IR_SE()
{
  if (BUBBLE_SE.read()==1)
    MUX_IR_SE.write(nop_i);
  else if (HOLD_SE.read()==1)
    MUX_IR_SE.write(IR_RE.read());
  else
    MUX_IR_SE.write(IR_RD.read());
}

void mux_execute::processMUX_I_TYPE_SE()
{
  if (BUBBLE_SE.read()==1)
    MUX_I_TYPE_SE.write(nop_type);
  else if (HOLD_SE.read()==1)
    MUX_I_TYPE_SE.write(I_TYPE_RE.read());
  else
    MUX_I_TYPE_SE.write(I_TYPE_RD.read());
}

void mux_execute::processMUX_OPCOD_SE()
{
  if (BUBBLE_SE.read()==1)
    MUX_OPCOD_SE.write(addu_i);
  else if (HOLD_SE.read()==1)
    MUX_OPCOD_SE.write(OPCOD_RE.read());
  else
    MUX_OPCOD_SE.write(OPCOD_RD.read());
}

void mux_execute::processMUX_BDSLOT_SE()
{
  if (BUBBLE_SE.read()==1)
    MUX_BDSLOT_SE.write(BDSLOT_RD.read());
  else if (HOLD_SE.read()==1)
    MUX_BDSLOT_SE.write(BDSLOT_RE.read());
  else
    MUX_BDSLOT_SE.write(BDSLOT_RD.read());
}

void mux_execute::processMUX_RD_SE()
{
  if (BUBBLE_SE.read()==1)
    MUX_RD_SE.write(0);
  else if (HOLD_SE.read()==1)
    MUX_RD_SE.write(RD_RE.read());
  else
    MUX_RD_SE.write(RD_RD.read());
}

void mux_execute::processMUX_COP0D_SE()
{
  if (BUBBLE_SE.read()==1)
    MUX_COP0D_SE.write(0);
  else if (HOLD_SE.read()==1)
    MUX_COP0D_SE.write(COP0D_RE.read());
  else
    MUX_COP0D_SE.write(COP0D_RD.read());
}

void mux_execute::processMUX_OVF_SE()
{
  if (BUBBLE_SE.read()==1)
    MUX_OVF_SE.write(0);
  else if (HOLD_SE.read()==1)
    MUX_OVF_SE.write(OVF_RE.read());
  else
    MUX_OVF_SE.write(OVF_XE.read());
}

void mux_execute::processMUX_IALMALGN_SE()
{
  if (BUBBLE_SE.read()==1)
    MUX_IAMALGN_SE.write(0);
  else if (HOLD_SE.read()==1)
    MUX_IAMALGN_SE.write(IAMALGN_RE.read());
  else
    MUX_IAMALGN_SE.write(IAMALGN_XE.read());
}

void mux_execute::processMUX_IASVIOL_SE()
{
  if (BUBBLE_SE.read()==1)
    MUX_IASVIOL_SE.write(0);
  else if (HOLD_SE.read()==1)
    MUX_IASVIOL_SE.write(IASVIOL_RE.read());
  else
    MUX_IASVIOL_SE.write(IASVIOL_XE.read());
}

void mux_execute::processMUX_IABUSER_SE()
{
  if (BUBBLE_SE.read()==1)
    MUX_IABUSER_SE.write(0);
  else if (HOLD_SE.read()==1)
    MUX_IABUSER_SE.write(IABUSER_RE.read());
  else
    MUX_IABUSER_SE.write(IABUSER_XE.read());
}

void mux_execute::processMUX_BREAK_SE()
{
  if (BUBBLE_SE.read()==1)
    MUX_BREAK_SE.write(0);
  else if (HOLD_SE.read()==1)
    MUX_BREAK_SE.write(BREAK_RE.read());
  else
    MUX_BREAK_SE.write(BREAK_XE.read());
}

void mux_execute::processMUX_SYSCALL_SE()
{
  if (BUBBLE_SE.read()==1)
    MUX_SYSCALL_SE.write(0);
  else if (HOLD_SE.read()==1)
    MUX_SYSCALL_SE.write(SYSCALL_RE.read());
  else
    MUX_SYSCALL_SE.write(SYSCALL_XE.read());
}

void mux_execute::processMUX_ILLGINS_SE()
{
  if (BUBBLE_SE.read()==1)
    MUX_ILLGINS_SE.write(0);
  else if (HOLD_SE.read()==1)
    MUX_ILLGINS_SE.write(ILLGINS_RE.read());
  else
    MUX_ILLGINS_SE.write(ILLGINS_RD.read());
}

void mux_execute::processMUX_C0UNUSE_SE()
{
  if (BUBBLE_SE.read()==1)
    MUX_C0UNUSE_SE.write(0);
  else if (HOLD_SE.read()==1)
    MUX_C0UNUSE_SE.write(C0UNUSE_RE.read());
  else
    MUX_C0UNUSE_SE.write(C0UNUSE_RD.read());
}

void mux_execute::processMUX_SWAP_SE()
{
  if (BUBBLE_SE.read()==1)
    MUX_SWAP_SE.write(0);
  else if (HOLD_SE.read()==1)
    MUX_SWAP_SE.write(SWAP_RE.read());
  else
    MUX_SWAP_SE.write(SWAP_RD.read());
}

void mux_execute::processMUX_PC_SE()
{
  if (HOLD_SE.read()==1)
    MUX_PC_SE.write(PC_RE.read());
  else
    MUX_PC_SE.write(PC_RD.read());
}

void mux_execute::processMUX_SR_SE()
{
  if (HOLD_SE.read()==1)
    MUX_SR_SE.write(SR_RE.read());
  else
    MUX_SR_SE.write(SR_RD.read());
}

void mux_execute::processMUX_NEXTPC_SE()
{
  if (KEEP_SE.read()==1)
    MUX_NEXTPC_SE.write(NEXTPC_RE.read());
  else
    MUX_NEXTPC_SE.write(NEXTPC_RD.read());
}

void mux_execute::processMUX_RES_SE()
{
  if (KEEP_SE.read()==1)
    MUX_RES_SE.write(RES_RE.read());
  else
    MUX_RES_SE.write(RES_SE.read());
}

void mux_execute::processMUX_WDATA_SE()
{
  if (KEEP_SE.read()==1)
    MUX_WDATA_SE.write(WDATA_RE.read());
  else
    MUX_WDATA_SE.write(Y_SE.read());
}

void mux_execute::processMUX_COPYCAP_SE()
{
  if (BUBBLE_SE.read()==1)
    MUX_COPYCAP_SE.write(0);
  else if (STALL_SM.read()==1)
    MUX_COPYCAP_SE.write(COPYCAP_RE.read());
  else if (COPY_SM.read()==1)
    MUX_COPYCAP_SE.write(0);
  else
    MUX_COPYCAP_SE.write(COPYCAP_SE.read());
}

void mux_execute::processMUX_REDOPC_SE()
{
  if (KEEP_SE.read()==1)
    MUX_REDOPC_SE.write(REDOPC_RE.read());
  else if (WREDOPC_SE.read()==0)
    MUX_REDOPC_SE.write(REDOPC_RE.read());
  else
    MUX_REDOPC_SE.write(PC_RD.read());
}
