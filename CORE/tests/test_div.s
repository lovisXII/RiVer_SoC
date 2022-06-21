.section .text
.global _start

_start:
    ori x7, x0, 4
    ori x8, x0, 6
    ori x9, x0, 24
 
    divu x10, x9, x7
    bne x10, x8, _bad

    ori x11, x0, -2
    ori x12, x0, -3

    div x10, x8, x11
    bne x10, x12, _bad

    remu x10, x9, x7
    bne x10, x0, _bad

    ori x7, x0, 5
    ori x9, x0, 1
    rem x10, x7, x8
    bne x10, x9, _bad
    
    j _good

    nop
    nop
