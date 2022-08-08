
../riscof/riscof_work/rv32i_m/privilege/src/ecall.S/dut/my.elf:     file format elf32-littleriscv


Disassembly of section .text.init:

<<<<<<< HEAD
80000000 <rvtest_entry_point>:
80000000:	00000097          	auipc	ra,0x0
80000004:	13408093          	addi	ra,ra,308 # 80000134 <init_mscratch>
80000008:	000080e7          	jalr	ra

8000000c <rvtest_prolog_done>:
8000000c:	feedc0b7          	lui	ra,0xfeedc
80000010:	ead08093          	addi	ra,ra,-339 # feedbead <_end+0x7eed9c7d>
80000014:	ff76e137          	lui	sp,0xff76e
80000018:	f5610113          	addi	sp,sp,-170 # ff76df56 <_end+0x7f76bd26>
8000001c:	7fbb71b7          	lui	gp,0x7fbb7
80000020:	fab18193          	addi	gp,gp,-85 # 7fbb6fab <value+0x7fbb6f9b>
80000024:	bfddb237          	lui	tp,0xbfddb
80000028:	7d520213          	addi	tp,tp,2005 # bfddb7d5 <_end+0x3fdd95a5>
8000002c:	00000297          	auipc	t0,0x0
80000030:	0d828293          	addi	t0,t0,216 # 80000104 <rvtest_code_begin>
80000034:	00002317          	auipc	t1,0x2
80000038:	fcc30313          	addi	t1,t1,-52 # 80002000 <rvtest_data_begin>
8000003c:	b7fbb3b7          	lui	t2,0xb7fbb
80000040:	6fa38393          	addi	t2,t2,1786 # b7fbb6fa <_end+0x37fb94ca>
80000044:	5bfde437          	lui	s0,0x5bfde
80000048:	b7d40413          	addi	s0,s0,-1155 # 5bfddb7d <value+0x5bfddb6d>
8000004c:	adfef4b7          	lui	s1,0xadfef
80000050:	dbe48493          	addi	s1,s1,-578 # adfeedbe <_end+0x2dfecb8e>
80000054:	56ff7537          	lui	a0,0x56ff7
80000058:	6df50513          	addi	a0,a0,1759 # 56ff76df <value+0x56ff76cf>
8000005c:	ab7fc5b7          	lui	a1,0xab7fc
80000060:	b6f58593          	addi	a1,a1,-1169 # ab7fbb6f <_end+0x2b7f993f>
80000064:	d5bfe637          	lui	a2,0xd5bfe
80000068:	db760613          	addi	a2,a2,-585 # d5bfddb7 <_end+0x55bfbb87>
8000006c:	eadff6b7          	lui	a3,0xeadff
80000070:	edb68693          	addi	a3,a3,-293 # eadfeedb <_end+0x6adfccab>
80000074:	f56ff737          	lui	a4,0xf56ff
80000078:	76d70713          	addi	a4,a4,1901 # f56ff76d <_end+0x756fd53d>
8000007c:	fab807b7          	lui	a5,0xfab80
80000080:	bb678793          	addi	a5,a5,-1098 # fab7fbb6 <_end+0x7ab7d986>
80000084:	7d5c0837          	lui	a6,0x7d5c0
80000088:	ddb80813          	addi	a6,a6,-549 # 7d5bfddb <value+0x7d5bfdcb>
8000008c:	beae08b7          	lui	a7,0xbeae0
80000090:	eed88893          	addi	a7,a7,-275 # beadfeed <_end+0x3eaddcbd>
80000094:	df570937          	lui	s2,0xdf570
80000098:	f7690913          	addi	s2,s2,-138 # df56ff76 <_end+0x5f56dd46>
8000009c:	6fab89b7          	lui	s3,0x6fab8
800000a0:	fbb98993          	addi	s3,s3,-69 # 6fab7fbb <value+0x6fab7fab>
800000a4:	b7d5ca37          	lui	s4,0xb7d5c
800000a8:	fdda0a13          	addi	s4,s4,-35 # b7d5bfdd <_end+0x37d59dad>
800000ac:	dbeaeab7          	lui	s5,0xdbeae
800000b0:	feea8a93          	addi	s5,s5,-18 # dbeadfee <_end+0x5beabdbe>
800000b4:	6df57b37          	lui	s6,0x6df57
800000b8:	ff7b0b13          	addi	s6,s6,-9 # 6df56ff7 <value+0x6df56fe7>
800000bc:	b6fabbb7          	lui	s7,0xb6fab
800000c0:	7fbb8b93          	addi	s7,s7,2043 # b6fab7fb <_end+0x36fa95cb>
800000c4:	db7d6c37          	lui	s8,0xdb7d6
800000c8:	bfdc0c13          	addi	s8,s8,-1027 # db7d5bfd <_end+0x5b7d39cd>
800000cc:	edbebcb7          	lui	s9,0xedbeb
800000d0:	dfec8c93          	addi	s9,s9,-514 # edbeadfe <_end+0x6dbe8bce>
800000d4:	76df5d37          	lui	s10,0x76df5
800000d8:	6ffd0d13          	addi	s10,s10,1791 # 76df56ff <value+0x76df56ef>
800000dc:	bb6fbdb7          	lui	s11,0xbb6fb
800000e0:	b7fd8d93          	addi	s11,s11,-1153 # bb6fab7f <_end+0x3b6f894f>
800000e4:	ddb7de37          	lui	t3,0xddb7d
800000e8:	5bfe0e13          	addi	t3,t3,1471 # ddb7d5bf <_end+0x5db7b38f>
800000ec:	eedbfeb7          	lui	t4,0xeedbf
800000f0:	adfe8e93          	addi	t4,t4,-1313 # eedbeadf <_end+0x6edbc8af>
800000f4:	f76dff37          	lui	t5,0xf76df
800000f8:	56ff0f13          	addi	t5,t5,1391 # f76df56f <_end+0x776dd33f>
800000fc:	fbb70fb7          	lui	t6,0xfbb70
80000100:	ab7f8f93          	addi	t6,t6,-1353 # fbb6fab7 <_end+0x7bb6d887>

80000104 <rvtest_code_begin>:
80000104:	00002097          	auipc	ra,0x2
80000108:	10c08093          	addi	ra,ra,268 # 80002210 <begin_signature>
8000010c:	11111137          	lui	sp,0x11111
80000110:	11110113          	addi	sp,sp,273 # 11111111 <value+0x11111101>
80000114:	00000073          	ecall
80000118:	00000013          	nop
8000011c:	00000013          	nop
80000120:	0000a023          	sw	zero,0(ra)
80000124:	0020a223          	sw	sp,4(ra)
80000128:	00000013          	nop
8000012c:	00000013          	nop

80000130 <rvtest_code_end>:
80000130:	44c0006f          	j	8000057c <exit_cleanup>

80000134 <init_mscratch>:
80000134:	00002317          	auipc	t1,0x2
80000138:	ecc30313          	addi	t1,t1,-308 # 80002000 <rvtest_data_begin>
8000013c:	34031373          	csrrw	t1,mscratch,t1
80000140:	00002397          	auipc	t2,0x2
80000144:	f2438393          	addi	t2,t2,-220 # 80002064 <mscratch_save>
80000148:	0063a023          	sw	t1,0(t2)
8000014c:	34002373          	csrr	t1,mscratch
80000150:	00002397          	auipc	t2,0x2
80000154:	0c838393          	addi	t2,t2,200 # 80002218 <mtrap_sigptr>
80000158:	00732023          	sw	t2,0(t1)

