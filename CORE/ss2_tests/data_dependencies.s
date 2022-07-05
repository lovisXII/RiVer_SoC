.section .text
.global _start

_start:
    addi x1,x0,10
    add  x2,x0,x1
    or x9,x8,x7
    sw x5,0(x6)
    add x10,x11,x12
    and x15,x14,x13
    sll x18,x19,x20
    beq x2,x1, _good
    nop
    