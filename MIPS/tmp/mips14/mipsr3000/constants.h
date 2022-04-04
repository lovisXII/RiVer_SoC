// ### -------------------------------------------------------------- ###
// # file       : constants.h                                           #
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

#ifndef _CONSTANTS
#define _CONSTANTS

const sc_uint<4> r_fmt_o=0x1;
const sc_uint<4> i_fmt_o=0x2;
const sc_uint<4> j_fmt_o=0x4;
const sc_uint<4> illgl_o=0x8;

const sc_uint<4> d_use_st_o=0xb;
const sc_uint<4> d_use_s_o=0xa;
const sc_uint<4> d_use_t_o=0x9;
const sc_uint<4> e_use_st_o=0x7;
const sc_uint<4> e_use_s_o=0x6;
const sc_uint<4> e_use_t_o=0x5;
const sc_uint<4> no_use_o=0x0;

const bool o_signd_o=1;
const bool o_usign_o=0;

const sc_uint<3> arith_o=0x4;
const sc_uint<3> logic_o=0x5;
const sc_uint<3> shift_o=0x6;
const sc_uint<3> test_o=0x7;
const sc_uint<3> toper_o=0x1;
const sc_uint<3> soper_o=0x2;
const sc_uint<3> noop_o=0x0;

const sc_uint<2> add_o=0x0;
const sc_uint<2> sub_o=0x1;

const sc_uint<2> sleft_o=0x0;
const sc_uint<2> sright_o=0x1;

const sc_uint<2> or_o=0x0;
const sc_uint<2> and_o=0x1;
const sc_uint<2> xor_o=0x2;
const sc_uint<2> nor_o=0x3;

const bool r_signd_o=1;
const bool r_usign_o=0;

const bool ovr_o=1;
const bool nov_o=0;

const sc_uint<3> e_pdc_rd_o=0x2;
const sc_uint<3> e_pdc_31_o=0x3;
const sc_uint<3> m_pdc_rd_o=0x4;
const sc_uint<3> no_pdc_o=0;

const bool nobra_o=0;
const bool brnch_o=1;

const sc_uint<5> loadw_o=0x11;
const sc_uint<5> loadh_o=0x12;
const sc_uint<5> loadb_o=0x14;
const sc_uint<5> storw_o=0x9;
const sc_uint<5> storh_o=0xa;
const sc_uint<5> storb_o=0xc;
const sc_uint<5> swapw_o=0x19;
const sc_uint<5> nomem_o=0x0;

const sc_uint<2> special_g=0; // special ins.
const sc_uint<3> bcond_g=0x2; // bcond ins.
const sc_uint<2> cop0_g=0x2; // copro. 0
const sc_uint<2> others_g=0x3; // other ins.

const sc_uint<6> special_i=0; // special
const sc_uint<6> bcond_i=0x1; // bcond
const sc_uint<6> cop0_i=0x10; // copro 0

const int add_i=0x20; // md add
const int addi_i=0xc8; // md addi
const int addu_i=0x21; // md addu
const int addui_i=0xc9; // md addui
const int and_i=0x24; // md and
const int andi_i=0xcc; // md andi
const int beq_i=0xc4; // md beq
const int bgez_i=0x41; // m  bgez
const int bgezal_i=0x51; // m  bgezal
const int bgtz_i=0xc7; // m  bgtz
const int blez_i=0xc6; // m  blez
const int bltz_i=0x40; // m  bltz
const int bltzal_i=0x50; // m  bltzal
const int bne_i=0xc5; // md bne
const int break_i=0xd; // m  break
const int j_i=0xc2; // md j
const int jal_i=0xc3; // md jal
const int jalr_i=0x9; // md jalr
const int jr_i=0x8; // md jr
const int lb_i=0xe0; // md lb
const int lbu_i=0xe4; // md lbu
const int lh_i=0xe1; // md lh
const int lhu_i=0xe5; // md lhu
const int lui_i=0xcf; // md lui
const int lw_i=0xe3; // md lw
const int mfc0_i=0x80; // m  mfc0
const int mtc0_i=0x81; // m  mtc0
const int nor_i=0x27; // m  nor
const int or_i=0x25; // md or
const int ori_i=0xcd; // md ori
const int rfe_i=0xb0; // m  rfe
const int sb_i=0xe8; // md sb
const int sh_i=0xe9; // md sh
const int sleep_i=0xf; //    sleep
const int sll_i=0x0; // md sll
const int sllv_i=0x4; // md sllv
const int slt_i=0x2a; // md slt
const int slti_i=0xca; // md slti
const int sltu_i=0x2b; // md sltu
const int sltui_i=0xcb; // md sltui
const int srl_i=0x2; // md srl
const int srlv_i=0x6; // md srlv
const int sra_i=0x3; // md sra
const int srav_i=0x7; // md srav
const int sub_i=0x22; // md sub
const int subu_i=0x23; // md subu
const int sw_i=0xeb; // md sw
const int swap_i=0xe7; //    swap
const int syscall_i=0xc; // m  syscall
const int xor_i=0x26; // md xor
const int xori_i=0xce; // md xori
const int mfhi_i=0x10; // m  mfhi
const int mthi_i=0x11; // m  mthi
const int mflo_i=0x12; // m  mflo
const int mtlo_i=0x13; // m  mtlo

