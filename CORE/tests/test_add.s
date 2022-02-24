.section .text
.global _start

_start:
    ori x1, x0, 3
    ori x2, x0, 10
    ori x3, x0, 20
    addi x1, x1, 7
    bne x1, x2, _bad
    add x1, x1, x2
    beq x1, x3, _good
    nop
    nop
_bad: 
    nop
_good:
    nop
