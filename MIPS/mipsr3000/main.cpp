
#include <systemc.h>
#include "mipsr3000.h"
//#include "fifo/fifo.h"
#include "ram_control.h"
#include "pi_default.h"
#include "pi_control.h"
#include "i_cache.h"

#include "rom.h"
#include "ram.h"
#include "mips_dec.h"

/* Clock generation function */
void next_cycle (sc_signal<bool> &signal_clk)
{
  signal_clk = false;
  sc_start(1, SC_NS);   //sc_cycle(1)
  signal_clk = true;
  sc_start(1, SC_NS);   //sc_cycle(1)
}

int sc_main(int ac, char *av[])
{
  sc_signal<bool>                 CK;             // external clock
  sc_signal<bool>                 RESET_N;        // external reset
  sc_signal<sc_uint<6> >          IT_N;           // external interrupts
  sc_signal<sc_uint<32> >        I_A;            // instruction address
  sc_signal<bool>                I_FTCH;         // instruction fetch
  sc_signal<bool>                I_ACK;          // inst. fetch acknow.
  sc_signal<bool>                 I_BERR_N;       // inst. bus error
  sc_signal<bool>                 I_FRZ;          // inst. fetch pending
  sc_signal<sc_uint<32> >         I;                      // instruction bus
  sc_signal<sc_uint<32> >        D_A;            // data address
  sc_signal<bool>                D_RQ;           // data access request
  sc_signal<bool>                D_LOCK;         // lock memory access
  sc_signal<sc_uint<2> >         D_ATYPE;        // data access type
  sc_signal<bool>                D_ACK;          // data fetch acknow.
  sc_signal<bool>                 D_BERR_N;       // data bus error
  sc_signal<bool>                 D_FRZ;          // data access pending
  sc_signal<sc_uint<32> >         D_IN;           // data bus (input )
  sc_signal<sc_uint<32> >        D_OUT;          // data bus (output)

  sc_signal<sc_uint<32> >        DAT_ROMU;
  sc_signal<sc_uint<32> >        DAT_ROMR;
  sc_signal<sc_uint<32> >        DAT_ROMX;
  sc_signal<sc_uint<32> >        DAT_RAMS;
  sc_signal<sc_uint<32> >        DAT_RAMU;
    
  sc_signal<sc_uint<10> >        I_A_11_2;
  sc_signal<sc_uint<10> >        D_A_11_2;
  sc_signal<sc_uint<4> >        E_RAMU;
  sc_signal<sc_uint<4> >        E_RAMS;
  sc_signal<sc_uint<32> >        DAT;
  sc_signal<bool>                 W_N;          // data access pending

  mipsr3000* mips1 = new mipsr3000("mips1");

  mips1->CK(CK);
  mips1->RESET_N(RESET_N);
  mips1->IT_N(IT_N);
  mips1->I_A(I_A);
  mips1->I_FTCH(I_FTCH);
  mips1->I_ACK(I_ACK);
  mips1->I_BERR_N(I_BERR_N);
  mips1->I_FRZ(I_FRZ);
  mips1->I(I);
  mips1->D_A(D_A);
  mips1->D_RQ(D_RQ);
  mips1->D_LOCK(D_LOCK);
  mips1->D_ATYPE(D_ATYPE);
  mips1->D_ACK(D_ACK);
  mips1->D_BERR_N(D_BERR_N);
  mips1->D_FRZ(D_FRZ);
  mips1->D_IN(D_IN);
  mips1->D_OUT(D_OUT);

  mips_dec *mips_dec1=new mips_dec("mips_dec1");
  mips_dec1->CK(CK);
  mips_dec1->I_ADR(I_A);
  mips_dec1->I_FTCH(I_FTCH);
  mips_dec1->I_FRZ(I_FRZ);          // inst. not available
  mips_dec1->I_BERR_N(I_BERR_N);       // inst. bus error
  mips_dec1->DAT_ROMU(DAT_ROMU);
  mips_dec1->DAT_ROMR(DAT_ROMR);
  mips_dec1->DAT_ROMX(DAT_ROMX);
  mips_dec1->DATA_TO_MIPS(I);
  mips_dec1->I_A_11_2(I_A_11_2);
  mips_dec1->D_A(D_A);
  mips_dec1->D_RQ(D_RQ);
  mips_dec1->D_LOCK(D_LOCK);
  mips_dec1->D_ATYPE(D_ATYPE);
  mips_dec1->D_ACK(D_ACK);
  mips_dec1->D_BERR_N(D_BERR_N);
  mips_dec1->D_FRZ(D_FRZ);
  mips_dec1->D_IN(D_IN);
  mips_dec1->D_OUT(D_OUT);
  mips_dec1->D_A_11_2(D_A_11_2);
  mips_dec1->DAT(DAT);
  mips_dec1->E_RAMU(E_RAMU);
  mips_dec1->E_RAMS(E_RAMS);
  mips_dec1->W_N(W_N);

  rom *romr1=new rom("romr1");
  romr1->initData("inifiles/romr.ini");
  romr1->print();
  romr1->ADR(I_A_11_2);
  romr1->DAT(DAT_ROMR);

  rom *romu1=new rom("romu1");
  romu1->initData("inifiles/romu.ini");
  romu1->print();
  romu1->ADR(I_A_11_2);
  romu1->DAT(DAT_ROMU);

  rom *romx1=new rom("romx1");
  romx1->initData("inifiles/romx.ini");
  romx1->print();
  romx1->ADR(I_A_11_2);
  romx1->DAT(DAT_ROMX);

  ram *ramu1=new ram("ramu1");
  ramu1->initData("inifiles/ramu.ini");
  ramu1->print();
  ramu1->ADR(D_A_11_2);
  ramu1->DAT(DAT_RAMU);
  ramu1->E(E_RAMU);
  ramu1->W_N(W_N);

  ram *rams1=new ram("rams1");
  rams1->ADR(D_A_11_2);
  rams1->DAT(DAT_RAMS);
  rams1->E(E_RAMS);
  rams1->W_N(W_N);

  sc_trace_file *tf;

  tf=sc_create_vcd_trace_file("tf");
  sc_trace(tf,CK,"CLK");
  sc_trace(tf,mips1->RESET_RX,"RESET_RX");
  sc_trace(tf,mips1->IR_RI,"IR_RI");     
  sc_trace(tf,mips1->I_TYPE_SD,"I_TYPE_SD");
  sc_trace(tf,mips1->I_TYPE_RD,"I_TYPE_RD");
  sc_trace(tf,mips1->NEXTPC_RD,"NEXTPC_RD");   
  sc_trace(tf,mips1->I,"I");
  sc_trace(tf,mips1->decode1->I_BRNCH_SD,"I_BRNCH_SD");
  sc_trace(tf,mips1->decode1->JMPADR_SD,"JMPADR_SD");
  sc_trace(tf,mips1->decode1->NEXTPC_SD,"NEXTPC_SD");
  
  sc_trace(tf,mips1->decode1->RD_SD,"RD_SD");
  sc_trace(tf,mips1->decode1->RS_SD,"RS_SD");
  sc_trace(tf,mips1->decode1->RT_SD,"RT_SD");

  sc_trace(tf,mips1->regfile1->registerFile[1],"R1");
  sc_trace(tf,mips1->regfile1->registerFile[12],"R12");
  sc_trace(tf,mips1->regfile1->registerFile[13],"R13");
  sc_trace(tf,mips1->regfile1->registerFile[14],"R14");
  sc_trace(tf,mips1->regfile1->registerFile[15],"R15");
  sc_trace(tf,mips1->regfile1->registerFile[16],"R16");
  sc_trace(tf,mips1->regfile1->registerFile[26],"R26");
  sc_trace(tf,mips1->regfile1->registerFile[30],"R30");
  
  sc_trace(tf,mips1->regfile1->WRC,"WRC");
  sc_trace(tf,mips1->ff_decode1->RD_RD,"RD");

  sc_trace(tf,mips1->alu1->XOPER_SE,"XOPER_SE");
  sc_trace(tf,mips1->alu1->YOPER_SE,"YOPER_SE");
  sc_trace(tf,mips1->alu1->RLOGIC_SE,"RLOGIC_SE");

  sc_trace(tf,mips1->alu1->XARITH_SE,"XARITH_SE");
  sc_trace(tf,mips1->alu1->YARITH_SE,"YARITH_SE");
  sc_trace(tf,mips1->alu1->CARITH_SE,"CARITH_SE");

  sc_trace(tf,mips1->IOPER_SD,"IOPER_SD");
  sc_trace(tf,mips1->IOPER_RD,"IOPER_RD");
  sc_trace(tf,mips1->RES_RE,"RES_RE");
  sc_trace(tf,mips1->forwardunit1->HZD_SDE_SD,"HZD_SDE_SD");
  sc_trace(tf,mips_dec1->SEL_ROMR,"SEL_ROMR");
  sc_trace(tf,mips_dec1->SEL_ROMU,"SEL_ROMU");
  sc_trace(tf,mips_dec1->SEL_ROMX,"SEL_ROMX");

  sc_trace(tf,mips1->forwardunit1->HAZARDS_SD,"HAZARDS_SD");
  sc_trace(tf,mips1->forwardunit1->HAZARDS_SE,"HAZARDS_SE");
  
  sc_trace(tf,mips1->TOPER_RD,"TOPER_RD");
  sc_trace(tf,mips1->Y_SE,"Y_SE");
  sc_trace(tf,mips1->WDATA_RE,"WDATA_RE");
  sc_trace(tf,mips1->memory1->I_WRITE_SM,"I_WRITE_SM");

  sc_trace(tf,mips_dec1->D_A,"D_A");
  sc_trace(tf,mips_dec1->D_IN,"D_IN");
  sc_trace(tf,mips_dec1->D_OUT,"D_OUT");
  sc_trace(tf,mips_dec1->W_N,"W_N");
  sc_trace(tf,mips_dec1->BYTE_SEL,"BYTE_SEL");
  sc_trace(tf,mips_dec1->BYTE,"BYTE");
  sc_trace(tf,mips_dec1->E_RAMU,"E_RAMU");
  sc_trace(tf,mips_dec1->D_ATYPE,"D_ATYPE");

  sc_start(SC_ZERO_TIME);
  RESET_N = false;
  IT_N = 0x3F;
  I_BERR_N = true;
  I_FRZ = false;
  I = 0x00000021;
  D_BERR_N = true;
  D_FRZ = false;

  next_cycle (CK);
  next_cycle (CK);
  next_cycle (CK);
  RESET_N = true;

  for (int i=0;i<30;i++)
    next_cycle(CK);

    sc_close_vcd_trace_file(tf);
  return 0; /* this is necessary */
}
