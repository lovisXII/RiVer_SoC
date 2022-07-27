
a.out:     file format elf32-littleriscv


Disassembly of section seg_text:

00010054 <_start>:
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

Disassembly of section .riscv.attributes:

00000000 <.riscv.attributes>:
   0:	2241                	jal	180 <_exception_occur+0x178>
   2:	0000                	unimp
   4:	7200                	flw	fs0,32(a2)
   6:	7369                	lui	t1,0xffffa
   8:	01007663          	bgeu	zero,a6,14 <_exception_occur+0xc>
   c:	0018                	0x18
   e:	0000                	unimp
  10:	7205                	lui	tp,0xfffe1
  12:	3376                	fld	ft6,376(sp)
  14:	6932                	flw	fs2,12(sp)
  16:	7032                	flw	ft0,44(sp)
  18:	5f30                	lw	a2,120(a4)
  1a:	326d                	jal	fffff9c4 <_isr_vector+0x7efff928>
  1c:	3070                	fld	fa2,224(s0)
  1e:	0800                	addi	s0,sp,16
  20:	0a01                	addi	s4,s4,0
  22:	0b              	Address 0x0000000000000022 is out of bounds.


Disassembly of section text:

00000000 <_bad>:
   0:	00000013          	nop

00000004 <_good>:
   4:	00000013          	nop

00000008 <_exception_occur>:
   8:	00000013          	nop

Disassembly of section seg_reset:

80000000 <_reset>:
80000000:	00010e37          	lui	t3,0x10
80000004:	054e0e13          	addi	t3,t3,84 # 10054 <_start>
80000008:	01000e97          	auipc	t4,0x1000
8000000c:	ff8e8e93          	addi	t4,t4,-8 # 81000000 <_exception>
80000010:	90000f37          	lui	t5,0x90000
80000014:	002ede93          	srli	t4,t4,0x2
80000018:	002e9e93          	slli	t4,t4,0x2
8000001c:	000e8e93          	mv	t4,t4
80000020:	341e1073          	csrw	mepc,t3
80000024:	305e9073          	csrw	mtvec,t4
80000028:	340f1073          	csrw	mscratch,t5
8000002c:	00010137          	lui	sp,0x10
80000030:	01000497          	auipc	s1,0x1000
80000034:	06c48493          	addi	s1,s1,108 # 8100009c <_isr_vector>
80000038:	01000517          	auipc	a0,0x1000
8000003c:	ff850513          	addi	a0,a0,-8 # 81000030 <_instruction_address_fault>
80000040:	01000597          	auipc	a1,0x1000
80000044:	ffc58593          	addi	a1,a1,-4 # 8100003c <_illegal_instruction>
80000048:	01000617          	auipc	a2,0x1000
8000004c:	fdc60613          	addi	a2,a2,-36 # 81000024 <_instruction_address_misagligned>
80000050:	01000697          	auipc	a3,0x1000
80000054:	02868693          	addi	a3,a3,40 # 81000078 <_env_call_u_mode>
80000058:	01000717          	auipc	a4,0x1000
8000005c:	02870713          	addi	a4,a4,40 # 81000080 <_env_call_s_mode>
80000060:	01000797          	auipc	a5,0x1000
80000064:	02878793          	addi	a5,a5,40 # 81000088 <_env_call_m_mode>
80000068:	01000817          	auipc	a6,0x1000
8000006c:	fe080813          	addi	a6,a6,-32 # 81000048 <_load_adress_missaligned>
80000070:	01000897          	auipc	a7,0x1000
80000074:	ff088893          	addi	a7,a7,-16 # 81000060 <_store_adress_missaligned>
80000078:	01000917          	auipc	s2,0x1000
8000007c:	fdc90913          	addi	s2,s2,-36 # 81000054 <_load_access_fault>
80000080:	01000997          	auipc	s3,0x1000
80000084:	fec98993          	addi	s3,s3,-20 # 8100006c <_store_access_fault>
80000088:	01000a17          	auipc	s4,0x1000
8000008c:	008a0a13          	addi	s4,s4,8 # 81000090 <_env_call_wrong_mode>
80000090:	00c4a023          	sw	a2,0(s1)
80000094:	00a4a223          	sw	a0,4(s1)
80000098:	00b4a423          	sw	a1,8(s1)
8000009c:	0104a823          	sw	a6,16(s1)
800000a0:	0124aa23          	sw	s2,20(s1)
800000a4:	0114ac23          	sw	a7,24(s1)
800000a8:	0134ae23          	sw	s3,28(s1)
800000ac:	02d4a023          	sw	a3,32(s1)
800000b0:	02e4a223          	sw	a4,36(s1)
800000b4:	02f4a423          	sw	a5,40(s1)
800000b8:	0744a023          	sw	s4,96(s1)
800000bc:	00000193          	li	gp,0
800000c0:	00000213          	li	tp,0
800000c4:	00000293          	li	t0,0
800000c8:	00000313          	li	t1,0
800000cc:	00000393          	li	t2,0
800000d0:	00000413          	li	s0,0
800000d4:	00000493          	li	s1,0
800000d8:	00000513          	li	a0,0
800000dc:	00000593          	li	a1,0
800000e0:	00000613          	li	a2,0
800000e4:	00000693          	li	a3,0
800000e8:	00000713          	li	a4,0
800000ec:	00000793          	li	a5,0
800000f0:	00000813          	li	a6,0
800000f4:	00000893          	li	a7,0
800000f8:	00000913          	li	s2,0
800000fc:	00000993          	li	s3,0
80000100:	00000a13          	li	s4,0
80000104:	00000a93          	li	s5,0
80000108:	00000b13          	li	s6,0
8000010c:	00000b93          	li	s7,0
80000110:	00000c13          	li	s8,0
80000114:	00000c93          	li	s9,0
80000118:	30200073          	mret

Disassembly of section seg_kernel:

81000000 <_exception>:
81000000:	34205573          	csrrwi	a0,mcause,0
81000004:	3400f5f3          	csrrci	a1,mscratch,1
81000008:	00a5a023          	sw	a0,0(a1)
8100000c:	00251513          	slli	a0,a0,0x2
81000010:	00000597          	auipc	a1,0x0
81000014:	08c58593          	addi	a1,a1,140 # 8100009c <_isr_vector>
81000018:	00a585b3          	add	a1,a1,a0
8100001c:	0005a603          	lw	a2,0(a1)
81000020:	00060067          	jr	a2

81000024 <_instruction_address_misagligned>:
81000024:	00000013          	nop
81000028:	00800093          	li	ra,8
8100002c:	00008067          	ret

81000030 <_instruction_address_fault>:
81000030:	00000013          	nop
81000034:	00800093          	li	ra,8
81000038:	00008067          	ret

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
8100007c:	30200073          	mret

81000080 <_env_call_s_mode>:
81000080:	00000013          	nop
81000084:	30200073          	mret

81000088 <_env_call_m_mode>:
81000088:	00000013          	nop
8100008c:	30200073          	mret

81000090 <_env_call_wrong_mode>:
81000090:	00000013          	nop
81000094:	00800093          	li	ra,8
81000098:	00008067          	ret
