.section .text
.global _start

_start :

    li x5, 23
    li x6, 46

    li x1,10
    li x2,12
    li x3,125
    li x4,23


    sw x4, 0(x0) 
    add x1,x0,x0 

    add x1,x0,x4 # x1 = 23
    nop

    lw x3,0(x0) # x3 = 23
    nop

    add x2,x1,x3 # x2=23+23=46
    nop

    bne x3,x5,_bad
    bne x2,x6, _bad

    j _good