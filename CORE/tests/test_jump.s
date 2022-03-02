.section .text
.global _start

_start:
    addi x0,x0,0
    addi x3,x0,10
    addi x0,x0,0
    nop
    nop
    nop
    beq x0,x0, _good
    nop
_bad: 
    nop
_good:
    nop
