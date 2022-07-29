.section .text
.global _start

_start:
    li x3,10
    nop 
    addi x1,x0,10
    add  x2,x0,x1
    beq x3,x1, _label1
    j _bad


_label1 :
    li x12,20
    li x13,50
    beq x12,x13,_bad
    j _label2

_label2 :
    j _good