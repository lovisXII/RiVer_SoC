.section .text
.global main

main:
    ori x1, x0, 3
    ori x1, x0, 4
    or x2, x1, x0
    bne x2,x1, _bad
    beq x1,x2, _good
    nop
    nop


