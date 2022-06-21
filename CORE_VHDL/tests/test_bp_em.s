.section .text
.global _start

_start:
    ori x1, x0, 8       # x1 = 8
    ori x2, x0, 10      # x2 = A
    ori x3, x0, 15      # x3 = F
    sw  x1, 32(x0)
    nop
    nop
    nop
    lw  x4, 32(x0)      # x4 = 8
    addi x5, x4, 7      # x5 = F
    nop
    nop
    nop
    sub x2, x5, x3      # x2 = 0
    nop
    nop
    nop
