#ifndef _MIPS_DEC
#define _MIPS_DEC

//#include "systemc.h"
#include <systemc.h>

SC_MODULE(mips_dec)
{
	sc_in<bool>				CK;			// external clock
    sc_in<sc_uint<32>> 	I_ADR; 		// instruction address
    sc_in<bool>   			I_FTCH;		// fetch new instruction
    sc_in<bool>   			I_FRZ;		// inst. not available
    sc_out<bool>			I_BERR_N;	// inst. bus error

    sc_in<sc_uint<32>>		DAT_ROMU;
    sc_in<sc_uint<32>>		DAT_ROMR;
    sc_in<sc_uint<32>>		DAT_ROMX;

    sc_out<sc_uint<32>>	DATA_TO_MIPS;
    sc_out<sc_uint<10>>	I_A_11_2;
    sc_out<sc_uint<10>>	D_A_11_2;

    sc_in<sc_uint<32>>		D_A;
    sc_in<bool>			D_RQ;
    sc_in<bool>			D_LOCK;
    sc_in<sc_uint<2>>		D_ATYPE;
    sc_in<bool>			D_ACK;
    sc_out<bool>		D_BERR_N;
    sc_out<bool>		W_N;
    sc_in<bool>			D_FRZ;
    sc_out<sc_uint<32>>	D_IN;
    sc_in<sc_uint<32>>		D_OUT;

    sc_out<sc_uint<4>>		E_RAMU;
    sc_out<sc_uint<4>>		E_RAMS;
    sc_inout<sc_uint<32>>	DAT;

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
    sc_signal<sc_uint<4>>	BYTE_SEL;
    sc_signal<sc_uint<4>>	BYTE;

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

        void processWRITE()
        {
                if (D_ATYPE.read()==0x1)
                        WRITE.write(0);
                else
                        WRITE.write(1);
        }

        void processW_N()
        {
                if (D_ATYPE.read()==0x1)
                        W_N.write(1);
                else
                        W_N.write(0);
        }

        void processDAT()
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

        // ### ------------------------------------------------------ ###
        // #   define the selected bytes                                #
        // ### ------------------------------------------------------ ###

        void processBYTE_SEL()
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

        void processBYTE()
        {
                if (D_RQ.read()==1)
                        BYTE.write(BYTE_SEL.read());
                else
                        BYTE.write(0x0);
        }

	void processROMU()
	{
		sc_uint<32> i_adr=I_ADR.read();

		if ((int)i_adr.range(31,12)==0x00400)
			ROMU.write(1);
		else
			ROMU.write(0);
	}

	void processROMX()
	{
		sc_uint<32> i_adr=I_ADR.read();

		if ((int)i_adr.range(31,12)==0x80000)
			ROMX.write(1);
		else
			ROMX.write(0);
	}

	void processROMR()
	{
		sc_uint<32> i_adr=I_ADR.read();

		if ((int)i_adr.range(31,12)==0xBFC00)
			ROMR.write(1);
		else
			ROMR.write(0);
	}

        void processRAMU()
        {
                sc_uint<32> d_a=D_A.read();

                if ((int)d_a.range(31,12)==0x00000)
                        RAMU.write(1);
                else
                        RAMU.write(0);
        }

        void processRAMS()
        {
                sc_uint<32> d_a=D_A.read();

                if ((int)d_a.range(31,12)==0xC0000)
                        RAMS.write(1);
                else
                        RAMS.write(0);
        }

	void processSEL_ROMU()
	{
		SEL_ROMU.write(ROMU.read() & !CK.read() & I_FTCH.read() & !(I_FRZ.read()));
	}

	void processSEL_ROMR()
	{
		SEL_ROMR.write(ROMR.read() & !CK.read() & I_FTCH.read() & !(I_FRZ.read()));
	}

	void processSEL_ROMX()
	{
		SEL_ROMX.write(ROMX.read() & !CK.read() & I_FTCH.read() & !(I_FRZ.read()));
	}

	void processE_RAMU()
	{
		sc_uint<4> rbyte=BYTE.read();
		sc_uint<4> sel_ramu=0x0;

		sel_ramu[0]=RAMU.read() & !CK.read() & rbyte[0] & !D_FRZ.read(); 
		sel_ramu[1]=RAMU.read() & !CK.read() & rbyte[1] & !D_FRZ.read(); 
		sel_ramu[2]=RAMU.read() & !CK.read() & rbyte[2] & !D_FRZ.read(); 
		sel_ramu[3]=RAMU.read() & !CK.read() & rbyte[3] & !D_FRZ.read(); 
		E_RAMU.write(sel_ramu);
	}

	void processE_RAMS()
	{
		sc_uint<4> rbyte=BYTE.read();
		sc_uint<4> sel_rams=0x0;

		sel_rams[0]=RAMS.read() & !CK.read() & rbyte[0] & !D_FRZ.read(); 
		sel_rams[1]=RAMS.read() & !CK.read() & rbyte[1] & !D_FRZ.read(); 
		sel_rams[2]=RAMS.read() & !CK.read() & rbyte[2] & !D_FRZ.read(); 
		sel_rams[3]=RAMS.read() & !CK.read() & rbyte[3] & !D_FRZ.read(); 
		E_RAMS.write(sel_rams);
	}

	void processI_BERR_N()
	{
  		I_BERR_N.write(!(!ROMU.read() & !ROMR.read() & !ROMX.read() &
                         !CK.read() & I_FTCH.read() & !I_FRZ.read()));
	}

        void processD_BERR_N()
        {
                D_BERR_N.write(!(!RAMU.read() & !RAMS.read() &
                         !CK.read() & D_RQ.read() & !D_FRZ.read()));
        }

	void processI_A_11_2()
	{
		sc_uint<32> i_adr=I_ADR.read();

		I_A_11_2.write(i_adr.range(11,2));
	}

	void processD_A_11_2()
	{
		sc_uint<32> d_a=D_A.read();

		D_A_11_2.write(d_a.range(11,2));
	}

	void processDATA_TO_MIPS()
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

	void processD_IN()
	{
		D_IN.write(DAT.read());
	}

};
#endif