8000015c <init_mtvec>:
8000015c:	00000317          	auipc	t1,0x0
80000160:	08030313          	addi	t1,t1,128 # 800001dc <mtrampoline>
80000164:	00002e97          	auipc	t4,0x2
80000168:	ef8e8e93          	addi	t4,t4,-264 # 8000205c <mtvec_save>
8000016c:	305313f3          	csrrw	t2,mtvec,t1
80000170:	007ea023          	sw	t2,0(t4)
80000174:	30502e73          	csrr	t3,mtvec
80000178:	e86e0ae3          	beq	t3,t1,8000000c <rvtest_prolog_done>

8000017c <init_tramp>:
8000017c:	30539073          	csrw	mtvec,t2
80000180:	00002e17          	auipc	t3,0x2
80000184:	e9ce0e13          	addi	t3,t3,-356 # 8000201c <tramptbl_sv>
80000188:	040e0e93          	addi	t4,t3,64

8000018c <overwrite_tt>:
8000018c:	0003af83          	lw	t6,0(t2)
80000190:	01fe2023          	sw	t6,0(t3)
80000194:	00032f03          	lw	t5,0(t1)
80000198:	01e3a023          	sw	t5,0(t2)
8000019c:	0003af83          	lw	t6,0(t2)
800001a0:	01ef9c63          	bne	t6,t5,800001b8 <resto_tramp>
800001a4:	00430313          	addi	t1,t1,4
800001a8:	00438393          	addi	t2,t2,4
800001ac:	004e0e13          	addi	t3,t3,4
800001b0:	fdde1ee3          	bne	t3,t4,8000018c <overwrite_tt>
800001b4:	e59ff06f          	j	8000000c <rvtest_prolog_done>

800001b8 <resto_tramp>:
800001b8:	010ea303          	lw	t1,16(t4)
800001bc:	34031073          	csrw	mscratch,t1
800001c0:	008eae83          	lw	t4,8(t4)

800001c4 <resto_loop>:
800001c4:	000e2f83          	lw	t6,0(t3)
800001c8:	01f3a023          	sw	t6,0(t2)
800001cc:	ffc38393          	addi	t2,t2,-4
800001d0:	ffce0e13          	addi	t3,t3,-4
800001d4:	ffd398e3          	bne	t2,t4,800001c4 <resto_loop>
800001d8:	3e00006f          	j	800005b8 <rvtest_end>

800001dc <mtrampoline>:
800001dc:	0800006f          	j	8000025c <mtrap_handler>
800001e0:	0880006f          	j	80000268 <mtrap_handler+0xc>
800001e4:	0900006f          	j	80000274 <mtrap_handler+0x18>
800001e8:	0980006f          	j	80000280 <mtrap_handler+0x24>
800001ec:	0a00006f          	j	8000028c <mtrap_handler+0x30>
800001f0:	0a80006f          	j	80000298 <mtrap_handler+0x3c>
800001f4:	0b00006f          	j	800002a4 <mtrap_handler+0x48>
800001f8:	0b80006f          	j	800002b0 <mtrap_handler+0x54>
800001fc:	0c00006f          	j	800002bc <mtrap_handler+0x60>
80000200:	0c80006f          	j	800002c8 <mtrap_handler+0x6c>
80000204:	0d00006f          	j	800002d4 <mtrap_handler+0x78>
80000208:	0d80006f          	j	800002e0 <mtrap_handler+0x84>
8000020c:	0e00006f          	j	800002ec <mtrap_handler+0x90>
80000210:	0e80006f          	j	800002f8 <mtrap_handler+0x9c>
80000214:	0f00006f          	j	80000304 <mtrap_handler+0xa8>
80000218:	0f80006f          	j	80000310 <mtrap_handler+0xb4>
8000021c:	30200073          	mret
80000220:	30200073          	mret
80000224:	30200073          	mret
80000228:	30200073          	mret
8000022c:	30200073          	mret
80000230:	30200073          	mret
80000234:	30200073          	mret
80000238:	30200073          	mret
8000023c:	30200073          	mret
80000240:	30200073          	mret
80000244:	30200073          	mret
80000248:	30200073          	mret
8000024c:	30200073          	mret
80000250:	30200073          	mret
80000254:	30200073          	mret
80000258:	30200073          	mret

8000025c <mtrap_handler>:
8000025c:	34011173          	csrrw	sp,mscratch,sp
80000260:	01f12c23          	sw	t6,24(sp)
80000264:	0b800fef          	jal	t6,8000031c <common_prolog>
80000268:	34011173          	csrrw	sp,mscratch,sp
8000026c:	01f12c23          	sw	t6,24(sp)
80000270:	0ac00fef          	jal	t6,8000031c <common_prolog>
80000274:	34011173          	csrrw	sp,mscratch,sp
80000278:	01f12c23          	sw	t6,24(sp)
8000027c:	0a000fef          	jal	t6,8000031c <common_prolog>
80000280:	34011173          	csrrw	sp,mscratch,sp
80000284:	01f12c23          	sw	t6,24(sp)
80000288:	09400fef          	jal	t6,8000031c <common_prolog>
8000028c:	34011173          	csrrw	sp,mscratch,sp
80000290:	01f12c23          	sw	t6,24(sp)
80000294:	08800fef          	jal	t6,8000031c <common_prolog>
80000298:	34011173          	csrrw	sp,mscratch,sp
8000029c:	01f12c23          	sw	t6,24(sp)
800002a0:	07c00fef          	jal	t6,8000031c <common_prolog>
800002a4:	34011173          	csrrw	sp,mscratch,sp
800002a8:	01f12c23          	sw	t6,24(sp)
800002ac:	07000fef          	jal	t6,8000031c <common_prolog>
800002b0:	34011173          	csrrw	sp,mscratch,sp
800002b4:	01f12c23          	sw	t6,24(sp)
800002b8:	06400fef          	jal	t6,8000031c <common_prolog>
800002bc:	34011173          	csrrw	sp,mscratch,sp
800002c0:	01f12c23          	sw	t6,24(sp)
800002c4:	05800fef          	jal	t6,8000031c <common_prolog>
800002c8:	34011173          	csrrw	sp,mscratch,sp
800002cc:	01f12c23          	sw	t6,24(sp)
800002d0:	04c00fef          	jal	t6,8000031c <common_prolog>
800002d4:	34011173          	csrrw	sp,mscratch,sp
800002d8:	01f12c23          	sw	t6,24(sp)
800002dc:	04000fef          	jal	t6,8000031c <common_prolog>
800002e0:	34011173          	csrrw	sp,mscratch,sp
800002e4:	01f12c23          	sw	t6,24(sp)
800002e8:	03400fef          	jal	t6,8000031c <common_prolog>
800002ec:	34011173          	csrrw	sp,mscratch,sp
800002f0:	01f12c23          	sw	t6,24(sp)
800002f4:	02800fef          	jal	t6,8000031c <common_prolog>
800002f8:	34011173          	csrrw	sp,mscratch,sp
800002fc:	01f12c23          	sw	t6,24(sp)
80000300:	01c00fef          	jal	t6,8000031c <common_prolog>
80000304:	34011173          	csrrw	sp,mscratch,sp
80000308:	01f12c23          	sw	t6,24(sp)
8000030c:	01000fef          	jal	t6,8000031c <common_prolog>
80000310:	34011173          	csrrw	sp,mscratch,sp
80000314:	01f12c23          	sw	t6,24(sp)
80000318:	00400fef          	jal	t6,8000031c <common_prolog>

