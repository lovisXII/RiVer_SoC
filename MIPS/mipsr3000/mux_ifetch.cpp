#include "mux_ifetch.h"


void mux_ifetch::processMUX_IR_SI()
{
  if (BUBBLE_SI.read())
    MUX_IR_SI.write(nop_i);
  else if (HOLD_SI.read())
    MUX_IR_SI.write(IR_RI.read());
  else
    MUX_IR_SI.write(I.read());
}

void mux_ifetch::processMUX_INSTRED_SI()
{
  if (BUBBLE_SI.read())
    MUX_INSTRED_SI.write(0);
  else if (HOLD_SI.read())
    MUX_INSTRED_SI.write(0);
  else
    MUX_INSTRED_SI.write(1);
}

void mux_ifetch::processMUX_BDSLOT_SI()
{
  if (BUBBLE_SI.read())
    MUX_BDSLOT_SI.write(BDSLOT_XI.read());
  else if (HOLD_SI.read())
    MUX_BDSLOT_SI.write(BDSLOT_RI.read());
  else
    MUX_BDSLOT_SI.write(BDSLOT_XI.read());
}

void mux_ifetch::processMUX_PC_SI()
{
  if (KEEP_SI.read())
    MUX_PC_SI.write(PC_RI.read());
  else
    MUX_PC_SI.write(NEXTPC_RD.read());
}

void mux_ifetch::processMUX_SR_SI()
{
  if (KEEP_SI.read())
    MUX_SR_SI.write(SR_RI.read());
  else
    MUX_SR_SI.write(SR_SI.read());
}
