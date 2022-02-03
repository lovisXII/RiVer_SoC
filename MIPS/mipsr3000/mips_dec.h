#ifndef _MIPS_DEC
#define _MIPS_DEC

#include <systemc.h>

SC_MODULE(mips_dec)
{
  sc_in<bool>				CK;			// external clock
  sc_in<sc_uint<32> > 	I_ADR; 		// instruction address
  sc_in<bool>   			I_FTCH;		// fetch new instruction
  sc_in<bool>   			I_FRZ;		// inst. not available
  sc_out<bool>			I_BERR_N;	// inst. bus error

  sc_in<sc_uint<32> >		DAT_ROMU;
  sc_in<sc_uint<32> >		DAT_ROMR;
  sc_in<sc_uint<32> >		DAT_ROMX;

  sc_out<sc_uint<32> >	DATA_TO_MIPS;
  sc_out<sc_uint<10> >	I_A_11_2;
  sc_out<sc_uint<10> >	D_A_11_2;

  sc_in<sc_uint<32> >		D_A;
  sc_in<bool>			D_RQ;
  sc_in<bool>			D_LOCK;
  sc_in<sc_uint<2> >		D_ATYPE;
  sc_in<bool>			D_ACK;
  sc_out<bool>		D_BERR_N;
  sc_out<bool>		W_N;
  sc_in<bool>			D_FRZ;
  sc_out<sc_uint<32> >	D_IN;
  sc_in<sc_uint<32> >		D_OUT;

  sc_out<sc_uint<4> >		E_RAMU;
  sc_out<sc_uint<4> >		E_RAMS;
  sc_inout<sc_uint<32> >	DAT;

  sc_signal<bool>		WRITE;	//
  sc_signal<bool>		D_RW;	//

  sc_signal<bool>		ROMU;	// user rom
  sc_signal<bool>		ROMR;	// bootstrap rom
  sc_signal<bool>		ROMX;	// exception rom
  sc_signal<bool>		RAMU;	//
  sc_signal<bool>		RAMS;	//
  sc_signal<bool>		SEL_ROMU;	// user rom
  sc_signal<bool>		SEL_ROMR;	// bootstrap rom
  sc_signal<bool>		SEL_ROMX;	// exception rom
  sc_signal<bool>		SEL_RAMU;	// ram user 
  sc_signal<bool>		SEL_RAMS;	// ram supervisor
  sc_signal<sc_uint<4> >	BYTE_SEL;
  sc_signal<sc_uint<4> >	BYTE;

  SC_CTOR(mips_dec)
  {
    SC_METHOD(processWRITE);
    sensitive << D_ATYPE;
    SC_METHOD(processW_N);
    sensitive << D_ATYPE;
    SC_METHOD(processDAT);
    sensitive << CK << WRITE << D_RQ << D_OUT << D_ATYPE;
    SC_METHOD(processBYTE_SEL);
    sensitive << D_A << D_ATYPE;
    SC_METHOD(processBYTE);
    sensitive << D_RQ << BYTE_SEL;
    SC_METHOD(processROMU);
    sensitive << I_ADR;
    SC_METHOD(processROMX);
    sensitive << I_ADR;
    SC_METHOD(processROMR);
    sensitive << I_ADR;
    SC_METHOD(processRAMU);
    sensitive << D_A;
    SC_METHOD(processRAMS);
    sensitive << D_A;
    SC_METHOD(processSEL_ROMU);
    sensitive << ROMU << CK << I_FTCH << I_FRZ;
    SC_METHOD(processSEL_ROMX);
    sensitive << ROMU << CK << I_FTCH << I_FRZ;
    SC_METHOD(processSEL_ROMR);
    sensitive << ROMU << CK << I_FTCH << I_FRZ;
    SC_METHOD(processE_RAMU);
    sensitive << RAMU << CK << BYTE << D_FRZ;
    SC_METHOD(processE_RAMS);
    sensitive << RAMS << CK << BYTE << D_FRZ;
    SC_METHOD(processI_BERR_N);
    sensitive << ROMU << ROMR << ROMX << CK << I_FTCH << I_FRZ;
    SC_METHOD(processD_BERR_N);
    sensitive << RAMU << RAMS << CK << D_RQ << D_FRZ;
    SC_METHOD(processI_A_11_2);
    sensitive << I_ADR;
    SC_METHOD(processD_A_11_2);
    sensitive << D_A;
    SC_METHOD(processDATA_TO_MIPS);
    sensitive << SEL_ROMU << DAT_ROMU << SEL_ROMR << DAT_ROMR << SEL_ROMX << DAT_ROMX;
    SC_METHOD(processD_IN);
    sensitive << DAT;
  }

// ### ------------------------------------------------------ ###
// #   align the data out of the processor                      #
// ### ------------------------------------------------------ ###

  void processWRITE();

  void processW_N();

  void processDAT();

// ### ------------------------------------------------------ ###
// #   define the selected bytes                                #
// ### ------------------------------------------------------ ###

  void processBYTE_SEL();

  void processBYTE();

  void processROMU();

  void processROMX();

  void processROMR();

  void processRAMU();

  void processRAMS();

  void processSEL_ROMU();

  void processSEL_ROMR();

  void processSEL_ROMX();

  void processE_RAMU();

  void processE_RAMS();

  void processI_BERR_N();

  void processD_BERR_N();

  void processI_A_11_2();

  void processD_A_11_2();

  void processDATA_TO_MIPS();

  void processD_IN();

};
#endif