8000031c <common_prolog>:
8000031c:	00000f17          	auipc	t5,0x0
80000320:	00cf0f13          	addi	t5,t5,12 # 80000328 <common_mhandler>
80000324:	000f0067          	jr	t5

80000328 <common_mhandler>:
80000328:	01e12a23          	sw	t5,20(sp)
8000032c:	01d12823          	sw	t4,16(sp)
80000330:	01c12623          	sw	t3,12(sp)
80000334:	00712423          	sw	t2,8(sp)
80000338:	00612223          	sw	t1,4(sp)
8000033c:	00012303          	lw	t1,0(sp)
80000340:	00000e17          	auipc	t3,0x0
80000344:	e9ce0e13          	addi	t3,t3,-356 # 800001dc <mtrampoline>
80000348:	41cf83b3          	sub	t2,t6,t3
8000034c:	00338393          	addi	t2,t2,3
80000350:	00732023          	sw	t2,0(t1)

80000354 <sv_mcause>:
80000354:	342023f3          	csrr	t2,mcause
80000358:	00732223          	sw	t2,4(t1)
8000035c:	0c03cc63          	bltz	t2,80000434 <common_mint_handler>

80000360 <common_mexcpt_handler>:
80000360:	341023f3          	csrr	t2,mepc

80000364 <sv_mepc>:
80000364:	00000e17          	auipc	t3,0x0
80000368:	ca8e0e13          	addi	t3,t3,-856 # 8000000c <rvtest_prolog_done>
8000036c:	41c38eb3          	sub	t4,t2,t3
80000370:	01d32423          	sw	t4,8(t1)

80000374 <adj_mepc>:
80000374:	0023fe93          	andi	t4,t2,2
80000378:	41d383b3          	sub	t2,t2,t4
8000037c:	00838393          	addi	t2,t2,8
80000380:	34139073          	csrw	mepc,t2

80000384 <adj_mtval>:
80000384:	342023f3          	csrr	t2,mcause
80000388:	d0080eb7          	lui	t4,0xd0080
8000038c:	007e9eb3          	sll	t4,t4,t2
80000390:	060ec863          	bltz	t4,80000400 <sv_mtval>
80000394:	00002e17          	auipc	t3,0x2
80000398:	e84e0e13          	addi	t3,t3,-380 # 80002218 <mtrap_sigptr>
8000039c:	0f050eb7          	lui	t4,0xf050
800003a0:	007e9eb3          	sll	t4,t4,t2
800003a4:	000ede63          	bgez	t4,800003c0 <no_adj>

800003a8 <sigbound_chk>:
800003a8:	34302ef3          	csrr	t4,mtval
800003ac:	05de5a63          	bge	t3,t4,80000400 <sv_mtval>
800003b0:	00002e17          	auipc	t3,0x2
800003b4:	c50e0e13          	addi	t3,t3,-944 # 80002000 <rvtest_data_begin>
800003b8:	05de4463          	blt	t3,t4,80000400 <sv_mtval>
800003bc:	000e0e93          	mv	t4,t3

800003c0 <no_adj>:
800003c0:	00000e13          	li	t3,0
800003c4:	ffe38393          	addi	t2,t2,-2
800003c8:	02039c63          	bnez	t2,80000400 <sv_mtval>
800003cc:	343023f3          	csrr	t2,mtval
800003d0:	02039863          	bnez	t2,80000400 <sv_mtval>

800003d4 <illop>:
800003d4:	00020f37          	lui	t5,0x20
800003d8:	300f2f73          	csrrs	t5,mstatus,t5
800003dc:	34102e73          	csrr	t3,mepc
800003e0:	000e5383          	lhu	t2,0(t3)
800003e4:	0033fe93          	andi	t4,t2,3
800003e8:	ffde8e93          	addi	t4,t4,-3 # f04fffd <value+0xf04ffed>
800003ec:	000e9a63          	bnez	t4,80000400 <sv_mtval>
800003f0:	002e5e83          	lhu	t4,2(t3)
800003f4:	010e9e93          	slli	t4,t4,0x10
800003f8:	01d3ee33          	or	t3,t2,t4
800003fc:	300f1073          	csrw	mstatus,t5

80000400 <sv_mtval>:
80000400:	343023f3          	csrr	t2,mtval
80000404:	41c383b3          	sub	t2,t2,t3
80000408:	00732623          	sw	t2,12(t1)

8000040c <resto_rtn>:
8000040c:	01030313          	addi	t1,t1,16
80000410:	00612023          	sw	t1,0(sp)
80000414:	00412303          	lw	t1,4(sp)
80000418:	00812383          	lw	t2,8(sp)
8000041c:	00c12e03          	lw	t3,12(sp)
80000420:	01012e83          	lw	t4,16(sp)
80000424:	01412f03          	lw	t5,20(sp)
80000428:	01812f83          	lw	t6,24(sp)
8000042c:	34011173          	csrrw	sp,mscratch,sp
80000430:	30200073          	mret

80000434 <common_mint_handler>:
80000434:	00100e13          	li	t3,1
80000438:	007e1e33          	sll	t3,t3,t2
8000043c:	344e3ef3          	csrrc	t4,mip,t3
80000440:	304e3ef3          	csrrc	t4,mie,t3

80000444 <sv_mip>:
80000444:	01d32423          	sw	t4,8(t1)
80000448:	00339393          	slli	t2,t2,0x3
8000044c:	00000e17          	auipc	t3,0x0
80000450:	02ce0e13          	addi	t3,t3,44 # 80000478 <clrint_tbl>
80000454:	007e0e33          	add	t3,t3,t2
80000458:	000e2e03          	lw	t3,0(t3)
8000045c:	000e0067          	jr	t3

80000460 <clr_sw_int>:
80000460:	020003b7          	lui	t2,0x2000
80000464:	0003a023          	sw	zero,0(t2) # 2000000 <value+0x1fffff0>
80000468:	fa5ff06f          	j	8000040c <resto_rtn>

8000046c <clr_tmr_int>:
8000046c:	fa1ff06f          	j	8000040c <resto_rtn>

80000470 <clr_ext_int>:
80000470:	ffc32a23          	sw	t3,-12(t1)
80000474:	f99ff06f          	j	8000040c <resto_rtn>

