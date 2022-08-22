.section .text
.global _start

_start :

    li x5,47
    nop
    
    addi x1,x0,12
    addi x2,x0,23
    
    add x3,x1,x2 # x3 = 35
    add x4,x0,12 # x4 = 12

    add x3,x4,x3 # x3 = 47
    beq x3,x5,_good
    j _bad