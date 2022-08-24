.section .text
.global _start

_start :
    csrrwi x0,mepc,0xF # mepc = 0xF
    li x1,1         
    csrrc x0,mepc,x1  #mepc = 0xE
    li x2, 0xE
    csrr x3,mepc
    beq x3,x2, _good
    j _bad