80000478 <clrint_tbl>:
80000478:	040c                	addi	a1,sp,512
8000047a:	8000                	0x8000
8000047c:	0000                	unimp
8000047e:	0000                	unimp
80000480:	0460                	addi	s0,sp,524
80000482:	8000                	0x8000
80000484:	0000                	unimp
80000486:	0000                	unimp
80000488:	040c                	addi	a1,sp,512
8000048a:	8000                	0x8000
8000048c:	0000                	unimp
8000048e:	0000                	unimp
80000490:	0460                	addi	s0,sp,524
80000492:	8000                	0x8000
80000494:	0000                	unimp
80000496:	0000                	unimp
80000498:	040c                	addi	a1,sp,512
8000049a:	8000                	0x8000
8000049c:	0000                	unimp
8000049e:	0000                	unimp
800004a0:	046c                	addi	a1,sp,524
800004a2:	8000                	0x8000
800004a4:	0000                	unimp
800004a6:	0000                	unimp
800004a8:	040c                	addi	a1,sp,512
800004aa:	8000                	0x8000
800004ac:	0000                	unimp
800004ae:	0000                	unimp
800004b0:	046c                	addi	a1,sp,524
800004b2:	8000                	0x8000
800004b4:	0000                	unimp
800004b6:	0000                	unimp
800004b8:	040c                	addi	a1,sp,512
800004ba:	8000                	0x8000
800004bc:	0000                	unimp
800004be:	0000                	unimp
800004c0:	0470                	addi	a2,sp,524
800004c2:	8000                	0x8000
800004c4:	0000                	unimp
800004c6:	0000                	unimp
800004c8:	040c                	addi	a1,sp,512
800004ca:	8000                	0x8000
800004cc:	0000                	unimp
800004ce:	0000                	unimp
800004d0:	0470                	addi	a2,sp,524
800004d2:	8000                	0x8000
800004d4:	0000                	unimp
800004d6:	0000                	unimp
800004d8:	040c                	addi	a1,sp,512
800004da:	8000                	0x8000
800004dc:	0000                	unimp
800004de:	0000                	unimp
800004e0:	040c                	addi	a1,sp,512
800004e2:	8000                	0x8000
800004e4:	0000                	unimp
800004e6:	0000                	unimp
800004e8:	040c                	addi	a1,sp,512
800004ea:	8000                	0x8000
800004ec:	0000                	unimp
800004ee:	0000                	unimp
800004f0:	040c                	addi	a1,sp,512
800004f2:	8000                	0x8000
800004f4:	0000                	unimp
800004f6:	0000                	unimp
800004f8:	040c                	addi	a1,sp,512
800004fa:	8000                	0x8000
800004fc:	0000                	unimp
800004fe:	0000                	unimp
80000500:	040c                	addi	a1,sp,512
80000502:	8000                	0x8000
80000504:	0000                	unimp
80000506:	0000                	unimp
80000508:	040c                	addi	a1,sp,512
8000050a:	8000                	0x8000
8000050c:	0000                	unimp
8000050e:	0000                	unimp
80000510:	040c                	addi	a1,sp,512
80000512:	8000                	0x8000
80000514:	0000                	unimp
80000516:	0000                	unimp
80000518:	040c                	addi	a1,sp,512
8000051a:	8000                	0x8000
8000051c:	0000                	unimp
8000051e:	0000                	unimp
80000520:	040c                	addi	a1,sp,512
80000522:	8000                	0x8000
80000524:	0000                	unimp
80000526:	0000                	unimp
80000528:	040c                	addi	a1,sp,512
8000052a:	8000                	0x8000
8000052c:	0000                	unimp
8000052e:	0000                	unimp
80000530:	040c                	addi	a1,sp,512
80000532:	8000                	0x8000
80000534:	0000                	unimp
80000536:	0000                	unimp
80000538:	040c                	addi	a1,sp,512
8000053a:	8000                	0x8000
8000053c:	0000                	unimp
8000053e:	0000                	unimp
80000540:	040c                	addi	a1,sp,512
80000542:	8000                	0x8000
80000544:	0000                	unimp
80000546:	0000                	unimp
80000548:	040c                	addi	a1,sp,512
8000054a:	8000                	0x8000
8000054c:	0000                	unimp
8000054e:	0000                	unimp
80000550:	040c                	addi	a1,sp,512
80000552:	8000                	0x8000
80000554:	0000                	unimp
80000556:	0000                	unimp
80000558:	040c                	addi	a1,sp,512
8000055a:	8000                	0x8000
8000055c:	0000                	unimp
8000055e:	0000                	unimp
80000560:	040c                	addi	a1,sp,512
80000562:	8000                	0x8000
80000564:	0000                	unimp
80000566:	0000                	unimp
80000568:	040c                	addi	a1,sp,512
8000056a:	8000                	0x8000
8000056c:	0000                	unimp
8000056e:	0000                	unimp
80000570:	040c                	addi	a1,sp,512
80000572:	8000                	0x8000
80000574:	0000                	unimp
80000576:	0000                	unimp
80000578:	00008067          	ret

8000057c <exit_cleanup>:
8000057c:	00002e17          	auipc	t3,0x2
80000580:	ae0e0e13          	addi	t3,t3,-1312 # 8000205c <mtvec_save>
80000584:	00002f17          	auipc	t5,0x2
80000588:	ad8f0f13          	addi	t5,t5,-1320 # 8000205c <mtvec_save>
8000058c:	008f2303          	lw	t1,8(t5)
80000590:	34031073          	csrw	mscratch,t1
80000594:	000f2e83          	lw	t4,0(t5)
80000598:	305e93f3          	csrrw	t2,mtvec,t4
8000059c:	007e9e63          	bne	t4,t2,800005b8 <rvtest_end>
800005a0:	040e8393          	addi	t2,t4,64

800005a4 <resto_vec>:
800005a4:	000e2f83          	lw	t6,0(t3)
800005a8:	01f3a023          	sw	t6,0(t2)
800005ac:	ffc38393          	addi	t2,t2,-4
800005b0:	ffce0e13          	addi	t3,t3,-4
800005b4:	ffd398e3          	bne	t2,t4,800005a4 <resto_vec>

800005b8 <rvtest_end>:
800005b8:	00100093          	li	ra,1

800005bc <write_tohost>:
800005bc:	00001f17          	auipc	t5,0x1
800005c0:	a41f2223          	sw	ra,-1468(t5) # 80001000 <tohost>
800005c4:	ff9ff06f          	j	800005bc <write_tohost>
	...

Disassembly of section .tohost:

80001000 <tohost>:
	...

80001100 <fromhost>:
	...

Disassembly of section .data:

80002000 <rvtest_data_begin>:
80002000:	deadbeef          	jal	t4,7ffdd5ea <value+0x7ffdd5da>
80002004:	deadbeef          	jal	t4,7ffdd5ee <value+0x7ffdd5de>
80002008:	deadbeef          	jal	t4,7ffdd5f2 <value+0x7ffdd5e2>
8000200c:	deadbeef          	jal	t4,7ffdd5f6 <value+0x7ffdd5e6>
80002010:	deadbeef          	jal	t4,7ffdd5fa <value+0x7ffdd5ea>
80002014:	deadbeef          	jal	t4,7ffdd5fe <value+0x7ffdd5ee>
80002018:	deadbeef          	jal	t4,7ffdd602 <value+0x7ffdd5f2>

