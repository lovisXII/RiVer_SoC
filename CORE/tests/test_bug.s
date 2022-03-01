.section .text
.global _start

#This programm bug, indeed the ori x1,x1,20 instruction should give 125012 as result
#but it gaves 1973, because it doesn't correctly wait for x1 to be wbk

_start:
    ori x1,x0,1953
    slli x1,x1,6 # x1 = 124992
    ori x1,x1,20 # x1 = 125012
    bne x5,x6, _bad
    beq x6,x5, _good
    nop
    nop
_bad: 
    nop
_good:
    nop


