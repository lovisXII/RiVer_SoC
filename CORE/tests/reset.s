.section .reset
.global _reset

_reset:
    nop
    nop
    la x1,0x10054       #loading main adress
    la x2,0x81000000    #loading exception handler adress
    csrrw x0, 0x341,x1 
    csrrw x0, 0x305,x2
    mret

.section .text
.global _start

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
