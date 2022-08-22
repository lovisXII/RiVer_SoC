.section .text
.global _start

_start:
    ori     x3, x0, 15
    ori     x4, x0, 1
    add     x5, x4, x3
    sw      x5, 60(x0)
    nop
    li      x4, 0x40000000
    sw      x3, 0(x4)
    nop
    nop