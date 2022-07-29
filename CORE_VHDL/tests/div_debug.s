.section .text
.global _start

_start:
    li x3, 10
    li x4, 2
    li x6, 5

    div x5, x3, x4

    beq x5, x6, _good
    j _bad
    nop
    nop
