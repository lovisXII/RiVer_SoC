.section .text
.global main

main:
    la x9, 0x80000000
    li x8, 0xffffffff
    sw x8, 0(x9)
    fence
    lw x3, 0(x9)
    beq x8, x3, _good
    j _bad
    nop
    nop