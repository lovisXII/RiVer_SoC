.section .text
.global _start

_start:
    ori x1, x0, 10
    ori x2, x0, 10
    beq x1, x2, _good
    nop
    nop
_bad: 
    nop
_good:
    nop
