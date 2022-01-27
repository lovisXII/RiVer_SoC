#include "pi_control.h"


void pi_control::processPI_IREQ()
{
  sc_uint<8> pi_ireq;

  pi_ireq[0]=PI_IREQ0.read();
  pi_ireq[1]=PI_IREQ1.read();
  pi_ireq[2]=PI_IREQ2.read();
  pi_ireq[3]=PI_IREQ3.read();
  pi_ireq[4]=PI_IREQ4.read();
  pi_ireq[5]=PI_IREQ5.read();
  pi_ireq[6]=PI_IREQ6.read();
  pi_ireq[7]=PI_IREQ7.read();
  PI_IREQ.write(pi_ireq);
}

void pi_control::processCK_SX()
{
  CK_SX.write(CK.read());
}

void pi_control::processREQUEST_SX()
{
  REQUEST_SX.write(PI_IREQ.read());
}

void pi_control::processCURPRIO_SX7()
{
  sc_uint<8> prior_rx=PRIOR_RX.read();
  sc_uint<8> request_sx=REQUEST_SX.read();

  CURPRIO_SX7.write(prior_rx[7] & (CURPRIO_SX6.read() | request_sx[7]));
}

void pi_control::processCURPRIO_SX6()
{
  sc_uint<8> prior_rx=PRIOR_RX.read();
  sc_uint<8> request_sx=REQUEST_SX.read();

  CURPRIO_SX6.write(prior_rx[6] & (CURPRIO_SX5.read() | request_sx[6]));
}

void pi_control::processCURPRIO_SX5()
{
  sc_uint<8> prior_rx=PRIOR_RX.read();
  sc_uint<8> request_sx=REQUEST_SX.read();

  CURPRIO_SX5.write(prior_rx[5] & (CURPRIO_SX4.read() | request_sx[5]));
}

void pi_control::processCURPRIO_SX4()
{
  sc_uint<8> prior_rx=PRIOR_RX.read();
  sc_uint<8> request_sx=REQUEST_SX.read();

  CURPRIO_SX4.write(prior_rx[4] & (CURPRIO_SX3.read() | request_sx[4]));
}

void pi_control::processCURPRIO_SX3()
{
  sc_uint<8> prior_rx=PRIOR_RX.read();
  sc_uint<8> request_sx=REQUEST_SX.read();

  CURPRIO_SX3.write(prior_rx[3] & (CURPRIO_SX2.read() | request_sx[3]));
}

void pi_control::processCURPRIO_SX2()
{
  sc_uint<8> prior_rx=PRIOR_RX.read();
  sc_uint<8> request_sx=REQUEST_SX.read();

  CURPRIO_SX2.write(prior_rx[2] & (CURPRIO_SX1.read() | request_sx[2]));
}

void pi_control::processCURPRIO_SX1()
{
  sc_uint<8> prior_rx=PRIOR_RX.read();
  sc_uint<8> request_sx=REQUEST_SX.read();

  CURPRIO_SX1.write(prior_rx[1] & (CURPRIO_SX0.read() | request_sx[1]));
}

void pi_control::processCURPRIO_SX0()
{
  sc_uint<8> prior_rx=PRIOR_RX.read();
  sc_uint<8> request_sx=REQUEST_SX.read();

  CURPRIO_SX0.write(prior_rx[0] & (CURPRIO_SX7.read() | request_sx[0]));
}

void pi_control::processCURPRIO_SX()
{
  sc_uint<8> curprio_sx=0x0;

  curprio_sx[0]=CURPRIO_SX0.read();
  curprio_sx[1]=CURPRIO_SX1.read();
  curprio_sx[2]=CURPRIO_SX2.read();
  curprio_sx[3]=CURPRIO_SX3.read();
  curprio_sx[4]=CURPRIO_SX4.read();
  curprio_sx[5]=CURPRIO_SX5.read();
  curprio_sx[6]=CURPRIO_SX6.read();
  curprio_sx[7]=CURPRIO_SX7.read();
  CURPRIO_SX.write(curprio_sx);
}

