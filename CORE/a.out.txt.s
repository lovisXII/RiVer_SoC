
a.out:     file format elf32-littleriscv


Disassembly of section seg_text:

00010054 <_start>:
   10054:	00306093          	ori	ra,zero,3
   10058:	00a06113          	ori	sp,zero,10
   
   1005c:	01406193          	ori	gp,zero,20
   10060:	00708093          	addi	ra,ra,7
   
   10064:	00208463          	beq	ra,sp,1006c <_start+0x18>
   10068:	f99ef06f          	j	0 <_bad>
   
   1006c:	002080b3          	add	ra,ra,sp
   10070:	00308463          	beq	ra,gp,10078 <_start+0x24>
   
   10070:	00308463          	beq	ra,gp,10078 <_start+0x24>
   10074:	f8def06f          	j	0 <_bad>

   
   10078:	ffd02093          	slti	ra,zero,-3
   1007c:	00100463          	beq	zero,ra,10084 <_start+0x30>
   
   1007c:	00100463          	beq	zero,ra,10084 <_start+0x30>
   10080:	f81ef06f          	j	0 <_bad>
   
   10084:	ffd03093          	sltiu	ra,zero,-3
   10088:	00101463          	bne	zero,ra,10090 <_start+0x3c>

   10088:	00101463          	bne	zero,ra,10090 <_start+0x3c>
   1008c:	f75ef06f          	j	0 <_bad>
   
   10090:	00302093          	slti	ra,zero,3
   10094:	00101463          	bne	zero,ra,1009c <_start+0x48>
   
   10094:	00101463          	bne	zero,ra,1009c <_start+0x48>
   10098:	f69ef06f          	j	0 <_bad>
   
   1009c:	00a14093          	xori	ra,sp,10
   100a0:	00100463          	beq	zero,ra,100a8 <_start+0x54>


   100a0:	00100463          	beq	zero,ra,100a8 <_start+0x54>
   100a4:	f5def06f          	j	0 <_bad>
   
   100a8:	00a17093          	andi	ra,sp,10
   100ac:	00110463          	beq	sp,ra,100b4 <_start+0x60>
   
   100ac:	00110463          	beq	sp,ra,100b4 <_start+0x60>
   100b0:	f51ef06f          	j	0 <_bad>
   
   100b4:	00111093          	slli	ra,sp,0x1
   100b8:	00308463          	beq	ra,gp,100c0 <_start+0x6c>

   100b8:	00308463          	beq	ra,gp,100c0 <_start+0x6c>
   100bc:	f45ef06f          	j	0 <_bad>
   
   100c0:	fec06213          	ori	tp,zero,-20
   100c4:	ff606293          	ori	t0,zero,-10
   
   100c8:	40125093          	srai	ra,tp,0x1
   100cc:	00508463          	beq	ra,t0,100d4 <_start+0x80>
   
   100d0:	f31ef06f          	j	0 <_bad>
   100d4:	00106313          	ori	t1,zero,1
   
   100d8:	01d31313          	slli	t1,t1,0x1d
   100dc:	00125093          	srli	ra,tp,0x1
   
   100e0:	0060d463          	bge	ra,t1,100e8 <_start+0x94>
   100e4:	f1def06f          	j	0 <_bad>
   
   100e8:	f1def06f          	j	4 <_good>
   100ec:	00000013          	nop

   100f0:	00000013          	nop
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
