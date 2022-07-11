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
    addi x2,x0,13
    addi x1,x0,12

    # bypass E2 -> E1 on x1

    add x4,x1,x0 # Bypass from addi x1,x0,x12
    addi x3,x0,12
    beq x4,x1,_good
    j _bad
