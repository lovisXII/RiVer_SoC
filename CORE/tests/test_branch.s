.section .text
.global _start

_start:
    addi x2,x0,4
    jalr x1, x2,8 
    nop
_bad: 
    nop
_good:
    nop