void pi_control::processGRANT_SX()
{
  sc_uint<8> grant_sx;
  sc_uint<8> request_sx=REQUEST_SX.read();
  sc_uint<8> curprio_sx=CURPRIO_SX.read();

  grant_sx[7]=(request_sx[7] & (!curprio_sx[6]));
  grant_sx[6]=(request_sx[6] & (!curprio_sx[5]));
  grant_sx[5]=(request_sx[5] & (!curprio_sx[4]));
  grant_sx[4]=(request_sx[4] & (!curprio_sx[3]));
  grant_sx[3]=(request_sx[3] & (!curprio_sx[2]));
  grant_sx[2]=(request_sx[2] & (!curprio_sx[1]));
  grant_sx[1]=(request_sx[1] & (!curprio_sx[0]));
  grant_sx[0]=(request_sx[0] & (!curprio_sx[7]));
  GRANT_SX.write(grant_sx);
}

void pi_control::processDFLMSTR_SX()
{
  sc_uint<8> dflmstr_sx=0x0;
  sc_uint<8> prior_rx=PRIOR_RX.read();

  dflmstr_sx.range(7,1)=prior_rx.range(7,1) & (~prior_rx.range(6,0));
  dflmstr_sx[0]=prior_rx[0] & (!prior_rx[7]);
  DFLMSTR_SX.write(dflmstr_sx);
}

void pi_control::processGLBRQST_SX()
{
  GLBRQST_SX.write((REQUEST_SX.read()==0x0)?0:1);
}

void pi_control::processCMPDFLT_SX()
{
  CMPDFLT_SX.write(DFLMSTR_SX.read() & REQUEST_SX.read());
}

void pi_control::processDFLTRQS_SX()
{
  DFLTRQS_SX.write((CMPDFLT_SX.read()==0x0)?0:1);
}

void pi_control::processSELECT_SX()
{
  sc_uint<32> pi_a=PI_A.read();
  sc_uint<32> pi_a_shft=pi_a << 2;

  if (pi_a_shft.range(31,12)==0x0)
    SELECT_SX.write(0x1);
  else if (pi_a_shft.range(31,12)==0x00400)
    SELECT_SX.write(0x2);
  else if (pi_a_shft.range(31,12)==0x80000)
    SELECT_SX.write(0x4);
  else if (pi_a_shft.range(31,12)==0xBFC00)
    SELECT_SX.write(0x8);
  else if (pi_a_shft.range(31,12)==0xC0000)
    SELECT_SX.write(0x10);
  else
    SELECT_SX.write(0x0);
}

void pi_control::processC_NXTS_SX()
{
  if (RESET_RX.read()==1)
    C_NXTS_SX.write(c_idle);
  else if ((C_STAT_RX.read() == c_idle) && (GLBRQST_SX.read() ==0))
    C_NXTS_SX.write(c_idle);
  else if ((C_STAT_RX.read() == c_ldat) && (DATARDY_SX.read() ==1) && (GLBRQST_SX.read() ==0))
    C_NXTS_SX.write(c_idle);
  else if ((C_STAT_RX.read() == c_idle) && (GLBRQST_SX.read() ==1) && (DFLTRQS_SX.read() ==0))
    C_NXTS_SX.write(c_fadr);
  else if ((C_STAT_RX.read() == c_idle) && (GLBRQST_SX.read() ==1) && (DFLTRQS_SX.read() ==1)&& (PI_LOCK.read() ==1))
    C_NXTS_SX.write(c_fadr);
  else if ((C_STAT_RX.read() == c_ldat) && (DATARDY_SX.read() ==1) && (GLBRQST_SX.read() ==1))
    C_NXTS_SX.write(c_fadr);
  else if ((C_STAT_RX.read() == c_fadr) && (PI_LOCK.read() ==1))
    C_NXTS_SX.write(c_nadr);
  else if ((C_STAT_RX.read() == c_nadr) && (PI_LOCK.read() ==1))
    C_NXTS_SX.write(c_nadr);
  else if ((C_STAT_RX.read() == c_idle) && (GLBRQST_SX.read() ==1) && (DFLTRQS_SX.read() ==1)&& (PI_LOCK.read() ==0))
    C_NXTS_SX.write(c_ldat);
  else if ((C_STAT_RX.read() == c_fadr) && (PI_LOCK.read() ==0))
    C_NXTS_SX.write(c_ldat);
  else if ((C_STAT_RX.read() == c_nadr) && (PI_LOCK.read() ==0))
    C_NXTS_SX.write(c_ldat);
  else if ((C_STAT_RX.read() == c_ldat) && (DATARDY_SX.read() ==0))
    C_NXTS_SX.write(c_ldat);
  else
    C_NXTS_SX.write(c_idle);
}

