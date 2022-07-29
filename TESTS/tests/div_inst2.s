.section .text
.global _start

_start:
li      x4, 0x2001
lui	    x2,0xffffe
addi	x2,x2,-1
li  	x3,-1
div	    x3,x2,x3

nop
nop
nop
nop

beq     x3, x4, _good 
j _bad

nop
nop
nop
nop