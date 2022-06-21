.section .text
.global _start

_start:
    ori x1, x0, 10  # x1 = A
    ori x2, x0, 5   # x2 = 5
    ori x3, x0, 20  # x3 = 14
    ori x4, x0, 15  # x4 = F

    add x5, x4, x2  # x4 = 14 (= A + 5) EXE/EXE bypass
    nop
    nop
    nop
    beq x4, x5, _nice
    ori x6, x0, 1
    ori x4, x0, 1
_nice: 
    ori x4, x0, 255
    nop
    nop

    