/*
const sc_uint<8> add_i=0x20; // md add
const sc_uint<8> addi_i=0xc8; // md addi
const sc_uint<8> addu_i=0x21; // md addu
const sc_uint<8> addui_i=0xc9; // md addui
const sc_uint<8> and_i=0x24; // md and
const sc_uint<8> andi_i=0xcc; // md andi
const sc_uint<8> beq_i=0xc4; // md beq
const sc_uint<8> bgez_i=0x41; // m  bgez
const sc_uint<8> bgezal_i=0x51; // m  bgezal
const sc_uint<8> bgtz_i=0xc7; // m  bgtz
const sc_uint<8> blez_i=0xc6; // m  blez
const sc_uint<8> bltz_i=0x40; // m  bltz
const sc_uint<8> bltzal_i=0x50; // m  bltzal
const sc_uint<8> bne_i=0xc5; // md bne
const sc_uint<8> break_i=0xd; // m  break
const sc_uint<8> j_i=0xc2; // md j
const sc_uint<8> jal_i=0xc3; // md jal
const sc_uint<8> jalr_i=0x9; // md jalr
const sc_uint<8> jr_i=0x8; // md jr
const sc_uint<8> lb_i=0xe0; // md lb
const sc_uint<8> lbu_i=0xe4; // md lbu
const sc_uint<8> lh_i=0xe1; // md lh
const sc_uint<8> lhu_i=0xe5; // md lhu
const sc_uint<8> lui_i=0xcf; // md lui
const sc_uint<8> lw_i=0xe3; // md lw
const sc_uint<8> mfc0_i=0x80; // m  mfc0
const sc_uint<8> mtc0_i=0x81; // m  mtc0
const sc_uint<8> nor_i=0x27; // m  nor
const sc_uint<8> or_i=0x25; // md or
const sc_uint<8> ori_i=0xcd; // md ori
const sc_uint<8> rfe_i=0xb0; // m  rfe
const sc_uint<8> sb_i=0xe8; // md sb
const sc_uint<8> sh_i=0xe9; // md sh
const sc_uint<8> sleep_i=0xf; //    sleep
const sc_uint<8> sll_i=0x0; // md sll
const sc_uint<8> sllv_i=0x4; // md sllv
const sc_uint<8> slt_i=0x2a; // md slt
const sc_uint<8> slti_i=0xca; // md slti
const sc_uint<8> sltu_i=0x2b; // md sltu
const sc_uint<8> sltui_i=0xcb; // md sltui
const sc_uint<8> srl_i=0x2; // md srl
const sc_uint<8> srlv_i=0x6; // md srlv
const sc_uint<8> sra_i=0x3; // md sra
const sc_uint<8> srav_i=0x7; // md srav
const sc_uint<8> sub_i=0x22; // md sub
const sc_uint<8> subu_i=0x23; // md subu
const sc_uint<8> sw_i=0xeb; // md sw
const sc_uint<8> swap_i=0xe7; //    swap
const sc_uint<8> syscall_i=0xc; // m  syscall
const sc_uint<8> xor_i=0x26; // md xor
const sc_uint<8> xori_i=0xce; // md xori

const sc_uint<8> mfhi_i=0x10; // m  mfhi
const sc_uint<8> mthi_i=0x11; // m  mthi
const sc_uint<8> mflo_i=0x12; // m  mflo
const sc_uint<8> mtlo_i=0x13; // m  mtlo
*/

const sc_uint<32> nop_i=0x00000021;
const sc_uint<32> nop_type=0x02E8080;

const int cause_s=0xD;
const int badvadr_s=0x8;
const int epc_s=0xE;
const int status_s=0xC;

const sc_uint<32> excphnd_a=0x80000080;
const sc_uint<32> boothnd_a=0xBFC00180;
const sc_uint<32> reset_a=0xBFC00000;

const sc_uint<2> m_writ_w=0x0;
const sc_uint<2> m_writ_h=0x2;
const sc_uint<2> m_writ_b=0x3;
const sc_uint<2> m_read_w=0x1;
#endif
