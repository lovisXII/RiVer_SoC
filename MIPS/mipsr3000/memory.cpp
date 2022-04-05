#include "memory.h"

void memory::processI_MPDC_SM()
{
  sc_uint<25> i_type_re=I_TYPE_RE.read();

  I_MPDC_SM.write(i_type_re[8]);
}

void memory::processI_WRITE_SM()
{
  sc_uint<25> i_type_re=I_TYPE_RE.read();

  I_WRITE_SM.write(i_type_re[8] | i_type_re[7]);
}

void memory::processI_LOAD_SM()
{
  sc_uint<25> i_type_re=I_TYPE_RE.read();

  I_LOAD_SM.write(i_type_re[4]);
}

void memory::processI_STOR_SM()
{
  sc_uint<25> i_type_re=I_TYPE_RE.read();

  I_STOR_SM.write(i_type_re[3]);
}

void memory::processI_BYTE_SM()
{
  sc_uint<25> i_type_re=I_TYPE_RE.read();

  I_BYTE_SM.write(i_type_re[2]);
}

void memory::processI_HALF_SM()
{
  sc_uint<25> i_type_re=I_TYPE_RE.read();

  I_HALF_SM.write(i_type_re[1]);
}

void memory::processI_WORD_SM()
{
  sc_uint<25> i_type_re=I_TYPE_RE.read();

  I_WORD_SM.write(i_type_re[0]);
}

void memory::processFSTSWAP_SM()
{
  FSTSWAP_SM.write(SWAP_RE.read() & COPYCAP_RE.read());
}

void memory::processDACCESS_SM()
{
  DACCESS_SM.write(I_STOR_SM.read() | I_LOAD_SM.read()) ;
}

void memory::processWRITE_SM()
{
  WRITE_SM.write(I_STOR_SM.read()  & !FSTSWAP_SM.read());
}

void memory::processDLOCK_SM()
{
  DLOCK_SM.write(FSTSWAP_SM.read());
}

void memory::processDATARED_SM()
{
  DATARED_SM.write(DACCESS_SM.read() & (!WRITE_SM.read()));
}

void memory::processRD_SM()
{
  RD_SM.write(((SWAP_RE.read() & !COPYCAP_RE.read()) == 1) ? (sc_uint<5>)(0x0) : RD_RE.read()) ;
}

void memory::processBYTSEL_SM()
{
  sc_uint<32> res_re=RES_RE.read();
  sc_uint<5> temp=(I_BYTE_SM.read() 
			<< 4) | (I_HALF_SM.read() 
			<< 3) | (I_WORD_SM.read() 
			<< 2) | res_re.range(1,0);

  if ((int)temp==0x10)
    BYTSEL_SM.write(0x1);
  else if ((int)temp==0x11)
    BYTSEL_SM.write(0x2);
  else if ((int)temp==0x12)
    BYTSEL_SM.write(0x4);
  else if ((int)temp==0x13)
    BYTSEL_SM.write(0x8);
  else if ((int)temp==0x08)
    BYTSEL_SM.write(0x3);
  else if ((int)temp==0x0A)
    BYTSEL_SM.write(0xC);
  else if ((int)temp==0x04)
    BYTSEL_SM.write(0xF);
  else
    BYTSEL_SM.write(0x0);
}

void memory::processREDDAT_SM()
{
  sc_uint<32> bytsel_sm=BYTSEL_SM.read();
  sc_uint<32> d_in=D_IN.read();

  if ((int)bytsel_sm[0]==1)
    REDDAT_SM.write(d_in.range(31,0));
  else if ((int)bytsel_sm[1]==1)
    REDDAT_SM.write(d_in.range(31,8));
  else if ((int)bytsel_sm[2]==1)
    REDDAT_SM.write(d_in.range(31,16));
  else
    REDDAT_SM.write(d_in.range(31,24));
}

void memory::processBSEXT_SM()
{
  sc_uint<32> reddat_sm=REDDAT_SM.read();

  if ((reddat_sm[7]==1)&&(OPCOD_RE.read()==lb_i))
    BSEXT_SM.write(0xFFFFFF);
  else
    BSEXT_SM.write(0x0);
}

