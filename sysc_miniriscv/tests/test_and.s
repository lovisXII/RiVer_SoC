.section .text
.global main

main:
    li   x10, 0x8000000
    andi x11, x10, 0x0
    beqz x11, _good
    nop
    nop
