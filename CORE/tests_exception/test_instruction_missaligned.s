.section .text
.global _start

_start :
    addi x1,x0,1
    jalr x0, 0(x1) 