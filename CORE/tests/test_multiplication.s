.section .text
.global _start

_start:
    ori x7, x0, 2
    ori x9, x0, 12
    ori x8, x0, 6
    mul x10, x7, x8
    bne x10, x9, _bad

    j _good

    nop
    nop
