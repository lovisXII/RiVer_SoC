#include "i_cache.h"


void i_cache::processCK_SX()
{
  CK_SX.write(CK.read());
}

void i_cache::processADRTAG_SX()
{
  sc_uint<32> i_a=I_A.read();

  ADRTAG_SX.write(i_a.range(31,6));
}

void i_cache::processBLCNBR_SX()
{
  sc_uint<32> i_a=I_A.read();

  BLCNBR_SX.write(i_a.range(5,4));
}

void i_cache::processBLCOFS_SX()
{
  sc_uint<32> i_a=I_A.read();

  BLCOFS_SX.write(i_a.range(3,2));
}

void i_cache::processTAG_SX()
{
  if (BLCNBR_SX.read()==0x0)
    TAG_SX.write(TAG0_RX.read());
  else if (BLCNBR_SX.read()==0x1)
    TAG_SX.write(TAG1_RX.read());
  else if (BLCNBR_SX.read()==0x2)
    TAG_SX.write(TAG2_RX.read());
  else if (BLCNBR_SX.read()==0x3)
    TAG_SX.write(TAG3_RX.read());
}

void i_cache::processPRES_SX()
{
  if (BLCNBR_SX.read()==0x0)
    PRES_SX.write(PRES0_RX.read());
  else if (BLCNBR_SX.read()==0x1)
    PRES_SX.write(PRES1_RX.read());
  else if (BLCNBR_SX.read()==0x2)
    PRES_SX.write(PRES2_RX.read());
  else if (BLCNBR_SX.read()==0x3)
    PRES_SX.write(PRES3_RX.read());
}

void i_cache::processBLOC_SX()
{
  if (BLCNBR_SX.read()==0x0)
    BLOC_SX.write(BLOC0_RX.read());
  else if (BLCNBR_SX.read()==0x1)
    BLOC_SX.write(BLOC1_RX.read());
  else if (BLCNBR_SX.read()==0x2)
    BLOC_SX.write(BLOC2_RX.read());
  else if (BLCNBR_SX.read()==0x3)
    BLOC_SX.write(BLOC3_RX.read());
}

void i_cache::processDATA_SX()
{
  sc_biguint<128> bloc_sx=BLOC_SX.read();
  sc_uint<32> res_sx=0x0;

// .range(x,y) on a sc_biguint<> does not work properly
  if (BLCOFS_SX.read()==0x0)
    for (int i=31;i>=0;i--)
      res_sx[i]=bloc_sx[i+96];
  else if (BLCOFS_SX.read()==0x1)
    for (int i=31;i>=0;i--)
      res_sx[i]=bloc_sx[i+64];
  else if (BLCOFS_SX.read()==0x2)
    for (int i=31;i>=0;i--)
      res_sx[i]=bloc_sx[i+32];
  else if (BLCOFS_SX.read()==0x3)
    for (int i=31;i>=0;i--)
      res_sx[i]=bloc_sx[i];
  DATA_SX.write(res_sx);
}

void i_cache::processCMPTAGS_SX()
{
  CMPTAGS_SX.write(ADRTAG_SX.read() ^ TAG_SX.read());
}

void i_cache::processEQLTAGS_SX()
{
  EQLTAGS_SX.write((CMPTAGS_SX.read()==0x0)?1:0);
}

void i_cache::processDIRMISS_SX()
{
  DIRMISS_SX.write(((PRES_SX.read()==0)||(EQLTAGS_SX.read()==0))?I_FTCH.read():0);
}

void i_cache::processCMPUADR_SX()
{
  CMPUADR_SX.write(I_A.read() ^ UCACADR_RX.read());
}

void i_cache::processEQLUADR_SX()
{
  EQLUADR_SX.write((CMPUADR_SX.read()==0x0)?UCACVLD_RX.read():0);
}

void i_cache::processMSK0ADR_SX()
{
  MSK0ADR_SX.write(I_A.read() & u0_msk);
}

void i_cache::processUNCMISS_SX()
{
  UNCMISS_SX.write(((MSK0ADR_SX.read()==u0_adr)&&(EQLUADR_SX.read()==0))?I_FTCH.read():0);
}

void i_cache::processMISS_SX()
{
  MISS_SX.write(UNCMISS_SX.read() | DIRMISS_SX.read());
}

