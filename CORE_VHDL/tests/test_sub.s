.section .text
.global _start

_start:
    ori x1, x0, 10  # x1 = 10 // 1010 
    ori x2, x0, 12  # x2 = 12 // 1100
    ori x3, x0, 1   # x3 = 1
    ori x6, x0, 15  # x6 = F
    nop
    nop
    nop
    sub x4, x1, x3  # x4 = 9
    sub x5, x3, x2  # x5 = -11
    sub x6, x1, x1  # x6 = 0
    nop
    nop
