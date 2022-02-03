#include "mux_misc.h"


void mux_misc::processMUX_CAUSE_SX()
{
  if (EXCRQ_XM.read()==1)
    MUX_CAUSE_SX.write(CAUSE_XM.read());
  else if (INTRQ_XX.read()==1)
    MUX_CAUSE_SX.write(CAUSE_XX.read());
  else if ((WCAUSE_SM.read()==1)&&(KEEP_SM.read()==0))
    MUX_CAUSE_SX.write(CAUSE_SM.read());
  else
    MUX_CAUSE_SX.write(CAUSE_SX.read());
}

void mux_misc::processMUX_EPC_SX()
{
  if (WEPC_XM.read()==1)
    MUX_EPC_SX.write(EPC_XM.read());
  else if (WEPC_XX.read()==1)
    MUX_EPC_SX.write(EPC_XX.read());
  else
    MUX_EPC_SX.write(EPC_RX.read());
}

void mux_misc::processMUX_NEXTSR_SX()
{
  if (WSR_XX.read()==1)
    MUX_NEXTSR_SX.write(NEXTSR_XX.read());
  else if ((WSR_SM.read()==1)&&(KEEP_SM.read()==0))
    MUX_NEXTSR_SX.write(NEXTSR_SM.read());
  else
    MUX_NEXTSR_SX.write(NEXTSR_RX.read());
}

void mux_misc::processMUX_RESET_SX()
{
  MUX_RESET_SX.write(!RESET_N.read());
}
