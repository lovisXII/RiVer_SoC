.section .text
.global main

main:
    auipc x1, 12
    j _good
    nop
    nop
    nop
    nop
    nop