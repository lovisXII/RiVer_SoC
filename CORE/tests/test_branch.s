.section .text
.global _start

_start:
    addi x1,x0,4
    or x0,x0,x0
    jal x1, -8
    nop
_bad: 
    nop
_good:
    nop
