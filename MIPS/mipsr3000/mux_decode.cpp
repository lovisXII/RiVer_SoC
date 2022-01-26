#include "mux_decode.h"


void mux_decode::processMUX_IR_SD()
{
  if (BUBBLE_SD.read()==1)
    MUX_IR_SD.write(nop_i);
  else if (HOLD_SD.read()==1)
    MUX_IR_SD.write(IR_RD.read());
  else
    MUX_IR_SD.write(IR_RI.read());
}

void mux_decode::processMUX_I_TYPE_SD()
{
  if (BUBBLE_SD.read()==1)
    MUX_I_TYPE_SD.write(nop_type);
  else if (HOLD_SD.read()==1)
    MUX_I_TYPE_SD.write(I_TYPE_RD.read());
  else
    MUX_I_TYPE_SD.write(I_TYPE_SD.read());
}

void mux_decode::processMUX_OPCOD_SD()
{
  if (BUBBLE_SD.read()==1)
    MUX_OPCOD_SD.write(addu_i);
  else if (HOLD_SD.read()==1)
    MUX_OPCOD_SD.write(OPCOD_RD.read());
  else
    MUX_OPCOD_SD.write(OPCOD_SD.read());
}

void mux_decode::processMUX_BDSLOT_SD()
{
  if (BUBBLE_SD.read()==1)
    MUX_BDSLOT_SD.write(BDSLOT_RI.read());
  else if (HOLD_SD.read()==1)
    MUX_BDSLOT_SD.write(BDSLOT_RD.read());
  else
    MUX_BDSLOT_SD.write(BDSLOT_RI.read());
}

void mux_decode::processMUX_SWAP_SD()
{
  if (BUBBLE_SD.read()==1)
    MUX_SWAP_SD.write(0);
  else if (HOLD_SD.read()==1)
    MUX_SWAP_SD.write(SWAP_RD.read());
  else
    MUX_SWAP_SD.write(SWAP_SD.read());
}

void mux_decode::processMUX_RS_SD()
{
  if (BUBBLE_SD.read()==1)
    MUX_RS_SD.write(0);
  else if (HOLD_SD.read()==1)
    MUX_RS_SD.write(RS_RD.read());
  else
    MUX_RS_SD.write(RS_SD.read());
}

void mux_decode::processMUX_RT_SD()
{
  if (BUBBLE_SD.read()==1)
    MUX_RT_SD.write(0);
  else if (HOLD_SD.read()==1)
    MUX_RT_SD.write(RT_RD.read());
  else
    MUX_RT_SD.write(RT_SD.read());
}

void mux_decode::processMUX_RD_SD()
{
  if (BUBBLE_SD.read()==1)
    MUX_RD_SD.write(0);
  else if (HOLD_SD.read()==1)
    MUX_RD_SD.write(RD_RD.read());
  else
    MUX_RD_SD.write(RD_SD.read());
}

void mux_decode::processMUX_COP0D_SD()
{
  if (BUBBLE_SD.read()==1)
    MUX_COP0D_SD.write(0);
  else if (HOLD_SD.read()==1)
    MUX_COP0D_SD.write(COP0D_RD.read());
  else
    MUX_COP0D_SD.write(COP0D_SD.read());
}

void mux_decode::processMUX_ILLGINS_SD()
{
  if (BUBBLE_SD.read()==1)
    MUX_ILLGINS_SD.write(0);
  else if (HOLD_SD.read()==1)
    MUX_ILLGINS_SD.write(ILLGINS_RD.read());
  else
    MUX_ILLGINS_SD.write(ILLGINS_XD.read());
}

void mux_decode::processMUX_C0UNUSE_SD()
{
  if (BUBBLE_SD.read()==1)
    MUX_C0UNUSE_SD.write(0);
  else if (HOLD_SD.read()==1)
    MUX_C0UNUSE_SD.write(C0UNUSE_RD.read());
  else
    MUX_C0UNUSE_SD.write(C0UNUSE_XD.read());
}

void mux_decode::processMUX_NEXTPC_SD()
{
  if ((INTRQ_XX.read()==1) || (EXCRQ_XM.read()==1) || (RESET_RX.read()==1))
    MUX_NEXTPC_SD.write(NEXTPC_XX.read());
  else if (KEEP_SD.read()==1)
    MUX_NEXTPC_SD.write(NEXTPC_RD.read());
  else
    MUX_NEXTPC_SD.write(NEXTPC_SD.read());
}

void mux_decode::processMUX_PC_SD()
{
  if (HOLD_SD.read()==1)
    MUX_PC_SD.write(PC_RD.read());
  else
    MUX_PC_SD.write(PC_RI.read());
}

void mux_decode::processMUX_SR_SD()
{
  if (HOLD_SD.read()==1)
    MUX_SR_SD.write(SR_RD.read());
  else
    MUX_SR_SD.write(SR_RI.read());
}

void mux_decode::processMUX_SOPER_SD()
{
  if (KEEP_SD.read()==1)
    MUX_SOPER_SD.write(X_SE.read());
  else
    MUX_SOPER_SD.write(SOPER_SD.read());
}

void mux_decode::processMUX_TOPER_SD()
{
  if (KEEP_SD.read()==1)
    MUX_TOPER_SD.write(Y_SE.read());
  else
    MUX_TOPER_SD.write(TOPER_SD.read());
}

void mux_decode::processMUX_IOPER_SD()
{
  if (KEEP_SD.read()==1)
    MUX_IOPER_SD.write(IOPER_RD.read());
  else
    MUX_IOPER_SD.write(IOPER_SD.read());
}
