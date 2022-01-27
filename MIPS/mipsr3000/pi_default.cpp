#include "pi_default.h"



void pi_default::processPI_GNT()
{
  sc_uint<6> pi_gnt=0x0;

  pi_gnt[0]=PI_GNT0.read();
  pi_gnt[1]=PI_GNT1.read();
  pi_gnt[2]=PI_GNT2.read();
  pi_gnt[3]=PI_GNT3.read();
  pi_gnt[4]=PI_GNT4.read();
  pi_gnt[5]=PI_GNT5.read();
  PI_GNT.write(pi_gnt);
}

void pi_default::processCK_SX()
{
  CK_SX.write(CK);
}

void pi_default::processPI_IREQ()
{
  PI_IREQ.write(0x0);
}

void pi_default::processDEFLTEN_SX()
{
  DEFLTEN_SX.write((PI_GNT.read()==0x0)?0:1);
}

void pi_default::processPI_LINES()
{
  if (DEFLTEN_SX.read()==1)
    {
      PI_A.write(0x0);
      PI_READ.write(p_red_red);
      PI_LOCK.write(0);
      PI_OPC.write(p_opc_nop);
    }
}

void pi_default::processPI_IREQS()
{
  sc_uint<6> pi_ireq=PI_IREQ.read();
  PI_IREQ0.write(pi_ireq[0]);
  PI_IREQ1.write(pi_ireq[1]);
  PI_IREQ2.write(pi_ireq[2]);
  PI_IREQ3.write(pi_ireq[3]);
  PI_IREQ4.write(pi_ireq[4]);
  PI_IREQ5.write(pi_ireq[5]);
}
