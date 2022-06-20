.section .text
.global _start

_start: 
    ori x1, x0, 10      # x1 = A
    ori x2, x0, 5       # x2 = 5
    ori x3, x0, 15      # x3 = F
    nop
    nop
    nop
    add x4, x2, x2      # x4 = 10
    nop 
    nop
    nop
    beq x1, x4, _noice 
    ori x3, x0, 1       # not exec 
    nop
    nop
    nop

_noice: 
    ori x3, x0, 255     # x3 = FF  
    nop
    nop
    nop
