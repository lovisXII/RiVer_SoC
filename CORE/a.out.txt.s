
a.out:     file format elf32-littleriscv


Disassembly of section seg_text:

00010054 <_start>:
   10054:	00408093          	addi	ra,ra,4
   10058:	40110133          	sub	sp,sp,ra
   1005c:	004002ef          	jal	t0,10060 <main>

Disassembly of section .text.startup:

00010060 <main>:
   10060:	ff010113          	addi	sp,sp,-16
   10064:	00112623          	sw	ra,12(sp)
   10068:	f9def0ef          	jal	ra,4 <_good>
   1006c:	00c12083          	lw	ra,12(sp)
   10070:	00000513          	li	a0,0
   10074:	01010113          	addi	sp,sp,16
   10078:	00008067          	ret

Disassembly of section .comment:

00000000 <.comment>:
   0:	3a434347          	fmsub.d	ft6,ft6,ft4,ft7,rmm
   4:	2820                	fld	fs0,80(s0)
   6:	29554e47          	fmsub.s	ft8,fa0,fs5,ft5,rmm
   a:	3120                	fld	fs0,96(a0)
   c:	2e30                	fld	fa2,88(a2)
   e:	2e32                	fld	ft8,264(sp)
  10:	0030                	addi	a2,sp,8

Disassembly of section .riscv.attributes:

00000000 <.riscv.attributes>:
   0:	2441                	jal	280 <_exception_occur+0x278>
   2:	0000                	unimp
   4:	7200                	flw	fs0,32(a2)
   6:	7369                	lui	t1,0xffffa
   8:	01007663          	bgeu	zero,a6,14 <_exception_occur+0xc>
   c:	001a                	c.slli	zero,0x6
   e:	0000                	unimp
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
