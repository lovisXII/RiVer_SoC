#ifndef _RAM
#define _RAM
#include "systemc.h"
#include "stdio.h"

#define READ_ADR	0
#define READ_VAL	1

SC_MODULE(ram)
{
	sc_in<sc_uint<10>> 		ADR;
	sc_inout<sc_uint<32>> 		DAT;
	sc_in<sc_uint<4>> 		E;
	sc_in<bool> 			W_N;	// W_N=0 for a write, W_N=1 for a read

	sc_uint<8> ramContents[1024][4];

	SC_CTOR(ram)
	{
		SC_METHOD(memAccess);
		sensitive 
			<< ADR 
			<< DAT 
			<< E 
			<< W_N;
	}

	void memAccess()
	{
		sc_uint<4> e=E.read();
		sc_uint<32> dat;

		if (e[0]==1)
		{
			if (W_N.read()==1) 		// read operation
				dat.range(7,0)=ramContents[(int)ADR.read()][0];
			else
			{
				dat=DAT.read();
				ramContents[(int)ADR.read()][0]=dat.range(7,0);
			}
		}
		if (e[1]==1)
		{
			if (W_N.read()==1) 		// read operation
				dat.range(15,8)=ramContents[(int)ADR.read()][1];
			else
			{
				dat=DAT.read();
				ramContents[(int)ADR.read()][1]=dat.range(15,8);
			}
		}
		if (e[2]==1)
		{
			if (W_N.read()==1) 		// read operation
				dat.range(23,16)=ramContents[(int)ADR.read()][2];
			else
			{
				dat=DAT.read();
				ramContents[(int)ADR.read()][2]=dat.range(23,16);
			}
		}
		if (e[3]==1)
		{
			if (W_N.read()==1) 		// read operation
				dat.range(31,24)=ramContents[(int)ADR.read()][3];
			else
			{
				dat=DAT.read();
				ramContents[(int)ADR.read()][3]=dat.range(31,24);
			}
		}
		if ((e!=0x0)&&(W_N.read()==1))
			DAT.write(dat);
	}

	void initData(char *filename)
	{
		FILE *fp;
		unsigned int cur_adr=0;
		unsigned int cur_val=0;
		unsigned int red_c=0;
		unsigned int red_val=0;
		unsigned int state=READ_ADR;
		unsigned int index=0;

  		printf ("loading initialization file %s\n", filename);

  		if ((fp = fopen (filename, "r")) == NULL)
    		{
    			fprintf (stderr, "cannot open RAM initialization file: %s\n", filename);
    			exit (1);
    		}

  		while ((red_c = fgetc (fp)) != (unsigned int)EOF)
    		{
    			switch (red_c)
      			{
      				case ' ' :
      				case '\t':
        				break;
      				case '\n':
          				index=cur_adr&0x3;
          				ramContents[(cur_adr & 0xFFF)>>2][index] = (sc_uint<8>)cur_val;
					//printf("adr=%x	index=%d cur_val=%x\n",cur_adr&0xFFF,index,cur_val);
        				state   = READ_ADR;
        				cur_adr = 0;
        				cur_val = 0;
        				break;
      				case ':' :
        				state = READ_VAL;
        				break;
      				default  :
        				if ((red_c >= '0') && (red_c <= '9'))
          					red_val = red_c - '0';
        				if ((red_c >= 'a') && (red_c <= 'f'))
          					red_val = red_c - 'a' + 10;
        				if ((red_c >= 'A') && (red_c <= 'F'))
          					red_val = red_c - 'A' + 10;
        				if (state == READ_ADR)
          					cur_adr = (cur_adr 
			<< 4) + red_val;
        				else
          					cur_val = (cur_val 
			<< 4) + red_val;
      			}
    		}
  		fclose (fp);
  	}

        void print()
        {
                int i;

                for (i=0;i<10;i++)
                        printf ("%x,%x,%x,%x\n",(unsigned char)ramContents[i][0],
					(unsigned char)ramContents[i][1],
					(unsigned char)ramContents[i][2],
					(unsigned char)ramContents[i][3]);
        }

};
#endif

