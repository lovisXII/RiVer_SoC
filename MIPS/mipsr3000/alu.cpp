#include "alu.h"

void alu::processXARITH_SE()
{
  XARITH_SE.write(XOPER_SE.read());
}

void alu::processYARITH_SE()
{
  if (I_SUB_SE.read()==0)
    YARITH_SE.write(YOPER_SE.read());
  else
    YARITH_SE.write(!YOPER_SE.read());
}

void alu::processRARITH_SE()
{
  sc_uint<32> carith_se=CARITH_SE.read();

  RARITH_SE.write(XARITH_SE.read() ^ carith_se.range(31,0) ^ YARITH_SE.read());
}

void alu::processCARITH_SE()
{
  sc_uint<33> r33;
  sc_uint<32> yarith_se=YARITH_SE.read();
  sc_uint<32> carith_se=CARITH_SE.read();

  r33.range(32,1)= (XARITH_SE.read() & yarith_se.range(31,0)) |
    (XARITH_SE.read() & carith_se.range(31,0)) |
    (YARITH_SE.read() & carith_se.range(31,0)) ;
  r33.range(0,0)= I_SUB_SE.read();
  CARITH_SE.write(r33);
}

void alu::processOVERFLW_SE()
{
  sc_uint<33> carith_se=CARITH_SE.read();

  OVERFLW_SE.write(carith_se[32] ^ carith_se[31]);
}

void alu::processS_LT_T_SE()
{
  sc_uint<32> rarith_se=RARITH_SE.read();

  if ((rarith_se[31] ^ OVERFLW_SE.read())==1)
    S_LT_T_SE.write(1);
  else
    S_LT_T_SE.write(0);
}

void alu::processS_LTU_T_SE()
{
  sc_uint<33> carith_se=CARITH_SE.read();

  if (carith_se[32]==1)
    S_LTU_T_SE.write(1);
  else
    S_LTU_T_SE.write(0);
}

void alu::processSETBIT_SE()
{
  if (((int)OPCOD_RD.read()==slt_i)||((int)OPCOD_RD.read()==slti_i))
    SETBIT_SE.write(S_LT_T_SE.read());
  else if (((int)OPCOD_RD.read()==sltu_i)||((int)OPCOD_RD.read()==sltui_i))
    SETBIT_SE.write(S_LTU_T_SE.read());
  else
    SETBIT_SE.write(0);
}

void alu::processRTEST_SE()
{
  RTEST_SE.write(SETBIT_SE.read());
}

void alu::processSHIFTIN_SE()
{
  sc_uint<32> yoper_se=YOPER_SE.read();

  if ((I_RIGHT_SE.read() & I_RSGND_SE.read() & yoper_se[31])==1)
    SHIFTIN_SE.write(0xFFFFFFFF);
  else
    SHIFTIN_SE.write(0x0);
}

void alu::processSHLEFT_SE()
{
  sc_uint<32> xoper_se=XOPER_SE.read();
  sc_uint<32> shiftin_se=SHIFTIN_SE.read();
  sc_uint<32> yoper_se=YOPER_SE.read();
  int sh=(int)xoper_se.range(4,0);
  sc_uint<32> shleft_se;

  switch(sh)
    {
      case 0:  shleft_se.range(31,0) =yoper_se.range(31,0); break;
      case 1:  shleft_se.range(31,1) =yoper_se.range(30,0); shleft_se.range( 0,0)=shiftin_se.range(31,31); break;
      case 2:  shleft_se.range(31,2) =yoper_se.range(29,0); shleft_se.range( 1,0)=shiftin_se.range(31,30); break;
      case 3:  shleft_se.range(31,3) =yoper_se.range(28,0); shleft_se.range( 2,0)=shiftin_se.range(31,29); break;
      case 4:  shleft_se.range(31,4) =yoper_se.range(27,0); shleft_se.range( 3,0)=shiftin_se.range(31,28); break;
      case 5:  shleft_se.range(31,5) =yoper_se.range(26,0); shleft_se.range( 4,0)=shiftin_se.range(31,27); break;
      case 6:  shleft_se.range(31,6) =yoper_se.range(25,0); shleft_se.range( 5,0)=shiftin_se.range(31,26); break;
      case 7:  shleft_se.range(31,7) =yoper_se.range(24,0); shleft_se.range( 6,0)=shiftin_se.range(31,25); break;
      case 8:  shleft_se.range(31,8) =yoper_se.range(23,0); shleft_se.range( 7,0)=shiftin_se.range(31,24); break;
      case 9:  shleft_se.range(31,9) =yoper_se.range(22,0); shleft_se.range( 8,0)=shiftin_se.range(31,23); break;
      case 10: shleft_se.range(31,10)=yoper_se.range(21,0); shleft_se.range( 9,0)=shiftin_se.range(31,22); break;
      case 11: shleft_se.range(31,11)=yoper_se.range(20,0); shleft_se.range(10,0)=shiftin_se.range(31,21); break;
      case 12: shleft_se.range(31,12)=yoper_se.range(19,0); shleft_se.range(11,0)=shiftin_se.range(31,20); break;
      case 13: shleft_se.range(31,13)=yoper_se.range(18,0); shleft_se.range(12,0)=shiftin_se.range(31,19); break;
      case 14: shleft_se.range(31,14)=yoper_se.range(17,0); shleft_se.range(13,0)=shiftin_se.range(31,18); break;
      case 15: shleft_se.range(31,15)=yoper_se.range(16,0); shleft_se.range(14,0)=shiftin_se.range(31,17); break;
      case 16: shleft_se.range(31,16)=yoper_se.range(15,0); shleft_se.range(15,0)=shiftin_se.range(31,16); break;
      case 17: shleft_se.range(31,17)=yoper_se.range(14,0); shleft_se.range(16,0)=shiftin_se.range(31,15); break;
      case 18: shleft_se.range(31,18)=yoper_se.range(13,0); shleft_se.range(17,0)=shiftin_se.range(31,14); break;
      case 19: shleft_se.range(31,19)=yoper_se.range(12,0); shleft_se.range(18,0)=shiftin_se.range(31,13); break;
      case 20: shleft_se.range(31,20)=yoper_se.range(11,0); shleft_se.range(19,0)=shiftin_se.range(31,12); break;
      case 21: shleft_se.range(31,21)=yoper_se.range(10,0); shleft_se.range(20,0)=shiftin_se.range(31,11); break;
      case 22: shleft_se.range(31,22)=yoper_se.range( 9,0); shleft_se.range(21,0)=shiftin_se.range(31,10); break;
      case 23: shleft_se.range(31,23)=yoper_se.range( 8,0); shleft_se.range(22,0)=shiftin_se.range(31, 9); break;
      case 24: shleft_se.range(31,24)=yoper_se.range( 7,0); shleft_se.range(23,0)=shiftin_se.range(31, 8); break;
      case 25: shleft_se.range(31,25)=yoper_se.range( 6,0); shleft_se.range(24,0)=shiftin_se.range(31, 7); break;
      case 26: shleft_se.range(31,26)=yoper_se.range( 5,0); shleft_se.range(25,0)=shiftin_se.range(31, 6); break;
      case 27: shleft_se.range(31,27)=yoper_se.range( 4,0); shleft_se.range(26,0)=shiftin_se.range(31, 5); break;
      case 28: shleft_se.range(31,28)=yoper_se.range( 3,0); shleft_se.range(27,0)=shiftin_se.range(31, 4); break;
      case 29: shleft_se.range(31,29)=yoper_se.range( 2,0); shleft_se.range(28,0)=shiftin_se.range(31, 3); break;
      case 30: shleft_se.range(31,30)=yoper_se.range( 1,0); shleft_se.range(29,0)=shiftin_se.range(31, 2); break;
      case 31: shleft_se.range(31,31)=yoper_se.range( 0,0); shleft_se.range(30,0)=shiftin_se.range(31, 1); break;
    }
  SHLEFT_SE.write(shleft_se);
}