void i_cache::processRSTCNT_SX()
{
  sc_uint<2> rstcnt_sx=0x0;
  sc_uint<2> rstcnt_rx=RSTCNT_RX.read();
  sc_uint<2> rstcry_sx=RSTCRY_SX.read();

  rstcnt_sx.range(1,0)=rstcnt_rx.range(1,0) ^ rstcry_sx.range(1,0);
  RSTCNT_SX.write(rstcnt_sx);
}

void i_cache::processRSTCRY_SX()
{
  sc_uint<2> rstcnt_rx=RSTCNT_RX.read();
  sc_uint<3> rstcry_sx=RSTCRY_SX.read();

  rstcry_sx.range(2,1)=rstcnt_rx.range(1,0) & rstcry_sx.range(1,0);
  rstcry_sx[0]=1;
  RSTCRY_SX.write(rstcry_sx);
}

void i_cache::processENDRST_SX()
{
  sc_uint<3> rstcry_sx=RSTCRY_SX.read();

  ENDRST_SX.write(rstcry_sx[2]);
}

void i_cache::processRSTPRS()
{
  if ((C_STAT_RX.read() == c_rset) && (RSTCNT_RX.read()==0x0))
    RSTPRS0_SX.write(1);
  else
    RSTPRS0_SX.write(0);
  if ((C_STAT_RX.read() == c_rset) && (RSTCNT_RX.read()==0x1))
    RSTPRS1_SX.write(1);
  else
    RSTPRS1_SX.write(0);
  if ((C_STAT_RX.read() == c_rset) && (RSTCNT_RX.read()==0x2))
    RSTPRS2_SX.write(1);
  else
    RSTPRS2_SX.write(0);
  if ((C_STAT_RX.read() == c_rset) && (RSTCNT_RX.read()==0x3))
    RSTPRS3_SX.write(1);
  else
    RSTPRS3_SX.write(0);
}

void i_cache::processC_NXTS_SX()
{
  if (RESET_RX.read()==1)
    C_NXTS_SX.write(c_rset);
  else if ((C_STAT_RX.read() == c_rset) && (ENDRST_SX.read()==0))
    C_NXTS_SX.write(c_rset);
  else if ((C_STAT_RX.read() == c_rset) && (ENDRST_SX.read()==1))
    C_NXTS_SX.write(c_redy);
  else if ((C_STAT_RX.read() == c_redy) && (MISS_SX.read()==0))
    C_NXTS_SX.write(c_redy);
  else if ((C_STAT_RX.read() == c_uwat) && (ANSEMTY_SX.read()==0))
    C_NXTS_SX.write(c_redy);
  else if (C_STAT_RX.read() == c_mwrt)
    C_NXTS_SX.write(c_redy);
  else if ((C_STAT_RX.read() == c_redy) && (UNCMISS_SX.read()==1))
    C_NXTS_SX.write(c_uwat);
  else if ((C_STAT_RX.read() == c_uwat) && (ANSEMTY_SX.read()==1))
    C_NXTS_SX.write(c_uwat);
  else if ((C_STAT_RX.read() == c_redy) && (UNCMISS_SX.read()==0) && (DIRMISS_SX.read()==1))
    C_NXTS_SX.write(c_mwat);
  else if ((C_STAT_RX.read() == c_mwat) && (ANSEMTY_SX.read()==1))
    C_NXTS_SX.write(c_mwat);
  else if ((C_STAT_RX.read() == c_mwat) && (ANSEMTY_SX.read()==0) && (ANSFLAG_SX.read()==a_cnt))
    C_NXTS_SX.write(c_mwat);
  else if ((C_STAT_RX.read() == c_mwat) && (ANSEMTY_SX.read()==0) && (ANSFLAG_SX.read()==a_end))
    C_NXTS_SX.write(c_mwrt);
  else
    C_NXTS_SX.write(c_rset);
}

void i_cache::processANSPOP_SX()
{
  if ((C_STAT_RX.read()==c_uwat) && (ANSEMTY_SX.read()==0))
    ANSPOP_SX.write(1);
  else if ((C_STAT_RX.read()==c_mwat) && (ANSEMTY_SX.read()==0))
    ANSPOP_SX.write(1);
  else
    ANSPOP_SX.write(0);
}

void i_cache::processWRTUCAC_SX()
{
  if ((C_STAT_RX.read()==c_uwat) && (ANSEMTY_SX.read()==0))
    WRTUCAC_SX.write(1);
  else
    WRTUCAC_SX.write(0);
}