8000201c <tramptbl_sv>:
8000201c:	0000006f          	j	8000201c <tramptbl_sv>
80002020:	0000006f          	j	80002020 <tramptbl_sv+0x4>
80002024:	0000006f          	j	80002024 <tramptbl_sv+0x8>
80002028:	0000006f          	j	80002028 <tramptbl_sv+0xc>
8000202c:	0000006f          	j	8000202c <tramptbl_sv+0x10>
80002030:	0000006f          	j	80002030 <tramptbl_sv+0x14>
80002034:	0000006f          	j	80002034 <tramptbl_sv+0x18>
80002038:	0000006f          	j	80002038 <tramptbl_sv+0x1c>
8000203c:	0000006f          	j	8000203c <tramptbl_sv+0x20>
80002040:	0000006f          	j	80002040 <tramptbl_sv+0x24>
80002044:	0000006f          	j	80002044 <tramptbl_sv+0x28>
80002048:	0000006f          	j	80002048 <tramptbl_sv+0x2c>
8000204c:	0000006f          	j	8000204c <tramptbl_sv+0x30>
80002050:	0000006f          	j	80002050 <tramptbl_sv+0x34>
80002054:	0000006f          	j	80002054 <tramptbl_sv+0x38>
80002058:	0000006f          	j	80002058 <tramptbl_sv+0x3c>

8000205c <mtvec_save>:
	...

80002064 <mscratch_save>:
	...

80002070 <rvtest_data_end>:
	...

80002100 <begin_regstate>:
80002100:	0080                	addi	s0,sp,64
	...

80002200 <end_regstate>:
80002200:	0004                	0x4
	...

80002210 <begin_signature>:
80002210:	deadbeef          	jal	t4,7ffdd7fa <value+0x7ffdd7ea>
80002214:	deadbeef          	jal	t4,7ffdd7fe <value+0x7ffdd7ee>

80002218 <mtrap_sigptr>:
80002218:	deadbeef          	jal	t4,7ffdd802 <value+0x7ffdd7f2>
8000221c:	deadbeef          	jal	t4,7ffdd806 <value+0x7ffdd7f6>
80002220:	deadbeef          	jal	t4,7ffdd80a <value+0x7ffdd7fa>
80002224:	deadbeef          	jal	t4,7ffdd80e <value+0x7ffdd7fe>
	...
=======
00010054 <_start>:
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
   10054:	00000e97          	auipc	t4,0x0
   10058:	02ce8e93          	addi	t4,t4,44 # 10080 <_trampoline>
   1005c:	002ede93          	srli	t4,t4,0x2
   10060:	002e9e93          	slli	t4,t4,0x2
   10064:	001e8e93          	addi	t4,t4,1
   10068:	305e9073          	csrw	mtvec,t4
   1006c:	00000073          	ecall
   10070:	00000013          	nop
   10074:	00000013          	nop
   10078:	00000013          	nop
   1007c:	00000013          	nop

00010080 <_trampoline>:
   10080:	f81ef06f          	j	0 <_bad>
   10084:	f7def06f          	j	0 <_bad>
   10088:	f79ef06f          	j	0 <_bad>
   1008c:	f75ef06f          	j	0 <_bad>
   10090:	f71ef06f          	j	0 <_bad>
   10094:	f6def06f          	j	0 <_bad>
   10098:	f69ef06f          	j	0 <_bad>
   1009c:	f65ef06f          	j	0 <_bad>
   100a0:	f65ef06f          	j	4 <_good>
   100a4:	f5def06f          	j	0 <_bad>
   100a8:	f59ef06f          	j	0 <_bad>
   100ac:	f55ef06f          	j	0 <_bad>
   100b0:	f51ef06f          	j	0 <_bad>
   100b4:	f4def06f          	j	0 <_bad>
>>>>>>> 31e2ff08 (solving issue with ecall and except gestion just wtf)
=======
=======
>>>>>>> 7b0db4ad (new csr for kernel adr)
   10054:	00a06093          	ori	ra,zero,10
   10058:	00c06113          	ori	sp,zero,12
   1005c:	00106193          	ori	gp,zero,1
   10060:	0030a233          	slt	tp,ra,gp
   10064:	0010b233          	sltu	tp,ra,ra
   10068:	0031f2b3          	and	t0,gp,gp
   1006c:	0020e333          	or	t1,ra,sp
   10070:	005243b3          	xor	t2,tp,t0
   10074:	00311433          	sll	s0,sp,gp
   10078:	003154b3          	srl	s1,sp,gp
   1007c:	0030d533          	srl	a0,ra,gp
   10080:	4030d5b3          	sra	a1,ra,gp
   10084:	00a58633          	add	a2,a1,a0
   10088:	008486b3          	add	a3,s1,s0
   1008c:	00638733          	add	a4,t2,t1
   10090:	00d607b3          	add	a5,a2,a3
   10094:	00e78833          	add	a6,a5,a4
   10098:	410808b3          	sub	a7,a6,a6
   1009c:	00088463          	beqz	a7,100a4 <_start+0x50>
   100a0:	f61ef06f          	j	0 <_bad>
   100a4:	002080b3          	add	ra,ra,sp
   100a8:	00089463          	bnez	a7,100b0 <_start+0x5c>
   100ac:	f59ef06f          	j	4 <_good>
   100b0:	00000013          	nop
   100b4:	00000013          	nop
<<<<<<< HEAD
>>>>>>> f8e6ffde (solving issue with stall with csr)
=======
   10054:	00408093          	addi	ra,ra,4
   10058:	40110133          	sub	sp,sp,ra
   1005c:	004002ef          	jal	t0,10060 <main>

Disassembly of section .text.startup:

00010060 <main>:
   10060:	ff010113          	addi	sp,sp,-16
   10064:	00112623          	sw	ra,12(sp)
   10068:	f9def0ef          	jal	ra,4 <_good>
   1006c:	f95ef0ef          	jal	ra,0 <_bad>
   10070:	00c12083          	lw	ra,12(sp)
   10074:	00000513          	li	a0,0
   10078:	01010113          	addi	sp,sp,16
   1007c:	00008067          	ret

Disassembly of section .comment:

00000000 <.comment>:
   0:	3a434347          	fmsub.d	ft6,ft6,ft4,ft7,rmm
   4:	2820                	fld	fs0,80(s0)
   6:	29554e47          	fmsub.s	ft8,fa0,fs5,ft5,rmm
   a:	3120                	fld	fs0,96(a0)
   c:	2e30                	fld	fa2,88(a2)
   e:	2e32                	fld	ft8,264(sp)
  10:	0030                	addi	a2,sp,8
>>>>>>> 32240fe1 (kernel is working)
=======
   10054:	900001b7          	lui	gp,0x90000
   10058:	0001a203          	lw	tp,0(gp) # 90000000 <_isr_vector+0xeffff64>
>>>>>>> da404326 (start working on exception issue)
=======
>>>>>>> 7b0db4ad (new csr for kernel adr)
=======
   10054:	800020b7          	lui	ra,0x80002
   10058:	31408093          	addi	ra,ra,788 # 80002314 <_isr_vector+0xff002278>
   1005c:	90002137          	lui	sp,0x90002
   10060:	31410113          	addi	sp,sp,788 # 90002314 <_isr_vector+0xf002278>
   10064:	0180006f          	j	1007c <_test>
   10068:	34109073          	csrw	mepc,ra

