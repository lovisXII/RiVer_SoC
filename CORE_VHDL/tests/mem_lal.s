.section .text
.global _start

_start :

    li x1,0xFFFF0000
    li x2,0x23
    sw x2,0(x1)
    auipc x3,0x2