.section .text
.global _start

_start:
lui	t1,0x40
lui	t1,0x40
ori x2, x0, 1
divu	t1,t1,t1
nop
nop
nop

beq t1, x2, _good
j _bad
nop
nop
nop
nop