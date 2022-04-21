.section .text
.global _start

_start:
    addi x4,x0,0x5
    csrrwi x3, 0x300, 0x4 #x3 = 0; 0x300 = 0x5
    csrrsi x3, 0x300, 0x1 # x3 = 0x5; 0x300 = 0x5 or 0x1
    csrrwi x3, 0x300, 0 #x3 = 0x6; 
    beq x3,x4, _good
    beq x3,x4, _bad
    nop
    nop
_bad: 
    nop
_good:
    nop


