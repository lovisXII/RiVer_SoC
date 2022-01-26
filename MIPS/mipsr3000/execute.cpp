#include "execute.h"


void execute::processI_EUSE_SE()
{
  sc_uint<25> i_type_rd=I_TYPE_RD.read();

  I_EUSE_SE.write(i_type_rd[19]);
}

void execute::processI_IFMT_SE()
{
  sc_uint<25> i_type_rd=I_TYPE_RD.read();

  I_IFMT_SE.write(i_type_rd[22]);
}

void execute::processI_READS_SE()
{
  sc_uint<25> i_type_rd=I_TYPE_RD.read();

  I_READS_SE.write(i_type_rd[18]);
}

void execute::processI_READT_SE()
{
  sc_uint<25> i_type_rd=I_TYPE_RD.read();

  I_READT_SE.write(i_type_rd[17]);
}

void execute::processI_OPER_SE()
{
  sc_uint<25> i_type_rd=I_TYPE_RD.read();

  I_OPER_SE.write(i_type_rd.range(15,13));
}

void execute::processI_LOGIC_SE()
{
  sc_uint<25> i_type_rd=I_TYPE_RD.read();

  I_LOGIC_SE.write(i_type_rd.range(12,11));
}

void execute::processI_SUB_SE()
{
  sc_uint<25> i_type_rd=I_TYPE_RD.read();

  I_SUB_SE.write(i_type_rd[11]);
}

void execute::processI_RIGHT_SE()
{
  sc_uint<25> i_type_rd=I_TYPE_RD.read();

  I_RIGHT_SE.write(i_type_rd[11]);
}

void execute::processI_RSGND_SE()
{
  sc_uint<25> i_type_rd=I_TYPE_RD.read();

  I_RSGND_SE.write(i_type_rd[10]);
}

void execute::processI_OVRF_SE()
{
  sc_uint<25> i_type_rd=I_TYPE_RD.read();

  I_OVRF_SE.write(i_type_rd[9]);
}

void execute::processI_MPDC_SE()
{
  sc_uint<25> i_type_rd=I_TYPE_RD.read();

  I_MPDC_SE.write(i_type_rd[8]);
}

void execute::processI_EPDC_SE()
{
  sc_uint<25> i_type_rd=I_TYPE_RD.read();

  I_EPDC_SE.write(i_type_rd[7]);
}

void execute::processI_WRITE_SE()
{
  sc_uint<25> i_type_rd=I_TYPE_RD.read();

  I_WRITE_SE.write(i_type_rd[8] | i_type_rd[7]);
}

void execute::processI_BRNCH_SE()
{
  sc_uint<25> i_type_rd=I_TYPE_RD.read();

  I_BRNCH_SE.write(i_type_rd[5]);
}

void execute::processXOPER_SE()
{
  sc_uint<32> sr_ri=SR_RI.read();

  if ((OPCOD_RD.read() == sll_i)||
     (OPCOD_RD.read() == srl_i)||
     (OPCOD_RD.read() == sra_i))
    XOPER_SE.write(IOPER_RD.read());
  else
    XOPER_SE.write(X_SE.read());
}

void execute::processYOPER_SE()
{
  YOPER_SE.write((I_IFMT_SE == 1) ? IOPER_RD.read() : Y_SE.read()) ;
}

void execute::processWREDOPC_SE()
{
  WREDOPC_SE.write(I_BRNCH_SE.read());
}

void execute::processIABUSER_XE()
{
  IABUSER_XE.write(!I_BERR_N.read());
}

void execute::processBREAK_XE()
{
  BREAK_XE.write((OPCOD_RD.read() == break_i) ? 1 : 0) ;
}

void execute::processSYSCALL_XE()
{
  SYSCALL_XE.write((OPCOD_RD.read() == syscall_i) ? 1 : 0) ;
}

void execute::processOVF_XE()
{
  OVF_XE.write(OVERFLW_SE.read() & I_OVRF_SE.read());
}

void execute::processIAMALGN_XE()
{
  sc_uint<32> nextpc_rd=NEXTPC_RD.read();

  IAMALGN_XE.write(nextpc_rd[1] | nextpc_rd[0]);
}

void execute::processIASVIOL_XE()
{
  sc_uint<32> nextpc_rd=NEXTPC_RD.read();
  sc_uint<32> nextsr_rx=NEXTSR_RX.read();

  if ((int)OPCOD_RD.read() == rfe_i)
    IASVIOL_XE.write(nextpc_rd[31] & nextsr_rx[3]);
  else
    IASVIOL_XE.write(nextpc_rd[31] & nextsr_rx[1]);
}

void execute::processCOPYCAP_SE()
{
  COPYCAP_SE.write(SWAP_RD.read());
}
