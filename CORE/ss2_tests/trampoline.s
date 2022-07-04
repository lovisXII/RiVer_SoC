.section .text
.global _start

_start :
    addi x1,x0,10
    addi x2,x0,10
    nop
    nop
    nop
    nop
    nop
    nop
    beq x1,x2, _branch1
    j _bad
_label1 :
    addi x2,x2,13
    nop
    nop
    nop
    nop
    nop
    blt x1,x2, _branch2
    j _bad
_label2 :
    bge x2,x1,_branch3
_label3 :
    j _good
_branch1 :
    j _label1
_branch2 :  
    j _label2
_branch3 :
    j _label3