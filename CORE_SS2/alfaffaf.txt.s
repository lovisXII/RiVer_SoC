





   10098:	00078513          	mv	a0,a5
   1009c:	01c12403          	lw	s0,28(sp)
   10078:	fec42703          	lw	a4,-20(s0)
   1007c:	fe842783          	lw	a5,-24(s0)
   10080:	40f707b3          	sub	a5,a4,a5
   
   10084:	fef42623          	sw	a5,-20(s0)

   10088:	fec42703          	lw	a4,-20(s0)
   1008c:	fe842783          	lw	a5,-24(s0)
   10090:	fef754e3          	bge	a4,a5,10078 <modulo+0x18>
   10094:	fec42783          	lw	a5,-20(s0)
   10098:	00078513          	mv	a0,a5
   1009c:	01c12403          	lw	s0,28(sp)
   100a0:	02010113          	addi	sp,sp,32
   100a4:	00008067          	ret
   100a8:	fe010113          	addi	sp,sp,-32
   100ac:	00112e23          	sw	ra,28(sp)
   100e0:	00050793          	mv	a5,a0
   100e4:	02079063          	bnez	a5,10104 <main+0x5c>
   100e4:	02079063          	bnez	a5,10104 <main+0x5c>
   100e8:	fe842583          	lw	a1,-24(s0)
   100e0:	00050793          	mv	a5,a0
   100e4:	02079063          	bnez	a5,10104 <main+0x5c>
   10104:	fe842783          	lw	a5,-24(s0)
   10108:	00178793          	addi	a5,a5,1
   1010c:	fef42423          	sw	a5,-24(s0)
    10110:	fe842703          	lw	a4,-24(s0)
   10114:	fe442783          	lw	a5,-28(s0)
   10118:	00e7c863          	blt	a5,a4,10128 <main+0x80>
   1011c:	fe842703          	lw	a4,-24(s0)
   10114:	fe442783          	lw	a5,-28(s0)
   10118:	00e7c863          	blt	a5,a4,10128 <main+0x80>
   10128:	fec42703          	lw	a4,-20(s0)
   1012c:	00100793          	li	a5,1 # a5 = 1
   10130:	00f71663          	bne	a4,a5,1013c <main+0x94> # this stuff branch and should not