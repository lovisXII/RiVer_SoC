.section .text
.global _start

_start:
    ori x1, x0, 10
    ori x2, x0, 5
    ori x3, x0, 15
    nop
    nop
    nop
    add x4, x1, x2
    beq x3, x4, _nice
    ori x10, x0, 1
    nop
_nice: 
    ori x10, x0, 255
    nop
    nop
    