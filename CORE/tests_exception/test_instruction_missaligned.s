.section .text
.global _start

_start :
    addi x1,x0,1
    beq x0,x0,_label
.space 2
_label :
    nop
    ori x2,x0,12
    nop
    nop