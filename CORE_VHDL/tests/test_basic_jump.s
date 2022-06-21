.section .text
.global _start

_start: 
    ori x1, x0, 10
    ori x2, x0, 15
    ori x3, x0, 1
    j _end 
    ori x3, x0, 255
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
_end:
    ori x3, x0, 0
    nop
    nop