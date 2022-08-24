.section .text
.global _start 

_start :
    li x1,120
    li x2,0x40000000
    li x3,15
    li x4,65

    sw x1,0(x2)
    lw x5,0(x2)
    sw x3,4(x2)
    lw x6,4(x2)

    bne x5,x1, _bad
    bne x3,x6, _bad
    j _good