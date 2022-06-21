.section .text
.global _start

_start:
    ori x1, x0, 5       # x1 = 5
    ori x2, x0, 10      # x2 = A
    ori x3, x0, 15      # x3 = F
    ori x10, x0, 1
    nop
    nop
    nop
    sw  x3, 32(x0)
    add x4, x2, x1      # x4 = F
    nop
    nop
    lw  x5, 32(x0)      # x5 = F
    beq x5, x3, _nice
    ori x10, x0, 255
    nop
    nop
_nice: 
    ori x10, x0, 0
    nop
    nop