void pi_control::processWRTPRIO_SX()
{
  if ((C_STAT_RX.read() == c_idle) && (GLBRQST_SX.read() ==1 ))
    WRTPRIO_SX.write(1);
  else if ((C_STAT_RX.read() == c_ldat) && (DATARDY_SX.read() ==1 )&& (GLBRQST_SX.read() ==1 ))
    WRTPRIO_SX.write(1);
  else
    WRTPRIO_SX.write(0);
}

void pi_control::processPI_TOUT()
{
  PI_TOUT <= '0';
}

void pi_control::processTRANRQS_SX()
{
  if ((PI_OPC.read()==0x0) || (PI_OPC.read()==0x1))
    TRANRQS_SX.write(0);
  else
    TRANRQS_SX.write(1);
}

void pi_control::processDATARDY_SX()
{
  if ((PI_ACK.read() == ack_rdy) || (PI_ACK.read() == ack_rdm) | (PI_ACK.read() == ack_spl))
    DATARDY_SX.write(1);
  else
    DATARDY_SX.write(0);
}

void pi_control::processPI_SEL()
{
  if ((C_STAT_RX.read() == c_idle) && (GLBRQST_SX.read() ==1) && (DFLTRQS_SX.read() ==1))
    PI_SEL.write(SELECT_SX.read());
  else if ((C_STAT_RX.read() == c_fadr) && (TRANRQS_SX.read() ==1))
    PI_SEL.write(SELECT_SX.read());
  else if ((C_STAT_RX.read() == c_nadr) && (TRANRQS_SX.read() ==1) && (DATARDY_SX.read() ==1))
    PI_SEL.write(SELECT_SX.read());
  else
    PI_SEL.write(0x0);
}

void pi_control::processPI_GNT()
{
  if ((C_STAT_RX.read() == c_idle) && (GLBRQST_SX.read()==0))
    PI_GNT.write(DFLMSTR_SX.read());
  else if ((C_STAT_RX.read() == c_ldat) && (DATARDY_SX.read()==1)&& (GLBRQST_SX.read()==0))
    PI_GNT.write(DFLMSTR_SX.read());
  else if ((C_STAT_RX.read() == c_idle) && (GLBRQST_SX.read()==1)&& (DFLTRQS_SX.read()==0))
    PI_GNT.write(GRANT_SX.read());
  else if ((C_STAT_RX.read() == c_ldat) && (DATARDY_SX.read()==1)&& (GLBRQST_SX.read()==1))
    PI_GNT.write(GRANT_SX.read());
  else
    PI_GNT.write(0x0);
}

void pi_control::processC_STAT_RX()
{
  C_STAT_RX.write(C_NXTS_SX.read());
}

void pi_control::processPRIOR_RX()
{
  if (RESET_RX.read()==1)
    PRIOR_RX.write(0x7F);
  else if (WRTPRIO_SX.read()==1)
    PRIOR_RX.write(~GRANT_SX.read());
}

void pi_control::processRESET_RX()
{
  RESET_RX.write(!RESET_N.read());
}

void pi_control::processGNTS()
{
  sc_uint<8> pi_gnt=PI_GNT.read();

  PI_GNT0.write(pi_gnt[0]);
  PI_GNT1.write(pi_gnt[1]);
  PI_GNT2.write(pi_gnt[2]);
  PI_GNT3.write(pi_gnt[3]);
  PI_GNT4.write(pi_gnt[4]);
  PI_GNT5.write(pi_gnt[5]);
  PI_GNT6.write(pi_gnt[6]);
  PI_GNT7.write(pi_gnt[7]);
}

void pi_control::processSELS()
{
  sc_uint<8> pi_sel=PI_SEL.read();

  PI_SEL0.write(pi_sel[0]);
  PI_SEL1.write(pi_sel[1]);
  PI_SEL2.write(pi_sel[2]);
  PI_SEL3.write(pi_sel[3]);
  PI_SEL4.write(pi_sel[4]);
  PI_SEL5.write(pi_sel[5]);
  PI_SEL6.write(pi_sel[6]);
  PI_SEL7.write(pi_sel[7]);
}
