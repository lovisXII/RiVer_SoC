.section .text
.global _start

_start :

    li x3,0x123
    li x4,0x5631
    lw x3,0(x4)
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