0001006c <_return>:
   1006c:	341011f3          	csrrw	gp,mepc,zero
   10070:	00119463          	bne	gp,ra,10078 <_return+0xc>
   10074:	f91ef06f          	j	4 <_good>
   10078:	f89ef06f          	j	0 <_bad>

0001007c <_test>:
   1007c:	ff1ff06f          	j	1006c <_return>
>>>>>>> 0af37f80 (error with csr in S2, try to find the source of it. Rewritte of csr to allow mem2 to write it)

Disassembly of section .riscv.attributes:

00000000 <.riscv.attributes>:
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
   0:	1d41                	addi	s10,s10,-16
   2:	0000                	unimp
   4:	7200                	flw	fs0,32(a2)
   6:	7369                	lui	t1,0xffffa
   8:	01007663          	bgeu	zero,a6,14 <value+0x4>
   c:	00000013          	nop
=======
   0:	2241                	jal	180 <_exception_occur+0x178>
=======
   0:	2441                	jal	280 <_exception_occur+0x278>
>>>>>>> 32240fe1 (kernel is working)
=======
   0:	2241                	jal	180 <_exception_occur+0x178>
>>>>>>> da404326 (start working on exception issue)
   2:	0000                	unimp
   4:	7200                	flw	fs0,32(a2)
   6:	7369                	lui	t1,0xffffa
   8:	01007663          	bgeu	zero,a6,14 <_exception_occur+0xc>
   c:	0018                	0x18
   e:	0000                	unimp
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> f8e6ffde (solving issue with stall with csr)
=======
>>>>>>> da404326 (start working on exception issue)
  10:	7205                	lui	tp,0xfffe1
  12:	3376                	fld	ft6,376(sp)
  14:	6932                	flw	fs2,12(sp)
  16:	7032                	flw	ft0,44(sp)
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
  18:	0030                	addi	a2,sp,8
  1a:	0108                	addi	a0,sp,128
  1c:	0b0a                	slli	s6,s6,0x2
=======
  18:	5f30                	lw	a2,120(a4)
  1a:	326d                	jal	fffff9c4 <_isr_vector+0x7efff91c>
=======
  18:	5f30                	lw	a2,120(a4)
  1a:	326d                	jal	fffff9c4 <_isr_vector+0x7efff928>
>>>>>>> f8e6ffde (solving issue with stall with csr)
=======
  18:	5f30                	lw	a2,120(a4)
  1a:	326d                	jal	fffff9c4 <_isr_vector+0x7efff928>
>>>>>>> da404326 (start working on exception issue)
  1c:	3070                	fld	fa2,224(s0)
  1e:	0800                	addi	s0,sp,16
  20:	0a01                	addi	s4,s4,0
  22:	0b              	Address 0x0000000000000022 is out of bounds.
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> 31e2ff08 (solving issue with ecall and except gestion just wtf)
=======
>>>>>>> f8e6ffde (solving issue with stall with csr)
=======
  10:	1004                	addi	s1,sp,32
  12:	7205                	lui	tp,0xfffe1
  14:	3376                	fld	ft6,376(sp)
  16:	6932                	flw	fs2,12(sp)
  18:	7032                	flw	ft0,44(sp)
  1a:	5f30                	lw	a2,120(a4)
  1c:	326d                	jal	fffff9c6 <_isr_vector+0x7efff92a>
  1e:	3070                	fld	fa2,224(s0)
  20:	0800                	addi	s0,sp,16
  22:	0a01                	addi	s4,s4,0
  24:	0b              	Address 0x0000000000000024 is out of bounds.
>>>>>>> 32240fe1 (kernel is working)
=======
>>>>>>> da404326 (start working on exception issue)

Disassembly of section .debug_line:

00000000 <.debug_line>:
   0:	00d6                	slli	ra,ra,0x15
   2:	0000                	unimp
   4:	007a0003          	lb	zero,7(s4)
   8:	0000                	unimp
   a:	0101                	addi	sp,sp,0
   c:	000d0efb          	0xd0efb
  10:	0101                	addi	sp,sp,0
  12:	0101                	addi	sp,sp,0
  14:	0000                	unimp
  16:	0100                	addi	s0,sp,128
  18:	0000                	unimp
  1a:	2f01                	jal	72a <value+0x71a>
  1c:	6f68                	flw	fa0,92(a4)
  1e:	656d                	lui	a0,0x1b
  20:	766f6c2f          	0x766f6c2f
  24:	7369                	lui	t1,0xffffa
  26:	7365442f          	0x7365442f
  2a:	706f746b          	0x706f746b
  2e:	5669522f          	0x5669522f
  32:	7265                	lui	tp,0xffff9
  34:	535f 436f 722f      	0x722f436f535f
  3a:	7369                	lui	t1,0xffffa
  3c:	2f666f63          	bltu	a2,s6,33a <value+0x32a>
  40:	6972                	flw	fs2,28(sp)
  42:	2d766373          	csrrsi	t1,0x2d7,12
  46:	7261                	lui	tp,0xffff8
  48:	742d6863          	bltu	s10,sp,798 <value+0x788>
  4c:	7365                	lui	t1,0xffff9
  4e:	2f74                	fld	fa3,216(a4)
  50:	6972                	flw	fs2,28(sp)
  52:	2d766373          	csrrsi	t1,0x2d7,12
  56:	6574                	flw	fa3,76(a0)
  58:	732d7473          	csrrci	s0,0x732,26
  5c:	6975                	lui	s2,0x1d
  5e:	6574                	flw	fa3,76(a0)
  60:	3376722f          	0x3376722f
  64:	6932                	flw	fs2,12(sp)
  66:	6d5f 702f 6972      	0x6972702f6d5f
  6c:	6976                	flw	fs2,92(sp)
  6e:	656c                	flw	fa1,76(a0)
  70:	732f6567          	0x732f6567
  74:	6372                	flw	ft6,28(sp)
  76:	0000                	unimp
  78:	6365                	lui	t1,0x19
  7a:	6c61                	lui	s8,0x18
  7c:	2e6c                	fld	fa1,216(a2)
  7e:	00010053          	fadd.s	ft0,ft2,ft0,rne
  82:	0000                	unimp
  84:	0500                	addi	s0,sp,640
  86:	0002                	c.slli64	zero
  88:	0000                	unimp
  8a:	0380                	addi	s0,sp,448
  8c:	0112                	slli	sp,sp,0x4
  8e:	04090603          	lb	a2,64(s2) # 1d040 <value+0x1d030>
  92:	0101                	addi	sp,sp,0
  94:	08090203          	lb	tp,128(s2)
  98:	0100                	addi	s0,sp,128
  9a:	08090103          	lb	sp,128(s2)
  9e:	0100                	addi	s0,sp,128
  a0:	04090103          	lb	sp,64(s2)
  a4:	0100                	addi	s0,sp,128
  a6:	04090103          	lb	sp,64(s2)
  aa:	0100                	addi	s0,sp,128
  ac:	04090103          	lb	sp,64(s2)
  b0:	0100                	addi	s0,sp,128
  b2:	04090103          	lb	sp,64(s2)
  b6:	0100                	addi	s0,sp,128
  b8:	0c090b03          	lb	s6,192(s2)
  bc:	0100                	addi	s0,sp,128
  be:	88090103          	lb	sp,-1920(s2)
  c2:	0104                	addi	s1,sp,128
  c4:	1009                	c.nop	-30
  c6:	0000                	unimp
  c8:	0101                	addi	sp,sp,0
  ca:	0500                	addi	s0,sp,640
  cc:	1c02                	slli	s8,s8,0x20
  ce:	0020                	addi	s0,sp,8
  d0:	0380                	addi	s0,sp,448
  d2:	012d                	addi	sp,sp,11
  d4:	1409                	addi	s0,s0,-30
  d6:	0002                	c.slli64	zero
  d8:	0101                	addi	sp,sp,0

