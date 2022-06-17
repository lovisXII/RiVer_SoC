.section .text
.global _start

_start:
    ori x1, x0, 2047    # x1 = 7FF
    ori x2, x0, 255     # x2 = FF
    ori x3, x0, 2       # x3 = 2
    ori x4, x0, 10      # x4 = 10      
    nop
    nop
    nop

    sb  x1, 2(x3)
    
    nop
    nop
    nop

    lb  x5, 2(x3)       # x5 = FF

    nop
    nop
    nop

    sub x4, x2, x5      # x4 = 0

    nop
    nop