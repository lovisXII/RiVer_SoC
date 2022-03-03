.section .text
.global _start

_start:
    addi x6,x0,10
    jalr x1,x0,8
    nop

_test :
    
_bad: 
    nop
_good:
    nop
