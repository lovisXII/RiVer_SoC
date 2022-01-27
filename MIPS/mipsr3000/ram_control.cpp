#include "ram_control.h"




void ram_control::processCK_SX()
{
  CK_SX.write(CK);
}

void ram_control::processMODE_SX()
{
  sc_uint<4> opcode_rx=OPCODE_RX.read();

  MODE_SX.write(opcode_rx.range(3,2));
}

void ram_control::processBYTE_SX()
{
  if (OPCODE_RX.read()==p_opc_wdu)
    BYTE_SX.write(0xF);
  else if (OPCODE_RX.read()==p_opc_hw0)
    BYTE_SX.write(0x3);
  else if (OPCODE_RX.read()==p_opc_hw1)
    BYTE_SX.write(0xC);
  else if (OPCODE_RX.read()==p_opc_by0)
    BYTE_SX.write(0x1);
  else if (OPCODE_RX.read()==p_opc_by1)
    BYTE_SX.write(0x2);
  else if (OPCODE_RX.read()==p_opc_by2)
    BYTE_SX.write(0x4);
  else if (OPCODE_RX.read()==p_opc_by3)
    BYTE_SX.write(0x8);
  else
    BYTE_SX.write(0x0);
}

void ram_control::processRAM_ADR()
{
  sc_uint<32> address_rx=ADDRESS_RX.read();

  unsigned int t=(int)address_rx;
  printf ("RAM_ADR address_rx=%x\n",t);
//RAM_ADR.write(address_rx.range(11,2));
  RAM_ADR.write(address_rx.range(9,0));
}

void ram_control::processRAM_W_N()
{
  RAM_W_N.write(READ_RX.read());
}

void ram_control::processRAM_E()
{
  if ((SELECT_RX.read()==1) && (CK_SX.read()==0))
    RAM_E.write(BYTE_SX.read());
  else
    RAM_E.write(0x0);
}

void ram_control::processRAM_D()
{
  sc_uint<32> pi_d=PI_D.read();
  sc_uint<32> res=0x0;

  if ((READ_RX.read()==0) && (CK_SX.read()==0))
    {
      if (MODE_SX.read()==m_word)
        RAM_D.write(pi_d);
      else if (MODE_SX.read()==m_half)
        {
          res.range(31,16)=pi_d.range(15,0);	
          res.range(15,0)=pi_d.range(15,0);	
          RAM_D.write(res);
        }
      else
        {
          res.range(31,24)=pi_d.range(7,0);	
          res.range(23,16)=pi_d.range(7,0);	
          res.range(15,8)=pi_d.range(7,0);	
          res.range(7,0)=pi_d.range(7,0);	
          RAM_D.write(res);
        }
    }
}

void ram_control::processPI_ACK()
{
  if (SELECT_RX.read()==1)
    PI_ACK.write(p_ack_rdy);
  else
    PI_ACK.write(7);
}

void ram_control::processPI_D()
{
  if ((SELECT_RX.read()==1) && (READ_RX.read()==1))
    PI_D.write(RAM_D.read());
}

void ram_control::processTRANSFER()
{
  ADDRESS_RX.write(PI_A.read());
  READ_RX.write(PI_READ.read());
  SELECT_RX.write((RESET_RX.read()==1)?0:PI_SEL.read());
  OPCODE_RX.write((RESET_RX.read()==1)?p_opc_nop:PI_OPC.read());
}

void ram_control::processRESET()
{
  RESET_RX.write(!RESET_N.read());
}
