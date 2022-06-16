.section .text
.global _start

_start:
    ori x1, x0, 10          # r1 = 10
    ori x2, x0, 4           # r2 = 4
    ori x3, x0, 5           # r3 = 5
    ori x24, x0, 2          # r24 = 2
    addi x4, x2, 50         # r4 = 54
    nop
    nop
    nop
    nop
    sw x1, 1000(x24)        
    ori x5, x0, 7           # r5 = 7
    ori x6, x0, 6           # r6 = 6
    nop
    lw x10, 1000(x24)       # r10 = 10
    nop
    nop
    sub x11, x10, x1
    nop
    nop