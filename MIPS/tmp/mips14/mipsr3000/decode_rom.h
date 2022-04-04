// ### -------------------------------------------------------------- ###
// # file       : decode_rom.h                                          #
// # date       : Oct 28 2003                                           #
// # version    : v2.0                                                  #
// #                                                                    #
// # origin     : this description has been developed by CAO-VLSI team  #
// #              at MASI laboratory, University Pierre et Marie Curie  #
// #              4 Place Jussieu 75252 Paris Cedex 05 - France         #
// #                                                                    #
// # descr.     : data flow description of a five stage pipelined Mips  #
// #              R3000 processor                                       #
// #                                                                    #
// # Original                                                           #
// # authors    : D. Hommais, P. Bazargan Sabet                         #
// #                                                                    #
// # System-C                                                           #
// # rewriting  : F. Pecheux                                            #
// ### -------------------------------------------------------------- ###

#ifndef _DECODE_ROM
#define _DECODE_ROM
//#include "systemc.h"
#include "constants.h"
#include <systemc.h>

typedef struct
{
	sc_uint<4> instf; 	// Instruction format
	sc_uint<4> sou;		// Source operand usage
	bool suso;		// signed/unsigned source operands
	sc_uint<3> ot;		// operation type
	sc_uint<2> o;		// operation
	bool sur;		// signed/unsigned result
	bool od;		// overflow detection
	sc_uint<3> wspr;	// which stage produces the result
	bool toi;		// type of instruction (branch)
	sc_uint<5> toma;	// type of memory access
} rom_out;