void i_cache::processWRTDATA_SX()
{
  if ((C_STAT_RX.read()==c_mwat) && (ANSEMTY_SX.read()==0))
    WRTDATA_SX.write(1);
  else
    WRTDATA_SX.write(0);
}

void i_cache::processWRTDAT_SX()
{
  sc_uint<32> ansadr_sx=ANSADR_SX.read();

  if (ansadr_sx.range(3,2)==0x0)
    WRTDAT0_SX.write(WRTDATA_SX.read());
  else
    WRTDAT0_SX.write(0);
  if (ansadr_sx.range(3,2)==0x1)
    WRTDAT1_SX.write(WRTDATA_SX.read());
  else
    WRTDAT1_SX.write(0);
  if (ansadr_sx.range(3,2)==0x2)
    WRTDAT2_SX.write(WRTDATA_SX.read());
  else
    WRTDAT2_SX.write(0);
  if (ansadr_sx.range(3,2)==0x3)
    WRTDAT3_SX.write(WRTDATA_SX.read());
  else
    WRTDAT3_SX.write(0);
}

void i_cache::processWRTADR_SX()
{
  if ((C_STAT_RX.read()==c_mwat) && (ANSEMTY_SX.read()==0) && (ANSFLAG_SX.read()==a_end))
    WRTADR_SX.write(1);
  else
    WRTADR_SX.write(0);
}

void i_cache::processWRTBLOC_SX()
{
  if (C_STAT_RX.read()==c_mwrt)
    WRTBLOC_SX.write(1);
  else
    WRTBLOC_SX.write(0);
}

void i_cache::processWRTBNBR_SX()
{
  sc_uint<32> rblocad_rx=RBLOCAD_RX.read();

  WRTBNBR_SX.write(rblocad_rx.range(5,4));
}

void i_cache::processWRTBTAG_SX()
{
  sc_uint<32> rblocad_rx=RBLOCAD_RX.read();

  WRTBTAG_SX.write(rblocad_rx.range(31,6));
}

void i_cache::processBLOCDIN_SX()
{
  sc_biguint<128> blocdin_sx=0x0;
  sc_uint<32> redblc0_rx=REDBLC0_RX.read();
  sc_uint<32> redblc1_rx=REDBLC1_RX.read();
  sc_uint<32> redblc2_rx=REDBLC2_RX.read();
  sc_uint<32> redblc3_rx=REDBLC3_RX.read();

// .range(x,y) on a sc_biguint<> does not work properly
  for (int i=31;i>=0;i--)
    blocdin_sx[i+96]=redblc0_rx[i];
  for (int i=31;i>=0;i--)
    blocdin_sx[i+64]=redblc1_rx[i];
  for (int i=31;i>=0;i--)
    blocdin_sx[i+32]=redblc2_rx[i];
  for (int i=31;i>=0;i--)
    blocdin_sx[i]=redblc3_rx[i];
  BLOCDIN_SX.write(blocdin_sx);
}

void i_cache::processRQSPSH_SX()
{
  if (C_STAT_RX.read()==c_redy)
    RQSPSH_SX.write(MISS_SX.read());
  else
    RQSPSH_SX.write(0);
}

void i_cache::processRQSDIN_SX()
{
  sc_biguint<39> rqsdin_sx=0x0;
  sc_uint<32> i_a=I_A.read();
  sc_uint<2> com=r_wrd;
  sc_uint<4> words=0;

  if (UNCMISS_SX.read()==1)
    words=0x0;
  else
    words=0x3;

// .range(x,y) on a sc_biguint<> does not work properly
  for (int i=31;i>=0;i--)
    rqsdin_sx[i+7]=i_a[i];
  rqsdin_sx[6]=r_red;
  for (int i=1;i>=0;i--)
    rqsdin_sx[i+4]=com[i];
  for (int i=3;i>=0;i--)
    rqsdin_sx[i]=words[i];

  RQSDIN_SX.write(rqsdin_sx);
}

void i_cache::processR_NXTS_SX()
{
  if (RESET_RX.read()==1)
    R_NXTS_SX.write(r_emty);
  else if ((R_STAT_RX.read() == r_emty) && (RQSPSH_SX.read()==0))
    R_NXTS_SX.write(r_emty);
  else if ((R_STAT_RX.read() == r_emty) && (RQSPSH_SX.read()==1))
    R_NXTS_SX.write(r_full);
  else if ((R_STAT_RX.read() == r_full) && (RQSPOP_SX.read()==0))
    R_NXTS_SX.write(r_full);
  else
    R_NXTS_SX.write(r_emty);
}

