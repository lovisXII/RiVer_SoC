.section .text
.global _start

_start:
    addi x1,x0,10
    add  x2,x0,x1
    beq x1,x0, _good
    j _bad
    nop
    