#include "decode.h"

void decode::processCOP0_SD()
{
  sc_uint<32> ir_ri = IR_RI.read();

  if (ir_ri[25]==0)
    COP0_SD.write((cop0_g << 6) | (ir_ri.range(22,21) << 3) | (ir_ri.range(24,23))) ;
  else
    COP0_SD.write((cop0_g << 6) | 0x20 | ir_ri.range(4,0));
}

void decode::processOPCOD_SD()
{
  sc_uint<32> ir_ri = IR_RI.read();

  if (ir_ri.range(31,26)==special_i)
    OPCOD_SD.write((special_g << 6) | ir_ri.range(5,0));
  else if (ir_ri.range(31,26)==bcond_i)
    OPCOD_SD.write((bcond_g << 5) | ir_ri.range(20,16));
  else if (ir_ri.range(31,26)==cop0_i)
    OPCOD_SD.write(COP0_SD.read());
  else
    OPCOD_SD.write((others_g << 6) | ir_ri.range(31,26));
}

void decode::processI_ILLG_SD()
{
  sc_uint<25> i_type_sd=I_TYPE_SD.read();

  I_ILLG_SD.write(i_type_sd[24]);
}

void decode::processI_JFMT_SD()
{
  sc_uint<25> i_type_sd=I_TYPE_SD.read();

  I_JFMT_SD.write(i_type_sd[23]);
}

void decode::processI_IFMT_SD()
{
  sc_uint<25> i_type_sd=I_TYPE_SD.read();

  I_IFMT_SD.write(i_type_sd[22]);
}

void decode::processI_RFMT_SD()
{
  sc_uint<25> i_type_sd=I_TYPE_SD.read();

  I_RFMT_SD.write(i_type_sd[21]);
}

void decode::processI_DUSE_SD()
{
  sc_uint<25> i_type_sd=I_TYPE_SD.read();

  I_DUSE_SD.write(i_type_sd[20]);
}

void decode::processI_READS_SD()
{
  sc_uint<25> i_type_sd=I_TYPE_SD.read();

  I_READS_SD.write(i_type_sd[18]);
}

void decode::processI_READT_SD()
{
  sc_uint<25> i_type_sd=I_TYPE_SD.read();

  I_READT_SD.write(i_type_sd[17]);
}

void decode::processI_OSGND_SD()
{
  sc_uint<25> i_type_sd=I_TYPE_SD.read();

  I_OSGND_SD.write(i_type_sd[16]);
}

void decode::processI_WRT31_SD()
{
  sc_uint<25> i_type_sd=I_TYPE_SD.read();

  I_WRT31_SD.write((i_type_sd[8] | i_type_sd[7]) & i_type_sd[6]);
}

void decode::processI_WRITE_SD()
{
  sc_uint<25> i_type_sd=I_TYPE_SD.read();

  I_WRITE_SD.write(i_type_sd[8] | i_type_sd[7]);
}

void decode::processI_BRNCH_SD()
{
  sc_uint<25> i_type_sd=I_TYPE_SD.read();

  I_BRNCH_SD.write(i_type_sd[5]);
}

void decode::processSWAP_SD()
{
  SWAP_SD.write(((int)OPCOD_SD.read() == swap_i) ? 1 : 0);
}

void decode::processCOP0D_SD()
{
  sc_uint<32> ir_ri=IR_RI.read();

  COP0D_SD.write(ir_ri.range(15,11));
}

void decode::processRS_SD()
{
  sc_uint<32> ir_ri=IR_RI.read();

  RS_SD.write(ir_ri.range(25,21));
}

void decode::processRT_SD()
{
  sc_uint<32> ir_ri=IR_RI.read();

  RT_SD.write(ir_ri.range(20,16));
}

void decode::processRD_SD()
{
  sc_uint<32> ir_ri=IR_RI.read();

  if (I_WRT31_SD.read() == 1)
    RD_SD.write(0x1F);
  else if ((I_WRITE_SD.read() & I_RFMT_SD.read()) == 1)
    RD_SD.write( ir_ri.range(15,11));
  else if ((I_WRITE_SD.read() & I_IFMT_SD.read()) == 1)
    RD_SD.write( ir_ri.range(20,16));
  else
    RD_SD.write( 0x0);
}

void decode::processCOP0OP_SD()
{
  sc_uint<32> ir_ri=IR_RI.read();

  if ((int)ir_ri.range(15,11)==badvadr_s)
    COP0OP_SD.write(BADVADR_RM.read());
  else if ((int)ir_ri.range(15,11)==status_s)
    COP0OP_SD.write(NEXTSR_RX.read());
  else if ((int)ir_ri.range(15,11)==epc_s)
    COP0OP_SD.write(EPC_RX.read());
  else if ((int)ir_ri.range(15,11)==cause_s)
    COP0OP_SD.write(CAUSE_RX.read());
  else
    COP0OP_SD.write(0x0);
}

void decode::processIMDSGN_SD()
{
  sc_uint<32> ir_ri=IR_RI.read();

  IMDSGN_SD.write(ir_ri[15]);
}

void decode::processIMDSEX_SD()
{
  sc_uint<32> ir_ri=IR_RI.read();

  if ((IMDSGN_SD.read() == 1) && (I_OSGND_SD.read() == 1))
    IMDSEX_SD.write(0xFFFF);
  else
    IMDSEX_SD.write(0x0);
}