void i_cache::processRQSEMTY_SX()
{
  if (R_STAT_RX.read() == r_emty)
    RQSEMTY_SX.write(1);
  else
    RQSEMTY_SX.write(0);
}

void i_cache::processRQSFULL_SX()
{
  if (R_STAT_RX.read() == r_full)
    RQSFULL_SX.write(1);
  else
    RQSFULL_SX.write(0);
}

void i_cache::processRQSADR_SX()
{
  sc_uint<39> rqsreg0_rx=RQSREG0_RX.read();

  RQSADR_SX.write(rqsreg0_rx.range(38,7));
}

void i_cache::processRQSACSC_SX()
{
  sc_uint<39> rqsreg0_rx=RQSREG0_RX.read();

  RQSACSC_SX.write(rqsreg0_rx.range(3,0));
}

void i_cache::processWRTRQS0_SX()
{
  if ((R_STAT_RX.read()==r_emty) && (RQSPSH_SX.read()==1))
    WRTRQS0_SX.write(1);
  else
    WRTRQS0_SX.write(0);
}

void i_cache::processA_NXTS_SX()
{
  if (RESET_RX.read()==1)
    A_NXTS_SX.write(a_emty);
  else if ((A_STAT_RX.read() == a_emty) && (ANSPSH_SX.read()==0))
    A_NXTS_SX.write(a_emty);
  else if ((A_STAT_RX.read() == a_half) && (ANSPSH_SX.read()==0) && (ANSPOP_SX.read()==1))
    A_NXTS_SX.write(a_emty);

  else if ((A_STAT_RX.read() == a_emty) && (ANSPSH_SX.read()==1))
    A_NXTS_SX.write(a_half);
  else if ((A_STAT_RX.read() == a_half) && (ANSPSH_SX.read()==0) && (ANSPOP_SX.read()==0))
    A_NXTS_SX.write(a_half);
  else if ((A_STAT_RX.read() == a_half) && (ANSPSH_SX.read()==1) && (ANSPOP_SX.read()==1))
    A_NXTS_SX.write(a_half);
  else if ((A_STAT_RX.read() == a_full) && (ANSPOP_SX.read()==1))
    A_NXTS_SX.write(a_half);

  else if ((A_STAT_RX.read() == a_half) && (ANSPSH_SX.read()==1) && (ANSPOP_SX.read()==0))
    A_NXTS_SX.write(a_full);
  else if ((A_STAT_RX.read() == a_full) && (ANSPOP_SX.read()==0))
    A_NXTS_SX.write(a_full);
  else
    A_NXTS_SX.write(a_emty);
}

void i_cache::processANSEMTY_SX()
{
  if (A_STAT_RX.read() == a_emty)
    ANSEMTY_SX.write(1);
  else
    ANSEMTY_SX.write(0);
}

void i_cache::processANSFULL_SX()
{
  if (A_STAT_RX.read() == a_full)
    ANSFULL_SX.write(1);
  else
    ANSFULL_SX.write(0);
}

void i_cache::processANSDATA_SX()
{
  sc_biguint<65> ansreg0_rx=ANSREG0_RX.read();
  sc_uint<32> v_ansreg0_rx=0x0;

  for (int i=31;i>=0;i--)
    v_ansreg0_rx[i]=ansreg0_rx[i+33];
  ANSDATA_SX.write(v_ansreg0_rx);
}

void i_cache::processANSADR_SX()
{
  sc_biguint<65> ansreg0_rx=ANSREG0_RX.read();
  sc_uint<32> v_ansreg0_rx=0x0;

  for (int i=31;i>=0;i--)
    v_ansreg0_rx[i]=ansreg0_rx[i+1];
  ANSADR_SX.write(v_ansreg0_rx);
}

void i_cache::processANSFLAG_SX()
{
  sc_biguint<65> ansreg0_rx=ANSREG0_RX.read();

  ANSFLAG_SX.write((bool)ansreg0_rx[0]);
}

