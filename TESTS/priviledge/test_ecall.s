.section .text
.global _start

_start:
    li x1,0x2654
    ori x2,x1,23
    andi x12,x12,15
    beq x0,x1,_bad
    ecall 
    lw x3,0(x1)