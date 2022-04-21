.section .text 
.global _start
.global _kernel

_start:
    addi x1,x0,1
    addi x10,x0,24
    lw x3,0(x1)
    addi x2,x0,10
    addi x4,x0,12
    add x5,x4,x2
    add x5,x0,x5
    beq x5, x10, _good
    addi x1,x0,0
    addi x1,x0,0
    addi x1,x0,0
    nop
_bad: 
    nop
_good:
    nop

.section .text.kernel 

_mtvec :
    addi x1,x0,10
    j _good