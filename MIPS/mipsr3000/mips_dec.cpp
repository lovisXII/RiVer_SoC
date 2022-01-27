#include "mips_dec.h"


void mips_dec::processWRITE()
{
  if (D_ATYPE.read()==0x1)
    WRITE.write(0);
  else
    WRITE.write(1);
}

void mips_dec::processW_N()
{
  if (D_ATYPE.read()==0x1)
    W_N.write(1);
  else
    W_N.write(0);
}

void mips_dec::processDAT()
{
  if ((CK.read()==0)&&(WRITE.read()==1)&&(D_RQ.read()==1))
    {
      sc_uint<32> d_out=D_OUT.read();
      if (D_ATYPE.read()==0)
        DAT.write(d_out.range(31,0));
      else if (D_ATYPE.read()==2)
        DAT.write(d_out.range(15,0)<<16 | d_out.range(15,0));
      else
        DAT.write(d_out.range(7,0)<<24 | d_out.range(7,0)<<16 |
                 d_out.range(7,0)<<8 | d_out.range(7,0));

    }
}

void mips_dec::processBYTE_SEL()
{
  sc_uint<32> d_a=D_A.read();

  if ((D_ATYPE.read()==0x3)&&(d_a.range(1,0)==0x0))
    BYTE_SEL.write(0x1);
  else if ((D_ATYPE.read()==0x3)&&(d_a.range(1,0)==0x1))
    BYTE_SEL.write(0x2);
  else if ((D_ATYPE.read()==0x3)&&(d_a.range(1,0)==0x2))
    BYTE_SEL.write(0x4);
  else if ((D_ATYPE.read()==0x3)&&(d_a.range(1,0)==0x3))
    BYTE_SEL.write(0x8);
  else if ((D_ATYPE.read()==0x2)&&(d_a[1]==0x0))
    BYTE_SEL.write(0x3);
  else if ((D_ATYPE.read()==0x2)&&(d_a[1]==0x1))
    BYTE_SEL.write(0xC);
  else
    BYTE_SEL.write(0xF);
}

void mips_dec::processBYTE()
{
  if (D_RQ.read()==1)
    BYTE.write(BYTE_SEL.read());
  else
    BYTE.write(0x0);
}

void mips_dec::processROMU()
{
  sc_uint<32> i_adr=I_ADR.read();

  if ((int)i_adr.range(31,12)==0x00400)
    ROMU.write(1);
  else
    ROMU.write(0);
}

void mips_dec::processROMX()
{
  sc_uint<32> i_adr=I_ADR.read();

  if ((int)i_adr.range(31,12)==0x80000)
    ROMX.write(1);
  else
    ROMX.write(0);
}

void mips_dec::processROMR()
{
  sc_uint<32> i_adr=I_ADR.read();

  if ((int)i_adr.range(31,12)==0xBFC00)
    ROMR.write(1);
  else
    ROMR.write(0);
}

void mips_dec::processRAMU()
{
  sc_uint<32> d_a=D_A.read();

  if ((int)d_a.range(31,12)==0x00000)
    RAMU.write(1);
  else
    RAMU.write(0);
}

void mips_dec::processRAMS()
{
  sc_uint<32> d_a=D_A.read();

  if ((int)d_a.range(31,12)==0xC0000)
    RAMS.write(1);
  else
    RAMS.write(0);
}

void mips_dec::processSEL_ROMU()
{
  SEL_ROMU.write(ROMU.read() & !CK.read() & I_FTCH.read() & !(I_FRZ.read()));
}

void mips_dec::processSEL_ROMR()
{
  SEL_ROMR.write(ROMR.read() & !CK.read() & I_FTCH.read() & !(I_FRZ.read()));
}

void mips_dec::processSEL_ROMX()
{
  SEL_ROMX.write(ROMX.read() & !CK.read() & I_FTCH.read() & !(I_FRZ.read()));
}

void mips_dec::processE_RAMU()
{
  sc_uint<4> rbyte=BYTE.read();
  sc_uint<4> sel_ramu=0x0;

  sel_ramu[0]=RAMU.read() & !CK.read() & rbyte[0] & !D_FRZ.read(); 
  sel_ramu[1]=RAMU.read() & !CK.read() & rbyte[1] & !D_FRZ.read(); 
  sel_ramu[2]=RAMU.read() & !CK.read() & rbyte[2] & !D_FRZ.read(); 
  sel_ramu[3]=RAMU.read() & !CK.read() & rbyte[3] & !D_FRZ.read(); 
  E_RAMU.write(sel_ramu);
}

void mips_dec::processE_RAMS()
{
  sc_uint<4> rbyte=BYTE.read();
  sc_uint<4> sel_rams=0x0;

  sel_rams[0]=RAMS.read() & !CK.read() & rbyte[0] & !D_FRZ.read(); 
  sel_rams[1]=RAMS.read() & !CK.read() & rbyte[1] & !D_FRZ.read(); 
  sel_rams[2]=RAMS.read() & !CK.read() & rbyte[2] & !D_FRZ.read(); 
  sel_rams[3]=RAMS.read() & !CK.read() & rbyte[3] & !D_FRZ.read(); 
  E_RAMS.write(sel_rams);
}

void mips_dec::processI_BERR_N()
{
  I_BERR_N.write(!(!ROMU.read() & !ROMR.read() & !ROMX.read() &
                  !CK.read() & I_FTCH.read() & !I_FRZ.read()));
}

void mips_dec::processD_BERR_N()
{
  D_BERR_N.write(!(!RAMU.read() & !RAMS.read() &
                  !CK.read() & D_RQ.read() & !D_FRZ.read()));
}

void mips_dec::processI_A_11_2()
{
  sc_uint<32> i_adr=I_ADR.read();

  I_A_11_2.write(i_adr.range(11,2));
}

void mips_dec::processD_A_11_2()
{
  sc_uint<32> d_a=D_A.read();

  D_A_11_2.write(d_a.range(11,2));
}

void mips_dec::processDATA_TO_MIPS()
{
  if (SEL_ROMU.read()==1)
    DATA_TO_MIPS.write(DAT_ROMU.read());
  else if (SEL_ROMR.read()==1)
    DATA_TO_MIPS.write(DAT_ROMR.read());
  else if (SEL_ROMX.read()==1)
    DATA_TO_MIPS.write(DAT_ROMX.read());
  else
    DATA_TO_MIPS.write(0x0);
}

void mips_dec::processD_IN()
{
  D_IN.write(DAT.read());
}