void i_cache::processWRTANS0_SX()
{
  if ((A_STAT_RX.read() == a_emty) && (ANSPSH_SX.read()==1))
    WRTANS0_SX.write(1);
  else if ((A_STAT_RX.read() == a_half) && (ANSPOP_SX.read()==1))
    WRTANS0_SX.write(1);
  else if ((A_STAT_RX.read() == a_full) && (ANSPOP_SX.read()==1))
    WRTANS0_SX.write(1);
  else
    WRTANS0_SX.write(0);
}

void i_cache::processWRTANS1_SX()
{
  if ((A_STAT_RX.read() == a_half) && (ANSPSH_SX.read()==1))
    WRTANS1_SX.write(1);
  else
    WRTANS1_SX.write(0);
}

void i_cache::processSELANS0_SX()
{
  if ((A_STAT_RX.read() == a_full) && (ANSPOP_SX.read()==1))
    SELANS0_SX.write(a_sft);
  else
    SELANS0_SX.write(a_new);
}

void i_cache::processP_NXTS_SX()
{
  if (RESET_RX.read()==1)
    P_NXTS_SX.write(p_idle);
  else if ((P_STAT_RX.read() == p_idle) && (RQSEMTY_SX.read()==1))
    P_NXTS_SX.write(p_idle);
  else if ((P_STAT_RX.read() == p_ldat) && (DATARDY_SX.read()==1))
    P_NXTS_SX.write(p_idle);

  else if ((P_STAT_RX.read() == p_idle) && (RQSEMTY_SX.read()==0) && (PI_GNT.read()==0))
    P_NXTS_SX.write(p_rqst);
  else if ((P_STAT_RX.read() == p_rqst) && (PI_GNT.read()==0))
    P_NXTS_SX.write(p_rqst);

  else if ((P_STAT_RX.read() == p_idle) && (RQSEMTY_SX.read()==0) && (PI_GNT.read()==1))
    P_NXTS_SX.write(p_fadr);
  else if ((P_STAT_RX.read() == p_rqst) && (PI_GNT.read()==1))
    P_NXTS_SX.write(p_fadr);

  else if ((P_STAT_RX.read() == p_fadr) && (MOREADR_SX.read()==1))
    P_NXTS_SX.write(p_nadr);
  else if ((P_STAT_RX.read() == p_nadr) && (MOREADR_SX.read()==1))
    P_NXTS_SX.write(p_nadr);

  else if ((P_STAT_RX.read() == p_fadr) && (MOREADR_SX.read()==0))
    P_NXTS_SX.write(p_ldat);
  else if ((P_STAT_RX.read() == p_nadr) && (MOREADR_SX.read()==0))
    P_NXTS_SX.write(p_ldat);
  else if ((P_STAT_RX.read() == p_ldat) && (DATARDY_SX.read()==0))
    P_NXTS_SX.write(p_ldat);

  else
    P_NXTS_SX.write(p_idle);
}

void i_cache::processDEFLTEN_SX()
{
// cout 
			<< name() << " lit " 
			<< PI_GNT.read() << " sur PI_GNT" 
			<< endl;
  DEFLTEN_SX.write(PI_GNT.read());
}

void i_cache::processPILINEN_SX()
{
  if ((P_STAT_RX.read() == p_fadr) || (P_STAT_RX.read() == p_nadr))
    PILINEN_SX.write(1);
  else if (DEFLTEN_SX.read() == 1)
    PILINEN_SX.write(1);
  else
    PILINEN_SX.write(0);
}

void i_cache::processANSPSH_SX()
{
  if ((P_STAT_RX.read() == p_nadr) && (DATARDY_SX.read() == 1))
    ANSPSH_SX.write(1);
  else if ((P_STAT_RX.read() == p_ldat) && (DATARDY_SX.read() == 1))
    ANSPSH_SX.write(1);
  else
    ANSPSH_SX.write(0);
}

void i_cache::processANSDIN_SX()
{
  sc_biguint<65> ansdin_sx=0x0;
  sc_uint<32> sendadr_rx=SENDADR_RX.read();
  sc_uint<32> pi_d=PI_D.read();

  if (P_STAT_RX.read() == p_nadr)
    ansdin_sx[0]=a_cnt;
  else
    ansdin_sx[0]=a_end;

  for (int i=31;i>=0;i--)
    ansdin_sx[i+1]=sendadr_rx[i];
  for (int i=31;i>=0;i--)
    ansdin_sx[i+33]=pi_d[i];
  ANSDIN_SX.write(ansdin_sx);
}

