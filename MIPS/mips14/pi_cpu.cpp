#include <systemc.h>
#include "mipsr3000/mipsr3000.h"
#include "ram_control.h"
#include "pi_default.h"
#include "pi_control.h"
#include "i_cache.h"
#include "d_cache.h"

#include "ram.h"

/* Clock generation function */
void next_cycle (sc_signal<bool> &signal_clk)
{
  signal_clk = false;
  //sc_cycle (1);//DG deprecated 
  wait(0, SC_NS) ;
  signal_clk = true;
  //sc_cycle (1);//DG deprecated 
  wait(0, SC_NS) ;
}

int sc_main(int ac, char *av[])
{
	sc_signal<bool> 	CK;// external clock
	sc_signal<bool> 	RESET_N;// external reset

    sc_signal<sc_uint<6>>          IT_N;           // external interrupts

	sc_signal<sc_uint<32>>	I_A; // instruction address
	sc_signal<bool> 	I_FRZ; // instruction fetch freezed
	sc_signal<bool> 	I_BERR_N; // instruction bus error
	sc_signal<bool> 	I_FTCH; // instruction fetch
	sc_signal<bool> 	I_ACK; // instruction fetch acknow.
	sc_signal<sc_uint<32>>	I; // instruction bus

    sc_signal<sc_uint<32>>        D_A;            // data address
    sc_signal<bool>                D_RQ;           // data access request
    sc_signal<bool>                D_LOCK;         // lock memory access
    sc_signal<sc_uint<2>>         D_ATYPE;        // data access type
    sc_signal<bool>                D_ACK;          // data fetch acknow.
    sc_signal<bool>                 D_BERR_N;       // data bus error
    sc_signal<bool>                 D_FRZ;          // data access pending
    sc_signal<sc_uint<32>>         D_IN;           // data bus (input )
    sc_signal<sc_uint<32>>        D_OUT;          // data bus (output)

	sc_signal<sc_uint<32>>	PI_A; // Pi address
	sc_signal<sc_uint<4>>	PI_OPC; // Pi operation code
	sc_signal<bool> 	PI_READ; // Pi read/write
	sc_signal<bool> 	PI_LOCK; // Pi access locked
	sc_signal<bool>		PI_IREQ[8]; // Pi requests
	sc_signal<bool>		PI_GNT[8]; // Pi master grants
	sc_signal<bool>		PI_SEL[8]; // Pi slave selection
	sc_signal<sc_uint<3>>	PI_ACK; // Pi acknowledge
	sc_signal<sc_uint<32>>	PI_D; // Pi data bus
	sc_signal<bool> 	PI_TOUT; // Pi time out

	sc_signal<sc_uint<4>>	RSTI_E; // reset memory enable
	sc_signal<bool> 	RSTI_W_N; // reset memory r/w
	sc_signal<sc_uint<10>>	RSTI_ADR; // reset memory address
	sc_signal<sc_uint<32>>	RSTI_D; // reset memory data

	sc_signal<sc_uint<4>>	EXCI_E; // exception memory enable
	sc_signal<bool> 	EXCI_W_N; // exception memory r/w
	sc_signal<sc_uint<10>>	EXCI_ADR; // exception memory address
	sc_signal<sc_uint<32>>	EXCI_D; // exception memory data

	sc_signal<sc_uint<4>>	SYSD_E; // syst data memory enable
	sc_signal<bool> 	SYSD_W_N; // syst data memory r/w
	sc_signal<sc_uint<10>>	SYSD_ADR; // syst data memory address
	sc_signal<sc_uint<32>>	SYSD_D; // syst data memory data

	sc_signal<sc_uint<4>>	USRI_E; // user instr memory enable
	sc_signal<bool> 	USRI_W_N; // user instr memory r/w
	sc_signal<sc_uint<10>>	USRI_ADR; // user instr memory address
	sc_signal<sc_uint<32>>	USRI_D; // user instr memory data

	sc_signal<sc_uint<4>>	USRD_E; // user data  memory enable
	sc_signal<bool> 	USRD_W_N; // user data  memory r/w
	sc_signal<sc_uint<10>>	USRD_ADR; // user data  memory address
	sc_signal<sc_uint<32>>	USRD_D; // user data  memory data

        mipsr3000 *mips1=new mipsr3000("mips1");

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

	i_cache *i_cache1=new i_cache("i_cache1");

	i_cache1->CK(CK);
	i_cache1->RESET_N(RESET_N);
	i_cache1->I_A(I_A);
	i_cache1->I_FTCH(I_FTCH);
	i_cache1->I_ACK(I_ACK);
	i_cache1->I(I);
	i_cache1->I_FRZ(I_FRZ);
	i_cache1->I_BERR_N(I_BERR_N);
	i_cache1->PI_A(PI_A);
	i_cache1->PI_OPC(PI_OPC);
	i_cache1->PI_READ(PI_READ);
	i_cache1->PI_LOCK(PI_LOCK);
	i_cache1->PI_IREQ(PI_IREQ[0]);
	i_cache1->PI_GNT(PI_GNT[0]);
	i_cache1->PI_ACK(PI_ACK);
	i_cache1->PI_D(PI_D);
 	i_cache1->PI_TOUT(PI_TOUT);

	d_cache *d_cache1=new d_cache("d_cache1");

	d_cache1->CK(CK);
	d_cache1->RESET_N(RESET_N);
	d_cache1->D_A(D_A);
	d_cache1->D_RQ(D_RQ);
	d_cache1->D_LOCK(D_LOCK);
	d_cache1->D_ATYPE(D_ATYPE);
	d_cache1->D_ACK(D_ACK);
	d_cache1->D_BERR_N(D_BERR_N);
	d_cache1->D_FRZ(D_FRZ);
	d_cache1->D_OUT(D_IN);
	d_cache1->D_IN(D_OUT);
	d_cache1->PI_A(PI_A);
	d_cache1->PI_OPC(PI_OPC);
	d_cache1->PI_READ(PI_READ);
	d_cache1->PI_LOCK(PI_LOCK);
	d_cache1->PI_IREQ(PI_IREQ[1]);
	d_cache1->PI_GNT(PI_GNT[1]);
	d_cache1->PI_ACK(PI_ACK);
	d_cache1->PI_D(PI_D);
 	d_cache1->PI_TOUT(PI_TOUT);

	pi_control *pi_control1=new pi_control("pi_control1");

 	pi_control1->CK(CK);
 	pi_control1->RESET_N(RESET_N);
 	pi_control1->PI_A(PI_A);
 	pi_control1->PI_OPC(PI_OPC);
 	pi_control1->PI_READ(PI_READ);
 	pi_control1->PI_LOCK(PI_LOCK);
 	pi_control1->PI_IREQ0(PI_IREQ[0]);
 	pi_control1->PI_IREQ1(PI_IREQ[1]);
 	pi_control1->PI_IREQ2(PI_IREQ[2]);
 	pi_control1->PI_IREQ3(PI_IREQ[3]);
 	pi_control1->PI_IREQ4(PI_IREQ[4]);
 	pi_control1->PI_IREQ5(PI_IREQ[5]);
 	pi_control1->PI_IREQ6(PI_IREQ[6]);
 	pi_control1->PI_IREQ7(PI_IREQ[7]);

 	pi_control1->PI_GNT0(PI_GNT[0]);
 	pi_control1->PI_GNT1(PI_GNT[1]);
 	pi_control1->PI_GNT2(PI_GNT[2]);
 	pi_control1->PI_GNT3(PI_GNT[3]);
 	pi_control1->PI_GNT4(PI_GNT[4]);
 	pi_control1->PI_GNT5(PI_GNT[5]);
 	pi_control1->PI_GNT6(PI_GNT[6]);
 	pi_control1->PI_GNT7(PI_GNT[7]);

 	pi_control1->PI_SEL0(PI_SEL[0]);
 	pi_control1->PI_SEL1(PI_SEL[1]);
 	pi_control1->PI_SEL2(PI_SEL[2]);
 	pi_control1->PI_SEL3(PI_SEL[3]);
 	pi_control1->PI_SEL4(PI_SEL[4]);
 	pi_control1->PI_SEL5(PI_SEL[5]);
 	pi_control1->PI_SEL6(PI_SEL[6]);
 	pi_control1->PI_SEL7(PI_SEL[7]);

  	pi_control1->PI_ACK(PI_ACK);
 	pi_control1->PI_TOUT(PI_TOUT);

	pi_default *pi_default1=new pi_default("pi_default1");

 	pi_default1->CK(CK);
 	pi_default1->PI_A(PI_A);
 	pi_default1->PI_OPC(PI_OPC);
 	pi_default1->PI_READ(PI_READ);
 	pi_default1->PI_LOCK(PI_LOCK);

 	pi_default1->PI_IREQ5(PI_IREQ[7]);
 	pi_default1->PI_IREQ4(PI_IREQ[6]);
 	pi_default1->PI_IREQ3(PI_IREQ[5]);
 	pi_default1->PI_IREQ2(PI_IREQ[4]);
 	pi_default1->PI_IREQ1(PI_IREQ[3]);
 	pi_default1->PI_IREQ0(PI_IREQ[2]);

 	pi_default1->PI_GNT5(PI_GNT[7]);
 	pi_default1->PI_GNT4(PI_GNT[6]);
 	pi_default1->PI_GNT3(PI_GNT[5]);
 	pi_default1->PI_GNT2(PI_GNT[4]);
 	pi_default1->PI_GNT1(PI_GNT[3]);
 	pi_default1->PI_GNT0(PI_GNT[2]);

	ram_control *romu_cntl1=new ram_control("romu_cntl1");

 	romu_cntl1->CK(CK);
 	romu_cntl1->RESET_N(RESET_N);
 	romu_cntl1->PI_A(PI_A);
 	romu_cntl1->PI_D(PI_D);
 	romu_cntl1->PI_OPC(PI_OPC);
 	romu_cntl1->PI_READ(PI_READ);
 	romu_cntl1->PI_SEL(PI_SEL[1]);
 	romu_cntl1->PI_ACK(PI_ACK);
 	romu_cntl1->RAM_E(USRI_E);
 	romu_cntl1->RAM_W_N(USRI_W_N);
 	romu_cntl1->RAM_ADR(USRI_ADR);
 	romu_cntl1->RAM_D(USRI_D);

	ram *romu_00400000_1=new ram("romu_00400000_1");
	romu_00400000_1->initData("romu.ini");
	romu_00400000_1->print();

 	romu_00400000_1->ADR(USRI_ADR);
 	romu_00400000_1->W_N(USRI_W_N);
 	romu_00400000_1->E(USRI_E);
 	romu_00400000_1->DAT(USRI_D);

	ram_control *romr_cntl1=new ram_control("romr_cntl1");

 	romr_cntl1->CK(CK);
 	romr_cntl1->RESET_N(RESET_N);
 	romr_cntl1->PI_A(PI_A);
 	romr_cntl1->PI_D(PI_D);
 	romr_cntl1->PI_OPC(PI_OPC);
 	romr_cntl1->PI_READ(PI_READ);
 	romr_cntl1->PI_SEL(PI_SEL[3]);
 	romr_cntl1->PI_ACK(PI_ACK);
 	romr_cntl1->RAM_E(RSTI_E);
 	romr_cntl1->RAM_W_N(RSTI_W_N);
 	romr_cntl1->RAM_ADR(RSTI_ADR);
 	romr_cntl1->RAM_D(RSTI_D);

	ram *romr_bfc00000_1=new ram("romr_bfc00000_1");

	romr_bfc00000_1->initData("romr.ini");
	romr_bfc00000_1->print();
 	romr_bfc00000_1->ADR(RSTI_ADR);
 	romr_bfc00000_1->W_N(RSTI_W_N);
 	romr_bfc00000_1->E(RSTI_E);
 	romr_bfc00000_1->DAT(RSTI_D);

	ram_control *romx_cntl1=new ram_control("romx_cntl1");

 	romx_cntl1->CK(CK);
 	romx_cntl1->RESET_N(RESET_N);
 	romx_cntl1->PI_A(PI_A);
 	romx_cntl1->PI_D(PI_D);
 	romx_cntl1->PI_OPC(PI_OPC);
 	romx_cntl1->PI_READ(PI_READ);
 	romx_cntl1->PI_SEL(PI_SEL[2]);
 	romx_cntl1->PI_ACK(PI_ACK);
 	romx_cntl1->RAM_E(EXCI_E);
 	romx_cntl1->RAM_W_N(EXCI_W_N);
 	romx_cntl1->RAM_ADR(EXCI_ADR);
 	romx_cntl1->RAM_D(EXCI_D);

	ram *romx_80000000_1=new ram("romx_80000000_1");

	romx_80000000_1->initData("romx.ini");
	romx_80000000_1->print();
 	romx_80000000_1->ADR(EXCI_ADR);
 	romx_80000000_1->W_N(EXCI_W_N);
 	romx_80000000_1->E(EXCI_E);
 	romx_80000000_1->DAT(EXCI_D);

        ram_control *ramu_cntl1=new ram_control("ramu_cntl1");

        ramu_cntl1->CK(CK);
        ramu_cntl1->RESET_N(RESET_N);
        ramu_cntl1->PI_A(PI_A);
        ramu_cntl1->PI_D(PI_D);
        ramu_cntl1->PI_OPC(PI_OPC);
        ramu_cntl1->PI_READ(PI_READ);
        ramu_cntl1->PI_SEL(PI_SEL[0]);
        ramu_cntl1->PI_ACK(PI_ACK);
        ramu_cntl1->RAM_E(USRD_E);
        ramu_cntl1->RAM_W_N(USRD_W_N);
        ramu_cntl1->RAM_ADR(USRD_ADR);
        ramu_cntl1->RAM_D(USRD_D);

        ram *ramu_00000000_1=new ram("ramu_00000000_1");

        ramu_00000000_1->initData("ramu.ini");
        ramu_00000000_1->print();
        ramu_00000000_1->ADR(USRD_ADR);
        ramu_00000000_1->W_N(USRD_W_N);
        ramu_00000000_1->E(USRD_E);
        ramu_00000000_1->DAT(USRD_D);

        sc_trace_file *tf;

        tf=sc_create_vcd_trace_file("tf");
        sc_trace(tf,CK,"CLK");
        sc_trace(tf,D_FRZ,"D_FRZ");
        sc_trace(tf,I_FRZ,"I_FRZ");
        sc_trace(tf,D_RQ,"D_RQ");
        sc_trace(tf,PI_A,"PI_A");
        sc_trace(tf,PI_D,"PI_D");
        sc_trace(tf,PI_ACK,"PI_ACK");
        sc_trace(tf,PI_READ,"PI_READ");
        sc_trace(tf,PI_LOCK,"PI_LOCK");

        //added 16.12. to identify writes into user memory ramu.h

       sc_trace(tf,ramu_00000000_1->DAT,"DAT");

        sc_trace(tf,mips1->ff_decode1->NEXTPC_RD,"NEXTPC_RD");
        //sc_trace(tf,mips1->ff_decode1->I_TYPE_RD,"I_TYPE_RD");
        //sc_trace(tf,mips1->ff_ifetch1->IR_RI,"IR_RI");
        //sc_trace(tf,mips1->ff_execute1->I_TYPE_RE,"I_TYPE_RE");
        //sc_trace(tf,mips1->memory1->I_LOAD_SM,"I_LOAD_SM");
        //sc_trace(tf,mips1->memory1->I_STOR_SM,"I_STOR_SM");
        sc_trace(tf,mips1->regfile1->registerFile[1],"R1");

        //sc_trace(tf,i_cache1->I_A,"i_cache1.I_A");
        sc_trace(tf,d_cache1->D_A,"d_cache1.D_A");
        //sc_trace(tf,i_cache1->RESET_RX,"i_cache1.RESET_RX");

	sc_trace(tf,d_cache1->RESET_RX,"d_cache1.RESET_RX");
        sc_trace(tf,d_cache1->MISS_SX,"d_cache1.MISS_SX");
        sc_trace(tf,d_cache1->PILINEN_SX,"d_cache1.PILINEN_SX");
        sc_trace(tf,d_cache1->DEFLTEN_SX,"d_cache1.DEFLTEN_SX");
	/* sc_trace(tf,d_cache1->PI_D,"d_cache1.PI_D");
        sc_trace(tf,d_cache1->ANSFULL_SX,"d_cache1.ANSFULL_SX");
        sc_trace(tf,d_cache1->ANSPSH_SX,"d_cache1.ANSPSH_SX");
        sc_trace(tf,d_cache1->ANSPOP_SX,"d_cache1.ANSPOP_SX");
        sc_trace(tf,d_cache1->DATREAD_SX,"d_cache1.DATREAD_SX");
        sc_trace(tf,d_cache1->A_STAT_RX,"d_cache1.A_STAT_RX");
        sc_trace(tf,d_cache1->ANSDATA_SX,"d_cache1.ANSDATA_SX");
        sc_trace(tf,d_cache1->ANSADR_SX,"d_cache1.ANSADR_SX");
        sc_trace(tf,d_cache1->ANSFLAG_SX,"d_cache1.ANSFLAG_SX");
        sc_trace(tf,d_cache1->PI_ADR_RX,"d_cache1.PI_ADR_RX");
        sc_trace(tf,d_cache1->ADRCRY_SX,"d_cache1.ADRCRY_SX");
        sc_trace(tf,d_cache1->ADRCNT_SX,"d_cache1.ADRCNT_SX");
        sc_trace(tf,d_cache1->DATARDY_SX,"d_cache1.DATARDY_SX");
        sc_trace(tf,d_cache1->ACSADR_RX,"d_cache1.ACSADR_RX");*/
        sc_trace(tf,d_cache1->C_NXTS_SX,"d_cache1.C_NXTS_SX");
	
        sc_trace(tf,i_cache1->ADRTAG_SX,"ADRTAG_SX");
        sc_trace(tf,i_cache1->BLCNBR_SX,"BLCNBR_SX");
        sc_trace(tf,i_cache1->BLCOFS_SX,"BLCOFS_SX");
        sc_trace(tf,i_cache1->DIRMISS_SX,"DIRMISS_SX");
        sc_trace(tf,i_cache1->MISS_SX,"MISS_SX");
	sc_trace(tf,i_cache1->RSTCNT_SX,"RSTCNT_SX");
        sc_trace(tf,i_cache1->RSTCRY_SX,"RSTCRY_SX");
        sc_trace(tf,i_cache1->ENDRST_SX,"ENDRST_SX");
        sc_trace(tf,i_cache1->C_STAT_RX,"C_STAT_RX");
        sc_trace(tf,i_cache1->RQSPSH_SX,"RQSPSH_SX");
        sc_trace(tf,i_cache1->RQSDIN_SX,"RQSDIN_SX");
        sc_trace(tf,i_cache1->RQSADR_SX,"RQSADR_SX");
        sc_trace(tf,i_cache1->ADRCNT_RX,"ADRCNT_RX");
        sc_trace(tf,i_cache1->SENDADR_RX,"SENDADR_RX");
        sc_trace(tf,i_cache1->RQSACSC_SX,"RQSACSC_SX");
        sc_trace(tf,i_cache1->R_STAT_RX,"R_STAT_RX");
        sc_trace(tf,i_cache1->RQSPOP_SX,"RQSPOP_SX");
        sc_trace(tf,i_cache1->C_STAT_RX,"i_cache1.C_STAT_RX");
        sc_trace(tf,i_cache1->P_STAT_RX,"i_cache1.P_STAT_RX");
        sc_trace(tf,i_cache1->PILINEN_SX,"i_cache1.PILINEN_SX");
        sc_trace(tf,i_cache1->PI_IREQ,"i_cache1.PI_IREQ");
	

        sc_trace(tf,d_cache1->C_STAT_RX,"d_cache1.C_STAT_RX");
        sc_trace(tf,d_cache1->P_STAT_RX,"d_cache1.P_STAT_RX");
        //sc_trace(tf,d_cache1->RQSADR_SX,"d_cache1.RQSADR_SX");
        //sc_trace(tf,d_cache1->RQSDAT_SX,"d_cache1.RQSDAT_SX");
        //sc_trace(tf,d_cache1->RQSTYP_SX,"d_cache1.RQSTYP_SX");
        //sc_trace(tf,d_cache1->RQSSIZ_SX,"d_cache1.RQSSIZ_SX");
        //sc_trace(tf,d_cache1->RQSMOD_SX,"d_cache1.RQSMOD_SX");
        //sc_trace(tf,d_cache1->RQSACSC_SX,"d_cache1.RQSACSC_SX");
        //sc_trace(tf,d_cache1->MOREADR_SX,"d_cache1.MOREADR_SX");
        //sc_trace(tf,d_cache1->ACSCNT_SX,"d_cache1.ACSCNT_SX");

        sc_trace(tf,d_cache1->WRTDAT0_SX,"d_cache1.WRTDAT0_SX");
        sc_trace(tf,d_cache1->WRTDAT1_SX,"d_cache1.WRTDAT1_SX");
        sc_trace(tf,d_cache1->WRTDAT2_SX,"d_cache1.WRTDAT2_SX");
        sc_trace(tf,d_cache1->WRTDAT3_SX,"d_cache1.WRTDAT3_SX");
        sc_trace(tf,d_cache1->WRTDATA_SX,"d_cache1.WRTDATA_SX");
        sc_trace(tf,d_cache1->REDBLC0_RX,"d_cache1.REDBLC0_RX");
        sc_trace(tf,d_cache1->REDBLC1_RX,"d_cache1.REDBLC1_RX");
        sc_trace(tf,d_cache1->REDBLC2_RX,"d_cache1.REDBLC2_RX");
        sc_trace(tf,d_cache1->REDBLC3_RX,"d_cache1.REDBLC3_RX");
	//
        sc_trace(tf,i_cache1->BLOC0_RX,"BLOC0_RX");
        //sc_trace(tf,i_cache1->BLOC1_RX,"BLOC1_RX");
        //sc_trace(tf,i_cache1->BLOC2_RX,"BLOC2_RX");
        //sc_trace(tf,i_cache1->BLOC3_RX,"BLOC3_RX");
        //sc_trace(tf,i_cache1->ANSPSH_SX,"ANSPSH_SX");
        //sc_trace(tf,i_cache1->ANSDIN_SX,"ANSDIN_SX");
        //sc_trace(tf,i_cache1->ANSREG0_RX,"ANSREG0_RX");
        //sc_trace(tf,i_cache1->ANSEMTY_SX,"ANSEMTY_SX");
        //sc_trace(tf,i_cache1->SENDADR_RX,"SENDADR_RX");
        //sc_trace(tf,i_cache1->ANSDATA_SX,"ANSDATA_SX");
        //sc_trace(tf,i_cache1->ANSADR_SX,"ANSADR_SX");
        //sc_trace(tf,i_cache1->ANSFLAG_SX,"ANSFLAG_SX");
        //sc_trace(tf,i_cache1->BLOCDIN_SX,"BLOCDIN_SX");
        //sc_trace(tf,i_cache1->DATA_SX,"DATA_SX");
        //sc_trace(tf,i_cache1->REDBLC0_RX,"REDBLC0_RX");
        //sc_trace(tf,i_cache1->REDBLC1_RX,"REDBLC1_RX");
        //sc_trace(tf,i_cache1->REDBLC2_RX,"REDBLC2_RX");
        //sc_trace(tf,i_cache1->REDBLC3_RX,"REDBLC3_RX");
        //sc_trace(tf,i_cache1->WRTDAT0_SX,"WRTDAT0_SX");
        //sc_trace(tf,i_cache1->WRTDAT1_SX,"WRTDAT1_SX");
        //sc_trace(tf,i_cache1->WRTDAT2_SX,"WRTDAT2_SX");
        //sc_trace(tf,i_cache1->WRTDAT3_SX,"WRTDAT3_SX");
        //sc_trace(tf,i_cache1->WRTDATA_SX,"WRTDATA_SX");
        sc_trace(tf,i_cache1->PI_ACK,"PI_ACK");
        //sc_trace(tf,i_cache1->PI_READ,"PI_READ");
        //sc_trace(tf,i_cache1->PI_LOCK,"PI_LOCK");
        //sc_trace(tf,i_cache1->PI_OPC,"PI_OPC");
        //sc_trace(tf,i_cache1->PILINEN_SX,"cachePILINEN_SX");
        //sc_trace(tf,i_cache1->MOREADR_SX,"cacheMOREADR_SX");
        //sc_trace(tf,i_cache1->ACSCNT_RX,"cacheACSCNT_RX");
        //sc_trace(tf,i_cache1->INITACS_SX,"INITACS_SX");

        //sc_trace(tf,pi_control1->CK_SX,"pi_control1.CK_SX");
        //sc_trace(tf,pi_control1->REQUEST_SX,"pi_control1.REQUEST_SX");
        sc_trace(tf,pi_control1->PI_IREQ,"pi_control1.PI_IREQ");
        sc_trace(tf,pi_control1->PI_GNT,"pi_control1.PI_GNT");
        sc_trace(tf,pi_control1->PI_SEL,"pi_control1.PI_SEL");
        //sc_trace(tf,pi_control1->CURPRIO_SX0,"pi_control1.CURPRIO_SX0");
        //sc_trace(tf,pi_control1->CURPRIO_SX1,"pi_control1.CURPRIO_SX1");
        //sc_trace(tf,pi_control1->CURPRIO_SX2,"pi_control1.CURPRIO_SX2");
        //sc_trace(tf,pi_control1->CURPRIO_SX3,"pi_control1.CURPRIO_SX3");
        //sc_trace(tf,pi_control1->CURPRIO_SX4,"pi_control1.CURPRIO_SX4");
        //sc_trace(tf,pi_control1->CURPRIO_SX5,"pi_control1.CURPRIO_SX5");
        //sc_trace(tf,pi_control1->CURPRIO_SX6,"pi_control1.CURPRIO_SX6");
        //sc_trace(tf,pi_control1->CURPRIO_SX7,"pi_control1.CURPRIO_SX7");
        //sc_trace(tf,pi_control1->CURPRIO_SX,"pi_control1.CURPRIO_SX");
        sc_trace(tf,pi_control1->PRIOR_RX,"pi_control1.PRIOR_RX");
        sc_trace(tf,pi_control1->GRANT_SX,"pi_control1.GRANT_SX");
        //sc_trace(tf,pi_control1->DFLMSTR_SX,"pi_control1.DFLMSTR_SX");
        //sc_trace(tf,pi_control1->WRTPRIO_SX,"pi_control1.WRTPRIO_SX");
        //sc_trace(tf,pi_control1->C_STAT_RX,"pi_control1.C_STAT_RX");
        //sc_trace(tf,pi_control1->GLBRQST_SX,"pi_control1.GLBRQST_SX");
        //sc_trace(tf,pi_control1->DATARDY_SX,"pi_control1.DATARDY_SX");
        //sc_trace(tf,pi_control1->RESET_RX,"pi_control1.RESET_RX");
        //sc_trace(tf,pi_control1->SELECT_SX,"pi_control1.SELECT_SX");
        //sc_trace(tf,pi_control1->PI_ACK,"pi_control1.PI_ACK");
        //sc_trace(tf,pi_default1->DEFLTEN_SX,"dDEFLTEN_SX");
        //sc_trace(tf,pi_default1->PI_GNT,"dPI_GNT");
        //sc_trace(tf,pi_control1->PI_SEL0,"cPI_SEL0");
        //sc_trace(tf,pi_control1->PI_SEL1,"cPI_SEL1");
        //sc_trace(tf,pi_control1->PI_SEL2,"cPI_SEL2");
        //sc_trace(tf,pi_control1->PI_SEL3,"cPI_SEL3");
        //sc_trace(tf,pi_control1->PI_SEL4,"cPI_SEL4");

        /*sc_trace(tf,romr_cntl1->ADDRESS_RX,"rsti.ADDRESS_RX");
        sc_trace(tf,romr_cntl1->READ_RX,"rsti.READ_RX");
        sc_trace(tf,romr_cntl1->OPCODE_RX,"rsti.OPCODE_RX");
        sc_trace(tf,romr_cntl1->SELECT_RX,"rsti.SELECT_RX");
        sc_trace(tf,romr_bfc00000_1->ADR,"romr.ADR");
        sc_trace(tf,romr_bfc00000_1->DAT,"romr.DAT");
        sc_trace(tf,romr_bfc00000_1->E,"romr.E");
        sc_trace(tf,romr_bfc00000_1->W_N,"romr.W_N");*/
	
        //sc_trace(tf,pi_control1->PI_LOCK,"cPI_LOCK");
        /*sc_trace(tf,ramu_cntl1->ADDRESS_RX,"usrd.ADDRESS_RX");
        sc_trace(tf,ramu_cntl1->READ_RX,"usrd.READ_RX");
        sc_trace(tf,ramu_cntl1->OPCODE_RX,"usrd.OPCODE_RX");
        sc_trace(tf,ramu_cntl1->SELECT_RX,"usrd.SELECT_RX");
        sc_trace(tf,ramu_00000000_1->ADR,"ramu.ADR");
        sc_trace(tf,ramu_00000000_1->DAT,"ramu.DAT");
        sc_trace(tf,ramu_00000000_1->E,"ramu.E");
        sc_trace(tf,ramu_00000000_1->W_N,"ramu.W_N");*/
        //sc_trace(tf,romr_cntl1->ADDRESS_RX,"romrADDRESS_RX");
        //sc_trace(tf,romr_cntl1->READ_RX,"romrREAD_RX");
        //sc_trace(tf,romr_cntl1->OPCODE_RX,"romrOPCODE_RX");
        //sc_trace(tf,romr_cntl1->SELECT_RX,"romrSELECT_RX");

        //sc_trace(tf,RSTI_ADR,"RSTI_ADR");
        //sc_trace(tf,RSTI_W_N,"RSTI_W_N");
        //sc_trace(tf,RSTI_E,"RSTI_E");
        //sc_trace(tf,RSTI_D,"RSTI_D");

        sc_initialize ();
	//I_A=0xbfc00000;
	//I_FTCH=1;
	//I_ACK=1;

        IT_N = 0x3F;

        RESET_N = false;
	next_cycle (CK);
        next_cycle (CK);
        next_cycle (CK);
        RESET_N = true;

        for (int i=0;i<200;i++)
                next_cycle(CK);

        sc_close_vcd_trace_file(tf);
        return 0;       /* this is necessary */
}

