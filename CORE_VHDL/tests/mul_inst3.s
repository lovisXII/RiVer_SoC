.section .text
.global _start

_start:
ori x3, x0, -513
ori x4, x0, 3
mul x5, x3, x4
nop
nop
nop
nop
nop
j _bad
nop 
nop