void alu::processSHRIGHT_SE()
{
  sc_uint<32> xoper_se=XOPER_SE.read();
  sc_uint<32> shiftin_se=SHIFTIN_SE.read();
  sc_uint<32> yoper_se=YOPER_SE.read();
  int sh=(int)xoper_se.range(4,0);
  sc_uint<32> shright_se;

  switch(sh)
    {
      case 0:  shright_se.range(31,0) =yoper_se.range(31,0); break;
      case 1:  shright_se.range(31,31)=shiftin_se.range(31,31); shright_se.range(30,0)=yoper_se.range(31, 1); break;
      case 2:  shright_se.range(31,30)=shiftin_se.range(31,30); shright_se.range(29,0)=yoper_se.range(31, 2); break;
      case 3:  shright_se.range(31,29)=shiftin_se.range(31,29); shright_se.range(28,0)=yoper_se.range(31, 3); break;
      case 4:  shright_se.range(31,28)=shiftin_se.range(31,28); shright_se.range(27,0)=yoper_se.range(31, 4); break;
      case 5:  shright_se.range(31,27)=shiftin_se.range(31,27); shright_se.range(26,0)=yoper_se.range(31, 5); break;
      case 6:  shright_se.range(31,26)=shiftin_se.range(31,26); shright_se.range(25,0)=yoper_se.range(31, 6); break;
      case 7:  shright_se.range(31,25)=shiftin_se.range(31,25); shright_se.range(24,0)=yoper_se.range(31, 7); break;
      case 8:  shright_se.range(31,24)=shiftin_se.range(31,24); shright_se.range(23,0)=yoper_se.range(31, 8); break;
      case 9:  shright_se.range(31,23)=shiftin_se.range(31,23); shright_se.range(22,0)=yoper_se.range(31, 9); break;
      case 10: shright_se.range(31,22)=shiftin_se.range(31,22); shright_se.range(21,0)=yoper_se.range(31,10); break;
      case 11: shright_se.range(31,21)=shiftin_se.range(31,21); shright_se.range(20,0)=yoper_se.range(31,11); break;
      case 12: shright_se.range(31,20)=shiftin_se.range(31,20); shright_se.range(19,0)=yoper_se.range(31,12); break;
      case 13: shright_se.range(31,19)=shiftin_se.range(31,19); shright_se.range(18,0)=yoper_se.range(31,13); break;
      case 14: shright_se.range(31,18)=shiftin_se.range(31,18); shright_se.range(17,0)=yoper_se.range(31,14); break;
      case 15: shright_se.range(31,17)=shiftin_se.range(31,17); shright_se.range(16,0)=yoper_se.range(31,15); break;
      case 16: shright_se.range(31,16)=shiftin_se.range(31,16); shright_se.range(15,0)=yoper_se.range(31,16); break;
      case 17: shright_se.range(31,15)=shiftin_se.range(31,15); shright_se.range(14,0)=yoper_se.range(31,17); break;
      case 18: shright_se.range(31,14)=shiftin_se.range(31,14); shright_se.range(13,0)=yoper_se.range(31,18); break;
      case 19: shright_se.range(31,13)=shiftin_se.range(31,13); shright_se.range(12,0)=yoper_se.range(31,19); break;
      case 20: shright_se.range(31,12)=shiftin_se.range(31,12); shright_se.range(11,0)=yoper_se.range(31,20); break;
      case 21: shright_se.range(31,11)=shiftin_se.range(31,11); shright_se.range(10,0)=yoper_se.range(31,21); break;
      case 22: shright_se.range(31,10)=shiftin_se.range(31,10); shright_se.range( 9,0)=yoper_se.range(31,22); break;
      case 23: shright_se.range(31, 9)=shiftin_se.range(31, 9); shright_se.range( 8,0)=yoper_se.range(31,23); break;
      case 24: shright_se.range(31, 8)=shiftin_se.range(31, 8); shright_se.range( 7,0)=yoper_se.range(31,24); break;
      case 25: shright_se.range(31, 7)=shiftin_se.range(31, 7); shright_se.range( 6,0)=yoper_se.range(31,25); break;
      case 26: shright_se.range(31, 6)=shiftin_se.range(31, 6); shright_se.range( 5,0)=yoper_se.range(31,26); break;
      case 27: shright_se.range(31, 5)=shiftin_se.range(31, 5); shright_se.range( 4,0)=yoper_se.range(31,27); break;
      case 28: shright_se.range(31, 4)=shiftin_se.range(31, 4); shright_se.range( 3,0)=yoper_se.range(31,28); break;
      case 29: shright_se.range(31, 3)=shiftin_se.range(31, 3); shright_se.range( 2,0)=yoper_se.range(31,29); break;
      case 30: shright_se.range(31, 2)=shiftin_se.range(31, 2); shright_se.range( 1,0)=yoper_se.range(31,30); break;
      case 31: shright_se.range(31, 1)=shiftin_se.range(31, 1); shright_se.range( 0,0)=yoper_se.range(31,31); break;
    }
  SHRIGHT_SE.write(shright_se);
}

void alu::processRSHIFT_SE()
{
  if (I_RIGHT_SE.read()==1)
    RSHIFT_SE.write(SHRIGHT_SE.read());
  else
    RSHIFT_SE.write(SHLEFT_SE.read());
}

void alu::processRLOGIC_SE()
{
  if (I_LOGIC_SE.read()==and_o)
    RLOGIC_SE.write(XOPER_SE.read() & YOPER_SE.read());
  else if (I_LOGIC_SE.read()==or_o)
    RLOGIC_SE.write(XOPER_SE.read() | YOPER_SE.read());
  else if (I_LOGIC_SE.read()==nor_o)
    RLOGIC_SE.write(!(XOPER_SE.read() | YOPER_SE.read()));
  else
    RLOGIC_SE.write(XOPER_SE.read() ^ YOPER_SE.read());
}

void alu::processRES_SE()
{
  if (I_OPER_SE.read()==arith_o)
    RES_SE.write(RARITH_SE.read());
  else if (I_OPER_SE.read()==logic_o)
    RES_SE.write(RLOGIC_SE.read());
  else if (I_OPER_SE.read()==shift_o)
    RES_SE.write(RSHIFT_SE.read());
  else if (I_OPER_SE.read()==test_o)
    RES_SE.write(RTEST_SE.read());
  else if (I_OPER_SE.read()==toper_o)
    RES_SE.write(Y_SE.read());
  else if (I_OPER_SE.read()==soper_o)
    RES_SE.write(X_SE.read());
  else
    RES_SE.write(IOPER_RD.read());
}