Disassembly of section .debug_info:

00000000 <.debug_info>:
   0:	001e                	c.slli	zero,0x7
   2:	0000                	unimp
   4:	0002                	c.slli64	zero
   6:	0000                	unimp
   8:	0000                	unimp
   a:	0104                	addi	s1,sp,128
	...
  18:	0064                	addi	s1,sp,12
  1a:	0000                	unimp
  1c:	000000b7          	lui	ra,0x0
  20:	8001                	c.srli64	s0

Disassembly of section .debug_abbrev:

00000000 <.debug_abbrev>:
   0:	1101                	addi	sp,sp,-32
   2:	1000                	addi	s0,sp,32
   4:	5506                	lw	a0,96(sp)
   6:	0306                	slli	t1,t1,0x1
   8:	1b0e                	slli	s6,s6,0x23
   a:	250e                	fld	fa0,192(sp)
   c:	130e                	slli	t1,t1,0x23
   e:	0005                	c.nop	1
	...

<<<<<<< HEAD
Disassembly of section .debug_aranges:
=======
80000000 <_reset>:
80000000:	00010e37          	lui	t3,0x10
80000004:	054e0e13          	addi	t3,t3,84 # 10054 <_start>
80000008:	01000e97          	auipc	t4,0x1000
8000000c:	ff8e8e93          	addi	t4,t4,-8 # 81000000 <_exception>
80000010:	90000f37          	lui	t5,0x90000
80000014:	80000db7          	lui	s11,0x80000
80000018:	002ede93          	srli	t4,t4,0x2
8000001c:	002e9e93          	slli	t4,t4,0x2
80000020:	000e8e93          	mv	t4,t4
80000024:	341e1073          	csrw	mepc,t3
80000028:	305e9073          	csrw	mtvec,t4
8000002c:	340f1073          	csrw	mscratch,t5
80000030:	800d9073          	csrw	0x800,s11
80000034:	00010137          	lui	sp,0x10
80000038:	01000497          	auipc	s1,0x1000
<<<<<<< HEAD
8000003c:	07048493          	addi	s1,s1,112 # 810000a8 <_isr_vector>
=======
8000003c:	06448493          	addi	s1,s1,100 # 8100009c <_isr_vector>
>>>>>>> f8109c37 (solving issue with curr mode)
80000040:	01000517          	auipc	a0,0x1000
80000044:	ff050513          	addi	a0,a0,-16 # 81000030 <_instruction_address_fault>
80000048:	01000597          	auipc	a1,0x1000
8000004c:	ff458593          	addi	a1,a1,-12 # 8100003c <_illegal_instruction>
80000050:	01000617          	auipc	a2,0x1000
80000054:	fd460613          	addi	a2,a2,-44 # 81000024 <_instruction_address_misagligned>
80000058:	01000697          	auipc	a3,0x1000
8000005c:	02068693          	addi	a3,a3,32 # 81000078 <_env_call_u_mode>
80000060:	01000717          	auipc	a4,0x1000
<<<<<<< HEAD
80000064:	02470713          	addi	a4,a4,36 # 81000084 <_env_call_s_mode>
80000068:	01000797          	auipc	a5,0x1000
8000006c:	02878793          	addi	a5,a5,40 # 81000090 <_env_call_m_mode>
=======
80000064:	02070713          	addi	a4,a4,32 # 81000080 <_env_call_s_mode>
80000068:	01000797          	auipc	a5,0x1000
8000006c:	02078793          	addi	a5,a5,32 # 81000088 <_env_call_m_mode>
>>>>>>> f8109c37 (solving issue with curr mode)
80000070:	01000817          	auipc	a6,0x1000
80000074:	fd880813          	addi	a6,a6,-40 # 81000048 <_load_adress_missaligned>
80000078:	01000897          	auipc	a7,0x1000
8000007c:	fe888893          	addi	a7,a7,-24 # 81000060 <_store_adress_missaligned>
80000080:	01000917          	auipc	s2,0x1000
80000084:	fd490913          	addi	s2,s2,-44 # 81000054 <_load_access_fault>
80000088:	01000997          	auipc	s3,0x1000
8000008c:	fe498993          	addi	s3,s3,-28 # 8100006c <_store_access_fault>
80000090:	01000a17          	auipc	s4,0x1000
<<<<<<< HEAD
80000094:	00ca0a13          	addi	s4,s4,12 # 8100009c <_env_call_wrong_mode>
=======
80000094:	000a0a13          	mv	s4,s4
>>>>>>> f8109c37 (solving issue with curr mode)
80000098:	00c4a023          	sw	a2,0(s1)
8000009c:	00a4a223          	sw	a0,4(s1)
800000a0:	00b4a423          	sw	a1,8(s1)
800000a4:	0104a823          	sw	a6,16(s1)
800000a8:	0124aa23          	sw	s2,20(s1)
800000ac:	0114ac23          	sw	a7,24(s1)
800000b0:	0134ae23          	sw	s3,28(s1)
800000b4:	02d4a023          	sw	a3,32(s1)
800000b8:	02e4a223          	sw	a4,36(s1)
800000bc:	02f4a423          	sw	a5,40(s1)
800000c0:	0744a023          	sw	s4,96(s1)
800000c4:	00000193          	li	gp,0
800000c8:	00000213          	li	tp,0
800000cc:	00000293          	li	t0,0
800000d0:	00000313          	li	t1,0
800000d4:	00000393          	li	t2,0
800000d8:	00000413          	li	s0,0
800000dc:	00000493          	li	s1,0
800000e0:	00000513          	li	a0,0
800000e4:	00000593          	li	a1,0
800000e8:	00000613          	li	a2,0
800000ec:	00000693          	li	a3,0
800000f0:	00000713          	li	a4,0
800000f4:	00000793          	li	a5,0
800000f8:	00000813          	li	a6,0
800000fc:	00000893          	li	a7,0
80000100:	00000913          	li	s2,0
80000104:	00000993          	li	s3,0
80000108:	00000a13          	li	s4,0
8000010c:	00000a93          	li	s5,0
80000110:	00000b13          	li	s6,0
80000114:	00000b93          	li	s7,0
80000118:	00000c13          	li	s8,0
8000011c:	00000c93          	li	s9,0
80000120:	30200073          	mret
<<<<<<< HEAD
>>>>>>> 31e2ff08 (solving issue with ecall and except gestion just wtf)
=======
>>>>>>> f8109c37 (solving issue with curr mode)

