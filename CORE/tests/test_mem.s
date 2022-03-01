.section .text
.global _start

_start:
    ori x1,x0,1953
    slli x1,x1,6 # x1 = 124992
    ori x1,x1,20 # x1 = 125012
    ori x2,x0,84 #less significant byte of 125012
    sw x1,32(x0)
    lw x3,32(x0)
    sb x1,36(x0)
    lb x4,36(x0)
    sub x5,x3,x4 #125012 - 84 = 124928
    ori x6,x0,61
    sll x6,x6,11 # sll = 124928
    bne x5,x6, _bad
    beq x6,x5, _good
    nop
    nop
_bad: 
    nop
_good:
    nop


