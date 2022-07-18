
a.out:     file format elf32-littleriscv


Disassembly of section seg_text:

00010054 <_start>:
   10054:	00500213          	li	tp,5
   10058:	00700293          	li	t0,7
   
   1005c:	02402023          	sw	tp,32(zero) # 20 <_exception_occur+0x18>
   10060:	00200093          	li	ra,2
   
   10064:	00308113          	addi	sp,ra,3
   10068:	00000013          	nop
   
   1006c:	00000013          	nop
   10070:	00000013          	nop
   
   10074:	00410463          	beq	sp,tp,1007c <_start+0x28>
   10078:	f89ef06f          	j	0 <_bad>
   
   1007c:	00200413          	li	s0,2
   10080:	00041463          	bnez	s0,10088 <_start+0x34>
   
   10080:	00041463          	bnez	s0,10088 <_start+0x34>
   10084:	f7def06f          	j	0 <_bad>
   
   10088:	02002383          	lw	t2,32(zero) # 20 <_exception_occur+0x18>
   1008c:	00238113          	addi	sp,t2,2

   1008c:	00238113          	addi	sp,t2,2
   10090:	00000013          	nop
   
   10094:	00000013          	nop
   10098:	00000013          	nop
   
   1009c:	00510463          	beq	sp,t0,100a4 <_start+0x50>
   100a0:	f61ef06f          	j	0 <_bad>
   
   100a4:	02052103          	lw	sp,32(a0)
   100a8:	00410463          	beq	sp,tp,100b0 <_start+0x5c>
   
   100a8:	00410463          	beq	sp,tp,100b0 <_start+0x5c>
   100ac:	f55ef06f          	j	0 <_bad>

   100b0:	f55ef06f          	j	4 <_good>
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
