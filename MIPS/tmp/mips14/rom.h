#ifndef _ROM
#define _ROM
//#include "systemc.h"
#include <systemc.h>
#include "stdio.h"

#define ROM_READ_ADR        0
#define ROM_READ_VAL        1

SC_MODULE(rom)
{
	sc_in<sc_uint<10>> 		ADR;
	sc_out<sc_uint<32>> 		DAT;

	sc_uint<8> romContents[1024][4];

	SC_CTOR(rom)
	{
		SC_METHOD(memAccess);
		sensitive 
			<< ADR;
	}

	void memAccess()
	{
		int adr=(int)ADR.read();
		//adr = (adr >> 2)&0x3FF; 
		adr = adr&0x3FF; 

		sc_uint<32> res=0x0;

		res.range(7,0)=romContents[adr][0];
		res.range(15,8)=romContents[adr][1];
		res.range(23,16)=romContents[adr][2];
		res.range(31,24)=romContents[adr][3];
		DAT.write(res);

	}

        void initData(char *filename)
        {
                FILE *fp;
                unsigned int cur_adr=0;
                unsigned char cur_val=0;
                unsigned int red_c=0;
                unsigned int red_val=0;
                unsigned int state=ROM_READ_ADR;
                unsigned int index=0;

                printf ("loading initialization file %s\n", filename);

                if ((fp = fopen (filename, "r")) == NULL)
                {
                        fprintf (stderr, "cannot open ROM initialization file: %s\n", filename);
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
                                        romContents[(cur_adr & 0xFFF)>>2][index] = (sc_uint<8>)cur_val;
                                        //printf("adr=%x        index=%d cur_val=|%x|\n",cur_adr&0xFFF,index,cur_val);
                                        state   = ROM_READ_ADR;
                                        cur_adr = 0;
                                        cur_val = 0;
                                        break;
                                case ':' :
                                        state = ROM_READ_VAL;
                                        break;
                                default  :
                                        if ((red_c >= '0') && (red_c <= '9'))
                                                red_val = red_c - '0';
                                        if ((red_c >= 'a') && (red_c <= 'f'))
                                                red_val = red_c - 'a' + 10;
                                        if ((red_c >= 'A') && (red_c <= 'F'))
                                                red_val = red_c - 'A' + 10;
                                        if (state == ROM_READ_ADR)
                                                cur_adr = (cur_adr 
			<< 4) + red_val;
                                        else
                                                cur_val = (cur_val 
			<< 4) + red_val;
					//printf("cur_adr=%x cur_val=%x\n",cur_adr,cur_val);
                        }
                }
                fclose (fp);
        }
	
	void print()
	{
		int i;

		for (i=0;i<10;i++)
			printf ("%x,%x,%x,%x\n",(unsigned char)romContents[i][0],(unsigned char)romContents[i][1],(unsigned char)romContents[i][2],(unsigned char)romContents[i][3]);
	}

};
#endif

