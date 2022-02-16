#include "mux_memory.h"


void mux_memory::processMUX_IR_SM()
{
  if (BUBBLE_SM.read()==1)
    MUX_IR_SM.write(nop_i);
  else if (HOLD_SM.read()==1)
    MUX_IR_SM.write(IR_RM.read());
  else
    MUX_IR_SM.write(IR_RE.read());
}

void mux_memory::processMUX_DATARED_SM()
{
  if (BUBBLE_SM.read()==1)
    MUX_DATARED_SM.write(0);
  else if (HOLD_SM.read()==1)
    MUX_DATARED_SM.write(0);
  else
    MUX_DATARED_SM.write(DATARED_SM.read());
}

void mux_memory::processMUX_I_TYPE_SM()
{
  if (BUBBLE_SM.read()==1)
    MUX_I_TYPE_SM.write(nop_type);
  else if (HOLD_SM.read()==1)
    MUX_I_TYPE_SM.write(I_TYPE_RM.read());
  else
    MUX_I_TYPE_SM.write(I_TYPE_RE.read());
}

void mux_memory::processMUX_RD_SM()
{
  if (BUBBLE_SM.read()==1)
    MUX_RD_SM.write(0);
  else if (HOLD_SM.read()==1)
    MUX_RD_SM.write(RD_RM.read());
  else
    MUX_RD_SM.write(RD_SM.read());
}

void mux_memory::processMUX_OPCOD_SM()
{
  if (BUBBLE_SM.read()==1)
    MUX_OPCOD_SM.write(addu_i);
  else if (HOLD_SM.read()==1)
    MUX_OPCOD_SM.write(OPCOD_RM.read());
  else
    MUX_OPCOD_SM.write(OPCOD_RE.read());
}

void mux_memory::processMUX_DATA_SM()
{
  if (KEEP_SM.read()==1)
    MUX_DATA_SM.write(DATA_RM.read());
  else
    MUX_DATA_SM.write(DATA_SM.read());
}


void mux_memory::processMUX_BADVADR_SM()
{
  // if bad @ save instruction
  // if bad instruction save @
  // else save bad adresse register
  
  if (BADDA_XM.read()==1)
    MUX_BADVADR_SM.write(RES_RE.read());
  else if (BADIA_XM.read()==1)
    MUX_BADVADR_SM.write(NEXTPC_RE.read());
  else
    MUX_BADVADR_SM.write(BADVADR_RM.read());
}
