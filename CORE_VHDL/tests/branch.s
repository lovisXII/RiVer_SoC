.section .text
.global _start

ori x1, x0, 1
ori x2, x0, 5
ori x3, x0, 6

add x4, x1, x3 
beq x4, x3, _good
j _bad
nop
nop
