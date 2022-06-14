.section .text
.global _start

_start:
    ori x7, x0, 4
    ori x8, x0, 6
    ori x9, x0, 24

    mul x10, x7, x8
    bne x10, x9, _bad

    mulhu x10, x7, x8
    bne x10, x0, _bad

    ori x7, x0, -4
    ori x8, x0, 4
    ori x9, x0, 0xFFFFFFFF
    mulhsu x10, x7, x8
    bne x10, x9, _bad

    ori x7, x0, -4
    ori x9, x0, 0x00000000

    mulh x10, x7, x7
    bne x10, x9, _bad

    j _good

    nop
    nop