SC_MODULE(decode_rom)
{
	sc_in<sc_uint<8> > OPCOD_SD;
	sc_out<sc_uint<25> > I_TYPE_SD;

	SC_CTOR(decode_rom):
		OPCOD_SD("OPCOD_SD"),
		I_TYPE_SD("I_TYPE_SD")
	{
		SC_METHOD(processROM);
		sensitive << OPCOD_SD;
	}

	void processROM()
	{
		int opcod=(int)OPCOD_SD.read();
		rom_out ro;

		switch (opcod)
		{
			case add_i:
				ro.instf=r_fmt_o;ro.sou=e_use_st_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_signd_o;
				ro.od=ovr_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case addi_i:
				ro.instf=i_fmt_o;ro.sou=e_use_s_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_signd_o;
				ro.od=ovr_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case addu_i:
				ro.instf=r_fmt_o;ro.sou=e_use_st_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case addui_i:
					ro.instf=i_fmt_o;ro.sou=e_use_s_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case and_i:
					ro.instf=r_fmt_o;ro.sou=e_use_st_o;ro.suso=o_usign_o;ro.ot=logic_o;ro.o=and_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case andi_i:
					ro.instf=i_fmt_o;ro.sou=e_use_s_o;ro.suso=o_usign_o;ro.ot=logic_o;ro.o=and_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case beq_i:
					ro.instf=i_fmt_o;ro.sou=d_use_st_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=no_pdc_o;ro.toi=brnch_o;ro.toma=nomem_o;
				break;
			case blez_i:
					ro.instf=i_fmt_o;ro.sou=d_use_s_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=no_pdc_o;ro.toi=brnch_o;ro.toma=nomem_o;
				break;
			case bgez_i:
					ro.instf=i_fmt_o;ro.sou=d_use_s_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=no_pdc_o;ro.toi=brnch_o;ro.toma=nomem_o;
				break;
			case bgezal_i:
					ro.instf=i_fmt_o;ro.sou=d_use_s_o;ro.suso=o_signd_o;ro.ot=noop_o;ro.o=add_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=e_pdc_31_o;ro.toi=brnch_o;ro.toma=nomem_o;
				break;
			case bgtz_i:
				ro.instf=i_fmt_o;ro.sou=d_use_s_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=no_pdc_o;ro.toi=brnch_o;ro.toma=nomem_o;
				break;
			case bltz_i:
				ro.instf=i_fmt_o;ro.sou=d_use_s_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=no_pdc_o;ro.toi=brnch_o;ro.toma=nomem_o;
				break;
			case bltzal_i:
				ro.instf=i_fmt_o;ro.sou=d_use_s_o;ro.suso=o_signd_o;ro.ot=noop_o;ro.o=add_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=e_pdc_31_o;ro.toi=brnch_o;ro.toma=nomem_o;
				break;
			case bne_i:
				ro.instf=i_fmt_o;ro.sou=d_use_st_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=no_pdc_o;ro.toi=brnch_o;ro.toma=nomem_o;
				break;
			case break_i:
				ro.instf=r_fmt_o;ro.sou=no_use_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=no_pdc_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case j_i:
				ro.instf=j_fmt_o;ro.sou=no_use_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=no_pdc_o;ro.toi=brnch_o;ro.toma=nomem_o;
				break;
			case jal_i:
				ro.instf=j_fmt_o;ro.sou=no_use_o;ro.suso=o_signd_o;ro.ot=noop_o;ro.o=add_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=e_pdc_31_o;ro.toi=brnch_o;ro.toma=nomem_o;
				break;
			case jalr_i:
				ro.instf=r_fmt_o;ro.sou=d_use_s_o;ro.suso=o_signd_o;ro.ot=noop_o;ro.o=add_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=brnch_o;ro.toma=nomem_o;
				break;
			case jr_i:
				ro.instf=r_fmt_o;ro.sou=d_use_s_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=no_pdc_o;ro.toi=brnch_o;ro.toma=nomem_o;
				break;
			case lb_i:
				ro.instf=i_fmt_o;ro.sou=e_use_s_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=m_pdc_rd_o;ro.toi=nobra_o;ro.toma=loadb_o;
				break;
			case lbu_i:
				ro.instf=i_fmt_o;ro.sou=e_use_s_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=m_pdc_rd_o;ro.toi=nobra_o;ro.toma=loadb_o;
				break;
			case lh_i:
				ro.instf=i_fmt_o;ro.sou=e_use_s_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=m_pdc_rd_o;ro.toi=nobra_o;ro.toma=loadh_o;
				break;
			case lhu_i:
				ro.instf=i_fmt_o;ro.sou=e_use_s_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=m_pdc_rd_o;ro.toi=nobra_o;ro.toma=loadh_o;
				break;
			case lui_i:
				ro.instf=i_fmt_o;ro.sou=no_use_o;ro.suso=o_signd_o;ro.ot=noop_o;ro.o=add_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case lw_i:
				ro.instf=i_fmt_o;ro.sou=e_use_s_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=m_pdc_rd_o;ro.toi=nobra_o;ro.toma=loadw_o;
				break;
			case mfc0_i:
				ro.instf=i_fmt_o;ro.sou=no_use_o;ro.suso=o_signd_o;ro.ot=noop_o;ro.o=add_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case mflo_i:
				ro.instf=r_fmt_o;ro.sou=no_use_o;ro.suso=o_signd_o;ro.ot=noop_o;ro.o=add_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case mfhi_i:
				ro.instf=r_fmt_o;ro.sou=no_use_o;ro.suso=o_signd_o;ro.ot=noop_o;ro.o=add_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case mthi_i:
				ro.instf=r_fmt_o;ro.sou=d_use_s_o;ro.suso=o_signd_o;ro.ot=soper_o;ro.o=add_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=no_pdc_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case mtlo_i:
				ro.instf=r_fmt_o;ro.sou=d_use_s_o;ro.suso=o_signd_o;ro.ot=soper_o;ro.o=add_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=no_pdc_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case nor_i:
				ro.instf=r_fmt_o;ro.sou=e_use_st_o;ro.suso=o_usign_o;ro.ot=logic_o;ro.o=nor_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case or_i:
				ro.instf=r_fmt_o;ro.sou=e_use_st_o;ro.suso=o_usign_o;ro.ot=logic_o;ro.o=or_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case ori_i:
				ro.instf=i_fmt_o;ro.sou=e_use_s_o;ro.suso=o_usign_o;ro.ot=logic_o;ro.o=or_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case rfe_i:
				ro.instf=r_fmt_o;ro.sou=no_use_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=no_pdc_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case sb_i:
				ro.instf=i_fmt_o;ro.sou=e_use_st_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=no_pdc_o;ro.toi=nobra_o;ro.toma=storb_o;
				break;
			case sh_i:
				ro.instf=i_fmt_o;ro.sou=e_use_st_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=no_pdc_o;ro.toi=nobra_o;ro.toma=storh_o;
				break;
			case sleep_i:
				ro.instf=r_fmt_o;ro.sou=no_use_o;ro.suso=o_signd_o;ro.ot=noop_o;ro.o=add_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=no_pdc_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case sllv_i:
				ro.instf=r_fmt_o;ro.sou=e_use_st_o;ro.suso=o_signd_o;ro.ot=shift_o;ro.o=sleft_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case sll_i:
				ro.instf=r_fmt_o;ro.sou=e_use_t_o;ro.suso=o_signd_o;ro.ot=shift_o;ro.o=sleft_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case slt_i:
				ro.instf=r_fmt_o;ro.sou=e_use_st_o;ro.suso=o_signd_o;ro.ot=test_o;ro.o=sub_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case sltu_i:
				ro.instf=r_fmt_o;ro.sou=e_use_st_o;ro.suso=o_signd_o;ro.ot=test_o;ro.o=sub_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case sra_i:
				ro.instf=r_fmt_o;ro.sou=e_use_t_o;ro.suso=o_signd_o;ro.ot=shift_o;ro.o=sright_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case srav_i:
				ro.instf=r_fmt_o;ro.sou=e_use_st_o;ro.suso=o_signd_o;ro.ot=shift_o;ro.o=sright_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case srl_i:
				ro.instf=r_fmt_o;ro.sou=e_use_t_o;ro.suso=o_signd_o;ro.ot=shift_o;ro.o=sright_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case srlv_i:
				ro.instf=r_fmt_o;ro.sou=e_use_st_o;ro.suso=o_signd_o;ro.ot=shift_o;ro.o=sright_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case sltui_i:
				ro.instf=i_fmt_o;ro.sou=e_use_s_o;ro.suso=o_signd_o;ro.ot=test_o;ro.o=sub_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case slti_i:
				ro.instf=i_fmt_o;ro.sou=e_use_s_o;ro.suso=o_signd_o;ro.ot=test_o;ro.o=sub_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case sub_i:
				ro.instf=r_fmt_o;ro.sou=e_use_st_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=sub_o;ro.sur=r_signd_o;
				ro.od=ovr_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case subu_i:
				ro.instf=r_fmt_o;ro.sou=e_use_st_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=sub_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case sw_i:
				ro.instf=i_fmt_o;ro.sou=e_use_st_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=no_pdc_o;ro.toi=nobra_o;ro.toma=storw_o;
				break;
			case swap_i:
				ro.instf=i_fmt_o;ro.sou=e_use_st_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=m_pdc_rd_o;ro.toi=nobra_o;ro.toma=swapw_o;
				break;
			case syscall_i:
				ro.instf=r_fmt_o;ro.sou=no_use_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=no_pdc_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case xor_i:
				ro.instf=r_fmt_o;ro.sou=e_use_st_o;ro.suso=o_usign_o;ro.ot=logic_o;ro.o=xor_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			case xori_i:
				ro.instf=i_fmt_o;ro.sou=e_use_s_o;ro.suso=o_usign_o;ro.ot=logic_o;ro.o=xor_o;ro.sur=r_usign_o;
				ro.od=nov_o;ro.wspr=e_pdc_rd_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
			default:
				ro.instf=illgl_o;ro.sou=no_use_o;ro.suso=o_signd_o;ro.ot=arith_o;ro.o=add_o;ro.sur=r_signd_o;
				ro.od=nov_o;ro.wspr=no_pdc_o;ro.toi=nobra_o;ro.toma=nomem_o;
				break;
		}

		sc_uint<25> rom_output;

		rom_output=ro.instf<<21 | ro.sou<<17 | ro.suso<<16 | ro.ot<<13 | ro.o<<11 | ro.sur<<10 | ro.od<<9 |
			ro.wspr<<6 | ro.toi<<5 | ro.toma;
		I_TYPE_SD.write(rom_output);
	}

};
#endif

