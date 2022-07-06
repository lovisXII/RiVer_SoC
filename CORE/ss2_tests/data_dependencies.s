.section .text
.global _start

_start:
    li x3,10
    nop 
    addi x1,x0,10
    add  x2,x0,x1
    beq x3,x1, _good
    j _bad
    nop
    