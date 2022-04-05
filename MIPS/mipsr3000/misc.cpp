#include "misc.h"

void misc::processCK_SX()
{
  CK_SX.write(CK.read());
}

void misc::processIT_XX()
{
  IT_XX.write(!IT_N.read());
}

void misc::processCAUSE_XX()
{
  sc_uint<32> cause_rx=CAUSE_RX.read();
  sc_uint<32> cause_xx=0x0;

  cause_xx[31]=BDSLOT_RD.read();
  cause_xx.range(29,28)=COPERR_XM.read();
  cause_xx.range(15,10)=IT_XX.read();
  cause_xx.range(9,8)=SWINT_XM.read();
  cause_xx.range(5,2)=EXCCODE_XM.read();
  CAUSE_XX.write(cause_xx);
}

void misc::processCAUSE_SX()
{
  sc_uint<32> res_re=RES_RE.read();
  sc_uint<32> cause_rx=CAUSE_RX.read();

  CAUSE_SX.write(cause_rx.range(31,16) 
			<< 16 | IT_XX.read() 
			<< 10 | cause_rx.range(9,0));
}

void misc::processBOOTEV_XX()
{
  sc_uint<32> nextsr_rx=NEXTSR_RX.read();
  BOOTEV_XX.write(nextsr_rx[22]);
}

void misc::processNEXTPC_XX()
{
  if (RESET_RX.read()==1)
    NEXTPC_XX.write(reset_a);
  else if (BOOTEV_XX.read()==1)
    NEXTPC_XX.write(boothnd_a);
  else
    NEXTPC_XX.write(excphnd_a);
}

void misc::processNEXTSR_XX()
{
  sc_uint<32> nextsr_rx=NEXTSR_RX.read();
  if (RESET_RX.read()==1)
    NEXTSR_XX.write(0x400000);
  else
    NEXTSR_XX.write(nextsr_rx.range(31,6)<<6 | nextsr_rx.range(3,0)<<2);
}

void misc::processWSR_XX()
{
  WSR_XX.write(EXCRQ_XM.read() | INTRQ_XX.read() | RESET_RX.read());
}

void misc::processEPC_XX()
{
  if (BDSLOT_RD.read()==1)
    EPC_XX.write(PC_RD.read());
  else
    EPC_XX.write(REDOPC_RE.read());
}

void misc::processWEPC_XX()
{
  WEPC_XX.write(INTRQ_XX.read() | RESET_RX.read());
}

void misc::processHWINTRQ_XX()
{
  sc_uint<32> cause_rx=CAUSE_RX.read();
  HWINTRQ_XX.write(cause_rx.range(15,10));
}

void misc::processGLBMSK_XX()
{
  sc_uint<32> nextsr_rx=NEXTSR_RX.read();
  GLBMSK_XX.write(nextsr_rx[0]);
}

void misc::processITMASK_XX()
{
  sc_uint<32> nextsr_rx=NEXTSR_RX.read();
  ITMASK_XX.write(nextsr_rx.range(15,8));
}

void misc::processINTRQ_XX()
{
  sc_uint<32> itmask_xx=ITMASK_XX.read();
  sc_uint<32> hwintrq_xx=HWINTRQ_XX.read();
  sc_uint<2> swintrq_xm=SWINTRQ_XM.read();

  INTRQ_XX.write(GLBMSK_XX.read() & ((itmask_xx [7] & hwintrq_xx [5]) |
                                    (itmask_xx[6] & hwintrq_xx[4]) |
                                    (itmask_xx[5] & hwintrq_xx[3]) |
                                    (itmask_xx[4] & hwintrq_xx[2]) |
                                    (itmask_xx[3] & hwintrq_xx[1]) |
                                    (itmask_xx[2] & hwintrq_xx[0]) |
                                    (itmask_xx[1] & swintrq_xm[1]) |
                                    (itmask_xx[0] & swintrq_xm[0])));
}

void misc::processD_A()
{
  D_A.write(RES_RE.read());
}

void misc::processI_A()
{
  I_A.write(NEXTPC_RD.read());
}

void misc::processD_OUT()
{
  D_OUT.write(WDATA_RE.read());
}

void misc::processD_ACK()
{
  D_ACK.write(DATARED_RM.read());
}

void misc::processI_FTCH()
{
  if ((int)OPCOD_RE.read()==sleep_i)
    I_FTCH.write(0);
  else
    I_FTCH.write(1);
}

void misc::processI_ACK()
{
  I_ACK.write(INSTRED_RI.read());
}

void misc::processD_ATYPE()
{
  if ((I_WORD_SM.read() & WRITE_SM.read())==1)
    D_ATYPE.write(m_writ_w);
  else if ((I_HALF_SM.read() & WRITE_SM.read())==1)
    D_ATYPE.write(m_writ_h);
  else if ((I_BYTE_SM.read() & WRITE_SM.read())==1)
    D_ATYPE.write(m_writ_b);
  else
    D_ATYPE.write(m_read_w);
}

void misc::processD_RQ()
{
  if ((EARLYEX_XM.read() | RESET_RX.read())==0)
    D_RQ.write(DACCESS_SM.read());
  else
    D_RQ.write(0);
}

void misc::processD_LOCK()
{
  D_LOCK.write(DLOCK_SM.read());
}