void i_cache::processINITACS_SX()
{
  if ((P_STAT_RX.read() == p_rqst) && (PI_GNT.read() == 1))
    INITACS_SX.write(1);
  else if ((P_STAT_RX.read() == p_idle) && (PI_GNT.read() == 1))
    INITACS_SX.write(1);
  else
    INITACS_SX.write(0);
}

void i_cache::processRQSPOP_SX()
{
  RQSPOP_SX.write(INITACS_SX.read());
}

void i_cache::processADRCNT_SX()
{
  sc_uint<32> adrcnt_rx=ADRCNT_RX.read();
  sc_uint<32> adrcry_sx=ADRCRY_SX.read();

  sc_uint<32> adrcnt_sx;

  for (int i=31;i>=0;i--)
    adrcnt_sx[i]=adrcnt_rx[i] ^ adrcry_sx[i];

//adrcnt_sx.range(31,0)=adrcnt_rx.range(31,0) ^ adrcry_sx.range(31,0);
  ADRCNT_SX.write(adrcnt_sx);
}

void i_cache::processADRCRY_SX()
{
  sc_uint<32> adrcry_sx=0;
  sc_uint<32> adrcnt_rx=ADRCNT_RX.read();

  adrcry_sx[2]=((P_STAT_RX.read()==p_nadr) && ((DATARDY_SX.read()==0) || (ANSFULL_SX.read()==1)))?0:1;
  adrcry_sx[3]=adrcnt_rx[2] & adrcry_sx[2];
//adrcry_sx.range(31,4)=0x0;
//adrcry_sx.range(1,0)=0x0;
  ADRCRY_SX.write(adrcry_sx);
}

void i_cache::processACSCNT_SX()
{
  sc_uint<4> acscnt_rx=ACSCNT_RX.read();

  if (!((P_STAT_RX.read()==p_nadr) && ((DATARDY_SX.read()==0) || (ANSFULL_SX.read()==1))))
    ACSCNT_SX.write(acscnt_rx-1);
}

/*
  void i_cache::processACSCRY_SX()
  {
  sc_uint<4> acscnt_rx=ACSCNT_RX.read();
  sc_uint<4> acscry_sx=ACSCRY_SX.read();

  ACSCNT_SX.write(~acscnt_rx.range(3,0) ^ acscry_sx.range(3,0));
  }
*/

void i_cache::processMOREADR_SX()
{
  if (ACSCNT_RX.read()==0x0)
    MOREADR_SX.write(0);
  else
    MOREADR_SX.write(1);
}

void i_cache::processDATARDY_SX()
{
  if ((PI_ACK.read()==p_ack_rdy) || (PI_ACK.read()==p_ack_rdm))
    DATARDY_SX.write(1);
  else
    DATARDY_SX.write(0);
}

void i_cache::processSNPBLC_SX()
{
  sc_uint<32> pi_a=PI_A.read();

  SNPBLC_SX.write(pi_a.range(5,4));
}

void i_cache::processSNPBTAG_SX()
{
  if (SNPBLC_SX.read()==0x0)
    SNPBTAG_SX.write(TAG0_RX.read());
  else if (SNPBLC_SX.read()==0x1)
    SNPBTAG_SX.write(TAG1_RX.read());
  else if (SNPBLC_SX.read()==0x2)
    SNPBTAG_SX.write(TAG2_RX.read());
  else if (SNPBLC_SX.read()==0x3)
    SNPBTAG_SX.write(TAG3_RX.read());
}

void i_cache::processSNPCMPT_SX()
{
  sc_uint<32> pi_a=PI_A.read();

  SNPCMPT_SX.write(SNPBTAG_SX.read() ^ pi_a.range(31,6));
}

void i_cache::processSNPINVL_SX()
{
  if ((SNPCMPT_SX.read()==0x0) && (PI_READ.read()==p_red_wrt))
    SNPINVL_SX.write(1);
  else
    SNPINVL_SX.write(0);
}