void memory::processHSEXT_SM()
{
  sc_uint<32> reddat_sm=REDDAT_SM.read();

  if ((reddat_sm[15]==1)&&(OPCOD_RE.read()==lh_i))
    HSEXT_SM.write(0xFFFF);
  else
    HSEXT_SM.write(0x0);
}

void memory::processDATA_SM()
{
  sc_uint<32> reddat_sm=REDDAT_SM.read();

  if ((OPCOD_RE.read()==lw_i)||
     (OPCOD_RE.read()==swap_i))
    DATA_SM.write(REDDAT_SM.read());
  else if ((OPCOD_RE.read()==lb_i)||
          (OPCOD_RE.read()==lbu_i))
    DATA_SM.write(BSEXT_SM.read() 
			<< 8 | reddat_sm.range(7,0));
  else if ((OPCOD_RE.read()==lh_i)||
          (OPCOD_RE.read()==lhu_i))
    DATA_SM.write(HSEXT_SM.read() 
			<< 16 | reddat_sm.range(15,0));
  else
    DATA_SM.write(RES_RE.read());
}

void memory::processDABUSER_XM()
{
  DABUSER_XM.write(!D_BERR_N.read());
}

void memory::processLAMALGN_XM()
{
  sc_uint<32> res_re=RES_RE.read();

  if ((I_WORD_SM.read()==1)&&(I_LOAD_SM.read()==1))
    LAMALGN_XM.write(res_re[1] | res_re[0]);
  else if ((I_HALF_SM.read()==1)&&(I_LOAD_SM.read()==1))
    LAMALGN_XM.write(res_re[0]);
  else
    LAMALGN_XM.write(0);
}

void memory::processSAMALGN_XM()
{
  sc_uint<32> res_re=RES_RE.read();

  if ((I_WORD_SM.read()==1)&&(I_STOR_SM.read()==1))
    SAMALGN_XM.write(res_re[1] | res_re[0]);
  else if ((I_HALF_SM.read()==1)&&(I_STOR_SM.read()==1))
    SAMALGN_XM.write(res_re[0]);
  else
    SAMALGN_XM.write(0);
}

void memory::processLASVIOL_XM()
{
  sc_uint<32> res_re=RES_RE.read();
  sc_uint<32> sr_re=SR_RE.read();

  if (I_LOAD_SM.read()==1)
    LASVIOL_XM.write(res_re[31] & sr_re[1]);
  else
    LASVIOL_XM.write(0);
}

void memory::processSASVIOL_XM()
{
  sc_uint<32> res_re=RES_RE.read();
  sc_uint<32> sr_re=SR_RE.read();

  if (I_STOR_SM.read()==1)
    SASVIOL_XM.write(res_re[31] & sr_re[1]);
  else
    SASVIOL_XM.write(0);
}

void memory::processBADDA_XM()
{
  BADDA_XM.write(SASVIOL_XM.read() | LASVIOL_XM.read() | LAMALGN_XM.read() | SAMALGN_XM.read());
}

void memory::processBADIA_XM()
{
  BADIA_XM.write(IASVIOL_RE.read() | IAMALGN_RE.read());
}

void memory::processCOPERR_XM()
{
  COPERR_XM.write(0x0);
}

void memory::processEXCCODE_XM()
{
  if (INTRQ_XX.read()==1)
    EXCCODE_XM.write(0);
  else if ((LAMALGN_XM.read() | LASVIOL_XM.read() | IAMALGN_RE.read() | IASVIOL_RE.read())==1)
    EXCCODE_XM.write(4);
  else if ((SAMALGN_XM.read() | SASVIOL_XM.read())==1)
    EXCCODE_XM.write(5);
  else if (IABUSER_RE.read()==1)
    EXCCODE_XM.write(6);
  else if (DABUSER_XM.read()==1)
    EXCCODE_XM.write(7);
  else if (SYSCALL_RE.read()==1)
    EXCCODE_XM.write(8);
  else if (BREAK_RE.read()==1)
    EXCCODE_XM.write(9);
  else if (ILLGINS_RE.read()==1)
    EXCCODE_XM.write(0xA);
  else if (C0UNUSE_RE.read()==1)
    EXCCODE_XM.write(0xB);
  else
    EXCCODE_XM.write(0xC);
}

