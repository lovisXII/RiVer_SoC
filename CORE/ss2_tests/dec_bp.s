.global .text
.global _start

_start :
    li x1,120
    li x2,152
    nop
    nop
    nop
    nop
    nop
    nop
    addi x1,x0,10
    addi x2,x0,10
    addi x2,x0,0
    beq x1,x2, _good
    j _bad
    j _bad
    j _bad
    j _bad
    j _bad