00000000 <.debug_aranges>:
   0:	0024                	addi	s1,sp,8
   2:	0000                	unimp
   4:	0002                	c.slli64	zero
   6:	0000                	unimp
   8:	0000                	unimp
   a:	0004                	0x4
   c:	0000                	unimp
   e:	0000                	unimp
  10:	0000                	unimp
  12:	8000                	0x8000
  14:	05c8                	addi	a0,sp,708
  16:	0000                	unimp
  18:	2000                	fld	fs0,0(s0)
  1a:	8000                	0x8000
  1c:	0230                	addi	a2,sp,264
	...

<<<<<<< HEAD
Disassembly of section .debug_str:
=======
81000000 <_exception>:
81000000:	34205573          	csrrwi	a0,mcause,0
81000004:	3400f5f3          	csrrci	a1,mscratch,1
81000008:	00a5a023          	sw	a0,0(a1)
8100000c:	00251513          	slli	a0,a0,0x2
81000010:	00000597          	auipc	a1,0x0
81000014:	09858593          	addi	a1,a1,152 # 810000a8 <_isr_vector>
81000018:	00a585b3          	add	a1,a1,a0
8100001c:	0005a603          	lw	a2,0(a1)
81000020:	00060067          	jr	a2
>>>>>>> 31e2ff08 (solving issue with ecall and except gestion just wtf)

00000000 <.debug_str>:
   0:	6d6f682f          	0x6d6f682f
   4:	2f65                	jal	7bc <value+0x7ac>
   6:	6f6c                	flw	fa1,92(a4)
   8:	6976                	flw	fs2,92(sp)
   a:	65442f73          	csrrs	t5,0x654,s0
   e:	6f746b73          	csrrsi	s6,0x6f7,8
  12:	2f70                	fld	fa2,216(a4)
  14:	6952                	flw	fs2,20(sp)
  16:	6556                	flw	fa0,84(sp)
  18:	5f72                	lw	t5,60(sp)
  1a:	2f436f53          	0x2f436f53
  1e:	6972                	flw	fs2,28(sp)
  20:	666f6373          	csrrsi	t1,0x666,30
  24:	7369722f          	0x7369722f
  28:	612d7663          	bgeu	s10,s2,634 <value+0x624>
  2c:	6372                	flw	ft6,28(sp)
  2e:	2d68                	fld	fa0,216(a0)
  30:	6574                	flw	fa3,76(a0)
  32:	722f7473          	csrrci	s0,0x722,30
  36:	7369                	lui	t1,0xffffa
  38:	742d7663          	bgeu	s10,sp,784 <value+0x774>
  3c:	7365                	lui	t1,0xffff9
  3e:	2d74                	fld	fa3,216(a0)
  40:	74697573          	csrrci	a0,0x746,18
  44:	2f65                	jal	7fc <value+0x7ec>
  46:	7672                	flw	fa2,60(sp)
  48:	5f693233          	0x5f693233
  4c:	2f6d                	jal	806 <value+0x7f6>
  4e:	7270                	flw	fa2,100(a2)
  50:	7669                	lui	a2,0xffffa
  52:	6c69                	lui	s8,0x1a
  54:	6765                	lui	a4,0x19
  56:	2f65                	jal	80e <value+0x7fe>
  58:	2f637273          	csrrci	tp,0x2f6,6
  5c:	6365                	lui	t1,0x19
  5e:	6c61                	lui	s8,0x18
  60:	2e6c                	fld	fa1,216(a2)
  62:	682f0053          	0x682f0053
  66:	2f656d6f          	jal	s10,5635c <value+0x5634c>
  6a:	6f6c                	flw	fa1,92(a4)
  6c:	6976                	flw	fs2,92(sp)
  6e:	65442f73          	csrrs	t5,0x654,s0
  72:	6f746b73          	csrrsi	s6,0x6f7,8
  76:	2f70                	fld	fa2,216(a4)
  78:	6952                	flw	fs2,20(sp)
  7a:	6556                	flw	fa0,84(sp)
  7c:	5f72                	lw	t5,60(sp)
  7e:	2f436f53          	0x2f436f53
  82:	6972                	flw	fs2,28(sp)
  84:	666f6373          	csrrsi	t1,0x666,30
  88:	7369722f          	0x7369722f
  8c:	5f666f63          	bltu	a2,s6,68a <value+0x67a>
  90:	6b726f77          	0x6b726f77
  94:	3376722f          	0x3376722f
  98:	6932                	flw	fs2,12(sp)
  9a:	6d5f 702f 6972      	0x6972702f6d5f
  a0:	6976                	flw	fs2,92(sp)
  a2:	656c                	flw	fa1,76(a0)
  a4:	732f6567          	0x732f6567
  a8:	6372                	flw	ft6,28(sp)
  aa:	6163652f          	0x6163652f
  ae:	6c6c                	flw	fa1,92(s0)
  b0:	532e                	lw	t1,232(sp)
  b2:	7475642f          	0x7475642f
  b6:	4700                	lw	s0,8(a4)
  b8:	554e                	lw	a0,240(sp)
  ba:	4120                	lw	s0,64(a0)
  bc:	2e322053          	0x2e322053
  c0:	          	snez	a0,zero

Disassembly of section .debug_ranges:

<<<<<<< HEAD
00000000 <.debug_ranges>:
   0:	ffff                	0xffff
   2:	ffff                	0xffff
   4:	0000                	unimp
   6:	0000                	unimp
   8:	0000                	unimp
   a:	8000                	0x8000
   c:	05c8                	addi	a0,sp,708
   e:	8000                	0x8000
  10:	2000                	fld	fs0,0(s0)
  12:	8000                	0x8000
  14:	2230                	fld	fa2,64(a2)
  16:	8000                	0x8000
	...
=======
8100003c <_illegal_instruction>:
8100003c:	00000013          	nop
81000040:	00800093          	li	ra,8
81000044:	00008067          	ret

81000048 <_load_adress_missaligned>:
81000048:	00000013          	nop
8100004c:	00800093          	li	ra,8
81000050:	00008067          	ret

81000054 <_load_access_fault>:
81000054:	00000013          	nop
81000058:	00800093          	li	ra,8
8100005c:	00008067          	ret

81000060 <_store_adress_missaligned>:
81000060:	00000013          	nop
81000064:	00800093          	li	ra,8
81000068:	00008067          	ret

8100006c <_store_access_fault>:
8100006c:	00000013          	nop
81000070:	00800093          	li	ra,8
81000074:	00008067          	ret

81000078 <_env_call_u_mode>:
81000078:	00000013          	nop
8100007c:	00800093          	li	ra,8
81000080:	00008067          	ret

81000084 <_env_call_s_mode>:
81000084:	00000013          	nop
81000088:	00800093          	li	ra,8
8100008c:	00008067          	ret

81000090 <_env_call_m_mode>:
81000090:	00000013          	nop
81000094:	00800093          	li	ra,8
81000098:	00008067          	ret

8100009c <_env_call_wrong_mode>:
8100009c:	00000013          	nop
810000a0:	00800093          	li	ra,8
810000a4:	00008067          	ret
>>>>>>> 31e2ff08 (solving issue with ecall and except gestion just wtf)