void memory::processCAUSE_XM()
{
  sc_uint<32> cause_rx=CAUSE_RX.read();
  sc_uint<32> cause_xm=0x0;

  cause_xm[31]=BDSLOT_RE.read();
  cause_xm.range(29,28)=COPERR_XM.read();
  cause_xm.range(15,10)=IT_XX.read();
  cause_xm.range(9,8)=cause_rx.range(9,8);
  cause_xm.range(5,2)=EXCCODE_XM.read();
  CAUSE_XM.write(cause_xm);
}

void memory::processSWINT_XM()
{
  sc_uint<32> res_re=RES_RE.read();
  sc_uint<32> cause_rx=CAUSE_RX.read();

  if (((int)OPCOD_RE.read() == mtc0_i ) && ((int)COP0D_RE.read() == cause_s) && (KEEP_SM.read()== 0))
    SWINT_XM.write(res_re.range(9,8));
  else
    SWINT_XM.write(cause_rx.range(9,8));
}

void memory::processWCAUSE_SM()
{
  if (((int)OPCOD_RE.read() == mtc0_i ) && ((int)COP0D_RE.read() == cause_s))
    WCAUSE_SM.write(1);
  else
    WCAUSE_SM.write(0);
}

void memory::processCAUSE_SM()
{
  sc_uint<32> res_re=RES_RE.read();
  sc_uint<32> cause_rx=CAUSE_RX.read();

  CAUSE_SM.write(cause_rx.range(31,16) 
			<< 16 | IT_XX.read() 
			<< 10 | res_re.range(9,8) 
			<< 8 | cause_rx.range(7,0));
}

void memory::processRSTORSR_SM()
{
  sc_uint<32> nextsr_rx=CAUSE_RX.read();

  RSTORSR_SM.write(nextsr_rx.range(31,4) 
			<< 4 | nextsr_rx.range(5,2));
}

void memory::processNEXTSR_SM()
{
  if ((int)OPCOD_RE.read()==rfe_i)
    NEXTSR_SM.write(RSTORSR_SM);
  else
    NEXTSR_SM.write(RES_RE);
}

void memory::processWSR_SM()
{
  if (((int)OPCOD_RE.read()==rfe_i)||(((int)OPCOD_RE.read() == mtc0_i)&&((int)COP0D_RE.read() == status_s)))
    WSR_SM.write(1);
  else
    WSR_SM.write(0);
}

void memory::processEPC_XM()
{
  if (BDSLOT_RE.read()==0)
    EPC_XM.write(PC_RE.read());
  else
    EPC_XM.write(REDOPC_RE.read());
}

void memory::processLATEEX_XM()
{
  LATEEX_XM.write(DABUSER_XM.read()) ;
}

void memory::processEARLYEX_XM()
{
  EARLYEX_XM.write(ILLGINS_RE.read() | C0UNUSE_RE.read() |
                  IAMALGN_RE.read() | IASVIOL_RE.read() | IABUSER_RE.read() |
                  OVF_RE.read()     | BREAK_RE.read()   | SYSCALL_RE.read() |
                  LAMALGN_XM.read() | LASVIOL_XM.read() |
                  SAMALGN_XM.read() | SASVIOL_XM.read());
}

void memory::processEXCRQ_XM()
{
  EXCRQ_XM.write(EARLYEX_XM.read() | LATEEX_XM.read());
}

void memory::processSWINTRQ_XM()
{
  sc_uint<32> cause_rx=CAUSE_RX.read();
  SWINTRQ_XM.write(cause_rx.range(9,8));
}

void memory::processWEPC_XM()
{
  WEPC_XM.write(EXCRQ_XM.read());
}
