.section .text
.global _start

_start :

    li x3,0x123
    li x4,0x91231451
    sw x3,0(x4)
    sw x3,4(x4)
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop