.section .text
.global _start

_start:
    li x2,123
    li x23,4
    csrrwi x2,mepc,5
    li x1,1
    csrrc x3,mepc,x1
    beq x3,x23,_good
    j _bad