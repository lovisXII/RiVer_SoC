.section .text
.global _start

_start:
    li      x3, 256
    li      x4, -1
_loop: 
    addi    x22, x22, 2
    sw      x22, 32(x0)
    bne     x3, x22, _loop
    sw      x4, 32(x0)
    j _good
    nop
    nop