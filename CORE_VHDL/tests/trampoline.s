.section .text
.global _trampoline

_trampoline:
    j _label12
_label1:
    j _label11
_label2:
    blt x2,x4,_label10
_label3:
    j _labelx
_label10:
    addi x6,x0,12
    bge x6,x5, _label3
_label11:
    addi x4,x0,10
    addi x5,x0,10
    ret
_label12:
    nop 
    addi x1,x0,2
    addi x2,x0,2
    bne x1,x2, _trampoline
    beq x1,x2, _label1
    j _bad
_labelx :
    nop
    nop
    j _good

_good: 
    nop
    nop

_bad: 
    nop
    nop