void i_cache::processINVPRS_SX()
{
  if (SNPBLC_SX.read()==0x0)
    INVPRS0_SX.write(SNPINVL_SX.read());
  else
    INVPRS0_SX.write(0);
  if (SNPBLC_SX.read()==0x1)
    INVPRS1_SX.write(SNPINVL_SX.read());
  else
    INVPRS1_SX.write(0);
  if (SNPBLC_SX.read()==0x2)
    INVPRS2_SX.write(SNPINVL_SX.read());
  else
    INVPRS2_SX.write(0);
  if (SNPBLC_SX.read()==0x3)
    INVPRS3_SX.write(SNPINVL_SX.read());
  else
    INVPRS3_SX.write(0);
}

void i_cache::processI_FRZ()
{
  if ((C_STAT_RX.read()==c_redy) && (MISS_SX.read()==0))
    I_FRZ.write(0);
  else
    I_FRZ.write(1);
}

void i_cache::processI()
{
  if (UCACVLD_RX.read()==1)
    I.write(UCACDAT_RX.read());
  else
    I.write(DATA_SX.read());
}

void i_cache::processPI_IREQ()
{
  if ((P_STAT_RX.read()==p_rqst) || (P_STAT_RX.read()==p_fadr))
    PI_IREQ.write(1);
  else
    PI_IREQ.write(0);
}

void i_cache::processPI_LINES()
{
  sc_uint<32> adrcnt_rx=ADRCNT_RX.read();

  if ((PILINEN_SX.read()==1)&&(CK_SX.read()==0)) //hack FPX fondamental pour que ca marche
    {
    //cout 
			<< name() << " ecrit " 
			<< ADRCNT_RX.read() << " sur PI_A" 
			<< endl;
    //PI_A.write(adrcnt_rx); // hack FP
      PI_A.write(adrcnt_rx.range(31,2)); // hack FP
      PI_READ.write(p_red_red);
      if (DEFLTEN_SX.read()==1)
        PI_LOCK.write(0);
      else if ((P_STAT_RX.read()==p_fadr)&&(MOREADR_SX.read()==0))
        PI_LOCK.write(0);
      else if ((P_STAT_RX.read()==p_nadr)&&(MOREADR_SX.read()==0))
        PI_LOCK.write(0);
      else
        PI_LOCK.write(1);
      if (DEFLTEN_SX.read()==1)
        PI_OPC.write(p_opc_nop);
      else if ((P_STAT_RX.read()==p_fadr)&&(ANSFULL_SX.read()==0))
        PI_OPC.write(p_opc_wdu);
      else if ((P_STAT_RX.read()==p_nadr)&&(ANSFULL_SX.read()==0))
        PI_OPC.write(p_opc_wdu);
      else
        PI_OPC.write(p_opc_nop);
    }
}


