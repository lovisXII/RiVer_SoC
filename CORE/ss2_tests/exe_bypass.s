.section .text
.global _start

_start :
    # loading random values in registers
    li x1,10
    li x2,20
    li x3,152
    li x4,520
    li x5,120
    li x6,63

    # Testing exe_bypass
    addi x1,x0,12
    addi x2,x0,13

    # bypass E1 -> E2 on x1

    addi x3,x0,12
    add x4,x0,x1 # Bypass from addi x1,x0,x12
    beq x4,x1,_good
    j _bad