void decode::processIOPER_SD()
{
  sc_uint<32> ir_ri=IR_RI.read();

  if (((int)OPCOD_SD.read() == bltzal_i)||
     ((int)OPCOD_SD.read() == jalr_i)||
     ((int)OPCOD_SD.read() == jal_i)||
     ((int)OPCOD_SD.read() == bgezal_i))
    IOPER_SD.write( SEQADR_SD.read());
  else if (((int)OPCOD_SD.read() == sra_i)||
          ((int)OPCOD_SD.read() == srl_i)||
          ((int)OPCOD_SD.read() == sll_i))
    IOPER_SD.write( ir_ri.range(13,6));
  else if ((int)OPCOD_SD.read() == lui_i)
    IOPER_SD.write( ir_ri.range(15,0)<<16);
  else if ((int)OPCOD_SD.read() == mflo_i)
    IOPER_SD.write(LO_RW.read());
  else if ((int)OPCOD_SD.read() == mfhi_i)
    IOPER_SD.write(HI_RW.read());
  else
    IOPER_SD.write(IMDSEX_SD.read() << 16 | ir_ri.range(15,0));
}

void decode::processOFFSET_SD()
{
  sc_uint<32> imdsex_sd=IMDSEX_SD.read();
  sc_uint<32> ir_ri=IR_RI.read();

  OFFSET_SD.write(imdsex_sd.range(13,0) << 18 | ir_ri.range(15,0) << 2);
}

// ### ------------------------------------------------------ ###
// #   conditional branches' condition				#
// ### ------------------------------------------------------ ###

void decode::processS_CMP_T_SD()
{
  S_CMP_T_SD.write(SOPER_SD.read() ^ TOPER_SD.read());
}

void decode::processS_EQ_T_SD()
{
  S_EQ_T_SD.write((S_CMP_T_SD.read()==0x0)?1:0);
}

void decode::processS_LT_Z_SD()
{
  sc_uint<32> soper_sd=SOPER_SD.read();

  S_LT_Z_SD.write((soper_sd[31]==1)?1:0);
}

void decode::processS_LE_Z_SD()
{
  sc_uint<32> soper_sd=SOPER_SD.read();

  S_LE_Z_SD.write(((soper_sd[31]==1)||(soper_sd==0x0))?1:0);
}

void decode::processSLEEP_SD()
{
  SLEEP_SD.write(((int)OPCOD_SD.read() == sleep_i) ? 1 : 0);
}

void decode::processNEXTPC_SD()
{
  if (((int)OPCOD_SD.read() == jr_i)||((int)OPCOD_SD.read() == jalr_i))
    NEXTPC_SD.write(SOPER_SD.read());
  else if (((int)OPCOD_SD.read() == j_i)||((int)OPCOD_SD.read() == jal_i))
    NEXTPC_SD.write(JMPADR_SD.read());
  else if ((((int)OPCOD_SD.read() == beq_i)&&(S_EQ_T_SD.read() == 1))||
           (((int)OPCOD_SD.read() == bne_i)&&(S_EQ_T_SD.read() == 0))||
           (((int)OPCOD_SD.read() == bltz_i)&&(S_LT_Z_SD.read() == 1))||
           (((int)OPCOD_SD.read() == bltzal_i)&&(S_LT_Z_SD.read() == 1))||
           (((int)OPCOD_SD.read() == blez_i)&&(S_LE_Z_SD.read() == 1))||
           (((int)OPCOD_SD.read() == bgtz_i)&&(S_LE_Z_SD.read() == 0))||
           (((int)OPCOD_SD.read() == bgez_i)&&(S_LT_Z_SD.read() == 0))||
           (((int)OPCOD_SD.read() == bgezal_i)&&(S_LT_Z_SD.read() == 0))
          )
    NEXTPC_SD.write(BRAADR_SD.read());
  else
    NEXTPC_SD.write(SEQADR_SD.read());
}

void decode::processILLGINS_XD()
{
  ILLGINS_XD.write(I_ILLG_SD.read());
}

void decode::processC0UNUSE_XD()
{
  sc_uint<32> sr_ri=SR_RI.read();

  if (((int)OPCOD_SD.read() == rfe_i)||
     ((int)OPCOD_SD.read() == mfc0_i)||
     ((int)OPCOD_SD.read() == mtc0_i))
    C0UNUSE_XD.write(sr_ri[1] & (!sr_ri[28]));
  else
    C0UNUSE_XD.write(0);
}

void decode::processJMPADR_SD()
{
  sc_uint<32> nextpc_rd=NEXTPC_RD.read();
  sc_uint<32> ir_ri = IR_RI.read();
  sc_uint<32> jres = 0x0;

  jres.range(31,28)=nextpc_rd.range(31,28);
  jres.range(27,2)=ir_ri.range(25,0);
  JMPADR_SD.write(jres);
}

void decode::processBRAADR_SD()
{
  BRAADR_SD.write(NEXTPC_RD.read() + OFFSET_SD.read());
}

void decode::processSEQADR_SD()
{
  SEQADR_SD.write(NEXTPC_RD.read() + 4);
}