void i_cache::processCLOCK()
{
// ### ------------------------------------------------------ ###
// #   address and access counters				#
// ### ------------------------------------------------------ ###

//cout << "processCLOCK " 
			<< RQSADR_SX.read() << "," 
			<< ADRCNT_SX.read() 
			<< endl; 
  ADRCNT_RX.write((INITACS_SX.read()==1)?RQSADR_SX.read():ADRCNT_SX.read());
  ACSCNT_RX.write((INITACS_SX.read()==1)?RQSACSC_SX.read():ACSCNT_SX.read());

// ### ------------------------------------------------------ ###
// #   save the address sent on the Pi-bus			#
// ### ------------------------------------------------------ ###

  SENDADR_RX.write(ADRCNT_RX.read());

// ### ------------------------------------------------------ ###
// #   request fifo						#
// ### ------------------------------------------------------ ###

  RQSREG0_RX.write((WRTRQS0_SX.read()==1)?RQSDIN_SX.read():RQSREG0_RX.read());

// ### ------------------------------------------------------ ###
// #   answer fifo						#
// ### ------------------------------------------------------ ###

  if ((WRTANS0_SX.read()==1) && (SELANS0_SX.read()==a_new))
    ANSREG0_RX.write(ANSDIN_SX.read());
  else if ((WRTANS0_SX.read()==1) && (SELANS0_SX.read()==a_sft))
    ANSREG0_RX.write(ANSREG1_RX.read());
  if (WRTANS1_SX.read()==1)
    ANSREG1_RX.write(ANSDIN_SX.read());

// ### ------------------------------------------------------ ###
// #   state machines' state register				#
// ### ------------------------------------------------------ ###

  C_STAT_RX.write(C_NXTS_SX);
  R_STAT_RX.write(R_NXTS_SX);
  P_STAT_RX.write(P_NXTS_SX);
  A_STAT_RX.write(A_NXTS_SX);

// ### ------------------------------------------------------ ###
// #   uncachable data						#
// ### ------------------------------------------------------ ###

  if (WRTUCAC_SX.read()==1)
    UCACDAT_RX.write(ANSDATA_SX.read());
  if (WRTUCAC_SX.read()==1)
    UCACADR_RX.write(ANSADR_SX.read());
  if (RESET_RX.read()==1)
    UCACVLD_RX.write(0);
  else if (WRTUCAC_SX.read()==1)
    UCACVLD_RX.write(1);
  else if ((I_ACK.read()==1)&&(I_FTCH.read()==1))
    UCACVLD_RX.write(0);

// ### ------------------------------------------------------ ###
// #   fetched bloc						#
// ### ------------------------------------------------------ ###

  if (WRTADR_SX.read()==1)
    RBLOCAD_RX.write(ANSADR_SX.read());
  if (WRTDAT0_SX.read()==1)
    REDBLC0_RX.write(ANSDATA_SX.read());
  if (WRTDAT1_SX.read()==1)
    REDBLC1_RX.write(ANSDATA_SX.read());
  if (WRTDAT2_SX.read()==1)
    REDBLC2_RX.write(ANSDATA_SX.read());
  if (WRTDAT3_SX.read()==1)
    REDBLC3_RX.write(ANSDATA_SX.read());

// ### ------------------------------------------------------ ###
// #   update the cache						#
// ### ------------------------------------------------------ ###

  if ((WRTBLOC_SX.read()==1)&&(WRTBNBR_SX.read()==0x0))
    BLOC0_RX.write(BLOCDIN_SX.read());
  if ((WRTBLOC_SX.read()==1)&&(WRTBNBR_SX.read()==0x1))
    BLOC1_RX.write(BLOCDIN_SX.read());
  if ((WRTBLOC_SX.read()==1)&&(WRTBNBR_SX.read()==0x2))
    BLOC2_RX.write(BLOCDIN_SX.read());
  if ((WRTBLOC_SX.read()==1)&&(WRTBNBR_SX.read()==0x3))
    BLOC3_RX.write(BLOCDIN_SX.read());

  if ((WRTBLOC_SX.read()==1)&&(WRTBNBR_SX.read()==0x0))
    TAG0_RX.write(WRTBTAG_SX.read());
  if ((WRTBLOC_SX.read()==1)&&(WRTBNBR_SX.read()==0x1))
    TAG1_RX.write(WRTBTAG_SX.read());
  if ((WRTBLOC_SX.read()==1)&&(WRTBNBR_SX.read()==0x2))
    TAG2_RX.write(WRTBTAG_SX.read());
  if ((WRTBLOC_SX.read()==1)&&(WRTBNBR_SX.read()==0x3))
    TAG3_RX.write(WRTBTAG_SX.read());

  if ((RSTPRS0_SX.read()==1) || (INVPRS0_SX.read()==1))
    PRES0_RX.write(0);
  else if ((WRTBLOC_SX.read()==1) && (WRTBNBR_SX.read()==0x0))
    PRES0_RX.write(1);
  if ((RSTPRS1_SX.read()==1) || (INVPRS1_SX.read()==1))
    PRES1_RX.write(0);
  else if ((WRTBLOC_SX.read()==1) && (WRTBNBR_SX.read()==0x1))
    PRES1_RX.write(1);
  if ((RSTPRS2_SX.read()==1) || (INVPRS2_SX.read()==1))
    PRES2_RX.write(0);
  else if ((WRTBLOC_SX.read()==1) && (WRTBNBR_SX.read()==0x2))
    PRES2_RX.write(1);
  if ((RSTPRS3_SX.read()==1) || (INVPRS3_SX.read()==1))
    PRES3_RX.write(0);
  else if ((WRTBLOC_SX.read()==1) && (WRTBNBR_SX.read()==0x3))
    PRES3_RX.write(1);

// ### ------------------------------------------------------ ###
// #   miscellaneous:						#
// #     - reset register					#
// #     - reset counter					#
// ### ------------------------------------------------------ ###

  if (RESET_RX.read()==1)
    RSTCNT_RX.write(0x0);
  else
    RSTCNT_RX.write(RSTCNT_SX.read());

  RESET_RX.write(!RESET_N.read());
}

void i_cache::processI_BERR_N()
{
  I_BERR_N.write(1);
}
